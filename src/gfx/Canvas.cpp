#include "Canvas.hpp"

uint16_t mix16(uint16_t a, uint16_t b, int t)
{
    int ar = a >> 11, ag = (a >> 5) & 63, ab = a & 31;
    int br = b >> 11, bg = (b >> 5) & 63, bb = b & 31;
    int r = ar + ((br - ar) * t) / 16;
    int g = ag + ((bg - ag) * t) / 16;
    int bl = ab + ((bb - ab) * t) / 16;
    return (uint16_t)((r << 11) | (g << 5) | bl);
}

uint16_t Canvas::blend15(uint16_t d, uint16_t s, int a)
{
    int dr = d >> 11, dg = (d >> 5) & 63, db = d & 31;
    int sr = s >> 11, sg = (s >> 5) & 63, sb = s & 31;
    // division par 15 arrondie
    int r = dr + ((sr - dr) * a * 17 + 128) / 256;
    int g = dg + ((sg - dg) * a * 17 + 128) / 256;
    int b = db + ((sb - db) * a * 17 + 128) / 256;
    return (uint16_t)((r << 11) | (g << 5) | b);
}

void Canvas::begin(uint16_t *vram)
{
    fb = vram;
    resetClip();
}

void Canvas::setClip(int x0, int y0, int x1, int y1)
{
    cx0 = x0 < 0 ? 0 : x0;
    cy0 = y0 < 0 ? 0 : y0;
    cx1 = x1 > SCREEN_W ? SCREEN_W : x1;
    cy1 = y1 > SCREEN_H ? SCREEN_H : y1;
}

void Canvas::resetClip()
{
    cx0 = 0; cy0 = 0; cx1 = SCREEN_W; cy1 = SCREEN_H;
}

void Canvas::fill(uint16_t c)
{
    fillRect(0, 0, SCREEN_W, SCREEN_H, c);
}

void Canvas::fillRect(int x, int y, int w, int h, uint16_t c)
{
    int x0 = x < cx0 ? cx0 : x, y0 = y < cy0 ? cy0 : y;
    int x1 = x + w > cx1 ? cx1 : x + w, y1 = y + h > cy1 ? cy1 : y + h;
    if (x0 >= x1 || y0 >= y1) return;
    for (int yy = y0; yy < y1; yy++) {
        uint16_t *p = fb + yy * SCREEN_W + x0;
        for (int xx = x0; xx < x1; xx++) *p++ = c;
    }
}

void Canvas::blendRect(int x, int y, int w, int h, uint16_t c, int alpha16)
{
    int x0 = x < cx0 ? cx0 : x, y0 = y < cy0 ? cy0 : y;
    int x1 = x + w > cx1 ? cx1 : x + w, y1 = y + h > cy1 ? cy1 : y + h;
    if (x0 >= x1 || y0 >= y1) return;
    for (int yy = y0; yy < y1; yy++) {
        uint16_t *p = fb + yy * SCREEN_W + x0;
        for (int xx = x0; xx < x1; xx++, p++) *p = mix16(*p, c, alpha16);
    }
}

void Canvas::frame(int x, int y, int w, int h, uint16_t c)
{
    hline(x, y, w, c);
    hline(x, y + h - 1, w, c);
    vline(x, y, h, c);
    vline(x + w - 1, y, h, c);
}

// Coins arrondis : couverture precalculee (quart de disque, 4 bits).
static const uint8_t corner2[2 * 2] = {5, 14, 14, 15};
static const uint8_t corner3[3 * 3] = {0, 9, 14, 9, 15, 15, 14, 15, 15};
static const uint8_t corner4[4 * 4] = {0, 2, 10, 14, 2, 14, 15, 15, 10, 15, 15, 15, 14, 15, 15, 15};
// Bord (anneau d'un pixel) correspondant.
static const uint8_t ring2[2 * 2] = {5, 14, 14, 3};
static const uint8_t ring3[3 * 3] = {0, 9, 14, 9, 10, 1, 14, 1, 0};
static const uint8_t ring4[4 * 4] = {0, 2, 10, 14, 2, 14, 6, 1, 10, 6, 0, 0, 14, 1, 0, 0};

void Canvas::roundRect(int x, int y, int w, int h, int r, uint16_t fillc, bool doFill,
                       uint16_t border, bool doBorder)
{
    if (r < 2) r = 0;
    if (r > 4) r = 4;
    if (w < 2 * r || h < 2 * r) r = 0;
    const uint8_t *cov = r == 2 ? corner2 : r == 3 ? corner3 : corner4;
    const uint8_t *ring = r == 2 ? ring2 : r == 3 ? ring3 : ring4;
    if (doFill) {
        fillRect(x + r, y, w - 2 * r, h, fillc);
        fillRect(x, y + r, r, h - 2 * r, fillc);
        fillRect(x + w - r, y + r, r, h - 2 * r, fillc);
    }
    if (doBorder) {
        hline(x + r, y, w - 2 * r, border);
        hline(x + r, y + h - 1, w - 2 * r, border);
        vline(x, y + r, h - 2 * r, border);
        vline(x + w - 1, y + r, h - 2 * r, border);
    }
    if (r == 0) {
        if (doFill && !doBorder) return;
        return;
    }
    for (int j = 0; j < r; j++) {
        for (int i = 0; i < r; i++) {
            int a = cov[j * r + i];
            int b = doBorder ? ring[j * r + i] : 0;
            // quatre coins par symetrie
            int px[4] = {x + i, x + w - 1 - i, x + i, x + w - 1 - i};
            int py[4] = {y + j, y + j, y + h - 1 - j, y + h - 1 - j};
            for (int k = 0; k < 4; k++) {
                if (doFill && a) blendPixel(px[k], py[k], fillc, a);
                if (b) blendPixel(px[k], py[k], border, b);
            }
        }
    }
}

void Canvas::alpha4(const uint8_t *bits, int w, int h, int x, int y, uint16_t c)
{
    if (w <= 0 || h <= 0) return;
    if (x >= cx1 || y >= cy1 || x + w <= cx0 || y + h <= cy0) return;
    int stride = (w + 1) >> 1;
    int j0 = y < cy0 ? cy0 - y : 0;
    int j1 = y + h > cy1 ? cy1 - y : h;
    int i0 = x < cx0 ? cx0 - x : 0;
    int i1 = x + w > cx1 ? cx1 - x : w;
    for (int j = j0; j < j1; j++) {
        const uint8_t *row = bits + j * stride;
        uint16_t *p = fb + (y + j) * SCREEN_W + x;
        for (int i = i0; i < i1; i++) {
            int a = (i & 1) ? (row[i >> 1] & 15) : (row[i >> 1] >> 4);
            if (!a) continue;
            if (a == 15) p[i] = c;
            else p[i] = blend15(p[i], c, a);
        }
    }
}

void Canvas::alpha4RowRepeat(const uint8_t *row, int w, int x, int y, int count, uint16_t c)
{
    for (int k = 0; k < count; k++) alpha4(row, w, 1, x, y + k, c);
}

void Canvas::alpha4ColRepeat(const uint8_t *bits, int h, int x, int y, int count, uint16_t c)
{
    for (int j = 0; j < h; j++) {
        int a = bits[j] >> 4;   // largeur 1 : un octet par ligne, poids fort
        if (!a) continue;
        for (int k = 0; k < count; k++) blendPixel(x + k, y + j, c, a);
    }
}
