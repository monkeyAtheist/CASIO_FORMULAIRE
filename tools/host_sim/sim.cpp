// Simulateur PC : execute la vraie boucle de src/main.cpp avec de faux
// en-tetes gint, un clavier scripte et des captures d'ecran (PPM).
//
// Script (une commande par ligne) :
//   key NOM [n]      appuie n fois sur une touche (UP, DOWN, EXE, F1, A..Z...)
//   type texte       tape des lettres (recherche)
//   shot fichier     capture de l'ecran courant
//   fuzz n graine    n touches au hasard
//   # commentaire
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gint/display.h>
#include <gint/keyboard.h>
#include <gint/gint.h>

static uint16_t s_vram[DWIDTH * DHEIGHT];
static uint16_t s_screen[DWIDTH * DHEIGHT];
uint16_t *gint_vram = s_vram;
static FILE *s_script;
static int s_pendingKey = -1, s_pendingCount = 0;
static const char *s_typing = nullptr;
static char s_typeBuf[128];
static long s_fuzzLeft = 0;
static unsigned s_rng = 1;
static long s_frames = 0;

void dupdate(void)
{
    memcpy(s_screen, s_vram, sizeof(s_screen));
    s_frames++;
}

void gint_osmenu(void) { }
void gint_setrestart(int) { }

static void shot(const char *file)
{
    FILE *f = fopen(file, "wb");
    if (!f) { perror(file); return; }
    fprintf(f, "P6\n%d %d\n255\n", DWIDTH, DHEIGHT);
    for (int i = 0; i < DWIDTH * DHEIGHT; i++) {
        uint16_t c = s_screen[i];
        unsigned char px[3] = {(unsigned char)(((c >> 11) & 31) * 255 / 31),
                               (unsigned char)(((c >> 5) & 63) * 255 / 63),
                               (unsigned char)((c & 31) * 255 / 31)};
        fwrite(px, 1, 3, f);
    }
    fclose(f);
}

struct KN { const char *name; int key; };
static const KN s_keys[] = {
    {"UP", KEY_UP}, {"DOWN", KEY_DOWN}, {"LEFT", KEY_LEFT}, {"RIGHT", KEY_RIGHT},
    {"EXE", KEY_EXE}, {"EXIT", KEY_EXIT}, {"SHIFT", KEY_SHIFT}, {"ALPHA", KEY_ALPHA},
    {"MENU", KEY_MENU}, {"F1", KEY_F1}, {"F2", KEY_F2}, {"F3", KEY_F3}, {"F4", KEY_F4},
    {"F5", KEY_F5}, {"F6", KEY_F6}, {"DEL", KEY_DEL}, {"AC", KEY_ACON}, {"OPTN", KEY_OPTN},
    {"VARS", KEY_VARS},
};
static const int s_letterKeys[27] = {
    KEY_XOT, KEY_LOG, KEY_LN, KEY_SIN, KEY_COS, KEY_TAN, KEY_FRAC, KEY_FD, KEY_LEFTP,
    KEY_RIGHTP, KEY_COMMA, KEY_ARROW, KEY_7, KEY_8, KEY_9, KEY_4, KEY_5, KEY_6, KEY_MUL,
    KEY_DIV, KEY_1, KEY_2, KEY_3, KEY_ADD, KEY_SUB, KEY_0, KEY_DOT,
};
static const int s_allKeys[] = {
    KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_EXE, KEY_EXIT, KEY_SHIFT, KEY_F1, KEY_F2,
    KEY_F3, KEY_F4, KEY_F5, KEY_DEL, KEY_ACON, KEY_XOT, KEY_SIN, KEY_7, KEY_8, KEY_DOT,
    KEY_UP, KEY_DOWN, KEY_DOWN, KEY_EXE, KEY_EXIT, KEY_MENU, KEY_ALPHA, KEY_OPTN,
};

static int keyByName(const char *n)
{
    for (unsigned i = 0; i < sizeof(s_keys) / sizeof(s_keys[0]); i++)
        if (!strcmp(s_keys[i].name, n)) return s_keys[i].key;
    if (strlen(n) == 1 && n[0] >= 'A' && n[0] <= 'Z') return s_letterKeys[n[0] - 'A'];
    if (!strcmp(n, "SPACE")) return KEY_DOT;
    fprintf(stderr, "touche inconnue : %s\n", n);
    exit(2);
}

static key_event_t ev(int k)
{
    key_event_t e;
    memset(&e, 0, sizeof(e));
    e.key = (unsigned)k;
    e.type = KEYEV_DOWN;
    return e;
}

key_event_t getkey_opt(int, volatile int *)
{
    for (;;) {
        if (s_fuzzLeft > 0) {
            s_fuzzLeft--;
            s_rng = s_rng * 1103515245u + 12345u;
            int k = s_allKeys[(s_rng >> 16) % (sizeof(s_allKeys) / sizeof(s_allKeys[0]))];
            return ev(k);
        }
        if (s_pendingCount > 0) {
            s_pendingCount--;
            return ev(s_pendingKey);
        }
        if (s_typing && *s_typing) {
            char c = *s_typing++;
            if (c >= 'a' && c <= 'z') return ev(s_letterKeys[c - 'a']);
            if (c >= 'A' && c <= 'Z') return ev(s_letterKeys[c - 'A']);
            return ev(KEY_DOT);
        }
        char line[256];
        if (!s_script || !fgets(line, sizeof(line), s_script)) {
            fprintf(stderr, "fin du script (%ld images)\n", s_frames);
            exit(0);
        }
        char *nl = strchr(line, '\n');
        if (nl) *nl = 0;
        if (!line[0] || line[0] == '#') continue;
        char cmd[32], arg[200];
        arg[0] = 0;
        int n = 1;
        if (sscanf(line, "%31s %199s %d", cmd, arg, &n) < 1) continue;
        if (!strcmp(cmd, "key")) {
            s_pendingKey = keyByName(arg);
            s_pendingCount = n;
        } else if (!strcmp(cmd, "type")) {
            const char *t = line + 5;
            strncpy(s_typeBuf, t, sizeof(s_typeBuf) - 1);
            s_typeBuf[sizeof(s_typeBuf) - 1] = 0;
            s_typing = s_typeBuf;
        } else if (!strcmp(cmd, "shot")) {
            shot(arg);
        } else if (!strcmp(cmd, "fuzz")) {
            s_fuzzLeft = atol(arg);
            s_rng = (unsigned)n;
        } else {
            fprintf(stderr, "commande inconnue : %s\n", cmd);
        }
    }
}

extern int app_main(void);

int main(int argc, char **argv)
{
    if (argc > 1) {
        s_script = fopen(argv[1], "r");
        if (!s_script) { perror(argv[1]); return 1; }
    }
    int r = app_main();
    fprintf(stderr, "application terminee (%d), %ld images\n", r, s_frames);
    return 0;
}
