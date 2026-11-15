#include "KeyMap.hpp"
#include "App.hpp"
#include <gint/keyboard.h>

int mapGintKey(int key, char &ch)
{
    ch = 0;
    switch (key) {
    case KEY_UP: return AK_UP;
    case KEY_DOWN: return AK_DOWN;
    case KEY_LEFT: return AK_LEFT;
    case KEY_RIGHT: return AK_RIGHT;
    case KEY_EXE: return AK_EXE;
    case KEY_EXIT: return AK_EXIT;
    case KEY_SHIFT: return AK_SHIFT;
    case KEY_ALPHA: return AK_ALPHA;
    case KEY_F1: return AK_F1;
    case KEY_F2: return AK_F2;
    case KEY_F3: return AK_F3;
    case KEY_F4: return AK_F4;
    case KEY_F5: return AK_F5;
    case KEY_F6: return AK_F6;
    case KEY_DEL: return AK_DEL;
    case KEY_ACON: return AK_AC;
    case KEY_OPTN: return AK_OPTN;
    case KEY_VARS: return AK_VARS;
    default: break;
    }
    // lettres rouges du clavier de la Graph 90+E
    static const struct { int key; char ch; } letters[] = {
        {KEY_XOT, 'a'}, {KEY_LOG, 'b'}, {KEY_LN, 'c'}, {KEY_SIN, 'd'}, {KEY_COS, 'e'},
        {KEY_TAN, 'f'}, {KEY_FRAC, 'g'}, {KEY_FD, 'h'}, {KEY_LEFTP, 'i'}, {KEY_RIGHTP, 'j'},
        {KEY_COMMA, 'k'}, {KEY_ARROW, 'l'}, {KEY_7, 'm'}, {KEY_8, 'n'}, {KEY_9, 'o'},
        {KEY_4, 'p'}, {KEY_5, 'q'}, {KEY_6, 'r'}, {KEY_MUL, 's'}, {KEY_DIV, 't'},
        {KEY_1, 'u'}, {KEY_2, 'v'}, {KEY_3, 'w'}, {KEY_ADD, 'x'}, {KEY_SUB, 'y'},
        {KEY_0, 'z'}, {KEY_DOT, ' '},
    };
    for (unsigned i = 0; i < sizeof(letters) / sizeof(letters[0]); i++) {
        if (letters[i].key == key) {
            ch = letters[i].ch;
            return AK_CHAR;
        }
    }
    return AK_NONE;
}
