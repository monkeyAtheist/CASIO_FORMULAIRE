#!/usr/bin/env python3
"""Generation des polices de Formulaire (Graph 90+E).

Produit src/gen/FontData.cpp et src/gen/FontIds.hpp a partir de :
  - DejaVu Math TeX Gyre (formules : lettres, symboles, grands operateurs,
    delimiteurs extensibles, radicaux, fleches, constantes MATH)
  - DejaVu Sans / DejaVu Sans Bold (textes)

Chaque glyphe est stocke en alpha 4 bits (2 pixels par octet).
Usage : python3 tools/fontgen/gen_fonts.py   (depuis la racine du projet)
Il faut Pillow, numpy et fontTools.
"""
import os
import sys
import math
import numpy as np
from PIL import Image, ImageDraw, ImageFont
from fontTools.ttLib import TTFont
from fontTools.ttLib.tables._c_m_a_p import CmapSubtable

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from raster import glyph_edges, rasterize, rasterize_bold  # noqa: E402

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', '..'))
OUT_DIR = os.path.join(ROOT, 'src', 'gen')

FONT_DIRS = ['/usr/share/fonts/truetype/dejavu',
             os.path.join(os.path.dirname(__file__), 'fonts')]


def find_font(name):
    for d in FONT_DIRS:
        p = os.path.join(d, name)
        if os.path.exists(p):
            return p
    raise SystemExit('Police introuvable : ' + name)


MATH_TTF = find_font('DejaVuMathTeXGyre.ttf')
SANS_TTF = find_font('DejaVuSans.ttf')
SANSB_TTF = find_font('DejaVuSans-Bold.ttf')

# --------------------------------------------------------------------------
# Tailles
# --------------------------------------------------------------------------
# Formules : (nom, px, variante ssty) ; ordre = Normal D/T, S, SS puis Zoom.
MATH_FACES = [
    ('MATH_N0', 16, ''),
    ('MATH_N1', 13, '.st'),
    ('MATH_N2', 10, '.sts'),
    ('MATH_Z0', 24, ''),
    ('MATH_Z1', 19, '.st'),
    ('MATH_Z2', 16, '.sts'),
]
TEXT_FACES = [
    ('SANS_12', SANS_TTF, 12),
    ('SANS_14', SANS_TTF, 14),
    ('SANS_16', SANS_TTF, 16),
    ('SANS_20', SANS_TTF, 20),
    ('BOLD_12', SANSB_TTF, 12),
    ('BOLD_14', SANSB_TTF, 14),
    ('BOLD_16', SANSB_TTF, 16),
    ('BOLD_20', SANSB_TTF, 20),
]

# --------------------------------------------------------------------------
# Jeux de caracteres
# --------------------------------------------------------------------------
def rng(a, b):
    return list(range(a, b + 1))


TEXT_CPS = (rng(0x20, 0x7E) + rng(0xA0, 0xFF) +
            [0x152, 0x153, 0x2013, 0x2014, 0x2018, 0x2019, 0x201C, 0x201D,
             0x2022, 0x2026, 0x2190, 0x2191, 0x2192, 0x2193, 0x21B5, 0x25B2,
             0x25BC, 0x25B6, 0x25C0, 0x2212, 0x2713, 0x2264, 0x2265, 0x2260,
             0x221E, 0x222B, 0x2211, 0x221A, 0x2202, 0x2207, 0x2208, 0x2248,
             0x203A, 0x2039, 0x00AB, 0x00BB,
             0x2032, 0x00B2, 0x00B3, 0x2074, 0x207F] +
            rng(0x3B1, 0x3C9) + rng(0x391, 0x3A9))

MATH_CPS = (rng(0x20, 0x7E) + rng(0xA0, 0xFF) + [0x152, 0x153, 0x131] +
            rng(0x1D400, 0x1D433) +                      # gras droit
            [c for c in rng(0x1D434, 0x1D467) if c != 0x1D455] + [0x210E] +  # italique
            [c for c in rng(0x1D538, 0x1D551)] +         # ajoure (avec trous)
            [0x2102, 0x210D, 0x2115, 0x2119, 0x211A, 0x211D, 0x2124] +
            [c for c in rng(0x1D49C, 0x1D4B5)] +         # calligraphique
            [0x212C, 0x2130, 0x2131, 0x210B, 0x2110, 0x2112, 0x2133, 0x211B] +
            rng(0x391, 0x3A9) + rng(0x3B1, 0x3C9) +      # grec droit
            rng(0x1D6FC, 0x1D71B) +                      # grec italique
            [0x1D6E4, 0x1D6E5, 0x1D6E9, 0x1D6EC, 0x1D6EF, 0x1D6F1, 0x1D6F4,
             0x1D6F6, 0x1D6F7, 0x1D6F9, 0x1D6FA] +
            [0x00B1, 0x00D7, 0x00F7, 0x00B7, 0x22C5, 0x2218, 0x2219, 0x2212,
             0x2213, 0x2217, 0x2297, 0x2295, 0x2299, 0x2227, 0x2228, 0x2229,
             0x222A, 0x2216, 0x2264, 0x2265, 0x2260, 0x2248, 0x2261, 0x2243,
             0x223C, 0x2245, 0x221D, 0x226A, 0x226B, 0x2282, 0x2283, 0x2286,
             0x2287, 0x2208, 0x2209, 0x220B, 0x22A5, 0x2225, 0x2226, 0x2192,
             0x2190, 0x2194, 0x21D2, 0x21D0, 0x21D4, 0x21A6, 0x27F6, 0x27F5,
             0x27F7, 0x27F9, 0x27F8, 0x27FA, 0x2191, 0x2193, 0x2197, 0x2198,
             0x2200, 0x2203, 0x2204, 0x00AC, 0x2205, 0x221E, 0x2202, 0x1D715,
             0x2207, 0x221A, 0x221B, 0x221C, 0x222B, 0x222C, 0x222D, 0x222E,
             0x222F, 0x2211, 0x220F, 0x2210, 0x22C3, 0x22C2, 0x2032, 0x2033,
             0x2034, 0x2026, 0x22EF, 0x22EE, 0x22F1, 0x00B0, 0x2113, 0x210F,
             0x27E8, 0x27E9, 0x230A, 0x230B, 0x2308, 0x2309, 0x2016, 0x2220,
             0x2234, 0x2235, 0x2223, 0x2224, 0x226C, 0x2270, 0x2271, 0x2A7D,
             0x2A7E, 0x2266, 0x2267, 0x227A, 0x227B, 0x2212, 0x2044, 0x2215,
             0x0302, 0x0304, 0x0305, 0x0307, 0x0308, 0x0303, 0x20D7, 0x030C,
             0x2190, 0x21C0, 0x21BC, 0x2135, 0x2118, 0x211C, 0x2111, 0x2127,
             0x29F8, 0x2013, 0x2014, 0x2018, 0x2019, 0x201C, 0x201D,
             # langage mathematique (v1.2)
             0x27E6, 0x27E7, 0x228A, 0x228B, 0x2201, 0x220E, 0x2284, 0x2285,
             0x2288, 0x2289, 0x22A4, 0x25A1])

# Bases extensibles : (code, vertical ?)
STRETCHY = [
    (0x28, True), (0x29, True), (0x5B, True), (0x5D, True), (0x7B, True),
    (0x7D, True), (0x7C, True), (0x2016, True), (0x27E8, True),
    (0x27E9, True), (0x230A, True), (0x230B, True), (0x2308, True),
    (0x2309, True), (0x221A, True),
    (0x222B, True), (0x222C, True), (0x222D, True), (0x222E, True),
    (0x222F, True), (0x2211, True), (0x220F, True), (0x22C3, True),
    (0x22C2, True),
    (0x2192, False), (0x2190, False), (0x2194, False), (0x21D2, False),
    (0x20D7, False), (0x0302, False), (0x0303, False),
]
MAX_VARIANTS = 8
MAX_PARTS = 5


# --------------------------------------------------------------------------
# Outils
# --------------------------------------------------------------------------
def to_alpha4(cov):
    """float 0..1 -> entiers 0..15 avec une legere correction gamma."""
    a = np.clip(cov, 0.0, 1.0) ** 0.85
    return np.clip(np.round(a * 15.0), 0, 15).astype(np.uint8)


def trim(a4):
    """Retire les lignes/colonnes vides. Renvoie (bitmap, dx, dy)."""
    if a4.size == 0 or not a4.any():
        return np.zeros((0, 0), np.uint8), 0, 0
    rows = np.where(a4.any(axis=1))[0]
    cols = np.where(a4.any(axis=0))[0]
    r0, r1 = rows[0], rows[-1] + 1
    c0, c1 = cols[0], cols[-1] + 1
    return a4[r0:r1, c0:c1], int(c0), int(r0)


class Blob:
    def __init__(self):
        self.data = bytearray()

    def add(self, a4):
        off = len(self.data)
        h, w = a4.shape
        for y in range(h):
            row = a4[y]
            for x in range(0, w, 2):
                hi = int(row[x])
                lo = int(row[x + 1]) if x + 1 < w else 0
                self.data.append((hi << 4) | lo)
        return off


class G:
    """Glyphe pret a etre emis."""
    __slots__ = ('key', 'off', 'adv', 'left', 'top', 'w', 'h', 'asc', 'desc',
                 'ic', 'acc')

    def __init__(self, key, off, adv, left, top, w, h, asc, desc, ic, acc):
        self.key, self.off, self.adv = key, off, adv
        self.left, self.top, self.w, self.h = left, top, w, h
        self.asc, self.desc, self.ic, self.acc = asc, desc, ic, acc


def clamp8(v):
    return max(-127, min(127, int(v)))


# --------------------------------------------------------------------------
# Police mathematique
# --------------------------------------------------------------------------
class MathFont:
    def __init__(self):
        self.tt = TTFont(MATH_TTF)
        self.gs = self.tt.getGlyphSet()
        self.cmap = self.tt.getBestCmap()
        self.upem = self.tt['head'].unitsPerEm
        self.order = set(self.tt.getGlyphOrder())
        m = self.tt['MATH'].table
        self.mc = m.MathConstants
        ic = m.MathGlyphInfo.MathItalicsCorrectionInfo
        self.ic = dict(zip(ic.Coverage.glyphs, [v.Value for v in ic.ItalicsCorrection]))
        ta = m.MathGlyphInfo.MathTopAccentAttachment
        self.ta = dict(zip(ta.TopAccentCoverage.glyphs, [v.Value for v in ta.TopAccentAttachment]))
        mv = m.MathVariants
        self.min_overlap = mv.MinConnectorOverlap
        self.vc = dict(zip(mv.VertGlyphCoverage.glyphs, mv.VertGlyphConstruction))
        self.hc = dict(zip(mv.HorizGlyphCoverage.glyphs, mv.HorizGlyphConstruction))
        self.bounds_cache = {}
        # police temporaire : noms sans code -> zone privee, pour FreeType
        self.pua = {}
        tmp = TTFont(MATH_TTF)
        sub = CmapSubtable.newSubtable(12)
        sub.platformID, sub.platEncID, sub.language = 3, 10, 0
        sub.cmap = {}
        for tb in tmp['cmap'].tables:
            if tb.format == 12:
                sub.cmap.update(tb.cmap)
        n = 0
        for name in tmp.getGlyphOrder():
            if name.endswith('.st') or name.endswith('.sts'):
                self.pua[name] = 0xF0000 + n
                sub.cmap[0xF0000 + n] = name
                n += 1
        tmp['cmap'].tables = [tb for tb in tmp['cmap'].tables if tb.format != 12] + [sub]
        self.tmp_path = '/tmp/formulaire_math_pua.ttf'
        tmp.save(self.tmp_path)

    def const(self, name):
        v = getattr(self.mc, name)
        return v.Value if hasattr(v, 'Value') else v

    def bounds(self, name):
        if name not in self.bounds_cache:
            from fontTools.pens.boundsPen import BoundsPen
            p = BoundsPen(self.gs)
            self.gs[name].draw(p)
            self.bounds_cache[name] = p.bounds or (0, 0, 0, 0)
        return self.bounds_cache[name]

    def adv(self, name):
        return self.tt['hmtx'][name][0]


def render_pil(font_path, px, ch):
    """Rendu FreeType (hinting) d'un caractere. Renvoie (a4, left, top, adv)."""
    font = ImageFont.truetype(font_path, px)
    S = px * 4
    img = Image.new('L', (S, S), 0)
    d = ImageDraw.Draw(img)
    ox, oy = px, px * 3
    d.text((ox, oy), ch, font=font, fill=255, anchor='ls')
    a = np.asarray(img).astype(np.float64) / 255.0
    a4 = to_alpha4(a)
    bm, dx, dy = trim(a4)
    adv = int(round(font.getlength(ch)))
    if bm.size == 0:
        return bm, 0, 0, adv
    return bm, dx - ox, oy - dy, adv


_font_cache = {}


def pil_font(path, px):
    k = (path, px)
    if k not in _font_cache:
        _font_cache[k] = ImageFont.truetype(path, px)
    return _font_cache[k]


def render_pil_fast(font_path, px, ch):
    font = pil_font(font_path, px)
    S = px * 4
    img = Image.new('L', (S, S), 0)
    d = ImageDraw.Draw(img)
    ox, oy = px, px * 3
    d.text((ox, oy), ch, font=font, fill=255, anchor='ls')
    a = np.asarray(img).astype(np.float64) / 255.0
    bm, dx, dy = trim(to_alpha4(a))
    adv = int(round(font.getlength(ch)))
    if bm.size == 0:
        return bm, 0, 0, adv
    return bm, dx - ox, oy - dy, adv


def embolden_for(px):
    """Epaississement des grands symboles (pixels) : plus lisibles sur l'ecran."""
    return 0.016 * px


BOLD_BASES = {0x222B, 0x222C, 0x222D, 0x222E, 0x222F, 0x2211, 0x220F, 0x22C3, 0x22C2}


def render_outline(mf, name, px, ox_frac=0.0, oy_frac=0.0, align_top=False, emb=0.0):
    """Rendu exact (sans hinting) d'un glyphe par son nom.

    Renvoie (a4 non rogne, left, top) : le pixel (0,0) du tableau est en
    (left, top) par rapport a l'origine (x vers la droite, top vers le haut).
    """
    s = px / mf.upem
    x0, y0, x1, y1 = mf.bounds(name)
    if align_top:
        oy_frac = math.ceil(y1 * s) - y1 * s
    left = int(math.floor(x0 * s + ox_frac)) - 1
    top = int(math.ceil(y1 * s + oy_frac)) + 1
    w = int(math.ceil(x1 * s + ox_frac)) - left + 2
    h = top - int(math.floor(y0 * s + oy_frac)) + 2
    pad = int(math.ceil(emb)) + 1 if emb > 0 else 0
    left -= pad
    top += pad
    w += 2 * pad
    h += 2 * pad
    edges = glyph_edges(mf.gs, name)
    cov = rasterize_bold(edges, s, -left + ox_frac, top + oy_frac, w, h, emb)
    return to_alpha4(cov), left, top


def math_glyph_metrics(mf, name, px):
    s = px / mf.upem
    x0, y0, x1, y1 = mf.bounds(name)
    asc = int(math.ceil(y1 * s - 0.3)) if y1 > 0 else -int(math.floor(-y1 * s + 0.3))
    desc = int(math.ceil(-y0 * s - 0.3)) if y0 < 0 else -int(math.floor(y0 * s + 0.3))
    ic = int(round(mf.ic.get(name, 0) * s))
    acc = mf.ta.get(name)
    acc = -128 if acc is None else clamp8(round(acc * s))
    return asc, desc, ic, acc


def build_math_face(mf, blob, px, ssty, pseudo):
    """pseudo : dict nom_de_glyphe -> cle (>= 0x110000) pour les variantes."""
    glyphs = []
    path = mf.tmp_path
    for cp in sorted(set(MATH_CPS)):
        name = mf.cmap.get(cp)
        if name is None:
            continue
        use = name
        ch = chr(cp)
        if ssty and (name + ssty) in mf.pua:
            use = name + ssty
            ch = chr(mf.pua[use])
        if cp in BOLD_BASES:
            a4, left, top = render_outline(mf, use, px, emb=embolden_for(px))
            bm, dx, dy = trim(a4)
            left += dx
            top -= dy
            adv = int(round(mf.adv(use) * px / mf.upem))
        elif cp in (0x0302, 0x0304, 0x0305, 0x0307, 0x0308, 0x0303, 0x20D7, 0x030C, 0x221A):
            # accents combinants et radical : rendu exact
            a4, left, top = render_outline(mf, use, px, align_top=(cp == 0x221A))
            if cp == 0x221A:
                a4 = fix_radical_top(a4)
            bm, dx, dy = trim(a4)
            left += dx
            top -= dy
            adv = int(round(mf.adv(use) * px / mf.upem))
        else:
            bm, left, top, adv = render_pil_fast(path, px, ch)
        asc, desc, ic, acc = math_glyph_metrics(mf, use, px)
        off = blob.add(bm)
        glyphs.append(G(cp, off, adv, left, top, bm.shape[1], bm.shape[0],
                        clamp8(asc), clamp8(desc), clamp8(ic), acc))
    # variantes et morceaux (rendu exact)
    for name, key in pseudo.items():
        info = PIECES.get(name)
        if info is None:
            is_rad = name.startswith('radical')
            emb = embolden_for(px) if name in BOLD_NAMES else 0.0
            a4, left, top = render_outline(mf, name, px, align_top=is_rad, emb=emb)
            if is_rad:
                a4 = fix_radical_top(a4)
            bm, dx, dy = trim(a4)
            left += dx
            top -= dy
        else:
            bm, left, top = render_piece(mf, name, px, info)
        asc, desc, ic, acc = math_glyph_metrics(mf, name, px)
        adv = int(round(mf.adv(name) * px / mf.upem))
        off = blob.add(bm)
        glyphs.append(G(key, off, adv, left, top, bm.shape[1], bm.shape[0],
                        clamp8(asc), clamp8(desc), clamp8(ic), acc))
    glyphs.sort(key=lambda g: g.key)
    return glyphs


def fix_radical_top(a4):
    """Rend pleine la premiere ligne du trait horizontal du radical, pour
    qu'elle se raccorde sans marche a la barre tracee par le programme."""
    a4 = a4.copy()
    rows = np.where(a4.any(axis=1))[0]
    if len(rows):
        r = rows[0]
        a4[r][a4[r] >= 6] = 15
    return a4


# Morceaux d'assemblage : nom -> (vertical, rogner_debut, rogner_fin, frame)
# rogner_* = longueur du connecteur (unites) a retirer du cote debut/fin.
PIECES = {}
PIECE_FRAME = {}
BOLD_NAMES = set()


def render_piece(mf, name, px, info):
    vertical, cut_start, cut_end, is_ext, frame = info
    s = px / mf.upem
    fx0, fy0, fx1, fy1 = frame
    x0, y0, x1, y1 = mf.bounds(name)
    if vertical:
        # cadre horizontal commun a tous les morceaux de l'assemblage
        left = int(math.floor(fx0 * s)) - 1
        w = int(math.ceil(fx1 * s)) - left + 1
        top = int(math.ceil(y1 * s)) + 1
        h = top - int(math.floor(y0 * s)) + 1
        emb = embolden_for(px) if name in BOLD_NAMES else 0.0
        pad = int(math.ceil(emb)) + 1 if emb > 0 else 0
        left -= pad
        w += 2 * pad
        top += pad
        h += 2 * pad
        edges = glyph_edges(mf.gs, name)
        cov = rasterize_bold(edges, s, -left, top, w, h, emb)
        a4 = to_alpha4(cov)
        if is_ext:
            mid = a4[h // 2:h // 2 + 1, :]
            return mid, left, 0
        # debut = bas (y0), fin = haut (y1)
        # lignes entierement dans la zone de connecteur -> retirees
        keep_top = 0
        keep_bot = h
        if cut_end > 0:
            ycut = (y1 - cut_end) * s       # au-dessus : connecteur droit
            # ligne r couvre [top - r - 1, top - r] en y ; on retire les lignes
            # entierement au-dessus de ycut
            r = 0
            while r < h and (top - r - 1) >= ycut:
                r += 1
            keep_top = max(0, r)
        if cut_start > 0:
            ycut = (y0 + cut_start) * s     # en dessous : connecteur droit
            r = h
            while r > 0 and (top - (r - 1)) <= ycut:
                r -= 1
            keep_bot = r
        a4 = a4[keep_top:keep_bot, :]
        top -= keep_top
        if cut_end == 0:      # extremite haute libre : retirer les lignes vides
            while a4.shape[0] > 1 and not a4[0].any():
                a4 = a4[1:, :]
                top -= 1
        if cut_start == 0:    # extremite basse libre
            while a4.shape[0] > 1 and not a4[-1].any():
                a4 = a4[:-1, :]
        return a4, left, top
    else:
        top = int(math.ceil(fy1 * s)) + 1
        h = top - int(math.floor(fy0 * s)) + 1
        left = int(math.floor(x0 * s)) - 1
        w = int(math.ceil(x1 * s)) - left + 1
        edges = glyph_edges(mf.gs, name)
        cov = rasterize(edges, s, -left, top, w, h)
        a4 = to_alpha4(cov)
        if is_ext:
            mid = a4[:, w // 2:w // 2 + 1]
            return mid, 0, top
        keep_l = 0
        keep_r = w
        if cut_start > 0:   # debut = gauche
            xcut = (x0 + cut_start) * s
            c = 0
            while c < w and (left + c + 1) <= xcut:
                c += 1
            keep_l = c
        if cut_end > 0:     # fin = droite
            xcut = (x1 - cut_end) * s
            c = w
            while c > 0 and (left + c - 1) >= xcut:
                c -= 1
            keep_r = c
        a4 = a4[:, keep_l:keep_r]
        left += keep_l
        if cut_start == 0:    # extremite gauche libre
            while a4.shape[1] > 1 and not a4[:, 0].any():
                a4 = a4[:, 1:]
                left += 1
        if cut_end == 0:      # extremite droite libre
            while a4.shape[1] > 1 and not a4[:, -1].any():
                a4 = a4[:, :-1]
        return a4, left, top


def collect_variants(mf):
    """Renvoie (liste des ensembles, dict nom -> cle pseudo)."""
    pseudo = {}
    sets = []

    for cp, vertical in STRETCHY:
        base = mf.cmap.get(cp)
        if base is None:
            continue
        table = mf.vc if vertical else mf.hc
        c = table.get(base)
        if c is None:
            continue
        variants = [v.VariantGlyph for v in c.MathGlyphVariantRecord][:MAX_VARIANTS]
        if cp in (0x222B, 0x222C, 0x222D, 0x222E, 0x222F):
            variants = variants[:5]   # au-dela : jamais utilise
        vkeys = []
        for v in variants:
            if v == base:
                vkeys.append(cp)
            else:
                if v not in pseudo:
                    pseudo[v] = 0x110000 + len(pseudo)
                vkeys.append(pseudo[v])
        parts = []
        if c.GlyphAssembly is not None:
            recs = c.GlyphAssembly.PartRecords
            # cadre commun
            bs = [mf.bounds(p.glyph) for p in recs]
            frame = (min(b[0] for b in bs), min(b[1] for b in bs),
                     max(b[2] for b in bs), max(b[3] for b in bs))
            for p in recs:
                ext = bool(p.PartFlags & 1)
                pname = p.glyph
                # un meme glyphe peut servir deux fois (accolade) : une seule cle
                info = (vertical, p.StartConnectorLength, p.EndConnectorLength, ext, frame)
                if pname in PIECES and PIECES[pname] != info:
                    print('ATTENTION : morceau %s utilise avec deux formes' % pname)
                PIECES[pname] = info
                if pname not in pseudo:
                    pseudo[pname] = 0x110000 + len(pseudo)
                parts.append((pseudo[pname], ext))
        if vertical and cp != 0x221A:
            for v in variants:
                if v != base:
                    BOLD_NAMES.add(v)
            if c.GlyphAssembly is not None:
                for p in c.GlyphAssembly.PartRecords:
                    BOLD_NAMES.add(p.glyph)
        sets.append((cp, vertical, vkeys, parts))
    return sets, pseudo


# --------------------------------------------------------------------------
# Constantes MATH -> pixels
# --------------------------------------------------------------------------
MC_FIELDS = [
    ('axis', 'AxisHeight'),
    ('fracRule', 'FractionRuleThickness'),
    ('fracNumUp', 'FractionNumeratorShiftUp'),
    ('fracNumUpD', 'FractionNumeratorDisplayStyleShiftUp'),
    ('fracDenDown', 'FractionDenominatorShiftDown'),
    ('fracDenDownD', 'FractionDenominatorDisplayStyleShiftDown'),
    ('fracNumGap', 'FractionNumeratorGapMin'),
    ('fracNumGapD', 'FractionNumDisplayStyleGapMin'),
    ('fracDenGap', 'FractionDenominatorGapMin'),
    ('fracDenGapD', 'FractionDenomDisplayStyleGapMin'),
    ('supUp', 'SuperscriptShiftUp'),
    ('supUpCramped', 'SuperscriptShiftUpCramped'),
    ('subDown', 'SubscriptShiftDown'),
    ('supBottomMin', 'SuperscriptBottomMin'),
    ('subTopMax', 'SubscriptTopMax'),
    ('subSupGap', 'SubSuperscriptGapMin'),
    ('supBottomMaxSub', 'SuperscriptBottomMaxWithSubscript'),
    ('supDropMax', 'SuperscriptBaselineDropMax'),
    ('subDropMin', 'SubscriptBaselineDropMin'),
    ('spaceAfterScript', 'SpaceAfterScript'),
    ('upLimGap', 'UpperLimitGapMin'),
    ('upLimRise', 'UpperLimitBaselineRiseMin'),
    ('lowLimGap', 'LowerLimitGapMin'),
    ('lowLimDrop', 'LowerLimitBaselineDropMin'),
    ('radGap', 'RadicalVerticalGap'),
    ('radGapD', 'RadicalDisplayStyleVerticalGap'),
    ('radRule', 'RadicalRuleThickness'),
    ('radExtra', 'RadicalExtraAscender'),
    ('radKernBefore', 'RadicalKernBeforeDegree'),
    ('radKernAfter', 'RadicalKernAfterDegree'),
    ('accentBase', 'AccentBaseHeight'),
    ('overGap', 'OverbarVerticalGap'),
    ('overRule', 'OverbarRuleThickness'),
    ('overExtra', 'OverbarExtraAscender'),
    ('stackTopUp', 'StackTopShiftUp'),
    ('stackTopUpD', 'StackTopDisplayStyleShiftUp'),
    ('stackBotDown', 'StackBottomShiftDown'),
    ('stackBotDownD', 'StackBottomDisplayStyleShiftDown'),
    ('stackGap', 'StackGapMin'),
    ('stackGapD', 'StackDisplayStyleGapMin'),
    ('delimMin', 'DelimitedSubFormulaMinHeight'),
    ('dispOpMin', 'DisplayOperatorMinHeight'),
    ('stretchGapAbove', 'StretchStackGapAboveMin'),
    ('stretchGapBelow', 'StretchStackGapBelowMin'),
]


def math_consts(mf, px):
    s = px / mf.upem
    out = []
    for field, name in MC_FIELDS:
        v = mf.const(name) * s
        iv = int(math.floor(v + 0.5))
        if field in ('fracRule', 'radRule', 'overRule'):
            iv = max(1, iv)
        out.append((field, iv))
    out.append(('radDegreeRaise', mf.const('RadicalDegreeBottomRaisePercent')))
    os2 = mf.tt['OS/2']
    out.append(('xHeight', int(round(os2.sxHeight * s))))
    out.append(('quad', px))
    out.append(('minOverlap', int(round(mf.min_overlap * s))))
    return out


# --------------------------------------------------------------------------
# Emission C++
# --------------------------------------------------------------------------
def emit_glyphs(f, name, glyphs):
    f.write('static const Glyph %s[] = {\n' % name)
    for g in glyphs:
        f.write('  {0x%06X,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d},\n' % (
            g.key, g.off, g.adv, g.left, g.top, g.w, g.h, g.asc, g.desc, g.ic, g.acc))
    f.write('};\n')


def main():
    os.makedirs(OUT_DIR, exist_ok=True)
    mf = MathFont()
    sets, pseudo = collect_variants(mf)
    blob = Blob()
    faces = []   # (id, glyphs, px, ascent, descent, isMath)

    for fid, px, ssty in MATH_FACES:
        gl = build_math_face(mf, blob, px, ssty, pseudo)
        hh = mf.tt['hhea']
        s = px / mf.upem
        faces.append((fid, gl, px, int(round(hh.ascent * s)), int(round(-hh.descent * s)), True))
        print('%s : %d glyphes, donnees %d octets' % (fid, len(gl), len(blob.data)))

    for fid, path, px in TEXT_FACES:
        tt = TTFont(path)
        cmap = tt.getBestCmap()
        gl = []
        for cp in sorted(set(TEXT_CPS)):
            if cp not in cmap:
                continue
            bm, left, top, adv = render_pil_fast(path, px, chr(cp))
            off = blob.add(bm)
            gl.append(G(cp, off, adv, left, top, bm.shape[1], bm.shape[0],
                        max(0, top), max(0, bm.shape[0] - top), 0, -128))
        gl.sort(key=lambda g: g.key)
        hh = tt['hhea']
        s = px / tt['head'].unitsPerEm
        # hauteurs utiles pour l'interligne (sans les accents des majuscules)
        gsx = tt.getGlyphSet()
        from fontTools.pens.boundsPen import BoundsPen
        bp = BoundsPen(gsx)
        gsx[cmap[ord('H')]].draw(bp)
        asc = int(math.ceil(bp.bounds[3] * s)) + 2
        desc = int(math.ceil(-tt['OS/2'].sTypoDescender * s))
        faces.append((fid, gl, px, asc, desc, False))
        print('%s : %d glyphes, donnees %d octets' % (fid, len(gl), len(blob.data)))

    # ---- en-tete des identifiants
    with open(os.path.join(OUT_DIR, 'FontIds.hpp'), 'w') as h:
        h.write('// Fichier genere par tools/fontgen/gen_fonts.py : ne pas modifier.\n')
        h.write('#pragma once\n\n')
        h.write('enum FaceId {\n')
        for i, fc in enumerate(faces):
            h.write('    %s = %d,\n' % (fc[0], i))
        h.write('    FACE_COUNT = %d\n};\n\n' % len(faces))
        h.write('enum { MATH_FACE_COUNT = %d, VARIANT_SET_COUNT = %d,\n' % (len(MATH_FACES), len(sets)))
        h.write('       MAX_VARIANTS = %d, MAX_PARTS = %d };\n' % (MAX_VARIANTS, MAX_PARTS))

    # ---- donnees
    with open(os.path.join(OUT_DIR, 'FontData.cpp'), 'w') as f:
        f.write('// Fichier genere par tools/fontgen/gen_fonts.py : ne pas modifier.\n')
        f.write('// Polices : DejaVu Sans, DejaVu Math TeX Gyre (licences dans README).\n')
        f.write('#include "../font/FontTypes.hpp"\n\n')
        f.write('static const uint8_t g_bits[%d] = {\n' % max(1, len(blob.data)))
        d = blob.data
        for i in range(0, len(d), 24):
            f.write('  ' + ','.join(str(b) for b in d[i:i + 24]) + ',\n')
        f.write('};\n\n')
        for fid, gl, px, asc, desc, is_math in faces:
            emit_glyphs(f, 'g_%s' % fid.lower(), gl)
        f.write('\nconst FontFace g_faces[FACE_COUNT] = {\n')
        for fid, gl, px, asc, desc, is_math in faces:
            f.write('  {g_%s, %d, g_bits, %d, %d, %d, %d},\n' % (
                fid.lower(), len(gl), px, asc, desc, 1 if is_math else 0))
        f.write('};\n\n')
        f.write('const MathConsts g_mathConsts[MATH_FACE_COUNT] = {\n')
        for fid, px, ssty in MATH_FACES:
            mc = math_consts(mf, px)
            f.write('  { // %s\n    ' % fid)
            f.write(', '.join('%d' % v for _, v in mc))
            f.write('\n  },\n')
        f.write('};\n\n')
        f.write('const VariantSet g_variantSets[VARIANT_SET_COUNT] = {\n')
        for cp, vertical, vkeys, parts in sets:
            vk = list(vkeys) + [0] * (MAX_VARIANTS - len(vkeys))
            pk = list(parts) + [(0, False)] * (MAX_PARTS - len(parts))
            f.write('  {0x%04X, %d, %d, %d, {%s}, {%s}},\n' % (
                cp, 1 if vertical else 0, len(vkeys), len(parts),
                ','.join('0x%06X' % k for k in vk),
                ','.join('{0x%06X,%d}' % (k, 1 if e else 0) for k, e in pk)))
        f.write('};\n')

    # ---- champs MathConsts pour l'en-tete C++ (verification)
    with open(os.path.join(OUT_DIR, 'MathConstsFields.inc'), 'w') as f:
        f.write('// Fichier genere : ordre des champs de MathConsts.\n')
        for field, _ in math_consts(mf, 16):
            f.write('    int16_t %s;\n' % field)
    print('Total bitmaps : %d octets, %d glyphes de variantes, %d ensembles' % (
        len(blob.data), len(pseudo), len(sets)))


if __name__ == '__main__':
    main()
