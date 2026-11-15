// Verifie tout le contenu : chaque page et chaque carte sont mises en page ;
// on signale les erreurs LaTeX, les elements trop larges, les glyphes absents
// et les depassements de capacite. Option -png DIR : image de chaque page.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../../src/doc/Index.hpp"
#include "../../src/doc/Content.hpp"
#include "../../src/doc/PageLayout.hpp"
#include "../../src/math/Math.hpp"
#include "../../src/font/Font.hpp"

static DrawOp s_ops[30000];
static uint16_t s_img[SCREEN_W * 6000];

static void savePage(const char *file, const DisplayList &dl, int H)
{
    if (H > 6000) H = 6000;
    for (int i = 0; i < SCREEN_W * H; i++) s_img[i] = rgb(242, 244, 248);
    Canvas cv;
    for (int band = 0; band < H; band += SCREEN_H) {
        cv.fb = s_img + band * SCREEN_W;
        cv.setClip(0, 0, SCREEN_W, H - band < SCREEN_H ? H - band : SCREEN_H);
        dl.draw(cv, 0, -band);
    }
    FILE *f = fopen(file, "wb");
    if (!f) return;
    fprintf(f, "P6\n%d %d\n255\n", SCREEN_W, H);
    for (int i = 0; i < SCREEN_W * H; i++) {
        uint16_t c = s_img[i];
        unsigned char px[3] = {(unsigned char)(((c >> 11) & 31) * 255 / 31),
                               (unsigned char)(((c >> 5) & 63) * 255 / 63),
                               (unsigned char)((c & 31) * 255 / 31)};
        fwrite(px, 1, 3, f);
    }
    fclose(f);
}

static bool s_quietOverflow = false;
static void onOverflow(const char *src, int len, int w, int avail)
{
    if (s_quietOverflow) return;
    if (len > 90) len = 90;
    printf("    trop large %d > %d : %.*s\n", w, avail, len, src);
}

int main(int argc, char **argv)
{
    g_overflowHook = onOverflow;
    const char *pngDir = nullptr;
    for (int i = 1; i < argc; i++)
        if (!strcmp(argv[i], "-png") && i + 1 < argc) pngDir = argv[++i];
    g_index.build();
    int problems = 0;
    if (g_index.overflow) { printf("INDEX : capacite depassee\n"); problems++; }
    printf("%d chapitres, %d pages, %d cartes\n", g_chapterCount, g_index.pageCount, g_index.cardCount);
    DisplayList dl;
    dl.init(s_ops, 30000);
    int maxOps = 0, maxZoomOps = 0, maxH = 0;
    for (int p = 0; p < g_index.pageCount; p++) {
        const PageRef &pg = g_index.pages[p];
        int e0 = math::g_errors, m0 = font::g_missing;
        PageLayout L;
        layoutPage(dl, p, L);
        char title[96];
        txt::copy(title, sizeof(title), pg.title, pg.titleLen);
        if (dl.n > maxOps) maxOps = dl.n;
        if (L.height > maxH) maxH = L.height;
        bool bad = L.errors || L.overflows || math::g_errors != e0 || font::g_missing != m0 ||
                   dl.n > 3200 || dl.overflow || L.height > 32000;
        if (bad) {
            problems++;
            printf("PAGE %d [%s / %s] : erreurs=%d (%s) larges=%d glyphes=%d (U+%X) ops=%d h=%d\n", p,
                   g_chapters[pg.chapter].label, title, L.errors, math::g_errMsg, L.overflows,
                   font::g_missing - m0, font::g_lastMissing, dl.n, L.height);
        }
        if (pngDir) {
            char file[256];
            snprintf(file, sizeof(file), "%s/p%03d.ppm", pngDir, p);
            savePage(file, dl, L.height);
        }
    }
    s_quietOverflow = true;
    for (int c = 0; c < g_index.cardCount; c++) {
        int e0 = math::g_errors, m0 = font::g_missing;
        PageLayout L;
        layoutCardZoom(dl, c, L);
        if (dl.n > maxZoomOps) maxZoomOps = dl.n;
        if (L.errors || math::g_errors != e0 || font::g_missing != m0 || dl.n > 1400 || dl.overflow) {
            problems++;
            char title[96];
            txt::copy(title, sizeof(title), g_index.cards[c].title, g_index.cards[c].titleLen);
            printf("ZOOM carte %d [%s] : erreurs=%d glyphes=%d ops=%d\n", c, title, L.errors,
                   font::g_missing - m0, dl.n);
        }
    }
    printf("ops max : page %d (capacite 3200), zoom %d (capacite 1400)\n", maxOps, maxZoomOps);
    printf("hauteur de page max : %d px\n", maxH);
    printf("memoire de pointe : boites %d o, noeuds %d o\n", math::boxBytesPeak(), math::nodeBytesPeak());
    if (math::g_errors) printf("derniere erreur LaTeX : %s\n", math::g_errMsg);
    printf(problems ? "%d PROBLEME(S)\n" : "OK\n", problems);
    return problems ? 1 : 0;
}
