// Application Formulaire : accueil, pages, vue agrandie, sommaire, recherche.
#pragma once
#include <stdint.h>
#include "../gfx/Canvas.hpp"
#include "../gfx/DisplayList.hpp"
#include "../doc/PageLayout.hpp"
#include "../doc/Index.hpp"

enum ViewId : uint8_t { V_HOME, V_PAGE, V_ZOOM, V_TOC, V_SEARCH };

// Touches de l'application (independantes de gint).
enum AppKey {
    AK_NONE, AK_UP, AK_DOWN, AK_LEFT, AK_RIGHT, AK_EXE, AK_EXIT, AK_SHIFT, AK_ALPHA,
    AK_F1, AK_F2, AK_F3, AK_F4, AK_F5, AK_F6, AK_DEL, AK_AC, AK_OPTN, AK_VARS,
    AK_CHAR,   // lettre (voir App::key, parametre ch)
};

enum { CONTENT_TOP = 20, CONTENT_BOTTOM = 206, CONTENT_H = CONTENT_BOTTOM - CONTENT_TOP };

struct TocRow {
    int16_t page;   // page de la ligne
    int16_t card;   // -1 : ligne de page
};

struct App {
    ViewId view;
    ViewId searchFrom;
    bool shift;

    // accueil
    int homeSel;
    int homeTop;            // premiere rangee de tuiles visible
    int homeChapters[32];
    int homeCount;
    int tileOpStart[32], tileOpEnd[32];

    // page
    int chapter;
    int page;
    int sel;
    int scroll;
    PageLayout layout;

    // zoom
    int zoomCard;
    int zx, zy;
    PageLayout zlayout;

    // sommaire
    TocRow toc[160];
    int tocCount, tocSel, tocScroll;

    // recherche
    char query[28];
    int qlen;
    int results[MAX_RESULTS];
    int nres, rsel, rscroll;

    // diagnostics (tests)
    int lastErrors, lastOverflows;

    void init();
    void draw(Canvas &cv);
    // Renvoie false pour quitter l'application.
    bool key(int k, char ch);

    // navigation
    void openChapter(int c);
    void openPage(int p, int card);
    void openZoom(int card);
    void openToc();
    void openSearch();

private:
    void drawHome(Canvas &cv);
    void drawPage(Canvas &cv);
    void drawZoom(Canvas &cv);
    void drawToc(Canvas &cv);
    void drawSearch(Canvas &cv);
    void drawHeader(Canvas &cv, uint16_t bg, const char *title, int titleLen,
                    const char *sub, int subLen, const char *right);
    void drawFKeys(Canvas &cv, const char *const labels[6]);

    bool keyHome(int k);
    void homeReveal();
    bool keyPage(int k);
    bool keyZoom(int k);
    bool keyToc(int k);
    bool keySearch(int k, char ch);

    void pageMove(int dir);
    void pageScrollBy(int dy);
    void revealCard(int i, bool down);
    void clampScroll();
    void runSearch();
};

extern App g_app;
