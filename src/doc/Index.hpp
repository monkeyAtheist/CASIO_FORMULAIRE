// Index du contenu : pages et cartes de tous les chapitres (calcule une fois
// au demarrage), et recherche par mots.
#pragma once
#include <stdint.h>

enum { MAX_PAGES = 120, MAX_CARDS = 480, MAX_RESULTS = 60 };

struct PageRef {
    const char *title;
    const char *begin, *end;   // source de la page (apres la ligne @page)
    int16_t titleLen;
    uint8_t chapter;
    int16_t firstCard, cardCount;
};

struct CardRef {
    const char *title;
    const char *tags;
    const char *begin, *end;   // source de la carte (apres la ligne @card)
    int16_t titleLen, tagsLen;
    int16_t page;
};

struct Index {
    PageRef pages[MAX_PAGES];
    CardRef cards[MAX_CARDS];
    int pageCount;
    int cardCount;
    int chapterFirstPage[32];
    int chapterPageCount[32];
    bool overflow;

    void build();
    int chapterOfCard(int card) const { return pages[cards[card].page].chapter; }
    // Recherche : renvoie le nombre de cartes trouvees (indices dans out).
    int search(const char *query, int *out, int max) const;
};

extern Index g_index;

// Outils texte communs.
namespace txt {
// Minuscule sans accent d'un caractere UTF-8 (renvoie 0 pour les separateurs).
char fold(uint32_t cp);
int copy(char *dst, int cap, const char *src, int len = -1);
}
