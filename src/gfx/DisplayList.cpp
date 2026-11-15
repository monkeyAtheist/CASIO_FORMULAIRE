#include "DisplayList.hpp"
#include "../font/Font.hpp"

DrawOp *DisplayList::push()
{
    static DrawOp sink;
    if (n >= cap) { overflow = true; return &sink; }
    return &ops[n++];
}

void DisplayList::glyph(int face, int index, int x, int base, uint16_t c)
{
    if (index < 0) return;
    DrawOp *o = push();
    o->kind = OP_GLYPH; o->face = (uint8_t)face;
    o->x = (int16_t)x; o->y = (int16_t)base;
    o->a = (uint16_t)index; o->b = 0; o->color = c; o->color2 = 0;
}

void DisplayList::rect(int x, int y, int w, int h, uint16_t c)
{
    if (w <= 0 || h <= 0) return;
    DrawOp *o = push();
    o->kind = OP_RECT; o->face = 0;
    o->x = (int16_t)x; o->y = (int16_t)y; o->a = (uint16_t)w; o->b = (uint16_t)h;
    o->color = c; o->color2 = 0;
}

void DisplayList::vext(int face, int index, int x, int top, int rows, uint16_t c)
{
    if (index < 0 || rows <= 0) return;
    DrawOp *o = push();
    o->kind = OP_VEXT; o->face = (uint8_t)face;
    o->x = (int16_t)x; o->y = (int16_t)top; o->a = (uint16_t)index; o->b = (uint16_t)rows;
    o->color = c; o->color2 = 0;
}

void DisplayList::hext(int face, int index, int x, int base, int cols, uint16_t c)
{
    if (index < 0 || cols <= 0) return;
    DrawOp *o = push();
    o->kind = OP_HEXT; o->face = (uint8_t)face;
    o->x = (int16_t)x; o->y = (int16_t)base; o->a = (uint16_t)index; o->b = (uint16_t)cols;
    o->color = c; o->color2 = 0;
}

void DisplayList::rrect(int x, int y, int w, int h, int r, uint16_t fill, bool doFill,
                        uint16_t border, bool doBorder)
{
    DrawOp *o = push();
    o->kind = OP_RRECT;
    o->face = (uint8_t)((r & 15) | (doFill ? RR_FILL : 0) | (doBorder ? RR_BORDER : 0));
    o->x = (int16_t)x; o->y = (int16_t)y; o->a = (uint16_t)w; o->b = (uint16_t)h;
    o->color = fill; o->color2 = border;
}

void DisplayList::shift(int from, int dx, int dy)
{
    for (int i = from; i < n; i++) {
        ops[i].x = (int16_t)(ops[i].x + dx);
        ops[i].y = (int16_t)(ops[i].y + dy);
    }
}

static void drawOps(const DrawOp *ops, int n, Canvas &cv, int dx, int dy, int from, int to,
                    bool force, uint16_t fc)
{
    if (to < 0 || to > n) to = n;
    for (int i = from; i < to; i++) {
        DrawOp o = ops[i];
        if (force) o.color = fc;
        int x = o.x + dx, y = o.y + dy;
        switch (o.kind) {
        case OP_GLYPH: {
            if (y - 90 > cv.cy1 || y + 90 < cv.cy0) break;
            const FontFace &f = g_faces[o.face];
            const Glyph &g = f.glyphs[o.a];
            if (g.w) cv.alpha4(f.bits + g.off, g.w, g.h, x + g.left, y - g.top, o.color);
            break;
        }
        case OP_RECT:
            cv.fillRect(x, y, o.a, o.b, o.color);
            break;
        case OP_VEXT: {
            if (y > cv.cy1 || y + o.b < cv.cy0) break;
            const FontFace &f = g_faces[o.face];
            const Glyph &g = f.glyphs[o.a];
            if (g.w) cv.alpha4RowRepeat(f.bits + g.off, g.w, x + g.left, y, o.b, o.color);
            break;
        }
        case OP_HEXT: {
            if (y - 90 > cv.cy1 || y + 90 < cv.cy0) break;
            const FontFace &f = g_faces[o.face];
            const Glyph &g = f.glyphs[o.a];
            if (g.h) cv.alpha4ColRepeat(f.bits + g.off, g.h, x, y - g.top, o.b, o.color);
            break;
        }
        case OP_RRECT:
            if (y > cv.cy1 || y + o.b < cv.cy0) break;
            cv.roundRect(x, y, o.a, o.b, o.face & 15, o.color, (o.face & RR_FILL) != 0,
                         o.color2, (o.face & RR_BORDER) != 0);
            break;
        }
    }
}

void DisplayList::draw(Canvas &cv, int dx, int dy, int from, int to) const
{
    drawOps(ops, n, cv, dx, dy, from, to, false, 0);
}

void DisplayList::drawColored(Canvas &cv, int dx, int dy, int from, int to, uint16_t color) const
{
    drawOps(ops, n, cv, dx, dy, from, to, true, color);
}
