#include "Font.hpp"

namespace font {

int g_missing = 0;
uint32_t g_lastMissing = 0;

int find(int face, uint32_t key)
{
    const FontFace &f = g_faces[face];
    int lo = 0, hi = (int)f.count - 1;
    while (lo <= hi) {
        int mid = (lo + hi) >> 1;
        uint32_t k = f.glyphs[mid].key;
        if (k == key) return mid;
        if (k < key) lo = mid + 1;
        else hi = mid - 1;
    }
    return -1;
}

const Glyph *get(int face, uint32_t key)
{
    int i = find(face, key);
    return i < 0 ? nullptr : &g_faces[face].glyphs[i];
}

void draw(Canvas &cv, int face, int index, int x, int base, uint16_t c)
{
    const FontFace &f = g_faces[face];
    const Glyph &g = f.glyphs[index];
    if (!g.w) return;
    cv.alpha4(f.bits + g.off, g.w, g.h, x + g.left, base - g.top, c);
}

uint32_t utf8Next(const char *&p, const char *end)
{
    const unsigned char *s = (const unsigned char *)p;
    if (p >= end) return 0;
    unsigned c = *s;
    int n = 0;
    uint32_t cp;
    if (c < 0x80) { p++; return c; }
    else if ((c & 0xE0) == 0xC0) { cp = c & 0x1F; n = 1; }
    else if ((c & 0xF0) == 0xE0) { cp = c & 0x0F; n = 2; }
    else if ((c & 0xF8) == 0xF0) { cp = c & 0x07; n = 3; }
    else { p++; return 0xFFFD; }
    if (p + n >= end) { p = end; return 0xFFFD; }
    for (int i = 1; i <= n; i++) {
        if ((s[i] & 0xC0) != 0x80) { p += i; return 0xFFFD; }
        cp = (cp << 6) | (s[i] & 0x3F);
    }
    p += n + 1;
    return cp;
}

static const char *endOf(const char *s, int len)
{
    if (len >= 0) return s + len;
    const char *e = s;
    while (*e) e++;
    return e;
}

int width(int face, const char *s, int len)
{
    const char *e = endOf(s, len);
    int w = 0;
    while (s < e) {
        uint32_t cp = utf8Next(s, e);
        if (cp == '\'') cp = 0x2019;
        int i = find(face, cp);
        if (i < 0) i = find(face, '?');
        if (i >= 0) w += at(face, i).adv;
    }
    return w;
}

int drawText(Canvas &cv, int face, int x, int base, const char *s, uint16_t c, int len)
{
    const char *e = endOf(s, len);
    while (s < e) {
        uint32_t cp = utf8Next(s, e);
        if (cp == '\'') cp = 0x2019;
        int i = find(face, cp);
        if (i < 0) {
            g_missing++;
            g_lastMissing = cp;
            i = find(face, '?');
            if (i < 0) continue;
        }
        draw(cv, face, i, x, base, c);
        x += at(face, i).adv;
    }
    return x;
}

void drawTextFit(Canvas &cv, int face, int x, int base, const char *s, uint16_t c, int maxw)
{
    const char *e = endOf(s, -1);
    if (width(face, s) <= maxw) { drawText(cv, face, x, base, s, c); return; }
    int ell = width(face, "\xE2\x80\xA6");
    const char *p = s;
    int w = 0;
    const char *cut = s;
    while (p < e) {
        const char *q = p;
        uint32_t cp = utf8Next(q, e);
        if (cp == '\'') cp = 0x2019;
        int i = find(face, cp);
        int a = i >= 0 ? at(face, i).adv : 0;
        if (w + a + ell > maxw) break;
        w += a;
        p = q;
        cut = p;
    }
    int xe = drawText(cv, face, x, base, s, c, (int)(cut - s));
    drawText(cv, face, xe, base, "\xE2\x80\xA6", c);
}

}  // namespace font
