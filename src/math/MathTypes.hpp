// Structures internes du moteur de formules : noeuds (analyse du LaTeX) et
// boites (mise en page). Les deux vivent dans des zones memoire statiques
// remises a zero entre deux formules : aucune allocation dynamique.
#pragma once
#include <stdint.h>

namespace math {

// Classes d'atomes de TeX (espacement automatique).
enum AtomClass : uint8_t {
    C_ORD, C_OP, C_BIN, C_REL, C_OPEN, C_CLOSE, C_PUNCT, C_INNER, C_NONE
};

// Variante de police d'un symbole.
enum FontVar : uint8_t {
    FV_AUTO,   // lettres en italique, chiffres droits
    FV_RM,     // \mathrm, \text, noms de fonctions
    FV_BF,     // \mathbf
    FV_BB,     // \mathbb
    FV_CAL,    // \mathcal
    FV_IT,     // \mathit
};

// Styles : D (hors-texte), T (texte), S (indice), SS (indice d'indice).
enum Style : uint8_t { ST_D, ST_T, ST_S, ST_SS, ST_SMALL = 0x80 };

enum NodeType : uint8_t {
    N_SYM,       // cp, fv
    N_GROUP,     // a = premier element (liste)
    N_FRAC,      // a = numerateur, b = denominateur ; aux = 0 auto, 1 D, 2 T ; flags F_NOBAR
    N_SCRIPTS,   // a = base, b = indice, c = exposant
    N_OP,        // grand operateur : cp (symbole) ou str (nom) ; flags F_LIMITS/F_NOLIMITS
    N_RAD,       // a = indice de racine (ou nul), b = radicande
    N_LR,        // \left cp ... \right cp2 ; a = contenu
    N_ACCENT,    // cp = accent ; a = base ; aux = genre d'accent
    N_ARRAY,     // a = cellules (ligne par ligne) ; aux = lignes, val = colonnes ; str = format
    N_SPACE,     // val = espace en mu (1/18 em), peut etre negatif
    N_TEXT,      // str, len : texte droit
    N_STYLE,     // aux = style
    N_STACK,     // \overset / \underset : a = haut, b = base, c = bas
    N_XARROW,    // cp = fleche ; a = dessus, b = dessous
    N_COLOR,     // val = couleur ; a = contenu
    N_BIGDELIM,  // \big( : cp, aux = taille 1..4
    N_OVERLINE,  // a = base ; aux : 0 dessus, 1 dessous
    N_BOXED,     // a = contenu
    N_PHANTOM,   // a = contenu (invisible)
};

enum NodeFlags : uint8_t {
    F_NOBAR = 1,
    F_LIMITS = 2,
    F_NOLIMITS = 4,
    F_PRIME = 8,       // groupe "base + prime"
    F_DISPLAY_LIMITS = 16,  // operateur avec bornes dessus/dessous en style D
};

enum AccentKind : uint8_t { AK_NORMAL, AK_WIDE, AK_STRETCH_ARROW };

enum ArrayKind : uint8_t {
    AR_MATRIX, AR_PMATRIX, AR_BMATRIX, AR_VMATRIX, AR_VVMATRIX, AR_CASES, AR_ARRAY,
    AR_BRACE_MATRIX,
};

struct Box;

struct Node {
    Node *next;
    Node *a, *b, *c;
    const char *str;
    Box *lbox;          // boite calculee (mise en page)
    uint32_t cp, cp2;
    int16_t len;
    int16_t val;
    uint8_t type, cls, flags, aux, fv, kind;
    uint8_t ecls;       // classe effective (apres regles Bin -> Ord)
    uint8_t lstyle;     // style en vigueur pour cet atome
};

enum BoxKind : uint8_t { B_LIST, B_GLYPH, B_RULE, B_VEXT, B_HEXT, B_FRAME };

struct Box {
    Box *next;
    Box *first;
    int16_t x, y;      // position dans le parent (y : decalage de la ligne de base, vers le bas)
    int16_t w, h, d;   // largeur, hauteur, profondeur
    int16_t ic;        // correction d'italique
    int16_t acc;       // point d'attache des accents (-1 : milieu)
    int16_t count;     // B_VEXT / B_HEXT : repetitions
    uint16_t color;
    uint16_t gidx;     // index du glyphe
    uint8_t kind, face, hasColor, isChar, hidden, pad;
};

}  // namespace math
