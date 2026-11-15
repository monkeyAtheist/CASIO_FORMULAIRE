// Mise en page des formules, d'apres les regles de TeX (annexe G du TeXbook)
// et de la table MATH OpenType (constantes de la police DejaVu Math TeX Gyre).
// Toutes les grandeurs sont en pixels entiers ; y vers le bas pour les
// positions, hauteur (h) au-dessus et profondeur (d) au-dessous de la ligne
// de base.
#include <string.h>
#include "Math.hpp"
#include "MathInternal.hpp"
#include "Symbols.hpp"
#include "../font/Font.hpp"

namespace math {

// --------------------------------------------------------------------------
// Zone memoire des boites
// --------------------------------------------------------------------------
enum { BOX_ARENA = 40 * 1024 };
alignas(8) static uint8_t s_boxMem[BOX_ARENA];
static int s_boxUsed = 0;
static int s_boxPeak = 0;
static Box s_dummyBox;

Box *newBox(uint8_t kind)
{
    const int sz = (int)((sizeof(Box) + 7) & ~7u);
    if (s_boxUsed + sz > BOX_ARENA) {
        g_oom = true;
        memset(&s_dummyBox, 0, sizeof(Box));
        s_dummyBox.kind = kind;
        s_dummyBox.acc = -1;
        return &s_dummyBox;
    }
    Box *b = (Box *)(s_boxMem + s_boxUsed);
    s_boxUsed += sz;
    if (s_boxUsed > s_boxPeak) s_boxPeak = s_boxUsed;
    memset(b, 0, sizeof(Box));
    b->kind = kind;
    b->acc = -1;
    return b;
}

bool isDummy(const Box *b) { return b == &s_dummyBox; }
void resetBoxes() { s_boxUsed = 0; }
int boxBytesPeak() { return s_boxPeak; }

// --------------------------------------------------------------------------
// Contexte
// --------------------------------------------------------------------------
struct Ctx {
    uint8_t style;
    bool cramped;
    bool zoom;
    bool small;   // tailles reduites d'un cran (formules dans les remarques)

    int face() const
    {
        int i = style <= ST_T ? 0 : style == ST_S ? 1 : 2;
        if (small && i < 2) i++;
        return (zoom ? 3 : 0) + i;
    }
    const MathConsts &mc() const { return g_mathConsts[face()]; }
    int px() const { return g_faces[face()].px; }
    bool script() const { return style >= ST_S; }
    Ctx sup() const
    {
        Ctx c = *this;
        c.style = style <= ST_T ? ST_S : ST_SS;
        return c;
    }
    Ctx sub() const
    {
        Ctx c = sup();
        c.cramped = true;
        return c;
    }
    Ctx num() const
    {
        Ctx c = *this;
        c.style = style == ST_D ? ST_T : style == ST_T ? ST_S : ST_SS;
        return c;
    }
    Ctx den() const
    {
        Ctx c = num();
        c.cramped = true;
        return c;
    }
    Ctx cramp() const
    {
        Ctx c = *this;
        c.cramped = true;
        return c;
    }
};

int mathFace(uint8_t style, bool zoom)
{
    Ctx c{(uint8_t)(style & 3), false, zoom, (style & ST_SMALL) != 0};
    return c.face();
}

static inline int imax(int a, int b) { return a > b ? a : b; }
static inline int imin(int a, int b) { return a < b ? a : b; }

// --------------------------------------------------------------------------
// Construction de boites
// --------------------------------------------------------------------------
static void addChild(Box *parent, Box *child, int x, int y)
{
    if (!child || isDummy(child) || isDummy(parent)) return;
    child->x = (int16_t)x;
    child->y = (int16_t)y;
    child->next = parent->first;
    parent->first = child;
}

static void computeHD(Box *b)
{
    int h = 0, d = 0;
    for (Box *c = b->first; c; c = c->next) {
        h = imax(h, c->h - c->y);
        d = imax(d, c->d + c->y);
    }
    b->h = (int16_t)h;
    b->d = (int16_t)d;
}

struct HB {
    Box *b;
    int x = 0;
    HB() { b = newBox(B_LIST); }
    void put(Box *c, int dy = 0)
    {
        addChild(b, c, x, dy);
        x += c->w;
    }
    void kern(int k) { x += k; }
    Box *done()
    {
        b->w = (int16_t)x;
        computeHD(b);
        return b;
    }
};

static Box *kernBox(int w)
{
    Box *b = newBox(B_LIST);
    b->w = (int16_t)w;
    return b;
}

static Box *glyphBox(int face, int gi)
{
    Box *b = newBox(B_GLYPH);
    if (gi < 0) return b;
    const Glyph &g = font::at(face, gi);
    b->face = (uint8_t)face;
    b->gidx = (uint16_t)gi;
    b->w = g.adv;
    b->h = g.asc;
    b->d = g.desc;
    b->ic = g.ic;
    b->acc = g.acc == -128 ? -1 : g.acc;
    b->isChar = 1;
    return b;
}

// Boite a la taille exacte du dessin (pour les morceaux et les radicaux).
static Box *glyphBoxInk(int face, int gi)
{
    Box *b = glyphBox(face, gi);
    if (gi < 0) return b;
    const Glyph &g = font::at(face, gi);
    b->h = g.top;
    b->d = (int16_t)(g.h - g.top);
    return b;
}

static Box *ruleBox(int w, int h, int d)
{
    Box *b = newBox(B_RULE);
    b->w = (int16_t)w;
    b->h = (int16_t)h;
    b->d = (int16_t)d;
    return b;
}

// Decale verticalement une boite (dy > 0 : vers le bas).
static Box *shifted(Box *in, int dy)
{
    if (dy == 0) return in;
    Box *b = newBox(B_LIST);
    addChild(b, in, 0, dy);
    b->w = in->w;
    b->ic = in->ic;
    b->h = (int16_t)(in->h - dy);
    b->d = (int16_t)(in->d + dy);
    return b;
}

static void hexOf(uint32_t v, char *out)
{
    static const char hx[] = "0123456789ABCDEF";
    int n = 0;
    char tmp[8];
    do { tmp[n++] = hx[v & 15]; v >>= 4; } while (v && n < 8);
    for (int i = 0; i < n; i++) out[i] = tmp[n - 1 - i];
    out[n] = 0;
}

static int glyphIndex(int face, uint32_t cp)
{
    int gi = font::find(face, cp);
    if (gi < 0) {
        char buf[12];
        hexOf(cp, buf);
        reportError("glyphe absent U+", buf, (int)strlen(buf));
        font::g_missing++;
        gi = font::find(face, '?');
    }
    return gi;
}

static uint32_t mapCp(uint32_t cp, uint8_t fv)
{
    bool up = cp >= 'A' && cp <= 'Z';
    bool lo = cp >= 'a' && cp <= 'z';
    if (up || lo) {
        int i = up ? (int)(cp - 'A') : (int)(cp - 'a');
        switch (fv) {
        case FV_AUTO:
        case FV_IT:
            if (lo && cp == 'h') return 0x210E;
            return up ? 0x1D434 + i : 0x1D44E + i;
        case FV_BF:
            return up ? 0x1D400 + i : 0x1D41A + i;
        case FV_BB:
            if (!up) return cp;
            switch (cp) {
            case 'C': return 0x2102;
            case 'H': return 0x210D;
            case 'N': return 0x2115;
            case 'P': return 0x2119;
            case 'Q': return 0x211A;
            case 'R': return 0x211D;
            case 'Z': return 0x2124;
            default: return 0x1D538 + i;
            }
        case FV_CAL:
            if (!up) return 0x1D44E + i;
            switch (cp) {
            case 'B': return 0x212C;
            case 'E': return 0x2130;
            case 'F': return 0x2131;
            case 'H': return 0x210B;
            case 'I': return 0x2110;
            case 'L': return 0x2112;
            case 'M': return 0x2133;
            case 'R': return 0x211B;
            default: return 0x1D49C + i;
            }
        default:
            return cp;
        }
    }
    // grec italique -> droit avec \mathrm
    if (fv == FV_RM && cp >= 0x1D6FC && cp <= 0x1D714) return 0x3B1 + (cp - 0x1D6FC);
    return cp;
}

static Box *charBox(const Ctx &c, uint32_t cp)
{
    int face = c.face();
    return glyphBox(face, glyphIndex(face, cp));
}

// Texte droit (noms de fonctions, \text).
static Box *textRun(const char *s, int len, const Ctx &c)
{
    HB hb;
    const char *e = s + len;
    while (s < e) {
        if (*s == '\\' && s + 1 < e) s++;
        uint32_t cp = font::utf8Next(s, e);
        if (cp == '\n' || cp == '\t') cp = ' ';
        if (cp == '\'') cp = 0x2019;
        hb.put(charBox(c, cp));
    }
    Box *b = hb.done();
    return b;
}

// --------------------------------------------------------------------------
// Symboles extensibles
// --------------------------------------------------------------------------
static const VariantSet *findSet(uint32_t cp, bool vertical)
{
    for (int i = 0; i < VARIANT_SET_COUNT; i++) {
        const VariantSet &v = g_variantSets[i];
        if (v.base == cp && (v.vertical != 0) == vertical) return &v;
    }
    return nullptr;
}

// Assemblage vertical de hauteur H (ligne de base en bas de la boite).
static Box *assembleV(const VariantSet *set, int H, int face)
{
    int n = set->nparts;
    int gb = font::find(face, set->parts[0].key);
    int gt = font::find(face, set->parts[n - 1].key);
    int gm = -1, ge = -1;
    for (int i = 0; i < n; i++) {
        if (set->parts[i].ext) { if (ge < 0) ge = font::find(face, set->parts[i].key); }
        else if (i != 0 && i != n - 1) gm = font::find(face, set->parts[i].key);
    }
    if (gb < 0 || gt < 0 || ge < 0) return kernBox(0);
    const Glyph &B = font::at(face, gb), &T = font::at(face, gt), &E = font::at(face, ge);
    int hb = B.h, ht = T.h, hm = gm >= 0 ? font::at(face, gm).h : 0;
    if (H < hb + ht + hm) H = hb + ht + hm;
    Box *b = newBox(B_LIST);
    int w = imax(imax(B.adv, T.adv), E.adv);
    // haut
    addChild(b, glyphBox(face, gt), 0, -H + T.top);
    // bas
    addChild(b, glyphBox(face, gb), 0, -hb + B.top);
    int m0 = H;   // pas de milieu
    if (gm >= 0) {
        const Glyph &M = font::at(face, gm);
        m0 = (H - hm) / 2;
        addChild(b, glyphBox(face, gm), 0, -H + m0 + M.top);
    }
    // remplissages
    auto ext = [&](int r0, int r1) {
        if (r1 <= r0) return;
        Box *e = newBox(B_VEXT);
        e->face = (uint8_t)face;
        e->gidx = (uint16_t)ge;
        e->count = (int16_t)(r1 - r0);
        e->h = 0;
        e->d = (int16_t)(r1 - r0);
        addChild(b, e, 0, -H + r0);
    };
    if (gm >= 0) {
        ext(ht, m0);
        ext(m0 + hm, H - hb);
    } else {
        ext(ht, H - hb);
    }
    b->w = (int16_t)w;
    b->h = (int16_t)H;
    b->d = 0;
    return b;
}

// Assemblage horizontal de largeur W (fleches).
static Box *assembleH(const VariantSet *set, int W, int face)
{
    int n = set->nparts;
    int gl = font::find(face, set->parts[0].key);
    int gr = font::find(face, set->parts[n - 1].key);
    int ge = -1;
    for (int i = 0; i < n; i++)
        if (set->parts[i].ext) { ge = font::find(face, set->parts[i].key); break; }
    if (gl < 0 || gr < 0 || ge < 0) return kernBox(W);
    const Glyph &L = font::at(face, gl), &R = font::at(face, gr);
    int wl = L.w, wr = R.w;
    if (W < wl + wr) W = wl + wr;
    Box *b = newBox(B_LIST);
    addChild(b, glyphBox(face, gl), -L.left, 0);
    addChild(b, glyphBox(face, gr), W - (R.left + R.w), 0);
    if (W - wl - wr > 0) {
        Box *e = newBox(B_HEXT);
        e->face = (uint8_t)face;
        e->gidx = (uint16_t)ge;
        e->count = (int16_t)(W - wl - wr);
        addChild(b, e, wl, 0);
    }
    b->w = (int16_t)W;
    b->h = (int16_t)imax(L.top, R.top);
    b->d = (int16_t)imax(L.h - L.top, R.h - R.top);
    return b;
}

// Symbole vertical d'au moins 'need' pixels (encre). exact : metriques = dessin.
static Box *stretchV(uint32_t cp, int need, const Ctx &c, bool exact)
{
    int face = c.face();
    const VariantSet *set = findSet(cp, true);
    if (!set) return charBox(c, cp);
    int last = -1;
    for (int i = 0; i < set->nvar; i++) {
        int gi = font::find(face, set->vars[i]);
        if (gi < 0) continue;
        last = gi;
        const Glyph &g = font::at(face, gi);
        int H = exact ? g.h : g.asc + g.desc;
        if (H >= need) return exact ? glyphBoxInk(face, gi) : glyphBox(face, gi);
    }
    if (set->nparts == 0) {
        if (last < 0) return charBox(c, cp);
        return exact ? glyphBoxInk(face, last) : glyphBox(face, last);
    }
    return assembleV(set, need, face);
}

// Encre horizontale d'une boite glyphe : [x0, x1[ par rapport au stylo.
static void inkX(const Box *b, int &x0, int &x1)
{
    if (b->kind == B_GLYPH) {
        const Glyph &g = font::at(b->face, b->gidx);
        x0 = g.left;
        x1 = g.left + g.w;
    } else {
        x0 = 0;
        x1 = b->w;
    }
}

// Symbole horizontal d'au moins W pixels d'encre.
static Box *stretchH(uint32_t cp, int W, const Ctx &c)
{
    int face = c.face();
    const VariantSet *set = findSet(cp, false);
    if (!set) return charBox(c, cp);
    int last = -1;
    for (int i = 0; i < set->nvar; i++) {
        int gi = font::find(face, set->vars[i]);
        if (gi < 0) continue;
        last = gi;
        if (font::at(face, gi).w >= W) return glyphBox(face, gi);
    }
    if (set->nparts == 0) return last >= 0 ? glyphBox(face, last) : charBox(c, cp);
    return assembleH(set, W, face);
}

// Delimiteur centre sur l'axe, de hauteur totale >= need. cp = 0 : vide.
static Box *makeDelim(uint32_t cp, int need, const Ctx &c)
{
    const MathConsts &mc = c.mc();
    if (cp == 0) return kernBox(c.zoom ? 2 : 1);
    Box *g = stretchV(cp, need, c, false);
    int shift = (g->h - g->d) / 2 - mc.axis;
    return shifted(g, shift);
}

static int delimNeed(const Box *body, const Ctx &c)
{
    const MathConsts &mc = c.mc();
    int delta = imax(body->h - mc.axis, body->d + mc.axis);
    int a = delta * 901 / 500;
    int b = 2 * delta - c.px() / 2;
    return imax(a, b);
}

// --------------------------------------------------------------------------
// Mise en page des noeuds
// --------------------------------------------------------------------------
static Box *layoutList(Node *first, Ctx ctx, bool leadingOrd);
static Box *layoutNode(Node *n, Ctx &ctx, uint8_t &cls);

static Box *layoutAny(Node *n, Ctx ctx)
{
    if (!n) return kernBox(0);
    if (n->type == N_GROUP) return layoutList(n->a, ctx, false);
    uint8_t cls;
    return layoutNode(n, ctx, cls);
}

static bool isSingleSym(Node *n, Node **out)
{
    while (n && n->type == N_GROUP && n->a && !n->a->next) n = n->a;
    if (n && n->type == N_SYM) { if (out) *out = n; return true; }
    return false;
}

static Box *layoutOpBox(Node *n, const Ctx &ctx)
{
    if (n->str) return textRun(n->str, n->len, ctx);
    const MathConsts &mc = ctx.mc();
    Box *g;
    if (ctx.style == ST_D) {
        int target = mc.dispOpMin;
        if (isIntegral(n->cp)) target = target * 3 / 2;   // plus grand : plus lisible
        g = stretchV(n->cp, target, ctx, false);
    } else {
        g = charBox(ctx, n->cp);
    }
    int shift = (g->h - g->d) / 2 - mc.axis;
    Box *b = shifted(g, shift);
    b->ic = g->ic;
    return b;
}

static Box *attachScripts(Box *base, bool isChar, Node *subN, Node *supN, const Ctx &ctx,
                          bool integral)
{
    const MathConsts &mc = ctx.mc();
    Box *sup = supN ? layoutAny(supN, ctx.sup()) : nullptr;
    Box *sub = subN ? layoutAny(subN, ctx.sub()) : nullptr;
    int u0 = 0, v0 = 0;
    if (!isChar) {
        u0 = base->h - mc.supDropMax;
        v0 = base->d + mc.subDropMin;
    }
    int u = 0, v = 0;
    if (sup) u = imax(imax(u0, ctx.cramped ? mc.supUpCramped : mc.supUp), sup->d + mc.supBottomMin);
    if (sub && !sup) {
        v = imax(imax(v0, mc.subDown), sub->h - mc.subTopMax);
    } else if (sub && sup) {
        v = imax(v0, mc.subDown);
        int gap = (u - sup->d) - (sub->h - v);
        if (gap < mc.subSupGap) {
            v += mc.subSupGap - gap;
            int psi = mc.supBottomMaxSub - (u - sup->d);
            if (psi > 0) { u += psi; v -= psi; }
        }
    }
    Box *b = newBox(B_LIST);
    addChild(b, base, 0, 0);
    int xsup = base->w + base->ic;
    int xsub = base->w - (isChar ? ctx.px() / 16 : 0);   // indice un peu rapproche
    if (integral) {
        xsup = base->w + base->ic / 2;
        xsub = base->w - base->ic / 4;
    }
    int w = base->w;
    if (sup) { addChild(b, sup, xsup, -u); w = imax(w, xsup + sup->w); }
    if (sub) { addChild(b, sub, xsub, v); w = imax(w, xsub + sub->w); }
    b->w = (int16_t)(w + mc.spaceAfterScript);
    computeHD(b);
    return b;
}

static Box *layoutLimits(Box *op, Node *subN, Node *supN, const Ctx &ctx)
{
    const MathConsts &mc = ctx.mc();
    Box *sup = supN ? layoutAny(supN, ctx.sup()) : nullptr;
    Box *sub = subN ? layoutAny(subN, ctx.sub()) : nullptr;
    int W = op->w;
    if (sup) W = imax(W, sup->w);
    if (sub) W = imax(W, sub->w);
    int ic = op->ic;
    Box *b = newBox(B_LIST);
    addChild(b, op, (W - op->w) / 2, 0);
    if (sup) {
        int rise = imax(mc.upLimRise, mc.upLimGap + sup->d);
        addChild(b, sup, (W - sup->w) / 2 + ic / 2, -(op->h + rise));
    }
    if (sub) {
        int drop = imax(mc.lowLimDrop, mc.lowLimGap + sub->h);
        addChild(b, sub, (W - sub->w) / 2 - ic / 2, op->d + drop);
    }
    b->w = (int16_t)W;
    computeHD(b);
    // un peu d'air au-dessus et au-dessous des bornes
    if (sup) b->h = (int16_t)(b->h + mc.upLimGap / 2);
    if (sub) b->d = (int16_t)(b->d + mc.lowLimGap / 2);
    return b;
}

static Box *layoutScripts(Node *n, Ctx &ctx, uint8_t &cls)
{
    Node *base = n->a;
    cls = base ? base->cls : (uint8_t)C_ORD;
    if (base && base->type == N_OP) {
        cls = C_OP;
        bool limits = (base->flags & F_LIMITS) ||
                      (!(base->flags & F_NOLIMITS) && ctx.style == ST_D &&
                       (base->flags & F_DISPLAY_LIMITS));
        Box *op = layoutOpBox(base, ctx);
        if (limits) return layoutLimits(op, n->b, n->c, ctx);
        bool integral = !base->str && isIntegral(base->cp);
        return attachScripts(op, false, n->b, n->c, ctx, integral);
    }
    uint8_t bcls = C_ORD;
    Box *bb;
    bool isChar = false;
    if (!base) bb = kernBox(0);
    else if (base->type == N_GROUP) {
        bb = layoutList(base->a, ctx, false);
        bcls = base->cls;
    } else {
        bb = layoutNode(base, ctx, bcls);
        isChar = base->type == N_SYM;
    }
    cls = bcls;
    return attachScripts(bb, isChar, n->b, n->c, ctx, false);
}

static Box *layoutFrac(Node *n, const Ctx &ctx0)
{
    Ctx ctx = ctx0;
    if (n->aux == 1) ctx.style = ST_D;
    else if (n->aux == 2 && ctx.style == ST_D) ctx.style = ST_T;
    const MathConsts &mc = ctx.mc();
    bool D = ctx.style == ST_D;
    Box *num = layoutAny(n->a, ctx.num());
    Box *den = layoutAny(n->b, ctx.den());
    int pad = ctx.zoom ? 2 : 1;
    Box *b = newBox(B_LIST);
    if (!(n->flags & F_NOBAR)) {
        int theta = mc.fracRule;
        int W = imax(num->w, den->w) + 2;
        int barBottom = mc.axis - theta / 2;
        int barTop = barBottom + theta;
        int u = imax(D ? mc.fracNumUpD : mc.fracNumUp,
                     barTop + (D ? mc.fracNumGapD : mc.fracNumGap) + num->d);
        int v = imax(D ? mc.fracDenDownD : mc.fracDenDown,
                     -barBottom + (D ? mc.fracDenGapD : mc.fracDenGap) + den->h);
        addChild(b, num, pad + (W - num->w) / 2, -u);
        addChild(b, den, pad + (W - den->w) / 2, v);
        addChild(b, ruleBox(W, barTop, -barBottom), pad, 0);
        b->w = (int16_t)(W + 2 * pad);
        computeHD(b);
        return b;
    }
    // sans barre (coefficient binomial)
    int W = imax(num->w, den->w);
    int u = D ? mc.stackTopUpD : mc.stackTopUp;
    int v = D ? mc.stackBotDownD : mc.stackBotDown;
    int gapMin = D ? mc.stackGapD : mc.stackGap;
    int gap = (u - num->d) - (den->h - v);
    if (gap < gapMin) {
        u += (gapMin - gap + 1) / 2;
        v += (gapMin - gap) / 2;
    }
    addChild(b, num, (W - num->w) / 2, -u);
    addChild(b, den, (W - den->w) / 2, v);
    b->w = (int16_t)W;
    computeHD(b);
    if (n->kind == 1) {
        int need = delimNeed(b, ctx);
        HB hb;
        hb.put(makeDelim('(', need, ctx));
        hb.put(b);
        hb.put(makeDelim(')', need, ctx));
        return hb.done();
    }
    return b;
}

static Box *layoutRad(Node *n, const Ctx &ctx)
{
    const MathConsts &mc = ctx.mc();
    Box *body = layoutAny(n->b, ctx.cramp());
    bool D = ctx.style == ST_D;
    int gap = D ? mc.radGapD : mc.radGap;
    int rule = mc.radRule;
    int need = body->h + body->d + gap + rule;
    Box *rad = stretchV(0x221A, need, ctx, true);
    int H = rad->h + rad->d;
    int ruleTop = body->h + gap + rule + (H > need ? (H - need) / 2 : 0);
    // bord droit de l'encre du radical (raccord avec la barre)
    int inkRight = 0;
    if (rad->kind == B_GLYPH) {
        const Glyph &g = font::at(rad->face, rad->gidx);
        inkRight = g.left + g.w;
    } else {
        for (Box *c = rad->first; c; c = c->next) {
            if (c->kind != B_GLYPH && c->kind != B_VEXT) continue;
            const Glyph &g = font::at(c->face, c->gidx);
            inkRight = imax(inkRight, g.left + g.w);
        }
    }
    if (inkRight <= 0) inkRight = rad->w;
    Box *b = newBox(B_LIST);
    int radX = 0;
    Box *deg = nullptr;
    if (n->a && n->a->a) {
        Ctx dc = ctx;
        dc.style = ST_SS;
        deg = layoutAny(n->a, dc);
        radX = mc.radKernBefore + deg->w + mc.radKernAfter;
        int shiftDeg = 0;
        if (radX < 0) { shiftDeg = -radX; radX = 0; }
        int glyphBottom = ruleTop - H;    // en y vers le haut
        int raise = glyphBottom + H * mc.radDegreeRaise / 100;
        addChild(b, deg, mc.radKernBefore + shiftDeg, -raise);
    }
    // radical : son haut sur le haut de la barre
    addChild(b, rad, radX, rad->h - ruleTop);
    int bodyX = radX + inkRight + (ctx.zoom ? 2 : 1);
    addChild(b, body, bodyX, 0);
    int ruleX = radX + inkRight - 1;
    int ruleW = bodyX + body->w + 1 - ruleX;
    addChild(b, ruleBox(ruleW, ruleTop, -(ruleTop - rule)), ruleX, 0);
    b->w = (int16_t)(bodyX + body->w + 1);
    computeHD(b);
    b->h = (int16_t)imax(b->h, ruleTop + mc.radExtra / 2);
    return b;
}

static Box *layoutLR(Node *n, const Ctx &ctx)
{
    Box *body = layoutList(n->a ? n->a->a : nullptr, ctx, false);
    int need = delimNeed(body, ctx);
    HB hb;
    hb.put(makeDelim(n->cp, need, ctx));
    hb.put(body);
    hb.put(makeDelim(n->cp2, need, ctx));
    return hb.done();
}

static Box *layoutBigDelim(Node *n, const Ctx &ctx)
{
    static const int tenths[5] = {10, 12, 18, 24, 30};
    int need = ctx.px() * tenths[n->aux > 4 ? 4 : n->aux] / 10;
    return makeDelim(n->cp, need, ctx);
}

static Box *layoutAccent(Node *n, const Ctx &ctx)
{
    const MathConsts &mc = ctx.mc();
    Box *base = layoutAny(n->a, ctx.cramp());
    Node *single = nullptr;
    bool isSingle = isSingleSym(n->a, &single);
    int attach = (isSingle && base->kind == B_GLYPH && base->acc >= 0) ? base->acc : base->w / 2;
    Box *acc;
    int ax;
    bool wide = n->kind == AK_STRETCH_ARROW || n->kind == AK_WIDE ||
                (n->cp == 0x20D7 && !isSingle);
    if (wide) {
        int W = base->w;
        if (n->kind == AK_STRETCH_ARROW || n->cp == 0x20D7) W = base->w + (ctx.zoom ? 2 : 1);
        acc = stretchH(n->cp, W, ctx);
        int x0, x1;
        inkX(acc, x0, x1);
        ax = (base->w - (x1 - x0)) / 2 - x0;
    } else {
        acc = charBox(ctx, n->cp);
        int x0, x1;
        inkX(acc, x0, x1);
        int aa = (x0 + x1) / 2;
        if (acc->kind == B_GLYPH) {
            const Glyph &g = font::at(acc->face, acc->gidx);
            if (g.acc != -128) aa = g.acc;
        }
        ax = attach - aa;
    }
    int delta = imax(0, base->h - mc.accentBase);
    Box *b = newBox(B_LIST);
    addChild(b, base, 0, 0);
    addChild(b, acc, ax, -delta);
    b->w = base->w;
    computeHD(b);
    b->ic = base->ic;
    return b;
}

static Box *layoutOverline(Node *n, const Ctx &ctx)
{
    const MathConsts &mc = ctx.mc();
    Box *base = layoutAny(n->a, ctx.cramp());
    int gap = mc.overGap, t = mc.overRule;
    Box *b = newBox(B_LIST);
    addChild(b, base, 0, 0);
    if (n->aux == 0) {
        int top = base->h + gap + t;
        addChild(b, ruleBox(base->w, top, -(top - t)), 0, 0);
        b->w = base->w;
        computeHD(b);
        b->h = (int16_t)(b->h + mc.overExtra);
    } else {
        int bot = base->d + gap + t;
        addChild(b, ruleBox(base->w, -(bot - t), bot), 0, 0);
        b->w = base->w;
        computeHD(b);
        b->d = (int16_t)(b->d + mc.overExtra);
    }
    return b;
}

static Box *layoutStack(Node *n, const Ctx &ctx)
{
    const MathConsts &mc = ctx.mc();
    Box *base = layoutAny(n->b, ctx);
    Box *top = n->a ? layoutAny(n->a, ctx.sup()) : nullptr;
    Box *bot = n->c ? layoutAny(n->c, ctx.sub()) : nullptr;
    int W = base->w;
    if (top) W = imax(W, top->w);
    if (bot) W = imax(W, bot->w);
    int gap = imax(2, mc.upLimGap);
    // un symbole bas (comme ~) garde de la place : au moins la hauteur d'x
    int bh = imax(base->h, mc.xHeight), bd = imax(base->d, 0);
    Box *b = newBox(B_LIST);
    addChild(b, base, (W - base->w) / 2, 0);
    // placement des bornes de TeX (\underset se comporte comme \lim)
    if (top) addChild(b, top, (W - top->w) / 2, -(bh + imax(mc.upLimRise, gap + top->d)));
    if (bot) addChild(b, bot, (W - bot->w) / 2, bd + imax(mc.lowLimDrop, gap + bot->h));
    b->w = (int16_t)W;
    computeHD(b);
    return b;
}

static Box *layoutXArrow(Node *n, const Ctx &ctx)
{
    const MathConsts &mc = ctx.mc();
    Box *above = n->a ? layoutAny(n->a, ctx.sup()) : nullptr;
    Box *below = n->b ? layoutAny(n->b, ctx.sub()) : nullptr;
    int px = ctx.px();
    int W = 0;
    if (above) W = imax(W, above->w);
    if (below) W = imax(W, below->w);
    W += px / 2 + 2;
    W = imax(W, px * 3 / 2);
    Box *arrow = stretchH(n->cp, W, ctx);
    int aw = arrow->w;
    if (arrow->kind == B_GLYPH) aw = arrow->w;
    int Wt = imax(aw, W);
    int gap = imax(1, mc.upLimGap - 1);
    Box *b = newBox(B_LIST);
    addChild(b, arrow, (Wt - aw) / 2, 0);
    if (above) addChild(b, above, (Wt - above->w) / 2, -(arrow->h + gap + above->d));
    if (below) addChild(b, below, (Wt - below->w) / 2, imax(arrow->d, 0) + gap + below->h);
    b->w = (int16_t)Wt;
    computeHD(b);
    return b;
}

static Box *layoutArray(Node *n, const Ctx &ctx0)
{
    Ctx ctx = ctx0;
    if (ctx.style == ST_D) ctx.style = ST_T;
    const MathConsts &mc = ctx.mc();
    int rows = n->aux, cols = n->val;
    enum { MAXC = 64 };
    Box *cell[MAXC];
    int colW[8] = {0}, rowH[16] = {0}, rowD[16] = {0};
    int px = ctx.px();
    int strutH = (px * 7 + 5) / 10, strutD = (px * 3 + 5) / 10;
    Node *c = n->a;
    for (int r = 0; r < rows; r++) {
        rowH[r] = strutH;
        rowD[r] = strutD;
        for (int k = 0; k < cols; k++) {
            int i = r * cols + k;
            Box *bx = layoutAny(c, ctx);
            if (i < MAXC) cell[i] = bx;
            colW[k] = imax(colW[k], bx->w);
            rowH[r] = imax(rowH[r], bx->h);
            rowD[r] = imax(rowD[r], bx->d);
            if (c) c = c->next;
        }
    }
    bool cases = n->kind == AR_CASES;
    int rowGap = cases ? px / 4 : px / 5;
    int colSep = cases ? px : px * 3 / 4;
    // ecart apres chaque ligne : plus grand si une des deux lignes est haute
    int gapAfter[16];
    for (int r = 0; r < rows; r++) {
        bool tall = rowH[r] > strutH * 11 / 10 || rowD[r] > strutD * 12 / 10;
        bool tallNext = r + 1 < rows && (rowH[r + 1] > strutH * 11 / 10 || rowD[r + 1] > strutD * 12 / 10);
        gapAfter[r] = r + 1 < rows ? rowGap + ((tall || tallNext) ? px / 4 : 0) : 0;
    }
    int H = 0;
    for (int r = 0; r < rows; r++) H += rowH[r] + rowD[r] + gapAfter[r];
    int top = mc.axis + H / 2;     // y vers le haut
    Box *inner = newBox(B_LIST);
    int y = -top;
    for (int r = 0; r < rows; r++) {
        y += rowH[r];
        int x = 0;
        for (int k = 0; k < cols; k++) {
            int i = r * cols + k;
            if (i >= MAXC) break;
            Box *bx = cell[i];
            char al = 'c';
            if (cases) al = 'l';
            else if (n->kind == AR_ARRAY && n->str && k < n->len) {
                // format : on ne garde que l, c, r
                int seen = -1;
                for (int s = 0; s < n->len; s++) {
                    char ch = n->str[s];
                    if (ch == 'l' || ch == 'c' || ch == 'r') {
                        if (++seen == k) { al = ch; break; }
                    }
                }
            }
            int dx = al == 'l' ? 0 : al == 'r' ? colW[k] - bx->w : (colW[k] - bx->w) / 2;
            addChild(inner, bx, x + dx, y);
            x += colW[k] + colSep;
        }
        y += rowD[r] + gapAfter[r];
    }
    int w = 0;
    for (int k = 0; k < cols; k++) w += colW[k];
    w += (cols > 1 ? cols - 1 : 0) * colSep;
    // array{cc|c} : traits verticaux entre colonnes (matrices augmentees)
    if (n->kind == AR_ARRAY && n->str) {
        int col = -1, xb = 0;
        for (int s = 0; s < n->len; s++) {
            char ch = n->str[s];
            if (ch == 'l' || ch == 'c' || ch == 'r') {
                col++;
                if (col < cols) xb += colW[col] + (col > 0 ? colSep : 0);
            } else if (ch == '|' && col >= 0 && col + 1 < cols) {
                addChild(inner, ruleBox(1, top, H - top), xb + colSep / 2, 0);
            }
        }
    }
    inner->w = (int16_t)w;
    inner->h = (int16_t)top;
    inner->d = (int16_t)(H - top);
    uint32_t L = 0, R = 0;
    switch (n->kind) {
    case AR_PMATRIX: L = '('; R = ')'; break;
    case AR_BMATRIX: L = '['; R = ']'; break;
    case AR_VMATRIX: L = '|'; R = '|'; break;
    case AR_VVMATRIX: L = 0x2016; R = 0x2016; break;
    case AR_BRACE_MATRIX: L = '{'; R = '}'; break;
    case AR_CASES: L = '{'; break;
    default: break;
    }
    if (!L && !R) return inner;
    int need = H + 2;
    HB hb;
    int padIn = cases ? px / 4 : px / 8;
    if (L) { hb.put(makeDelim(L, need, ctx)); hb.kern(padIn); }
    hb.put(inner);
    if (R) { hb.kern(padIn); hb.put(makeDelim(R, need, ctx)); }
    return hb.done();
}

static Box *layoutBoxed(Node *n, const Ctx &ctx)
{
    Box *body = layoutAny(n->a, ctx);
    int pad = ctx.zoom ? 4 : 3;
    Box *b = newBox(B_LIST);
    addChild(b, body, pad + 1, 0);
    int W = body->w + 2 * pad + 2;
    int top = body->h + pad + 1, bot = body->d + pad + 1;
    addChild(b, ruleBox(W, top, -(top - 1)), 0, 0);
    addChild(b, ruleBox(W, -(bot - 1), bot), 0, 0);
    addChild(b, ruleBox(1, top, bot), 0, 0);
    addChild(b, ruleBox(1, top, bot), W - 1, 0);
    b->w = (int16_t)W;
    computeHD(b);
    return b;
}

static Box *layoutNode(Node *n, Ctx &ctx, uint8_t &cls)
{
    cls = n->cls;
    switch (n->type) {
    case N_SYM:
        return charBox(ctx, mapCp(n->cp, n->fv));
    case N_GROUP: {
        if ((n->flags & F_PRIME) && n->a && n->a->next) {
            // base + prime : la prime suit le haut d'une base haute
            uint8_t bc;
            Node *bn = n->a, *pn = n->a->next;
            Box *bb = bn->type == N_GROUP ? layoutList(bn->a, ctx, false) : layoutNode(bn, ctx, bc);
            Box *pb = layoutNode(pn, ctx, bc);
            int raise = imax(0, bb->h - pb->h);
            HB hb;
            hb.put(bb);
            hb.kern(bn->type == N_SYM ? 0 : 1);
            hb.put(pb, -raise);
            cls = n->cls;
            return hb.done();
        }
        Box *b = layoutList(n->a, ctx, false);
        cls = (n->flags & F_PRIME) ? n->cls : (uint8_t)C_ORD;
        return b;
    }
    case N_FRAC:
        cls = C_INNER;
        return layoutFrac(n, ctx);
    case N_SCRIPTS:
        return layoutScripts(n, ctx, cls);
    case N_OP:
        cls = C_OP;
        return layoutOpBox(n, ctx);
    case N_RAD:
        cls = C_ORD;
        return layoutRad(n, ctx);
    case N_LR:
        cls = C_INNER;
        return layoutLR(n, ctx);
    case N_BIGDELIM:
        return layoutBigDelim(n, ctx);
    case N_ACCENT:
        cls = C_ORD;
        return layoutAccent(n, ctx);
    case N_OVERLINE:
        cls = C_ORD;
        return layoutOverline(n, ctx);
    case N_ARRAY:
        cls = C_INNER;
        return layoutArray(n, ctx);
    case N_TEXT:
        cls = C_ORD;
        return textRun(n->str, n->len, ctx);
    case N_STACK:
        return layoutStack(n, ctx);
    case N_XARROW:
        cls = C_REL;
        return layoutXArrow(n, ctx);
    case N_COLOR: {
        Box *b = layoutAny(n->a, ctx);
        if (!isDummy(b)) {
            b->hasColor = 1;
            b->color = (uint16_t)n->val;
        }
        cls = C_ORD;
        return b;
    }
    case N_BOXED:
        cls = C_ORD;
        return layoutBoxed(n, ctx);
    case N_PHANTOM: {
        Box *b = layoutAny(n->a, ctx);
        if (!isDummy(b)) b->hidden = 1;
        cls = C_ORD;
        return b;
    }
    case N_SPACE: {
        cls = C_NONE;
        int px = ctx.px();
        int k = n->val >= 0 ? (n->val * px + 9) / 18 : -((-n->val * px + 9) / 18);
        return kernBox(k);
    }
    default:
        cls = C_ORD;
        return kernBox(0);
    }
}

// Table d'espacement de TeX : 1 fin, 2 moyen, 3 epais ; +8 : seulement en D/T.
static const uint8_t s_spacing[8][8] = {
    // Ord  Op    Bin    Rel    Open   Close  Punct  Inner
    {0,     1,    2 | 8, 3 | 8, 0,     0,     0,     1 | 8},  // Ord
    {1,     1,    0,     3 | 8, 0,     0,     0,     1 | 8},  // Op
    {2 | 8, 2 | 8, 0,    0,     2 | 8, 0,     0,     2 | 8},  // Bin
    {3 | 8, 3 | 8, 0,    0,     3 | 8, 0,     0,     3 | 8},  // Rel
    {0,     0,    0,     0,     0,     0,     0,     0},      // Open
    {0,     1,    2 | 8, 3 | 8, 0,     0,     0,     1 | 8},  // Close
    {1 | 8, 1 | 8, 0,    1 | 8, 1 | 8, 1 | 8, 1 | 8, 1 | 8},  // Punct
    {1 | 8, 1,    2 | 8, 3 | 8, 1 | 8, 0,     1 | 8, 1 | 8},  // Inner
};

static int spaceBetween(uint8_t a, uint8_t b, const Ctx &ctx)
{
    if (a > C_INNER || b > C_INNER) return 0;
    int s = s_spacing[a][b];
    if ((s & 8) && ctx.script()) return 0;
    s &= 7;
    if (!s) return 0;
    static const int mu[4] = {0, 3, 4, 5};
    int px = ctx.px();
    return (mu[s] * px + 9) / 18;
}

static Box *layoutList(Node *first, Ctx ctx, bool leadingOrd)
{
    // 1) boites et classes
    Ctx cur = ctx;
    for (Node *n = first; n; n = n->next) {
        if (n->type == N_STYLE) {
            cur.style = n->aux;
            cur.cramped = false;
            n->lbox = nullptr;
            n->ecls = C_NONE;
            continue;
        }
        uint8_t cls;
        n->lbox = layoutNode(n, cur, cls);
        n->ecls = cls;
        n->lstyle = cur.style;
    }
    // 2a) intervalles a la francaise : ]a,b[ ; un ']' en debut d'expression
    //     ouvre, un '[' en fin d'expression ferme.
    {
        uint8_t pc = leadingOrd ? C_ORD : C_NONE;
        for (Node *n = first; n; n = n->next) {
            if (n->ecls == C_NONE) continue;
            if (n->type == N_SYM && n->cp == ']' &&
                (pc == C_NONE || pc == C_BIN || pc == C_REL || pc == C_OPEN || pc == C_PUNCT))
                n->ecls = C_OPEN;
            if (n->type == N_SYM && n->cp == '[') {
                Node *m = n->next;
                while (m && m->ecls == C_NONE) m = m->next;
                uint8_t nc = m ? m->ecls : (uint8_t)C_NONE;
                // un '[' fermant suit toujours un operande (]0,1[ - ... ;
                // pas x\in[-1,1]).
                bool afterOperand = !(pc == C_NONE || pc == C_BIN || pc == C_REL || pc == C_OPEN ||
                                      pc == C_PUNCT || pc == C_OP);
                if (afterOperand &&
                    (nc == C_NONE || nc == C_PUNCT || nc == C_REL || nc == C_CLOSE || nc == C_BIN))
                    n->ecls = C_CLOSE;
            }
            pc = n->ecls;
        }
    }
    // 2b) Bin -> Ord
    uint8_t prev = leadingOrd ? C_ORD : C_NONE;
    Node *prevAtom = nullptr;
    for (Node *n = first; n; n = n->next) {
        if (n->ecls == C_NONE) continue;
        if (n->ecls == C_BIN &&
            (prev == C_NONE || prev == C_BIN || prev == C_OP || prev == C_REL ||
             prev == C_OPEN || prev == C_PUNCT))
            n->ecls = C_ORD;
        if ((n->ecls == C_REL || n->ecls == C_CLOSE || n->ecls == C_PUNCT) &&
            prevAtom && prevAtom->ecls == C_BIN)
            prevAtom->ecls = C_ORD;
        prev = n->ecls;
        prevAtom = n;
    }
    if (prevAtom && prevAtom->ecls == C_BIN) prevAtom->ecls = C_ORD;
    // 3) assemblage horizontal avec espaces
    HB hb;
    uint8_t last = leadingOrd ? C_ORD : C_NONE;
    for (Node *n = first; n; n = n->next) {
        if (!n->lbox) continue;
        if (n->ecls != C_NONE) {
            if (last != C_NONE) {
                Ctx sc = ctx;
                sc.style = n->lstyle;
                hb.kern(spaceBetween(last, n->ecls, sc));
            }
            last = n->ecls;
        }
        hb.put(n->lbox);
    }
    Box *b = hb.done();
    // correction d'italique et point d'attache d'un element unique
    if (first && !first->next && first->lbox) {
        b->ic = first->lbox->ic;
        b->acc = first->lbox->acc;
    }
    return b;
}

Box *layoutRoot(Node *root, uint8_t style, bool zoom, bool leadingOrd)
{
    Ctx c{(uint8_t)(style & 3), false, zoom, (style & ST_SMALL) != 0};
    return layoutList(root ? root->a : nullptr, c, leadingOrd);
}

Box *typesetAligned(const char *src, int len, uint8_t style, bool zoom, bool leadingOrd)
{
    Node *root = parse(src, len);
    Box *b = layoutRoot(root, style, zoom, leadingOrd);
    resetNodes();
    return b;
}

Box *typeset(const char *src, int len, uint8_t style, bool zoom)
{
    return typesetAligned(src, len, style, zoom, false);
}

// --------------------------------------------------------------------------
// Emission vers la liste d'affichage
// --------------------------------------------------------------------------
static void emitBox(DisplayList &dl, const Box *b, int x, int base, uint16_t color)
{
    if (b->hasColor) color = b->color;
    if (b->hidden) return;
    switch (b->kind) {
    case B_LIST:
        for (const Box *c = b->first; c; c = c->next) emitBox(dl, c, x + c->x, base + c->y, color);
        break;
    case B_GLYPH:
        dl.glyph(b->face, b->gidx, x, base, color);
        break;
    case B_RULE:
        dl.rect(x, base - b->h, b->w, b->h + b->d, color);
        break;
    case B_VEXT:
        dl.vext(b->face, b->gidx, x, base, b->count, color);
        break;
    case B_HEXT:
        dl.hext(b->face, b->gidx, x, base, b->count, color);
        break;
    default:
        break;
    }
}

void emit(DisplayList &dl, const Box *b, int x, int base, uint16_t color)
{
    if (!b || isDummy(b)) return;
    emitBox(dl, b, x, base, color);
}

}  // namespace math
