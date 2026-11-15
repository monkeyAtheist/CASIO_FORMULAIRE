// Structures des polices generees (src/gen/FontData.cpp).
#pragma once
#include <stdint.h>
#include "../gen/FontIds.hpp"

// Un glyphe : bitmap alpha 4 bits (2 pixels par octet, poids fort a gauche,
// chaque ligne commence sur un octet). Coordonnees en pixels, y vers le haut
// par rapport a la ligne de base : le pixel en haut a gauche du bitmap est en
// (stylo + left, base - top).
struct Glyph {
    uint32_t key;    // code Unicode, ou >= 0x110000 pour une variante
    uint32_t off;    // decalage dans le tableau de bits
    int16_t adv;     // avance
    int16_t left;
    int16_t top;
    uint8_t w, h;
    int8_t asc;      // encre au-dessus de la ligne de base (metrique police)
    int8_t desc;     // encre au-dessous
    int8_t ic;       // correction d'italique
    int8_t acc;      // point d'attache des accents (-128 : milieu)
};

struct FontFace {
    const Glyph *glyphs;
    uint16_t count;
    const uint8_t *bits;
    uint8_t px;
    int8_t ascent;   // pour l'interligne
    int8_t descent;
    uint8_t isMath;
};

// Constantes de la table MATH converties en pixels pour une taille donnee.
struct MathConsts {
#include "../gen/MathConstsFields.inc"
};

struct VariantPart {
    uint32_t key;
    uint8_t ext;     // 1 : morceau extensible (repete)
};

// Variantes de taille et assemblage d'un symbole extensible.
struct VariantSet {
    uint32_t base;
    uint8_t vertical;
    uint8_t nvar;
    uint8_t nparts;
    uint32_t vars[MAX_VARIANTS];
    VariantPart parts[MAX_PARTS];
};

extern const FontFace g_faces[FACE_COUNT];
extern const MathConsts g_mathConsts[MATH_FACE_COUNT];
extern const VariantSet g_variantSets[VARIANT_SET_COUNT];
