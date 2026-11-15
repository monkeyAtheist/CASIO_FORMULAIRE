// Acces aux polices generees : recherche de glyphes, mesure et dessin de texte.
#pragma once
#include <stdint.h>
#include "FontTypes.hpp"
#include "../gfx/Canvas.hpp"

namespace font {

// Index du glyphe dans la table de la police, ou -1.
int find(int face, uint32_t key);
inline const Glyph &at(int face, int index) { return g_faces[face].glyphs[index]; }
const Glyph *get(int face, uint32_t key);

// Dessine le glyphe d'index donne, stylo en (x, base).
void draw(Canvas &cv, int face, int index, int x, int base, uint16_t c);

// Decodage UTF-8 (renvoie 0xFFFD si sequence invalide). Avance p.
uint32_t utf8Next(const char *&p, const char *end);

// Texte simple (une ligne). len < 0 : jusqu'au 0 final.
int width(int face, const char *s, int len = -1);
int drawText(Canvas &cv, int face, int x, int base, const char *s, uint16_t c, int len = -1);
// Tronque avec '…' pour tenir dans maxw.
void drawTextFit(Canvas &cv, int face, int x, int base, const char *s, uint16_t c, int maxw);

inline int ascent(int face) { return g_faces[face].ascent; }
inline int descent(int face) { return g_faces[face].descent; }

// Glyphes absents rencontres (pour les tests sur PC).
extern int g_missing;
extern uint32_t g_lastMissing;

}  // namespace font
