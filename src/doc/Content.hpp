// Contenu du formulaire : une source texte par chapitre (voir content/*.cpp).
//
// Format d'une source (une directive par ligne, une directive se poursuit
// jusqu'a la ligne qui commence par @ suivante) :
//   @page Titre de la page           nouvelle page (entree du sommaire)
//   @card Titre de la carte          nouvelle carte (bloc selectionnable)
//   @tags mots cles                  mots cles pour la recherche
//   @math formule LaTeX              formule centree (style hors-texte)
//   @align a &= b                    lignes alignees sur & (lignes consecutives)
//   @table[ccl] f & f' & domaine     tableau : en-tete (cellules en LaTeX)
//   @row x^n & nx^{n-1} & ...        ligne du tableau
//   @text texte avec $x^2$ en ligne   paragraphe (**gras** possible)
//   @note remarque                   petite remarque grise
//   @li element de liste             puce
//   @sub sous-titre                  sous-titre dans une carte
//   % commentaire                    ligne ignoree
#pragma once
#include <stdint.h>

struct Chapter {
    const char *title;       // titre complet
    const char *label;       // libelle court (accueil)
    const char *icon;        // formule de l'icone (LaTeX)
    uint16_t color;          // couleur du theme
    const char *src;         // source
    uint8_t hidden;          // 1 : n'apparait pas dans la grille (aide)
};

extern const Chapter g_chapters[];
extern const int g_chapterCount;
