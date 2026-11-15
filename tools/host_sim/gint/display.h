// Faux en-tete gint pour le simulateur PC (tools/host_sim).
#pragma once
#include <stdint.h>
#define DWIDTH 396
#define DHEIGHT 224
extern uint16_t *gint_vram;
void dupdate(void);
