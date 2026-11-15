// Liste d'affichage : resultat de la mise en page (glyphes et rectangles a des
// positions absolues). Elle est calculee une fois par page puis redessinee a
// chaque defilement.
#pragma once
#include <stdint.h>
#include "Canvas.hpp"

enum OpKind : uint8_t {
    OP_GLYPH,     // a = index du glyphe, face ; (x, y) = stylo, ligne de base
    OP_RECT,      // (x, y) = coin haut gauche, a = largeur, b = hauteur
    OP_VEXT,      // ligne d'un glyphe repetee : (x, y) = stylo, haut ; b = nb de lignes
    OP_HEXT,      // colonne d'un glyphe repetee : (x, y) = gauche, base ; b = nb de colonnes
    OP_RRECT,     // rectangle arrondi : color = fond, color2 = bord, face = rayon | drapeaux
};

enum { RR_FILL = 0x10, RR_BORDER = 0x20 };

struct DrawOp {
    int16_t x, y;
    uint16_t a, b;
    uint16_t color, color2;
    uint8_t kind, face;
};

struct DisplayList {
    DrawOp *ops;
    int cap;
    int n;
    bool overflow;

    void init(DrawOp *buf, int capacity) { ops = buf; cap = capacity; n = 0; overflow = false; }
    void clear() { n = 0; overflow = false; }
    DrawOp *push();

    void glyph(int face, int index, int x, int base, uint16_t c);
    void rect(int x, int y, int w, int h, uint16_t c);
    void vext(int face, int index, int x, int top, int rows, uint16_t c);
    void hext(int face, int index, int x, int base, int cols, uint16_t c);
    void rrect(int x, int y, int w, int h, int r, uint16_t fill, bool doFill,
               uint16_t border, bool doBorder);

    // Dessine les operations d'index [from, to[ decalees de (dx, dy).
    void draw(Canvas &cv, int dx, int dy, int from = 0, int to = -1) const;
    // Idem en forcant la couleur de toutes les operations.
    void drawColored(Canvas &cv, int dx, int dy, int from, int to, uint16_t color) const;
    // Decale les operations [from, n[ de (dx, dy).
    void shift(int from, int dx, int dy);
};
