#include "App.hpp"
#include "../doc/Content.hpp"
#include "../font/Font.hpp"
#include "../gfx/Color.hpp"
#include "../math/Math.hpp"
#include <string.h>

App g_app;

enum { PAGE_OPS = 3200, ZOOM_OPS = 1400, HOME_OPS = 900 };
static DrawOp s_pageOps[PAGE_OPS];
static DrawOp s_zoomOps[ZOOM_OPS];
static DrawOp s_homeOps[HOME_OPS];
static DisplayList s_pageDL, s_zoomDL, s_homeDL;

static const uint16_t HOME_BG = rgb(34, 40, 54);
static const int STEP = 56;
// Accueil : grille de 4 colonnes ; 4 rangees visibles, defilement au-dela.
enum { TILE_W = 94, TILE_H = 43, TILE_DX = 97, TILE_DY = 46, HOME_ROWS = 4 };

static inline int imax(int a, int b) { return a > b ? a : b; }
static inline int imin(int a, int b) { return a < b ? a : b; }

// Petit entier -> texte.
static int itoa10(int v, char *out)
{
    char tmp[12];
    int n = 0;
    bool neg = v < 0;
    if (neg) v = -v;
    do { tmp[n++] = (char)('0' + v % 10); v /= 10; } while (v && n < 11);
    int k = 0;
    if (neg) out[k++] = '-';
    while (n) out[k++] = tmp[--n];
    out[k] = 0;
    return k;
}

// --------------------------------------------------------------------------
// Initialisation
// --------------------------------------------------------------------------
void App::init()
{
    s_pageDL.init(s_pageOps, PAGE_OPS);
    s_zoomDL.init(s_zoomOps, ZOOM_OPS);
    s_homeDL.init(s_homeOps, HOME_OPS);
    g_index.build();
    view = V_HOME;
    searchFrom = V_HOME;
    shift = false;
    homeSel = 0;
    homeTop = 0;
    homeCount = 0;
    for (int c = 0; c < g_chapterCount && homeCount < 32; c++)
        if (!g_chapters[c].hidden && g_index.chapterPageCount[c] > 0) homeChapters[homeCount++] = c;
    // icones des tuiles
    s_homeDL.clear();
    for (int i = 0; i < homeCount; i++) {
        const Chapter &ch = g_chapters[homeChapters[i]];
        int col = i % 4, row = i / 4;
        int tx = 4 + col * TILE_DX, ty = CONTENT_TOP + 3 + row * TILE_DY;
        tileOpStart[i] = s_homeDL.n;
        layoutFormulaAt(s_homeDL, ch.icon, tx + 47, ty + 17, ch.color, math::ST_T, false);
        tileOpEnd[i] = s_homeDL.n;
    }
    chapter = 0;
    page = -1;
    sel = 0;
    scroll = 0;
    zoomCard = -1;
    qlen = 0;
    query[0] = 0;
    nres = 0;
    rsel = 0;
    rscroll = 0;
    tocCount = 0;
    lastErrors = 0;
    lastOverflows = 0;
}

// --------------------------------------------------------------------------
// Elements communs
// --------------------------------------------------------------------------
void App::drawHeader(Canvas &cv, uint16_t bg, const char *title, int titleLen,
                     const char *sub, int subLen, const char *right)
{
    cv.fillRect(0, 0, SCREEN_W, CONTENT_TOP, bg);
    cv.fillRect(0, CONTENT_TOP - 1, SCREEN_W, 1, mix16(bg, col::black, 4));
    int rx = SCREEN_W - 4;
    if (shift) {
        int w = font::width(BOLD_12, "SHIFT") + 6;
        rx -= w;
        cv.roundRect(rx, 3, w, 14, 2, col::white, true, 0, false);
        font::drawText(cv, BOLD_12, rx + 3, 14, "SHIFT", bg);
        rx -= 4;
    }
    if (right) {
        int w = font::width(SANS_12, right);
        rx -= w;
        font::drawText(cv, SANS_12, rx, 14, right, col::white);
        rx -= 6;
    }
    char buf[96];
    int n = txt::copy(buf, sizeof(buf), title, titleLen);
    int avail = rx - 6;
    int tw = font::width(BOLD_14, buf);
    if (tw > avail || !sub) {
        font::drawTextFit(cv, BOLD_14, 6, 15, buf, col::white, avail);
        return;
    }
    font::drawText(cv, BOLD_14, 6, 15, buf, col::white);
    int x = 6 + tw;
    char sb[96];
    int k = 0;
    sb[k++] = ' ';
    k += txt::copy(sb + k, 4, "›");
    sb[k++] = ' ';
    k += txt::copy(sb + k, (int)sizeof(sb) - k, sub, subLen);
    (void)n;
    font::drawTextFit(cv, SANS_14, x, 15, sb, mix16(col::white, bg, 3), avail - tw);
}

void App::drawFKeys(Canvas &cv, const char *const labels[6])
{
    int y = CONTENT_BOTTOM;
    cv.fillRect(0, y, SCREEN_W, SCREEN_H - y, col::fbar);
    for (int i = 0; i < 6; i++) {
        if (!labels[i] || !labels[i][0]) continue;
        int x = i * 66 + 1;
        cv.roundRect(x, y + 2, 64, SCREEN_H - y - 3, 2, rgb(74, 84, 104), true, 0, false);
        int w = font::width(SANS_12, labels[i]);
        if (w > 62) font::drawTextFit(cv, SANS_12, x + 2, y + 14, labels[i], col::fbarText, 60);
        else font::drawText(cv, SANS_12, x + (64 - w) / 2, y + 14, labels[i], col::fbarText);
    }
}

static void drawScrollbar(Canvas &cv, int x, int y, int h, int total, int offset)
{
    if (total <= h) return;
    cv.fillRect(x, y, 3, h, rgb(220, 224, 232));
    int th = imax(12, h * h / total);
    int ty = y + (h - th) * offset / (total - h);
    cv.roundRect(x, ty, 3, th, 0, rgb(120, 128, 146), true, 0, false);
}

// --------------------------------------------------------------------------
// Dessin
// --------------------------------------------------------------------------
void App::draw(Canvas &cv)
{
    switch (view) {
    case V_HOME: drawHome(cv); break;
    case V_PAGE: drawPage(cv); break;
    case V_ZOOM: drawZoom(cv); break;
    case V_TOC: drawToc(cv); break;
    case V_SEARCH: drawSearch(cv); break;
    }
}

void App::drawHome(Canvas &cv)
{
    cv.fillRect(0, CONTENT_TOP, SCREEN_W, CONTENT_H, col::pageBg);
    drawHeader(cv, HOME_BG, "Formulaire de mathématiques", -1, nullptr, 0, "Graph 90+E");
    int rows = (homeCount + 3) / 4;
    for (int i = 0; i < homeCount; i++) {
        const Chapter &ch = g_chapters[homeChapters[i]];
        int col = i % 4, row = i / 4;
        if (row < homeTop || row >= homeTop + HOME_ROWS) continue;
        int dy = -homeTop * TILE_DY;   // decalage des icones precalculees
        int tx = 4 + col * TILE_DX, ty = CONTENT_TOP + 3 + row * TILE_DY + dy;
        bool on = i == homeSel;
        if (on) {
            cv.roundRect(tx, ty, TILE_W, TILE_H, 3, ch.color, true, ch.color, true);
            s_homeDL.drawColored(cv, 0, dy, tileOpStart[i], tileOpEnd[i], col::white);
        } else {
            cv.roundRect(tx, ty, TILE_W, TILE_H, 3, col::white, true, col::line, true);
            cv.fillRect(tx + 3, ty + 40, 88, 2, mix16(col::white, ch.color, 9));
            s_homeDL.draw(cv, 0, dy, tileOpStart[i], tileOpEnd[i]);
        }
        int w = font::width(on ? BOLD_12 : SANS_12, ch.label);
        int f = on ? BOLD_12 : SANS_12;
        if (w > 90) font::drawTextFit(cv, f, tx + 2, ty + 37, ch.label, on ? col::white : col::ink, 90);
        else font::drawText(cv, f, tx + (TILE_W - w) / 2, ty + 37, ch.label, on ? col::white : col::ink);
    }
    if (rows > HOME_ROWS)
        drawScrollbar(cv, SCREEN_W - 5, CONTENT_TOP + 3, HOME_ROWS * TILE_DY - 3, rows * TILE_DY - 3,
                      homeTop * TILE_DY);
    static const char *const fk[6] = {"Ouvrir", "", "", "Chercher", "Aide", "Quitter"};
    drawFKeys(cv, fk);
}

void App::drawPage(Canvas &cv)
{
    const Chapter &ch = g_chapters[chapter];
    const PageRef &pg = g_index.pages[page];
    cv.fillRect(0, CONTENT_TOP, SCREEN_W, CONTENT_H, col::pageBg);
    cv.setClip(0, CONTENT_TOP, SCREEN_W, CONTENT_BOTTOM);
    s_pageDL.draw(cv, 0, CONTENT_TOP - scroll);
    if (sel >= 0 && sel < layout.cardCount) {
        const CardBox &c = layout.cards[sel];
        int y = CONTENT_TOP + c.top - scroll;
        int h = c.bottom - c.top;
        cv.roundRect(PAGE_CARD_X, y, PAGE_CARD_W, h, 3, 0, false, ch.color, true);
        cv.frame(PAGE_CARD_X + 1, y + 1, PAGE_CARD_W - 2, h - 2, ch.color);
    }
    drawScrollbar(cv, SCREEN_W - 5, CONTENT_TOP + 2, CONTENT_H - 4, layout.height, scroll);
    cv.resetClip();
    char right[16];
    int first = g_index.chapterFirstPage[chapter];
    int k = itoa10(page - first + 1, right);
    right[k++] = '/';
    itoa10(g_index.chapterPageCount[chapter], right + k);
    drawHeader(cv, ch.color, ch.label, -1, pg.title, pg.titleLen, right);
    static const char *const fk[6] = {"Sommaire", "◀ Page", "Page ▶", "Chercher", "Zoom",
                                      "Thèmes"};
    drawFKeys(cv, fk);
}

void App::drawZoom(Canvas &cv)
{
    const CardRef &c = g_index.cards[zoomCard];
    const Chapter &ch = g_chapters[g_index.chapterOfCard(zoomCard)];
    cv.fillRect(0, CONTENT_TOP, SCREEN_W, CONTENT_H, col::white);
    cv.setClip(0, CONTENT_TOP, SCREEN_W, CONTENT_BOTTOM);
    s_zoomDL.draw(cv, -zx, CONTENT_TOP - zy);
    drawScrollbar(cv, SCREEN_W - 5, CONTENT_TOP + 2, CONTENT_H - 4, zlayout.height, zy);
    if (zlayout.width > SCREEN_W + 2) {
        int total = zlayout.width, h = SCREEN_W - 8;
        cv.fillRect(4, CONTENT_BOTTOM - 4, h, 3, rgb(220, 224, 232));
        int tw = imax(16, h * h / total);
        int tx = 4 + (h - tw) * zx / (total - SCREEN_W);
        cv.fillRect(tx, CONTENT_BOTTOM - 4, tw, 3, rgb(120, 128, 146));
    }
    cv.resetClip();
    drawHeader(cv, ch.color, c.title, c.titleLen, nullptr, 0, "Zoom");
    bool wide = zlayout.width > SCREEN_W + 2;
    static const char *const fkN[6] = {"◀ Carte", "Carte ▶", "", "", "", "Retour"};
    static const char *const fkW[6] = {"◀ Carte", "Carte ▶", "", "", "← → défile", "Retour"};
    drawFKeys(cv, wide ? fkW : fkN);
}

void App::drawToc(Canvas &cv)
{
    const Chapter &ch = g_chapters[chapter];
    cv.fillRect(0, CONTENT_TOP, SCREEN_W, CONTENT_H, col::white);
    cv.setClip(0, CONTENT_TOP, SCREEN_W, CONTENT_BOTTOM);
    const int RH = 18;
    for (int i = 0; i < tocCount; i++) {
        int y = CONTENT_TOP + 2 + i * RH - tocScroll;
        if (y + RH < CONTENT_TOP || y > CONTENT_BOTTOM) continue;
        bool on = i == tocSel;
        const TocRow &r = toc[i];
        if (on) cv.roundRect(2, y, SCREEN_W - 10, RH, 2, mix16(col::white, ch.color, 3), true, ch.color, true);
        if (r.card < 0) {
            const PageRef &pg = g_index.pages[r.page];
            char buf[96];
            txt::copy(buf, sizeof(buf), pg.title, pg.titleLen);
            font::drawTextFit(cv, BOLD_14, 8, y + 14, buf, ch.color, SCREEN_W - 24);
        } else {
            const CardRef &c = g_index.cards[r.card];
            char buf[96];
            txt::copy(buf, sizeof(buf), c.title, c.titleLen);
            font::drawText(cv, SANS_14, 20, y + 14, "•", mix16(col::white, ch.color, 10));
            font::drawTextFit(cv, SANS_14, 30, y + 14, buf, col::ink, SCREEN_W - 44);
        }
    }
    drawScrollbar(cv, SCREEN_W - 5, CONTENT_TOP + 2, CONTENT_H - 4, tocCount * RH + 4, tocScroll);
    cv.resetClip();
    drawHeader(cv, ch.color, "Sommaire", -1, ch.title, -1, nullptr);
    static const char *const fk[6] = {"Ouvrir", "", "", "", "", "Retour"};
    drawFKeys(cv, fk);
}

void App::drawSearch(Canvas &cv)
{
    cv.fillRect(0, CONTENT_TOP, SCREEN_W, CONTENT_H, col::pageBg);
    // champ de saisie
    cv.roundRect(4, CONTENT_TOP + 4, SCREEN_W - 8, 24, 3, col::white, true, rgb(120, 128, 146), true);
    int x = 10;
    if (qlen) x = font::drawText(cv, BOLD_16, 10, CONTENT_TOP + 22, query, col::ink);
    else font::drawText(cv, SANS_14, 10, CONTENT_TOP + 21, "Tapez un mot (les lettres rouges)", col::note);
    cv.fillRect(x + 1, CONTENT_TOP + 9, 2, 15, col::blue);
    const int RH = 30, top = CONTENT_TOP + 32;
    cv.setClip(0, top, SCREEN_W, CONTENT_BOTTOM);
    if (!qlen) {
        font::drawText(cv, SANS_14, 10, top + 18, "Exemples : ipp, arctan, rot, taylor,", col::inkSoft);
        font::drawText(cv, SANS_14, 10, top + 36, "matrice, cylindr, geometr, cos", col::inkSoft);
        font::drawText(cv, SANS_12, 10, top + 62, "Lettres = touches avec une lettre rouge", col::note);
        font::drawText(cv, SANS_12, 10, top + 78, "(sans appuyer sur ALPHA). [.] = espace.", col::note);
    } else if (!nres) {
        font::drawText(cv, SANS_14, 10, top + 18, "Aucun résultat", col::inkSoft);
    }
    for (int i = 0; i < nres; i++) {
        int y = top + i * RH - rscroll;
        if (y + RH < top || y > CONTENT_BOTTOM) continue;
        const CardRef &c = g_index.cards[results[i]];
        const PageRef &pg = g_index.pages[c.page];
        const Chapter &ch = g_chapters[pg.chapter];
        bool on = i == rsel;
        if (on) cv.roundRect(4, y, SCREEN_W - 12, RH - 2, 2, mix16(col::white, ch.color, 3), true, ch.color, true);
        char buf[96];
        txt::copy(buf, sizeof(buf), c.title, c.titleLen);
        font::drawTextFit(cv, on ? BOLD_14 : SANS_14, 10, y + 14, buf, col::ink, SCREEN_W - 26);
        char where[128];
        int k = txt::copy(where, sizeof(where), ch.label);
        k += txt::copy(where + k, (int)sizeof(where) - k, " › ");
        txt::copy(where + k, (int)sizeof(where) - k, pg.title, pg.titleLen);
        font::drawTextFit(cv, SANS_12, 18, y + 26, where, ch.color, SCREEN_W - 34);
    }
    drawScrollbar(cv, SCREEN_W - 5, top, CONTENT_BOTTOM - top, nres * RH, rscroll);
    cv.resetClip();
    char right[16];
    right[0] = 0;
    if (qlen) {
        int k = itoa10(nres, right);
        txt::copy(right + k, 16 - k, nres > 1 ? " cartes" : " carte");
    }
    drawHeader(cv, HOME_BG, "Recherche", -1, nullptr, 0, qlen ? right : nullptr);
    static const char *const fk[6] = {"Ouvrir", "", "", "", "Effacer", "Retour"};
    drawFKeys(cv, fk);
}

// --------------------------------------------------------------------------
// Navigation
// --------------------------------------------------------------------------
void App::openChapter(int c)
{
    if (c < 0 || c >= g_chapterCount || g_index.chapterPageCount[c] <= 0) return;
    openPage(g_index.chapterFirstPage[c], -1);
}

void App::openPage(int p, int card)
{
    if (p < 0 || p >= g_index.pageCount) return;
    page = p;
    chapter = g_index.pages[p].chapter;
    math::g_errors = 0;
    layoutPage(s_pageDL, p, layout);
    lastErrors = layout.errors;
    lastOverflows = layout.overflows;
    sel = 0;
    scroll = 0;
    if (card >= 0) {
        for (int i = 0; i < layout.cardCount; i++)
            if (layout.cards[i].card == card) { sel = i; break; }
        revealCard(sel, true);
    }
    view = V_PAGE;
}

void App::openZoom(int card)
{
    if (card < 0 || card >= g_index.cardCount) return;
    zoomCard = card;
    layoutCardZoom(s_zoomDL, card, zlayout);
    zx = 0;
    zy = 0;
    view = V_ZOOM;
}

void App::openToc()
{
    tocCount = 0;
    tocSel = 0;
    int first = g_index.chapterFirstPage[chapter];
    int cnt = g_index.chapterPageCount[chapter];
    int curCard = (sel >= 0 && sel < layout.cardCount) ? layout.cards[sel].card : -1;
    for (int p = first; p < first + cnt && tocCount < 160; p++) {
        toc[tocCount].page = (int16_t)p;
        toc[tocCount].card = -1;
        tocCount++;
        const PageRef &pg = g_index.pages[p];
        for (int k = 0; k < pg.cardCount && tocCount < 160; k++) {
            int c = pg.firstCard + k;
            if (c == curCard) tocSel = tocCount;
            toc[tocCount].page = (int16_t)p;
            toc[tocCount].card = (int16_t)c;
            tocCount++;
        }
    }
    const int RH = 18;
    tocScroll = imax(0, tocSel * RH - CONTENT_H / 2);
    view = V_TOC;
}

void App::openSearch()
{
    if (view != V_SEARCH) searchFrom = view;
    rsel = 0;
    rscroll = 0;
    view = V_SEARCH;
}

void App::runSearch()
{
    nres = g_index.search(query, results, MAX_RESULTS);
    rsel = 0;
    rscroll = 0;
}

void App::clampScroll()
{
    int maxS = imax(0, layout.height - CONTENT_H);
    if (scroll > maxS) scroll = maxS;
    if (scroll < 0) scroll = 0;
}

void App::revealCard(int i, bool down)
{
    if (i < 0 || i >= layout.cardCount) return;
    const CardBox &c = layout.cards[i];
    int h = c.bottom - c.top;
    if (h + 10 <= CONTENT_H) {
        if (c.top - 5 < scroll) scroll = c.top - 5;
        else if (c.bottom + 5 > scroll + CONTENT_H) scroll = c.bottom + 5 - CONTENT_H;
    } else {
        scroll = down ? c.top - 5 : c.bottom + 5 - CONTENT_H;
    }
    clampScroll();
}

void App::pageMove(int dir)
{
    if (layout.cardCount == 0) return;
    const CardBox &c = layout.cards[sel];
    if (dir > 0) {
        if (c.bottom + 5 > scroll + CONTENT_H) {
            scroll = imin(scroll + STEP, c.bottom + 5 - CONTENT_H);
            clampScroll();
            return;
        }
        if (sel + 1 < layout.cardCount) {
            sel++;
            revealCard(sel, true);
        }
    } else {
        if (c.top - 5 < scroll) {
            scroll = imax(scroll - STEP, c.top - 5);
            clampScroll();
            return;
        }
        if (sel > 0) {
            sel--;
            revealCard(sel, false);
        }
    }
}

void App::pageScrollBy(int dy)
{
    scroll += dy;
    clampScroll();
    // la carte selectionnee devient la premiere visible
    for (int i = 0; i < layout.cardCount; i++) {
        if (layout.cards[i].bottom > scroll + 12) { sel = i; break; }
    }
}

// --------------------------------------------------------------------------
// Clavier
// --------------------------------------------------------------------------
bool App::key(int k, char ch)
{
    if (k == AK_SHIFT) {
        shift = !shift;
        return true;
    }
    bool r;
    switch (view) {
    case V_HOME: r = keyHome(k); break;
    case V_PAGE: r = keyPage(k); break;
    case V_ZOOM: r = keyZoom(k); break;
    case V_TOC: r = keyToc(k); break;
    case V_SEARCH: r = keySearch(k, ch); break;
    default: r = true; break;
    }
    if (k != AK_NONE) shift = false;
    return r;
}

bool App::keyHome(int k)
{
    int n = homeCount;
    if (!n) return k != AK_F6;
    switch (k) {
    case AK_LEFT: homeSel = (homeSel + n - 1) % n; break;
    case AK_RIGHT: homeSel = (homeSel + 1) % n; break;
    case AK_UP: homeSel = homeSel >= 4 ? homeSel - 4 : homeSel; break;
    case AK_DOWN:
        if (homeSel + 4 < n) homeSel += 4;
        else if (homeSel / 4 < (n - 1) / 4) homeSel = n - 1;   // derniere rangee incomplete
        break;
    case AK_EXE:
    case AK_F1: openChapter(homeChapters[homeSel]); break;
    case AK_F4: openSearch(); break;
    case AK_F5:
        for (int c = 0; c < g_chapterCount; c++)
            if (g_chapters[c].hidden) { openChapter(c); break; }
        break;
    case AK_F6: return false;
    default: break;
    }
    homeReveal();
    return true;
}

// Fait defiler l'accueil pour que la tuile choisie soit visible.
void App::homeReveal()
{
    int row = homeSel / 4;
    int rows = (homeCount + 3) / 4;
    if (row < homeTop) homeTop = row;
    if (row >= homeTop + HOME_ROWS) homeTop = row - HOME_ROWS + 1;
    if (homeTop > rows - HOME_ROWS) homeTop = rows - HOME_ROWS;
    if (homeTop < 0) homeTop = 0;
}

bool App::keyPage(int k)
{
    int first = g_index.chapterFirstPage[chapter];
    int last = first + g_index.chapterPageCount[chapter] - 1;
    switch (k) {
    case AK_DOWN:
        if (shift) pageScrollBy(CONTENT_H - 24);
        else pageMove(1);
        break;
    case AK_UP:
        if (shift) pageScrollBy(-(CONTENT_H - 24));
        else pageMove(-1);
        break;
    case AK_LEFT:
    case AK_F2:
        if (page > first) openPage(page - 1, -1);
        break;
    case AK_RIGHT:
    case AK_F3:
        if (page < last) openPage(page + 1, -1);
        break;
    case AK_EXE:
    case AK_F5:
        if (sel >= 0 && sel < layout.cardCount) openZoom(layout.cards[sel].card);
        break;
    case AK_F1: openToc(); break;
    case AK_F4: openSearch(); break;
    case AK_F6:
    case AK_EXIT:
        view = V_HOME;
        for (int i = 0; i < homeCount; i++)
            if (homeChapters[i] == chapter) homeSel = i;
        homeReveal();
        break;
    default: break;
    }
    return true;
}

bool App::keyZoom(int k)
{
    int maxY = imax(0, zlayout.height - CONTENT_H);
    int maxX = zlayout.width > SCREEN_W + 2 ? zlayout.width - SCREEN_W : 0;
    bool wide = maxX > 0;
    switch (k) {
    case AK_DOWN: zy = imin(maxY, zy + (shift ? CONTENT_H - 24 : 32)); break;
    case AK_UP: zy = imax(0, zy - (shift ? CONTENT_H - 24 : 32)); break;
    case AK_RIGHT:
        if (wide) { zx = imin(maxX, zx + 48); break; }
        /* fallthrough */
    case AK_F2:
        if (zoomCard + 1 < g_index.cardCount &&
            g_index.chapterOfCard(zoomCard + 1) == g_index.chapterOfCard(zoomCard))
            openZoom(zoomCard + 1);
        break;
    case AK_LEFT:
        if (wide) { zx = imax(0, zx - 48); break; }
        /* fallthrough */
    case AK_F1:
        if (zoomCard > 0 && g_index.chapterOfCard(zoomCard - 1) == g_index.chapterOfCard(zoomCard))
            openZoom(zoomCard - 1);
        break;
    case AK_EXE:
    case AK_EXIT:
    case AK_F6: {
        // revient a la page de la carte affichee
        int card = zoomCard;
        int p = g_index.cards[card].page;
        if (p != page) openPage(p, card);
        else {
            for (int i = 0; i < layout.cardCount; i++)
                if (layout.cards[i].card == card) { sel = i; revealCard(i, true); }
            view = V_PAGE;
        }
        break;
    }
    default: break;
    }
    return true;
}

bool App::keyToc(int k)
{
    const int RH = 18;
    switch (k) {
    case AK_DOWN: if (tocSel + 1 < tocCount) tocSel++; break;
    case AK_UP: if (tocSel > 0) tocSel--; break;
    case AK_RIGHT: tocSel = imin(tocCount - 1, tocSel + 8); break;
    case AK_LEFT: tocSel = imax(0, tocSel - 8); break;
    case AK_EXE:
    case AK_F1:
        if (tocSel < tocCount) openPage(toc[tocSel].page, toc[tocSel].card);
        return true;
    case AK_EXIT:
    case AK_F6:
        view = V_PAGE;
        return true;
    default: break;
    }
    if (tocSel * RH < tocScroll) tocScroll = tocSel * RH;
    if ((tocSel + 1) * RH + 4 > tocScroll + CONTENT_H) tocScroll = (tocSel + 1) * RH + 4 - CONTENT_H;
    return true;
}

bool App::keySearch(int k, char ch)
{
    const int RH = 30, listH = CONTENT_BOTTOM - (CONTENT_TOP + 32);
    switch (k) {
    case AK_CHAR:
        if (qlen < (int)sizeof(query) - 1 && !(ch == ' ' && (qlen == 0 || query[qlen - 1] == ' '))) {
            query[qlen++] = ch;
            query[qlen] = 0;
            runSearch();
        }
        break;
    case AK_DEL:
        if (qlen) {
            query[--qlen] = 0;
            runSearch();
        }
        break;
    case AK_AC:
    case AK_F5:
        qlen = 0;
        query[0] = 0;
        nres = 0;
        break;
    case AK_DOWN: if (rsel + 1 < nres) rsel++; break;
    case AK_UP: if (rsel > 0) rsel--; break;
    case AK_EXE:
    case AK_F1:
        if (nres) {
            int card = results[rsel];
            openPage(g_index.cards[card].page, card);
        }
        return true;
    case AK_EXIT:
    case AK_F6:
        view = (searchFrom == V_PAGE && page >= 0) ? V_PAGE : V_HOME;
        return true;
    default: break;
    }
    if (rsel * RH < rscroll) rscroll = rsel * RH;
    if ((rsel + 1) * RH > rscroll + listH) rscroll = (rsel + 1) * RH - listH;
    return true;
}
