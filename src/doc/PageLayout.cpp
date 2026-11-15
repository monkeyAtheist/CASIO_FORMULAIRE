#include "PageLayout.hpp"
#include "Content.hpp"
#include "Index.hpp"
#include "../math/Math.hpp"
#include "../font/Font.hpp"
#include "../gfx/Color.hpp"
#include <string.h>
#ifdef TABLE_DEBUG
#include <stdio.h>
#endif

void (*g_overflowHook)(const char *src, int len, int width, int avail) = nullptr;

static void reportOverflow(const char *s, int len, int w, int avail)
{
    if (g_overflowHook) g_overflowHook(s, len, w, avail);
}

static const Theme s_normal = {
    false, SANS_14, BOLD_14, SANS_12, BOLD_14, BOLD_12,
    21, 16, 4, 6, 2, 5, 3,
};
static const Theme s_zoom = {
    true, SANS_20, BOLD_20, SANS_16, BOLD_20, BOLD_16,
    26, 20, 6, 8, 3, 7, 4,
};

const Theme &themeNormal() { return s_normal; }
const Theme &themeZoom() { return s_zoom; }

static inline int imax(int a, int b) { return a > b ? a : b; }
static inline int imin(int a, int b) { return a < b ? a : b; }

// --------------------------------------------------------------------------
// Decoupage d'une carte en elements
// --------------------------------------------------------------------------
enum ItemKind : uint8_t {
    IT_MATH, IT_ALIGN, IT_TABLE, IT_ROW, IT_TEXT, IT_NOTE, IT_LI, IT_SUB, IT_SKIP,
};

struct Item {
    const char *s;
    const char *spec;
    int16_t len;
    int8_t specLen;
    uint8_t kind;
};

enum { MAX_ITEMS = 72 };
static Item s_items[MAX_ITEMS];

static int splitCard(const char *p, const char *end)
{
    int n = 0;
    while (p < end) {
        // debut de ligne
        if (*p != '@') {
            while (p < end && *p != '\n') p++;
            if (p < end) p++;
            continue;
        }
        const char *name = p + 1;
        const char *q = name;
        while (q < end && *q >= 'a' && *q <= 'z') q++;
        int nl = (int)(q - name);
        Item it;
        it.spec = nullptr;
        it.specLen = 0;
        auto is = [&](const char *s) { return (int)strlen(s) == nl && !strncmp(s, name, nl); };
        if (is("math")) it.kind = IT_MATH;
        else if (is("align")) it.kind = IT_ALIGN;
        else if (is("table")) it.kind = IT_TABLE;
        else if (is("row")) it.kind = IT_ROW;
        else if (is("text")) it.kind = IT_TEXT;
        else if (is("note")) it.kind = IT_NOTE;
        else if (is("li")) it.kind = IT_LI;
        else if (is("sub")) it.kind = IT_SUB;
        else it.kind = IT_SKIP;
        if (q < end && *q == '[') {
            const char *sp = ++q;
            while (q < end && *q != ']' && *q != '\n') q++;
            it.spec = sp;
            it.specLen = (int8_t)(q - sp);
            if (q < end && *q == ']') q++;
        }
        while (q < end && (*q == ' ' || *q == '\t')) q++;
        // l'element continue jusqu'a la prochaine ligne commencant par @ ou %
        const char *e = q;
        for (;;) {
            while (e < end && *e != '\n') e++;
            if (e >= end) break;
            if (e + 1 >= end || e[1] == '@' || e[1] == '%') break;
            e++;
        }
        const char *te = e;
        while (te > q && (te[-1] == ' ' || te[-1] == '\n' || te[-1] == '\r' || te[-1] == '\t')) te--;
        it.s = q;
        it.len = (int16_t)(te - q);
        if (it.kind != IT_SKIP && n < MAX_ITEMS) s_items[n++] = it;
        p = e < end ? e + 1 : end;
    }
    return n;
}

// Decoupe une ligne LaTeX sur les & de premier niveau.
static int splitCells(const char *s, int len, const char **cs, int *cl, int max)
{
    int n = 0, depth = 0, env = 0;
    const char *e = s + len, *start = s;
    for (const char *p = s; p < e; p++) {
        if (*p == '\\') {
            if (p + 6 <= e && !strncmp(p, "\\begin", 6)) env++;
            else if (p + 4 <= e && !strncmp(p, "\\end", 4)) env--;
            p++;
            continue;
        }
        if (*p == '{') depth++;
        else if (*p == '}') depth--;
        else if (*p == '&' && depth == 0 && env == 0) {
            if (n < max) { cs[n] = start; cl[n] = (int)(p - start); n++; }
            start = p + 1;
        }
    }
    if (n < max) { cs[n] = start; cl[n] = (int)(e - start); n++; }
    return n;
}

// --------------------------------------------------------------------------
// Constructeur
// --------------------------------------------------------------------------
struct Builder {
    DisplayList &dl;
    const Theme &th;
    uint16_t accent;
    int innerX, innerW;
    int maxRight = 0;
    int overflows = 0;
    int errors = 0;

    Builder(DisplayList &d, const Theme &t, uint16_t a) : dl(d), th(t), accent(a) {}

    void begin() { math::g_oom = false; }
    void end()
    {
        if (math::g_oom) errors++;
        math::resetBoxes();
    }

    void noteRight(int x)
    {
        if (x > maxRight) maxRight = x;
    }

    int text(int face, int x, int base, const char *s, int len, uint16_t c)
    {
        const char *e = s + len;
        while (s < e) {
            uint32_t cp = font::utf8Next(s, e);
            if (cp == '\'') cp = 0x2019;   // apostrophe typographique
            int gi = font::find(face, cp);
            if (gi < 0) {
                font::g_missing++;
                font::g_lastMissing = cp;
                gi = font::find(face, '?');
                if (gi < 0) continue;
            }
            dl.glyph(face, gi, x, base, c);
            x += font::at(face, gi).adv;
        }
        return x;
    }

    // ---- formule centree
    // Decoupe une formule sur les \qquad de premier niveau. Renvoie le nombre
    // de morceaux (debuts/longueurs dans ps/pl).
    static int splitQquad(const char *s, int len, const char **ps, int *pl, int max)
    {
        int n = 0, depth = 0, env = 0;
        const char *e = s + len, *start = s;
        for (const char *p = s; p < e; p++) {
            if (*p == '{') { depth++; continue; }
            if (*p == '}') { depth--; continue; }
            if (*p != '\\') continue;
            if (p + 6 <= e && !strncmp(p, "\\begin", 6)) env++;
            else if (p + 4 <= e && !strncmp(p, "\\end", 4)) env--;
            else if (depth == 0 && env == 0 && p + 7 <= e && !strncmp(p, "\\qquad", 6) &&
                     !((p[6] >= 'a' && p[6] <= 'z') || (p[6] >= 'A' && p[6] <= 'Z'))) {
                if (n < max - 1) {
                    ps[n] = start;
                    pl[n] = (int)(p - start);
                    n++;
                    start = p + 6;
                }
                p += 5;
                continue;
            }
            p++;
        }
        ps[n] = start;
        pl[n] = (int)(e - start);
        return n + 1;
    }

    // ---- formule centree (coupee aux \qquad si elle est trop large)
    int itemMath(const Item &it, int y)
    {
        begin();
        int e0 = math::g_errors;
        math::Box *b = math::typeset(it.s, it.len, math::ST_D, th.zoom);
        int top = y + 2;
        int h;
        enum { MAXP = 8 };
        const char *ps[MAXP];
        int pl[MAXP];
        int np = b->w > innerW ? splitQquad(it.s, it.len, ps, pl, MAXP) : 1;
        if (np <= 1) {
            int x = innerX + (innerW - b->w) / 2;
            if (b->w > innerW) { x = innerX; overflows++; reportOverflow(it.s, it.len, b->w, innerW); }
            math::emit(dl, b, x, top + b->h, col::ink);
            noteRight(x + b->w);
            h = b->h + b->d + 4;
        } else {
            // morceaux places en lignes, regroupes tant qu'ils tiennent
            math::Box *part[MAXP];
            for (int k = 0; k < np; k++) part[k] = math::typeset(ps[k], pl[k], math::ST_D, th.zoom);
            int gap = 2 * g_faces[math::mathFace(math::ST_D, th.zoom)].px;
            int yy = top;
            int k = 0;
            while (k < np) {
                int j = k + 1, w = part[k]->w;
                while (j < np && w + gap + part[j]->w <= innerW) w += gap + part[j]->w, j++;
                int hh = 0, dd = 0;
                for (int m = k; m < j; m++) {
                    hh = imax(hh, part[m]->h);
                    dd = imax(dd, part[m]->d);
                }
                int x = innerX + (innerW - w) / 2;
                if (w > innerW) { x = innerX; overflows++; reportOverflow(ps[k], pl[k], w, innerW); }
                for (int m = k; m < j; m++) {
                    math::emit(dl, part[m], x, yy + hh, col::ink);
                    x += part[m]->w + gap;
                }
                noteRight(x - gap);
                yy += hh + dd + (j < np ? th.lineGap + 2 : 0);
                k = j;
            }
            h = yy - top + 4;
        }
        if (math::g_errors != e0) errors++;
        end();
        return h;
    }

    // ---- lignes alignees (groupe d'elements @align consecutifs)
    int itemAlign(const Item *its, int n, int y)
    {
        enum { MAXL = 12, MAXK = 6 };
        if (n > MAXL) n = MAXL;
        math::Box *cell[MAXL][MAXK];
        int ncell[MAXL];
        int colW[MAXK] = {0};
        int maxCells = 1;
        begin();
        int e0 = math::g_errors;
        for (int l = 0; l < n; l++) {
            const char *cs[MAXK];
            int cl[MAXK];
            int k = splitCells(its[l].s, its[l].len, cs, cl, MAXK);
            ncell[l] = k;
            maxCells = imax(maxCells, k);
            for (int j = 0; j < k; j++) {
                cell[l][j] = math::typesetAligned(cs[j], cl[j], math::ST_D, th.zoom, (j & 1) != 0);
                if (k > 1) colW[j] = imax(colW[j], cell[l][j]->w);
            }
        }
        int px = g_faces[math::mathFace(math::ST_D, th.zoom)].px;
        int pairGap = px;
        // largeur totale
        int W = 0;
        for (int j = 0; j < maxCells; j++) {
            W += colW[j];
            if (j > 0 && (j & 1) == 0) W += pairGap;
        }
        for (int l = 0; l < n; l++)
            if (ncell[l] == 1) W = imax(W, cell[l][0]->w);
        int x0 = innerX + (innerW - W) / 2;
        if (W > innerW) { x0 = innerX; overflows++; reportOverflow(its[0].s, its[0].len, W, innerW); }
        int yy = y + 2;
        for (int l = 0; l < n; l++) {
            int h = 0, d = 0;
            for (int j = 0; j < ncell[l]; j++) {
                h = imax(h, cell[l][j]->h);
                d = imax(d, cell[l][j]->d);
            }
            int base = yy + h;
            if (ncell[l] == 1) {
                math::Box *b = cell[l][0];
                // premiere ligne d'un groupe aligne : a gauche (formule coupee)
                int x = (l == 0 && maxCells > 1) ? x0 : x0 + (W - b->w) / 2;
                math::emit(dl, b, x, base, col::ink);
                noteRight(x + b->w);
            } else {
                int x = x0;
                for (int j = 0; j < ncell[l]; j++) {
                    math::Box *b = cell[l][j];
                    if (j > 0 && (j & 1) == 0) x += pairGap;
                    int bx = (j & 1) ? x : x + colW[j] - b->w;
                    math::emit(dl, b, bx, base, col::ink);
                    noteRight(bx + b->w);
                    x += colW[j];
                }
            }
            yy = base + d + (l + 1 < n ? th.lineGap + 2 : 0);
        }
        if (math::g_errors != e0) errors++;
        end();
        return yy - y + 2;
    }

    // ---- tableau
    int itemTable(const Item *its, int n, int y)
    {
        enum { MAXR = 24, MAXK = 6 };
        if (n > MAXR) n = MAXR;
        math::Box *cell[MAXR][MAXK];
        int ncell[MAXR];
        int colW[MAXK] = {0}, rowH[MAXR], rowD[MAXR];
        int cols = 1;
        begin();
        int e0 = math::g_errors;
        for (int r = 0; r < n; r++) {
            const char *cs[MAXK];
            int cl[MAXK];
            int k = splitCells(its[r].s, its[r].len, cs, cl, MAXK);
            ncell[r] = k;
            cols = imax(cols, k);
            rowH[r] = 0;
            rowD[r] = 0;
            for (int j = 0; j < k; j++) {
                math::Box *b = math::typeset(cs[j], cl[j], r == 0 ? math::ST_T : math::ST_D, th.zoom);
                cell[r][j] = b;
                colW[j] = imax(colW[j], b->w + 2 * th.cellPadX);
                rowH[r] = imax(rowH[r], b->h);
                rowD[r] = imax(rowD[r], b->d);
            }
        }
        int W = 0;
        for (int j = 0; j < cols; j++) W += colW[j];
        if (W < innerW) {
            int extra = innerW - W;
            for (int j = 0; j < cols; j++) {
                int add = extra / (cols - j);
                colW[j] += add;
                extra -= add;
            }
            W = innerW;
        } else if (W > innerW) {
            overflows++;
            reportOverflow(its[0].s, its[0].len, W, innerW);
#ifdef TABLE_DEBUG
            for (int r = 0; r < n; r++) {
                printf("      ");
                for (int j = 0; j < ncell[r]; j++) printf("%4d", cell[r][j]->w + 2 * th.cellPadX);
                printf("   %.*s\n", its[r].len > 60 ? 60 : its[r].len, its[r].s);
            }
#endif
        }
        int x0 = innerX;
        const char *spec = its[0].spec;
        int specLen = its[0].specLen;
        uint16_t head = mix16(col::white, accent, 3);
        uint16_t grid = mix16(col::line, accent, 4);
        int yy = y + 2;
        int top = yy;
        for (int r = 0; r < n; r++) {
            int rh = rowH[r] + rowD[r] + 2 * th.cellPadY;
            if (r == 0) dl.rect(x0, yy, W, rh, head);
            int base = yy + th.cellPadY + rowH[r];
            int x = x0;
            for (int j = 0; j < ncell[r]; j++) {
                math::Box *b = cell[r][j];
                char al = (spec && j < specLen) ? spec[j] : 'c';
                int bx;
                if (al == 'l') bx = x + th.cellPadX;
                else if (al == 'r') bx = x + colW[j] - th.cellPadX - b->w;
                else bx = x + (colW[j] - b->w) / 2;
                math::emit(dl, b, bx, base, col::ink);
                x += colW[j];
            }
            yy += rh;
            if (r + 1 < n) dl.rect(x0, yy, W, 1, r == 0 ? accent : grid);
            if (r + 1 < n) yy += 1;
        }
        // cadre et separateurs de colonnes
        dl.rect(x0, top, W, 1, grid);
        dl.rect(x0, yy, W, 1, grid);
        dl.rect(x0, top, 1, yy - top + 1, grid);
        dl.rect(x0 + W - 1, top, 1, yy - top + 1, grid);
        int x = x0;
        for (int j = 0; j + 1 < cols; j++) {
            x += colW[j];
            dl.rect(x, top, 1, yy - top, grid);
        }
        noteRight(x0 + W);
        if (math::g_errors != e0) errors++;
        end();
        return yy + 1 - y + 2;
    }

    // ---- paragraphe (texte + formules en ligne)
    struct Unit {
        const char *s;
        math::Box *box;
        int16_t len, w, h, d;
        uint8_t face, glue;   // glue : colle a l'unite precedente (pas d'espace)
    };

    int itemText(const Item &it, int y, int face, int boldFace, uint16_t color,
                 bool bullet, uint8_t mathStyle)
    {
        enum { MAXU = 200 };
        static Unit u[MAXU];
        int nu = 0;
        begin();
        int e0 = math::g_errors;
        const char *p = it.s, *e = it.s + it.len;
        bool bold = false;
        bool space = false;
        while (p < e && nu < MAXU) {
            char c = *p;
            if (c == ' ' || c == '\n' || c == '\t' || c == '\r') { space = true; p++; continue; }
            if (c == '*' && p + 1 < e && p[1] == '*') { bold = !bold; p += 2; continue; }
            Unit &un = u[nu];
            un.glue = (nu > 0 && !space) ? 1 : 0;
            space = false;
            if (c == '$') {
                const char *s = ++p;
                while (p < e && !(*p == '$' && p[-1] != '\\')) p++;
                math::Box *b = math::typeset(s, (int)(p - s), mathStyle, th.zoom);
                if (p < e) p++;
                un.box = b;
                un.s = nullptr;
                un.len = 0;
                un.w = b->w;
                un.h = b->h;
                un.d = b->d;
                un.face = 0;
                nu++;
                continue;
            }
            const char *s = p;
            while (p < e && *p != ' ' && *p != '\n' && *p != '\t' && *p != '$' &&
                   !(*p == '*' && p + 1 < e && p[1] == '*'))
                p++;
            int f = bold ? boldFace : face;
            un.box = nullptr;
            un.s = s;
            un.len = (int16_t)(p - s);
            un.w = (int16_t)font::width(f, s, (int)(p - s));
            un.h = (int16_t)font::ascent(f);
            un.d = (int16_t)font::descent(f);
            un.face = (uint8_t)f;
            nu++;
        }
        int spaceW = font::width(face, " ");
        int indent = 0;
        int bulletW = 0;
        if (bullet) {
            bulletW = font::width(face, "\xE2\x80\xA2");
            indent = bulletW + spaceW + 2;
        }
        int x0 = innerX + indent, w = innerW - indent;
        int yy = y + 1;
        int i = 0;
        bool first = true;
        while (i < nu) {
            // une ligne : groupes d'unites collees
            int j = i, lineW = 0;
            while (j < nu) {
                int k = j + 1, gw = u[j].w;
                while (k < nu && u[k].glue) gw += u[k].w, k++;
                int add = (j > i ? spaceW : 0) + gw;
                if (lineW + add > w && j > i) break;
                lineW += add;
                j = k;
            }
            int asc = font::ascent(face), desc = font::descent(face);
            for (int k = i; k < j; k++) {
                asc = imax(asc, u[k].h);
                desc = imax(desc, u[k].d);
            }
            int base = yy + asc;
            if (first && bullet)
                text(face, innerX + 2, base, "\xE2\x80\xA2", 3, accent);
            int x = x0;
            for (int k = i; k < j; k++) {
                if (k > i && !u[k].glue) x += spaceW;
                if (u[k].box) math::emit(dl, u[k].box, x, base, color);
                else text(u[k].face, x, base, u[k].s, u[k].len, color);
                x += u[k].w;
            }
            if (lineW > w) { overflows++; reportOverflow(it.s, it.len, lineW, w); }
            noteRight(x);
            yy = base + desc + th.lineGap - 1;
            i = j;
            first = false;
        }
        if (math::g_errors != e0) errors++;
        end();
        return yy - y + 1;
    }

    int itemSub(const Item &it, int y)
    {
        int f = th.sub;
        int base = y + 2 + font::ascent(f);
        text(f, innerX, base, it.s, it.len, accent);
        return font::ascent(f) + font::descent(f) + 3;
    }

    // ---- contenu d'une carte (elements) : renvoie la hauteur
    int cardBody(int card, int y)
    {
        const CardRef &c = g_index.cards[card];
        int n = splitCard(c.begin, c.end);
        int y0 = y;
        int i = 0;
        while (i < n) {
            const Item &it = s_items[i];
            int h = 0;
            switch (it.kind) {
            case IT_MATH:
                h = itemMath(it, y);
                i++;
                break;
            case IT_ALIGN: {
                int j = i + 1;
                while (j < n && s_items[j].kind == IT_ALIGN) j++;
                h = itemAlign(&s_items[i], j - i, y);
                i = j;
                break;
            }
            case IT_TABLE:
            case IT_ROW: {
                int j = i + 1;
                while (j < n && s_items[j].kind == IT_ROW) j++;
                h = itemTable(&s_items[i], j - i, y);
                i = j;
                break;
            }
            case IT_TEXT:
                h = itemText(it, y, th.prose, th.bold, col::ink, false, math::ST_T);
                i++;
                break;
            case IT_NOTE:
                h = itemText(it, y, th.note, th.note, col::note, false, math::ST_T | math::ST_SMALL);
                i++;
                break;
            case IT_LI:
                h = itemText(it, y, th.prose, th.bold, col::ink, true, math::ST_T);
                i++;
                break;
            case IT_SUB:
                h = itemSub(it, y);
                i++;
                break;
            default:
                i++;
                break;
            }
            y += h + th.itemGap;
        }
        return y - y0 - (n ? th.itemGap : 0);
    }
};

static uint16_t chapterColorOfCard(int card)
{
    return g_chapters[g_index.chapterOfCard(card)].color;
}

void layoutPage(DisplayList &dl, int page, PageLayout &out)
{
    const Theme &th = themeNormal();
    const PageRef &pg = g_index.pages[page];
    uint16_t accent = g_chapters[pg.chapter].color;
    Builder B(dl, th, accent);
    dl.clear();
    out = PageLayout();
    int y = 5;
    const int cx = PAGE_CARD_X, cw = PAGE_CARD_W;
    B.innerX = cx + 9;
    B.innerW = cw - 9 - 6;
    for (int k = 0; k < pg.cardCount && k < MAX_PAGE_CARDS; k++) {
        int card = pg.firstCard + k;
        const CardRef &c = g_index.cards[card];
        int top = y;
        int bgOp = dl.n;
        dl.rrect(cx, top, cw, 10, 3, col::white, true, col::line, true);
        int barOp = dl.n;
        dl.rect(cx + 1, top + 3, 3, 4, accent);
        // titre
        int base = top + th.titleBase;
        B.text(th.title, B.innerX, base, c.title, c.titleLen, accent);
        y = top + th.titleH;
        int h = B.cardBody(card, y);
        y += h + 5;
        int ch = y - top;
        if (bgOp < dl.n) dl.ops[bgOp].b = (uint16_t)ch;
        if (barOp < dl.n) dl.ops[barOp].b = (uint16_t)(ch - 6);
        out.cards[out.cardCount].top = (int16_t)top;
        out.cards[out.cardCount].bottom = (int16_t)y;
        out.cards[out.cardCount].card = (int16_t)card;
        out.cardCount++;
        y += th.cardGap;
    }
    out.height = y;
    out.width = PAGE_CARD_X + PAGE_CARD_W;
    out.overflows = B.overflows;
    out.errors = B.errors;
}

void layoutCardZoom(DisplayList &dl, int card, PageLayout &out)
{
    const Theme &th = themeZoom();
    uint16_t accent = chapterColorOfCard(card);
    Builder B(dl, th, accent);
    dl.clear();
    out = PageLayout();
    B.innerX = 8;
    B.innerW = SCREEN_W - 16;
    int y = 6;
    int h = B.cardBody(card, y);
    y += h + 8;
    out.height = y;
    out.width = imax(B.maxRight + 8, SCREEN_W);
    out.cardCount = 1;
    out.cards[0].top = 0;
    out.cards[0].bottom = (int16_t)y;
    out.cards[0].card = (int16_t)card;
    out.overflows = B.overflows;
    out.errors = B.errors;
}

int layoutFormulaAt(DisplayList &dl, const char *tex, int cx, int cy, uint16_t color,
                    uint8_t style, bool zoom)
{
    math::g_oom = false;
    math::Box *b = math::typeset(tex, -1, style, zoom);
    int x = cx - b->w / 2;
    int base = cy + (b->h - b->d) / 2;
    math::emit(dl, b, x, base, color);
    int w = b->w;
    math::resetBoxes();
    return w;
}
