#pragma once
#include <stdint.h>
enum {
    KEY_F1 = 0x91, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6,
    KEY_SHIFT = 0x81, KEY_OPTN, KEY_VARS, KEY_MENU, KEY_LEFT, KEY_UP,
    KEY_ALPHA = 0x71, KEY_SQUARE, KEY_POWER, KEY_EXIT, KEY_DOWN, KEY_RIGHT,
    KEY_XOT = 0x61, KEY_LOG, KEY_LN, KEY_SIN, KEY_COS, KEY_TAN,
    KEY_FRAC = 0x51, KEY_FD, KEY_LEFTP, KEY_RIGHTP, KEY_COMMA, KEY_ARROW,
    KEY_7 = 0x41, KEY_8, KEY_9, KEY_DEL,
    KEY_4 = 0x31, KEY_5, KEY_6, KEY_MUL, KEY_DIV,
    KEY_1 = 0x21, KEY_2, KEY_3, KEY_ADD, KEY_SUB,
    KEY_0 = 0x11, KEY_DOT, KEY_EXP, KEY_NEG, KEY_EXE,
    KEY_ACON = 0x07,
};
enum { KEYEV_NONE = 0, KEYEV_DOWN = 1, KEYEV_UP = 2, KEYEV_HOLD = 3 };
enum { GETKEY_REP_ARROWS = 0x20 };
typedef struct {
    unsigned key;
    unsigned type;
    unsigned shift, alpha, mod;
} key_event_t;
key_event_t getkey_opt(int options, volatile int *timeout);
