#!/usr/bin/env python3
"""Icones de l'add-in (92x64) : assets-cg/icon-uns.png et icon-sel.png."""
import os
from PIL import Image, ImageDraw, ImageFont

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', '..'))
MATH = '/usr/share/fonts/truetype/dejavu/DejaVuMathTeXGyre.ttf'
SANSB = '/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf'


def icon(selected):
    S = 4                       # sur-echantillonnage
    W, H = 92 * S, 64 * S
    img = Image.new('RGB', (W, H), (255, 255, 255))
    d = ImageDraw.Draw(img)
    if selected:
        top, bot = (40, 95, 200), (20, 50, 130)
    else:
        top, bot = (236, 241, 250), (206, 218, 240)
    for y in range(H):
        t = y / (H - 1)
        c = tuple(int(top[i] + (bot[i] - top[i]) * t) for i in range(3))
        d.line([(0, y), (W, y)], fill=c)
    d.rounded_rectangle([2 * S, 2 * S, W - 2 * S - 1, H - 2 * S - 1], radius=8 * S,
                        outline=(255, 255, 255) if selected else (90, 120, 190), width=2 * S)
    ink = (255, 255, 255) if selected else (25, 60, 150)
    acc = (255, 210, 90) if selected else (200, 60, 40)
    big = ImageFont.truetype(MATH, 40 * S)
    mid = ImageFont.truetype(MATH, 30 * S)
    sm = ImageFont.truetype(MATH, 15 * S)
    d.text((8 * S, 47 * S), '\u222B', font=big, fill=ink, anchor='ls')          # integrale
    d.text((33 * S, 19 * S), '\U0001D44F', font=sm, fill=ink, anchor='ls')       # b
    d.text((24 * S, 58 * S), '\U0001D44E', font=sm, fill=ink, anchor='ls')       # a
    d.text((43 * S, 42 * S), '\u2211', font=mid, fill=acc, anchor='ls')         # somme
    d.text((70 * S, 31 * S), '\u2202', font=ImageFont.truetype(MATH, 20 * S), fill=ink, anchor='ls')
    d.text((69 * S, 54 * S), '\u2207', font=ImageFont.truetype(MATH, 18 * S), fill=acc, anchor='ls')
    return img.resize((92, 64), Image.LANCZOS)


os.makedirs(os.path.join(ROOT, 'assets-cg'), exist_ok=True)
icon(False).save(os.path.join(ROOT, 'assets-cg', 'icon-uns.png'))
icon(True).save(os.path.join(ROOT, 'assets-cg', 'icon-sel.png'))
print('icones ecrites')
