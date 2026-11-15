// Formulaire de mathematiques pour Casio Graph 90+E (gint).
//
// La boucle principale est volontairement dans main() : quand on appuie sur
// MENU, gint_osmenu() est appele avec une pile presque vide (meme methode que
// CASIO_GAME et EM_FIELD). L'application est une variable globale : son etat
// est conserve si on revient dans l'appli depuis le menu.
#include <stddef.h>
#include <gint/display.h>
#include <gint/keyboard.h>
#include <gint/gint.h>
#include "ui/App.hpp"
#include "ui/KeyMap.hpp"

int main(void)
{
    gint_setrestart(1);
    g_app.init();
    Canvas cv;
    for (;;) {
        cv.begin(gint_vram);
        g_app.draw(cv);
        dupdate();
        key_event_t ev = getkey_opt(GETKEY_REP_ARROWS, NULL);
        if (ev.key == KEY_MENU) {
            gint_osmenu();
            continue;
        }
        char ch;
        int k = mapGintKey(ev.key, ch);
        if (k == AK_NONE) continue;
        if (!g_app.key(k, ch)) break;
    }
    return 1;
}
