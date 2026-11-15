// Analyse d'un sous-ensemble de LaTeX (mode mathematique).
#include <string.h>
#include "Math.hpp"
#include "MathInternal.hpp"
#include "Symbols.hpp"
#include "../font/Font.hpp"
#include "../gfx/Color.hpp"

namespace math {

int g_errors = 0;
char g_errMsg[160];
bool g_oom = false;

// --------------------------------------------------------------------------
// Zone memoire des noeuds
// --------------------------------------------------------------------------
enum { NODE_ARENA = 16 * 1024 };
alignas(8) static uint8_t s_nodeMem[NODE_ARENA];
static int s_nodeUsed = 0;
static int s_nodePeak = 0;
static Node s_dummyNode;

Node *newNode(uint8_t type)
{
    const int sz = (int)((sizeof(Node) + 7) & ~7u);
    if (s_nodeUsed + sz > NODE_ARENA) {
        g_oom = true;
        memset(&s_dummyNode, 0, sizeof(Node));
        s_dummyNode.type = type;
        return &s_dummyNode;
    }
    Node *n = (Node *)(s_nodeMem + s_nodeUsed);
    s_nodeUsed += sz;
    if (s_nodeUsed > s_nodePeak) s_nodePeak = s_nodeUsed;
    memset(n, 0, sizeof(Node));
    n->type = type;
    n->cls = C_ORD;
    return n;
}

bool isDummy(const Node *n) { return n == &s_dummyNode; }
void resetNodes() { s_nodeUsed = 0; }
int nodeBytesPeak() { return s_nodePeak; }

static void appendStr(char *dst, int cap, int &pos, const char *s, int len)
{
    for (int i = 0; (len < 0 ? s[i] != 0 : i < len) && pos < cap - 1; i++) dst[pos++] = s[i];
    dst[pos] = 0;
}

void reportError(const char *what, const char *where, int len)
{
    g_errors++;
    int pos = 0;
    appendStr(g_errMsg, sizeof(g_errMsg), pos, what, -1);
    if (where) {
        appendStr(g_errMsg, sizeof(g_errMsg), pos, " : ", -1);
        if (len > 40) len = 40;
        appendStr(g_errMsg, sizeof(g_errMsg), pos, where, len);
    }
}

// --------------------------------------------------------------------------
// Analyseur
// --------------------------------------------------------------------------
enum {
    STOP_BRACE = 1, STOP_AMP = 2, STOP_ROW = 4, STOP_RIGHT = 8, STOP_END = 16,
    STOP_BRACKET = 32,
};
enum Reason { SR_EOF, SR_BRACE, SR_AMP, SR_ROW, SR_RIGHT, SR_END, SR_BRACKET };

struct ListB {
    Node *first = nullptr, *last = nullptr, *prev = nullptr;
    void add(Node *n)
    {
        if (!n || isDummy(n)) return;
        n->next = nullptr;
        if (last) { last->next = n; prev = last; }
        else first = n;
        last = n;
    }
    void replaceLast(Node *n)
    {
        if (!n || isDummy(n)) return;
        n->next = nullptr;
        if (!last) { add(n); return; }
        if (prev) prev->next = n;
        else first = n;
        last = n;
    }
};

static bool isLetter(char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); }

struct Parser {
    const char *p;
    const char *end;
    uint8_t fv = FV_AUTO;
    int depth = 0;
    Reason reason = SR_EOF;

    void skipSpaces()
    {
        while (p < end && (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r')) p++;
    }

    // Vrai si p pointe sur "\name" (non suivi d'une lettre).
    bool atCmd(const char *name) const
    {
        int n = (int)strlen(name);
        if (p + 1 + n > end || *p != '\\') return false;
        if (strncmp(p + 1, name, n) != 0) return false;
        return p + 1 + n >= end || !isLetter(p[1 + n]);
    }

    void err(const char *what, const char *where = nullptr, int len = 0)
    {
        reportError(what, where ? where : p, where ? len : (int)(end - p));
    }

    Node *space(int mu)
    {
        Node *n = newNode(N_SPACE);
        n->val = (int16_t)mu;
        n->cls = C_NONE;
        return n;
    }

    Node *sym(uint32_t cp, uint8_t cls)
    {
        if (cls == C_OP) {
            Node *n = newNode(N_OP);
            n->cp = cp;
            n->cls = C_OP;
            if (isBigOpLimits(cp)) n->flags |= F_DISPLAY_LIMITS;
            return n;
        }
        Node *n = newNode(N_SYM);
        n->cp = cp;
        n->cls = cls;
        n->fv = fv;
        return n;
    }

    Node *group(Node *first)
    {
        Node *g = newNode(N_GROUP);
        g->a = first;
        return g;
    }

    // Liste jusqu'a une condition d'arret.
    Node *parseList(int stops)
    {
        ListB L;
        reason = SR_EOF;
        if (++depth > 24) {
            err("formule trop imbriquee");
            p = end;
            depth--;
            return group(nullptr);
        }
        for (;;) {
            skipSpaces();
            if (p >= end) { reason = SR_EOF; break; }
            char c = *p;
            if (c == '}') {
                p++;
                if (stops & STOP_BRACE) { reason = SR_BRACE; break; }
                err("accolade fermante en trop");
                continue;
            }
            if (c == ']' && (stops & STOP_BRACKET)) { p++; reason = SR_BRACKET; break; }
            if (c == '&') {
                p++;
                if (stops & STOP_AMP) { reason = SR_AMP; break; }
                err("& hors d'un tableau");
                continue;
            }
            if (c == '\\' && p + 1 < end && p[1] == '\\') {
                p += 2;
                // \\[espace] : l'espacement optionnel est ignore
                const char *q = p;
                while (q < end && *q == ' ') q++;
                if (q < end && *q == '[') {
                    while (q < end && *q != ']') q++;
                    if (q < end) p = q + 1;
                }
                if (stops & STOP_ROW) { reason = SR_ROW; break; }
                continue;   // retour a la ligne ignore
            }
            if (c == '\\' && atCmd("right")) {
                if (stops & STOP_RIGHT) { reason = SR_RIGHT; break; }
                err("\\right sans \\left");
                p += 6;
                readDelim();
                continue;
            }
            if (c == '\\' && atCmd("end")) {
                if (stops & STOP_END) { reason = SR_END; break; }
                err("\\end sans \\begin");
                p += 4;
                readRaw(nullptr);
                continue;
            }
            if (c == '\\' && (atCmd("limits") || atCmd("nolimits"))) {
                bool lim = atCmd("limits");
                p += lim ? 7 : 9;
                Node *t = L.last;
                if (t && t->type == N_OP) {
                    t->flags &= (uint8_t)~(F_LIMITS | F_NOLIMITS);
                    t->flags |= lim ? F_LIMITS : F_NOLIMITS;
                }
                continue;
            }
            if (c == '^' || c == '_') {
                p++;
                Node *arg = parseArg();
                attachScript(L, c == '^', arg);
                continue;
            }
            if (c == '\'') {
                int n = 0;
                while (p < end && *p == '\'') { n++; p++; }
                attachPrime(L, n);
                continue;
            }
            Node *n = parseAtom();
            if (n) L.add(n);
        }
        depth--;
        Reason r = reason;
        Node *g = group(L.first);
        reason = r;
        return g;
    }

    void attachScript(ListB &L, bool sup, Node *arg)
    {
        Node *last = L.last;
        if (!last || last->type == N_SPACE || last->type == N_STYLE) {
            last = group(nullptr);
            L.add(last);
        }
        if (last->type == N_SCRIPTS && (sup ? !last->c : !last->b)) {
            if (sup) last->c = arg;
            else last->b = arg;
            return;
        }
        Node *s = newNode(N_SCRIPTS);
        s->a = last;
        s->cls = last->cls;
        if (sup) s->c = arg;
        else s->b = arg;
        L.replaceLast(s);
    }

    void attachPrime(ListB &L, int n)
    {
        uint32_t cp = n == 1 ? 0x2032 : n == 2 ? 0x2033 : 0x2034;
        Node *pr = newNode(N_SYM);
        pr->cp = cp;
        pr->fv = FV_RM;
        Node *last = L.last;
        if (!last) { L.add(pr); return; }
        Node *g = newNode(N_GROUP);
        if (isDummy(g)) return;
        uint8_t cls = last->cls;
        L.replaceLast(g);
        g->a = last;
        last->next = pr;
        pr->next = nullptr;
        g->cls = cls;
        g->flags |= F_PRIME;
    }

    // Argument obligatoire : {groupe} ou un seul element.
    Node *parseArg()
    {
        skipSpaces();
        if (p >= end) { err("argument manquant"); return group(nullptr); }
        if (*p == '{') {
            p++;
            Node *g = parseList(STOP_BRACE);
            if (reason != SR_BRACE) err("accolade non fermee");
            return g;
        }
        Node *n = parseAtom();
        return n ? n : group(nullptr);
    }

    Node *parseArgFv(uint8_t newFv)
    {
        uint8_t save = fv;
        fv = newFv;
        Node *n = parseArg();
        fv = save;
        return n;
    }

    Node *parseAtom()
    {
        char c = *p;
        if (c == '{') {
            p++;
            Node *g = parseList(STOP_BRACE);
            if (reason != SR_BRACE) err("accolade non fermee");
            return g;
        }
        if (c == '\\') return parseCommand();
        if (c == '~') { p++; return space(6); }
        uint32_t cp = font::utf8Next(p, end);
        if (cp == '-') cp = 0x2212;
        else if (cp == '*') cp = 0x2217;
        return sym(cp, charClass(cp));
    }

    // Texte brut entre accolades (sans interpretation).
    bool readRaw(const char **out, int *outLen = nullptr)
    {
        skipSpaces();
        if (p >= end) return false;
        if (*p != '{') {
            const char *s = p;
            font::utf8Next(p, end);
            if (out) { *out = s; *outLen = (int)(p - s); }
            return true;
        }
        p++;
        const char *s = p;
        int lvl = 1;
        while (p < end) {
            if (*p == '\\' && p + 1 < end) { p += 2; continue; }
            if (*p == '{') lvl++;
            else if (*p == '}') { if (--lvl == 0) break; }
            p++;
        }
        if (out) { *out = s; *outLen = (int)(p - s); }
        if (p < end) p++;
        else err("accolade non fermee");
        return true;
    }

    uint32_t readDelim()
    {
        skipSpaces();
        if (p >= end) { err("delimiteur manquant"); return 0; }
        if (*p == '\\') {
            p++;
            if (p < end && !isLetter(*p)) {
                char c = *p++;
                if (c == '{') return '{';
                if (c == '}') return '}';
                if (c == '|') return 0x2016;
                err("delimiteur inconnu", p - 2, 2);
                return 0;
            }
            const char *s = p;
            while (p < end && isLetter(*p)) p++;
            int n = (int)(p - s);
            struct D { const char *name; uint32_t cp; };
            static const D ds[] = {
                {"langle", 0x27E8}, {"rangle", 0x27E9}, {"lfloor", 0x230A},
                {"rfloor", 0x230B}, {"lceil", 0x2308}, {"rceil", 0x2309},
                {"vert", '|'}, {"Vert", 0x2016}, {"lvert", '|'}, {"rvert", '|'},
                {"lVert", 0x2016}, {"rVert", 0x2016}, {"lbrace", '{'}, {"rbrace", '}'},
            };
            for (unsigned i = 0; i < sizeof(ds) / sizeof(ds[0]); i++)
                if ((int)strlen(ds[i].name) == n && !strncmp(ds[i].name, s, n)) return ds[i].cp;
            err("delimiteur inconnu", s, n);
            return 0;
        }
        char c = *p++;
        switch (c) {
        case '(': case ')': case '[': case ']': case '|': case '/': return (uint32_t)c;
        case '.': return 0;
        default:
            err("delimiteur inconnu", p - 1, 1);
            return 0;
        }
    }

    Node *parseCommand()
    {
        const char *start = p;
        p++;   // '\'
        if (p >= end) { err("\\ isole"); return nullptr; }
        char c = *p;
        if (!isLetter(c)) {
            p++;
            switch (c) {
            case ',': return space(3);
            case ':': case '>': return space(4);
            case ';': return space(5);
            case '!': return space(-3);
            case ' ': return space(6);
            default: {
                const SymDef *s = findSymbol(&c, 1);
                if (s) return sym(s->cp, s->cls);
                err("commande inconnue", start, 2);
                return sym('?', C_ORD);
            }
            }
        }
        const char *name = p;
        while (p < end && isLetter(*p)) p++;
        int n = (int)(p - name);
        auto is = [&](const char *s) { return (int)strlen(s) == n && !strncmp(s, name, n); };

        if (is("quad")) return space(18);
        if (is("qquad")) return space(36);
        if (is("frac") || is("dfrac") || is("tfrac")) {
            Node *f = newNode(N_FRAC);
            Node *num = parseArg();
            Node *den = parseArg();
            f->a = num;
            f->b = den;
            f->aux = is("dfrac") ? 1 : is("tfrac") ? 2 : 0;
            return f;
        }
        if (is("binom") || is("dbinom")) {
            Node *f = newNode(N_FRAC);
            Node *num = parseArg();
            Node *den = parseArg();
            f->a = num;
            f->b = den;
            f->flags |= F_NOBAR;
            f->kind = 1;
            f->aux = is("dbinom") ? 1 : 0;
            return f;
        }
        if (is("sqrt")) {
            Node *r = newNode(N_RAD);
            skipSpaces();
            if (p < end && *p == '[') {
                p++;
                r->a = parseList(STOP_BRACKET);
                if (reason != SR_BRACKET) err("] manquant");
            }
            r->b = parseArg();
            return r;
        }
        if (is("left")) {
            Node *lr = newNode(N_LR);
            lr->cp = readDelim();
            lr->a = parseList(STOP_RIGHT);
            if (reason == SR_RIGHT) {
                p += 6;
                lr->cp2 = readDelim();
            } else {
                err("\\left sans \\right", start, 6);
            }
            lr->cls = C_INNER;
            return lr;
        }
        {
            static const char *bigs[] = {"big", "Big", "bigg", "Bigg"};
            for (int k = 0; k < 4; k++) {
                int bl = (int)strlen(bigs[k]);
                if (n >= bl && n <= bl + 1 && !strncmp(name, bigs[k], bl)) {
                    char suf = n > bl ? name[bl] : 0;
                    if (suf && suf != 'l' && suf != 'r' && suf != 'm') break;
                    Node *d = newNode(N_BIGDELIM);
                    d->cp = readDelim();
                    d->aux = (uint8_t)(k + 1);
                    d->cls = suf == 'l' ? (uint8_t)C_OPEN : suf == 'r' ? (uint8_t)C_CLOSE : charClass(d->cp);
                    return d;
                }
            }
        }
        if (is("text") || is("textrm") || is("mbox") || is("textnormal") || is("textup")) {
            Node *t = newNode(N_TEXT);
            const char *s = nullptr;
            int len = 0;
            readRaw(&s, &len);
            t->str = s;
            t->len = (int16_t)len;
            return t;
        }
        if (is("mathrm") || is("mathsf") || is("mathup")) return parseArgFv(FV_RM);
        if (is("mathbf") || is("boldsymbol") || is("bm")) return parseArgFv(FV_BF);
        if (is("mathbb")) return parseArgFv(FV_BB);
        if (is("mathcal") || is("mathscr")) return parseArgFv(FV_CAL);
        if (is("mathit")) return parseArgFv(FV_IT);
        if (is("operatorname")) {
            Node *o = newNode(N_OP);
            const char *s = nullptr;
            int len = 0;
            readRaw(&s, &len);
            o->str = s;
            o->len = (int16_t)len;
            o->cls = C_OP;
            return o;
        }
        {
            struct A { const char *name; uint32_t cp; uint8_t kind; };
            static const A acc[] = {
                {"hat", 0x302, AK_NORMAL}, {"widehat", 0x302, AK_WIDE},
                {"bar", 0x304, AK_NORMAL}, {"dot", 0x307, AK_NORMAL},
                {"ddot", 0x308, AK_NORMAL}, {"tilde", 0x303, AK_NORMAL},
                {"widetilde", 0x303, AK_WIDE}, {"vec", 0x20D7, AK_NORMAL},
                {"check", 0x30C, AK_NORMAL}, {"overrightarrow", 0x20D7, AK_STRETCH_ARROW},
            };
            for (unsigned k = 0; k < sizeof(acc) / sizeof(acc[0]); k++) {
                if (is(acc[k].name)) {
                    Node *a = newNode(N_ACCENT);
                    a->cp = acc[k].cp;
                    a->kind = acc[k].kind;
                    a->a = parseArg();
                    return a;
                }
            }
        }
        if (is("overline") || is("underline")) {
            Node *o = newNode(N_OVERLINE);
            o->aux = is("underline") ? 1 : 0;
            o->a = parseArg();
            return o;
        }
        if (is("begin")) return parseEnv(start);
        if (is("overset") || is("stackrel") || is("underset")) {
            Node *s = newNode(N_STACK);
            Node *x = parseArg();
            Node *base = parseArg();
            if (is("underset")) s->c = x;
            else s->a = x;
            s->b = base;
            s->cls = C_REL;
            // classe de la base si c'est un seul symbole
            Node *b0 = base;
            if (b0 && b0->type == N_GROUP && b0->a && !b0->a->next) b0 = b0->a;
            if (b0 && b0->type == N_SYM) s->cls = b0->cls;
            return s;
        }
        if (is("xrightarrow") || is("xleftarrow")) {
            Node *x = newNode(N_XARROW);
            x->cp = is("xleftarrow") ? 0x2190 : 0x2192;
            skipSpaces();
            if (p < end && *p == '[') {
                p++;
                x->b = parseList(STOP_BRACKET);
                if (reason != SR_BRACKET) err("] manquant");
            }
            x->a = parseArg();
            x->cls = C_REL;
            return x;
        }
        if (is("color") || is("textcolor")) {
            Node *cn = newNode(N_COLOR);
            const char *s = nullptr;
            int len = 0;
            readRaw(&s, &len);
            cn->val = (int16_t)colorByName(s, len);
            cn->a = parseArg();
            return cn;
        }
        if (is("displaystyle") || is("textstyle") || is("scriptstyle") || is("scriptscriptstyle")) {
            Node *st = newNode(N_STYLE);
            st->cls = C_NONE;
            st->aux = is("displaystyle") ? ST_D : is("textstyle") ? ST_T
                     : is("scriptstyle") ? ST_S : ST_SS;
            return st;
        }
        if (is("boxed")) {
            Node *b = newNode(N_BOXED);
            b->a = parseArg();
            return b;
        }
        if (is("phantom")) {
            Node *b = newNode(N_PHANTOM);
            b->a = parseArg();
            return b;
        }
        if (is("not")) {
            skipSpaces();
            if (p < end && *p == '=') { p++; return sym(0x2260, C_REL); }
            if (atCmd("in")) { p += 3; return sym(0x2209, C_REL); }
            if (atCmd("subseteq")) { p += 9; return sym(0x2288, C_REL); }
            if (atCmd("subset")) { p += 7; return sym(0x2284, C_REL); }
            if (atCmd("supseteq")) { p += 9; return sym(0x2289, C_REL); }
            if (atCmd("supset")) { p += 7; return sym(0x2285, C_REL); }
            if (atCmd("exists")) { p += 7; return sym(0x2204, C_ORD); }
            err("\\not non gere", start, (int)(p - start) + 2);
            return nullptr;
        }
        const SymDef *s = findSymbol(name, n);
        if (s) return sym(s->cp, s->cls);
        const NamedOp *op = findNamedOp(name, n);
        if (op) {
            Node *o = newNode(N_OP);
            o->str = op->text;
            o->len = (int16_t)strlen(op->text);
            o->cls = C_OP;
            if (op->limits) o->flags |= F_DISPLAY_LIMITS;
            return o;
        }
        err("commande inconnue", start, n + 1);
        return sym('?', C_ORD);
    }

    static int colorByName(const char *s, int len)
    {
        struct C { const char *name; uint16_t c; };
        static const C cs[] = {
            {"red", col::red}, {"blue", col::blue}, {"green", col::green},
            {"orange", col::orange}, {"purple", col::purple}, {"teal", col::teal},
            {"gray", col::gray}, {"grey", col::gray}, {"black", col::ink},
        };
        for (unsigned i = 0; i < sizeof(cs) / sizeof(cs[0]); i++)
            if (s && (int)strlen(cs[i].name) == len && !strncmp(cs[i].name, s, len)) return cs[i].c;
        reportError("couleur inconnue", s, len);
        return col::red;
    }

    Node *parseEnv(const char *start)
    {
        const char *env = nullptr;
        int envLen = 0;
        readRaw(&env, &envLen);
        auto envIs = [&](const char *s) { return env && (int)strlen(s) == envLen && !strncmp(s, env, envLen); };
        uint8_t kind;
        if (envIs("matrix")) kind = AR_MATRIX;
        else if (envIs("pmatrix")) kind = AR_PMATRIX;
        else if (envIs("bmatrix")) kind = AR_BMATRIX;
        else if (envIs("vmatrix")) kind = AR_VMATRIX;
        else if (envIs("Vmatrix")) kind = AR_VVMATRIX;
        else if (envIs("Bmatrix")) kind = AR_BRACE_MATRIX;
        else if (envIs("cases")) kind = AR_CASES;
        else if (envIs("array")) kind = AR_ARRAY;
        else {
            err("environnement inconnu", env, envLen);
            return nullptr;
        }
        const char *spec = nullptr;
        int specLen = 0;
        if (kind == AR_ARRAY) readRaw(&spec, &specLen);

        enum { MAXC = 64 };
        Node *cells[MAXC];
        uint8_t rowOf[MAXC], colOf[MAXC];
        int nc = 0, row = 0, col = 0, maxCol = 0;
        for (;;) {
            Node *cell = parseList(STOP_AMP | STOP_ROW | STOP_END);
            Reason r = reason;
            if (nc < MAXC) {
                cells[nc] = cell;
                rowOf[nc] = (uint8_t)row;
                colOf[nc] = (uint8_t)col;
                nc++;
            } else {
                err("tableau trop grand", start, 8);
            }
            if (col > maxCol) maxCol = col;
            if (r == SR_AMP) col++;
            else if (r == SR_ROW) { row++; col = 0; }
            else if (r == SR_END) {
                p += 4;
                readRaw(nullptr);
                break;
            } else {
                err("\\end manquant", start, 10);
                break;
            }
        }
        // derniere ligne vide (\\ avant \end)
        if (nc > 1 && colOf[nc - 1] == 0 && cells[nc - 1]->a == nullptr && rowOf[nc - 1] > 0) nc--;
        int rows = nc ? rowOf[nc - 1] + 1 : 0;
        int cols = maxCol + 1;
        if (rows > 16) rows = 16;
        if (cols > 8) cols = 8;
        Node *arr = newNode(N_ARRAY);
        arr->kind = kind;
        arr->aux = (uint8_t)rows;
        arr->val = (int16_t)cols;
        arr->str = spec;
        arr->len = (int16_t)specLen;
        arr->cls = C_INNER;
        ListB L;
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                Node *found = nullptr;
                for (int k = 0; k < nc; k++)
                    if (rowOf[k] == r && colOf[k] == c) { found = cells[k]; break; }
                if (!found) found = group(nullptr);
                L.add(found);
            }
        }
        arr->a = L.first;
        return arr;
    }
};

Node *parse(const char *src, int len)
{
    Parser P;
    P.p = src;
    P.end = src + (len < 0 ? (int)strlen(src) : len);
    Node *g = P.parseList(0);
    return g;
}

}  // namespace math
