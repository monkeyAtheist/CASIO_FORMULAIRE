// Couleurs RGB565 (format de la VRAM de la Graph 90+E).
#pragma once
#include <stdint.h>

constexpr uint16_t rgb(unsigned r, unsigned g, unsigned b)
{
    return (uint16_t)(((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3));
}

// Melange de deux couleurs : t de 0 (a) a 16 (b).
uint16_t mix16(uint16_t a, uint16_t b, int t);

namespace col {
constexpr uint16_t white = rgb(255, 255, 255);
constexpr uint16_t black = rgb(0, 0, 0);
constexpr uint16_t ink = rgb(20, 24, 36);         // texte principal
constexpr uint16_t inkSoft = rgb(70, 76, 92);     // texte secondaire
constexpr uint16_t note = rgb(96, 104, 120);      // remarques
constexpr uint16_t line = rgb(206, 212, 222);     // filets
constexpr uint16_t pageBg = rgb(242, 244, 248);   // fond derriere les cartes
constexpr uint16_t fbar = rgb(38, 44, 58);        // barre des touches F
constexpr uint16_t fbarText = rgb(240, 242, 246);
constexpr uint16_t red = rgb(200, 30, 40);
constexpr uint16_t blue = rgb(20, 80, 200);
constexpr uint16_t green = rgb(10, 130, 60);
constexpr uint16_t orange = rgb(215, 105, 0);
constexpr uint16_t purple = rgb(130, 40, 170);
constexpr uint16_t teal = rgb(0, 125, 135);
constexpr uint16_t gray = rgb(110, 115, 125);
}
