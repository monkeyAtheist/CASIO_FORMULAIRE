// Moteur de formules : LaTeX simplifie -> boites -> liste d'affichage.
//
// Exemple :
//   math::Box *b = math::typeset("\\int_a^b f(x)\\,dx", -1, math::ST_D, false);
//   math::emit(dl, b, x, base, couleur);
//   math::resetBoxes();
#pragma once
#include <stdint.h>
#include "MathTypes.hpp"
#include "../gfx/DisplayList.hpp"

namespace math {

// Remise a zero des zones memoire (a faire apres chaque groupe de formules).
void resetNodes();
void resetBoxes();

// Analyse + mise en page. len < 0 : chaine terminee par 0.
// zoom : utilise les grandes tailles (vue agrandie).
Box *typeset(const char *src, int len, uint8_t style, bool zoom);

// Variante qui ajoute un atome ordinaire vide devant (colonnes d'alignement :
// "&= b" garde l'espace devant le signe =).
Box *typesetAligned(const char *src, int len, uint8_t style, bool zoom, bool leadingOrd);

// Ajoute la formule a la liste d'affichage, ligne de base en (x, base).
void emit(DisplayList &dl, const Box *b, int x, int base, uint16_t color);

// Taille de police (px) du style D/T selon le mode.
int mathFace(uint8_t style, bool zoom);

// Diagnostics (utilises par les tests sur PC).
extern int g_errors;
extern char g_errMsg[160];
extern bool g_oom;
int nodeBytesPeak();
int boxBytesPeak();

}  // namespace math
