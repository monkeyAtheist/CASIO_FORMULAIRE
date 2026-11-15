#include "Index.hpp"
#include "Content.hpp"
#include "../font/Font.hpp"
#include <string.h>

Index g_index;

namespace txt {

char fold(uint32_t cp)
{
    if (cp >= 'A' && cp <= 'Z') return (char)(cp - 'A' + 'a');
    if ((cp >= 'a' && cp <= 'z') || (cp >= '0' && cp <= '9')) return (char)cp;
    switch (cp) {
    case 0xE0: case 0xE2: case 0xE4: case 0xC0: case 0xC2: case 0xC4: return 'a';
    case 0xE7: case 0xC7: return 'c';
    case 0xE8: case 0xE9: case 0xEA: case 0xEB: case 0xC8: case 0xC9: case 0xCA: case 0xCB: return 'e';
    case 0xEE: case 0xEF: case 0xCE: case 0xCF: return 'i';
    case 0xF4: case 0xF6: case 0xD4: case 0xD6: return 'o';
    case 0xF9: case 0xFB: case 0xFC: case 0xD9: case 0xDB: case 0xDC: return 'u';
    case 0x153: case 0x152: return 'o';
    case '\'': case 0x2032: return '\'';
    default: return 0;
    }
}

int copy(char *dst, int cap, const char *src, int len)
{
    int i = 0;
    for (; i < cap - 1 && (len < 0 ? src[i] != 0 : i < len); i++) dst[i] = src[i];
    dst[i] = 0;
    return i;
}

}  // namespace txt

// Ligne suivante : renvoie le debut de la ligne suivante.
static const char *nextLine(const char *p, const char *end)
{
    while (p < end && *p != '\n') p++;
    return p < end ? p + 1 : end;
}

static bool isDirective(const char *p, const char *end, const char *name)
{
    int n = (int)strlen(name);
    if (p + 1 + n > end || p[0] != '@' || strncmp(p + 1, name, n) != 0) return false;
    return p + 1 + n == end || p[1 + n] == ' ' || p[1 + n] == '\n' || p[1 + n] == '\t' ||
           p[1 + n] == '\r' || p[1 + n] == '[';
}

// Texte d'une directive sur sa ligne (sans espaces de tete/queue).
static void lineArg(const char *p, const char *end, const char *&s, int &len)
{
    while (p < end && *p != ' ' && *p != '\n') p++;
    while (p < end && *p == ' ') p++;
    const char *e = p;
    while (e < end && *e != '\n') e++;
    while (e > p && (e[-1] == ' ' || e[-1] == '\r')) e--;
    s = p;
    len = (int)(e - p);
}

void Index::build()
{
    pageCount = 0;
    cardCount = 0;
    overflow = false;
    for (int c = 0; c < g_chapterCount && c < 32; c++) {
        chapterFirstPage[c] = pageCount;
        const char *src = g_chapters[c].src;
        const char *end = src + strlen(src);
        PageRef *pg = nullptr;
        CardRef *cd = nullptr;
        for (const char *p = src; p < end; p = nextLine(p, end)) {
            if (*p != '@') continue;
            if (isDirective(p, end, "page")) {
                if (cd) { cd->end = p; cd = nullptr; }
                if (pg) pg->end = p;
                if (pageCount >= MAX_PAGES) { overflow = true; pg = nullptr; continue; }
                pg = &pages[pageCount++];
                int len;
                lineArg(p, end, pg->title, len);
                pg->titleLen = (int16_t)len;
                pg->begin = nextLine(p, end);
                pg->end = end;
                pg->chapter = (uint8_t)c;
                pg->firstCard = (int16_t)cardCount;
                pg->cardCount = 0;
            } else if (isDirective(p, end, "card")) {
                if (cd) cd->end = p;
                if (!pg || cardCount >= MAX_CARDS) { overflow = true; cd = nullptr; continue; }
                cd = &cards[cardCount++];
                int len;
                lineArg(p, end, cd->title, len);
                cd->titleLen = (int16_t)len;
                cd->tags = nullptr;
                cd->tagsLen = 0;
                cd->begin = nextLine(p, end);
                cd->end = end;
                cd->page = (int16_t)(pg - pages);
                pg->cardCount++;
            } else if (isDirective(p, end, "tags") && cd) {
                int len;
                lineArg(p, end, cd->tags, len);
                cd->tagsLen = (int16_t)len;
            }
        }
        if (cd) cd->end = end;
        if (pg) pg->end = end;
        chapterPageCount[c] = pageCount - chapterFirstPage[c];
    }
}

// Normalise une chaine (minuscules, sans accents, separateurs -> espace).
static int normalize(const char *s, int len, char *out, int cap)
{
    const char *e = s + len;
    int n = 0;
    bool lastSpace = true;
    while (s < e && n < cap - 1) {
        uint32_t cp = font::utf8Next(s, e);
        char c = txt::fold(cp);
        if (!c) {
            if (!lastSpace) { out[n++] = ' '; lastSpace = true; }
            continue;
        }
        out[n++] = c;
        lastSpace = false;
    }
    while (n > 0 && out[n - 1] == ' ') n--;
    out[n] = 0;
    return n;
}

static bool contains(const char *hay, const char *needle)
{
    int n = (int)strlen(needle);
    if (!n) return true;
    for (const char *h = hay; *h; h++)
        if (!strncmp(h, needle, n)) return true;
    return false;
}

int Index::search(const char *query, int *out, int max) const
{
    char q[48];
    int ql = normalize(query, (int)strlen(query), q, sizeof(q));
    if (ql == 0) return 0;
    // mots de la requete
    const char *words[8];
    int nw = 0;
    char *p = q;
    while (*p && nw < 8) {
        while (*p == ' ') *p++ = 0;
        if (!*p) break;
        words[nw++] = p;
        while (*p && *p != ' ') p++;
    }
    int found = 0;
    // deux passes : d'abord les titres de cartes, puis le reste
    for (int pass = 0; pass < 2 && found < max; pass++) {
        for (int i = 0; i < cardCount && found < max; i++) {
            if (g_chapters[pages[cards[i].page].chapter].hidden) continue;
            char hay[320];
            int n = normalize(cards[i].title, cards[i].titleLen, hay, 120);
            if (pass == 1) {
                hay[n++] = ' ';
                const PageRef &pg = pages[cards[i].page];
                n += normalize(pg.title, pg.titleLen, hay + n, 80);
                hay[n++] = ' ';
                const Chapter &ch = g_chapters[pg.chapter];
                n += normalize(ch.title, (int)strlen(ch.title), hay + n, 60);
                if (cards[i].tags) {
                    hay[n++] = ' ';
                    n += normalize(cards[i].tags, cards[i].tagsLen, hay + n, (int)sizeof(hay) - n - 1);
                }
            }
            bool ok = true;
            for (int w = 0; w < nw && ok; w++) ok = contains(hay, words[w]);
            if (!ok) continue;
            bool dup = false;
            for (int k = 0; k < found; k++) if (out[k] == i) { dup = true; break; }
            if (!dup) out[found++] = i;
        }
    }
    return found;
}
