// Mise en page des pages du formulaire (cartes, formules, tableaux, textes).
#pragma once
#include <stdint.h>
#include "../gfx/DisplayList.hpp"

struct Theme {
    bool zoom;
    int prose, bold, note, title, sub;   // polices de texte
    int titleH, titleBase;               // hauteur de la ligne de titre d'une carte
    int itemGap, cardGap, lineGap;
    int cellPadX, cellPadY;
};

const Theme &themeNormal();
const Theme &themeZoom();

enum { MAX_PAGE_CARDS = 40 };

struct CardBox {
    int16_t top, bottom;   // en coordonnees de page
    int16_t card;          // indice global de la carte
};

struct PageLayout {
    int height;            // hauteur totale du contenu
    int width;             // bord droit le plus a droite (vue zoom)
    int cardCount;
    CardBox cards[MAX_PAGE_CARDS];
    int overflows;         // elements plus larges que la page
    int errors;            // erreurs de syntaxe dans les formules
};

// Largeur du contenu d'une page en vue normale.
enum { PAGE_CARD_X = 4, PAGE_CARD_W = 384 };

// Appele pour chaque element trop large (tests sur PC ; nul sinon).
extern void (*g_overflowHook)(const char *src, int len, int width, int avail);

void layoutPage(DisplayList &dl, int page, PageLayout &out);
void layoutCardZoom(DisplayList &dl, int card, PageLayout &out);

// Petite formule centree en (cx, cy) (icones de l'accueil). Renvoie sa largeur.
int layoutFormulaAt(DisplayList &dl, const char *tex, int cx, int cy, uint16_t color,
                    uint8_t style, bool zoom);
