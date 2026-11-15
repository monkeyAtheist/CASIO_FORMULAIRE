// Planche de test du moteur de formules : rend une liste de formules dans une
// image PPM (agrandie x3 en option) pour verification visuelle.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../../src/math/Math.hpp"
#include "../../src/font/Font.hpp"

static uint16_t fb[SCREEN_W * 4000];
static DrawOp ops[20000];

int main(int argc, char **argv)
{
    const char *out = argc > 1 ? argv[1] : "sheet.ppm";
    bool zoom = argc > 2 && !strcmp(argv[2], "zoom");
    static const char *forms[] = {
        "f(x) = x^2 + 3x - 1",
        "\\int_a^b u'(x)v(x)\\,dx = \\Big[u(x)v(x)\\Big]_a^b - \\int_a^b u(x)v'(x)\\,dx",
        "\\int_{\\varphi(a)}^{\\varphi(b)} f(x)\\,dx = \\int_a^b f(\\varphi(t))\\,\\varphi'(t)\\,dt",
        "\\sum_{k=0}^{n} q^k = \\frac{1-q^{n+1}}{1-q}",
        "\\left(\\frac{u}{v}\\right)' = \\frac{u'v - uv'}{v^2}",
        "\\sqrt{x^2+y^2} \\quad \\sqrt[3]{\\frac{a}{b}} \\quad \\sqrt{\\frac{1+\\cos x}{2}}",
        "\\vec{E} = \\frac{q}{4\\pi\\varepsilon_0 r^2}\\,\\vec{u}_r \\quad \\overrightarrow{AB}\\cdot\\overrightarrow{AC}",
        "\\begin{pmatrix} a & b \\\\ c & d \\end{pmatrix}^{-1} = \\frac{1}{ad-bc}\\begin{pmatrix} d & -b \\\\ -c & a \\end{pmatrix}",
        "\\det A = \\begin{vmatrix} a_{11} & a_{12} & a_{13} \\\\ a_{21} & a_{22} & a_{23} \\\\ a_{31} & a_{32} & a_{33} \\end{vmatrix}",
        "|x| = \\begin{cases} x & \\text{si } x \\geq 0 \\\\ -x & \\text{si } x < 0 \\end{cases}",
        "\\lim_{x \\to 0} \\frac{\\sin x}{x} = 1 \\qquad e^{i\\theta} = \\cos\\theta + i\\sin\\theta",
        "\\oint_{\\mathcal{C}} \\vec{B}\\cdot d\\vec{\\ell} = \\mu_0 I \\qquad \\iint_S \\overrightarrow{\\mathrm{rot}}\\,\\vec{A}\\cdot d\\vec{S}",
        "\\oiint_{\\Sigma} \\vec{E}\\cdot d\\vec{S} = \\frac{Q_{\\text{int}}}{\\varepsilon_0} \\quad \\iiint_V \\mathrm{div}\\,\\vec{E}\\,d\\tau",
        "\\vec{e}_x\\ \\vec{u}_r\\ \\vec{u}_{\\theta}\\ A_z\\ e_x\\ \\vec{E}\\ \\vec{\\nabla}f\\ \\overrightarrow{OM}\\ \\dot{r}\\ \\ddot{\\theta}",
        "u_n = u_0 + nr \\quad u_n \\xrightarrow[n\\to+\\infty]{} \\ell \\quad f \\underset{0}{\\sim} g",
        "\\binom{n}{k} = \\frac{n!}{k!\\,(n-k)!} \\quad (a+b)^n = \\sum_{k=0}^n \\binom{n}{k} a^k b^{n-k}",
        "x\\in\\mathbb{R},\\ n\\in\\mathbb{N}^*,\\ \\mathcal{C}^1 \\quad \\alpha\\beta\\gamma\\delta\\Delta\\Omega\\omega\\pi\\rho\\sigma\\theta",
        "\\left[ F(x) \\right]_a^b \\quad \\left\\{ x \\mid x > 0 \\right\\} \\quad \\left\\| \\vec{u} \\right\\| \\quad \\hat{u}\\ \\bar{z}\\ \\dot{x}\\ \\ddot{x}",
        "e^{x^2} \\quad x_{i}^{2} \\quad 2^{2^n} \\quad \\cos^2 x + \\sin^2 x = 1",
        "\\left( \\begin{matrix} 1 & 0 & 0 \\\\ 0 & 1 & 0 \\\\ 0 & 0 & 1 \\end{matrix} \\right) \\quad \\left(\\frac{\\frac{a}{b}}{\\frac{c}{d}}\\right)",
    };
    int n = sizeof(forms) / sizeof(forms[0]);
    for (int i = 0; i < SCREEN_W * 4000; i++) fb[i] = rgb(255, 255, 255);
    DisplayList dl;
    dl.init(ops, 20000);
    int y = 6;
    for (int i = 0; i < n; i++) {
        math::Box *b = math::typeset(forms[i], -1, math::ST_D, zoom);
        int base = y + b->h;
        math::emit(dl, b, 4, base, rgb(20, 24, 36));
        // repere de la boite
        y = base + b->d + 8;
        dl.rect(0, y - 4, 396, 1, rgb(230, 230, 240));
        if (b->w > 388) fprintf(stderr, "trop large (%d px) : %s\n", b->w, forms[i]);
        math::resetBoxes();
    }
    int H = y + 4;
    Canvas cv;
    cv.begin(fb);
    cv.setClip(0, 0, SCREEN_W, H);
    // Canvas suppose une hauteur d'ecran fixe pour le decoupage : on dessine
    // par bandes de 224 lignes.
    for (int band = 0; band < H; band += SCREEN_H) {
        cv.fb = fb + band * SCREEN_W;
        cv.setClip(0, 0, SCREEN_W, SCREEN_H);
        dl.draw(cv, 0, -band);
    }
    FILE *f = fopen(out, "wb");
    fprintf(f, "P6\n%d %d\n255\n", SCREEN_W, H);
    for (int i = 0; i < SCREEN_W * H; i++) {
        uint16_t c = fb[i];
        unsigned char px[3] = {(unsigned char)((c >> 11) << 3), (unsigned char)(((c >> 5) & 63) << 2),
                               (unsigned char)((c & 31) << 3)};
        fwrite(px, 1, 3, f);
    }
    fclose(f);
    printf("erreurs : %d  %s\nglyphes absents : %d\nboites (pic) : %d octets, noeuds : %d\n",
           math::g_errors, math::g_errors ? math::g_errMsg : "", font::g_missing,
           math::boxBytesPeak(), math::nodeBytesPeak());
    return 0;
}
