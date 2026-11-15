"""Rasteriseur de contours (sans hinting, couverture exacte en x, 16 sous-lignes en y).

Utilise pour les glyphes de la police mathematique qui n'ont pas de code
Unicode (variantes de taille, morceaux d'assemblage, variantes 'ssty').
"""
import numpy as np
from fontTools.pens.basePen import BasePen


class FlattenPen(BasePen):
    """Collecte les contours sous forme de segments (courbes subdivisees)."""

    def __init__(self, glyphSet, steps=12):
        super().__init__(glyphSet)
        self.contours = []
        self.cur = []
        self.steps = steps

    def _moveTo(self, p):
        self.cur = [p]

    def _lineTo(self, p):
        self.cur.append(p)

    def _curveToOne(self, p1, p2, p3):
        p0 = self.cur[-1]
        for i in range(1, self.steps + 1):
            t = i / self.steps
            mt = 1 - t
            x = mt**3*p0[0] + 3*mt*mt*t*p1[0] + 3*mt*t*t*p2[0] + t**3*p3[0]
            y = mt**3*p0[1] + 3*mt*mt*t*p1[1] + 3*mt*t*t*p2[1] + t**3*p3[1]
            self.cur.append((x, y))

    def _qCurveToOne(self, p1, p2):
        p0 = self.cur[-1]
        for i in range(1, self.steps + 1):
            t = i / self.steps
            mt = 1 - t
            x = mt*mt*p0[0] + 2*mt*t*p1[0] + t*t*p2[0]
            y = mt*mt*p0[1] + 2*mt*t*p1[1] + t*t*p2[1]
            self.cur.append((x, y))

    def _closePath(self):
        if len(self.cur) > 2:
            self.contours.append(self.cur)
        self.cur = []

    _endPath = _closePath


def glyph_edges(glyphSet, name):
    pen = FlattenPen(glyphSet)
    glyphSet[name].draw(pen)
    edges = []
    for c in pen.contours:
        n = len(c)
        for i in range(n):
            x0, y0 = c[i]
            x1, y1 = c[(i + 1) % n]
            if y0 != y1:
                edges.append((x0, y0, x1, y1))
    return edges


def rasterize(edges, scale, ox, oy, w, h, sub=16):
    """Remplit (regle non nulle) les segments donnes en unites de police.

    Point (x, y) police -> pixel (ox + x*scale, oy - y*scale) (y vers le bas).
    Renvoie un tableau float (h, w) de couverture 0..1.
    """
    cov = np.zeros((h, w + 2), dtype=np.float64)
    if not edges:
        return cov[:, :w]
    E = np.array(edges, dtype=np.float64)
    X0 = ox + E[:, 0] * scale
    Y0 = oy - E[:, 1] * scale
    X1 = ox + E[:, 2] * scale
    Y1 = oy - E[:, 3] * scale
    wind = np.where(Y1 > Y0, 1, -1)
    ymin = np.minimum(Y0, Y1)
    ymax = np.maximum(Y0, Y1)
    for py in range(h):
        for s in range(sub):
            y = py + (s + 0.5) / sub
            act = (ymin <= y) & (ymax > y)
            if not act.any():
                continue
            t = (y - Y0[act]) / (Y1[act] - Y0[act])
            xs = X0[act] + t * (X1[act] - X0[act])
            ws = wind[act]
            order = np.argsort(xs, kind='stable')
            xs = xs[order]
            ws = ws[order]
            acc = 0
            for i in range(len(xs) - 1):
                acc += ws[i]
                if acc != 0:
                    a = max(xs[i], 0.0)
                    b = min(xs[i + 1], float(w))
                    if b <= a:
                        continue
                    ia = int(np.floor(a))
                    ib = int(np.floor(b))
                    if ia == ib:
                        cov[py, ia] += (b - a) / sub
                    else:
                        cov[py, ia] += (ia + 1 - a) / sub
                        cov[py, ia + 1:ib] += 1.0 / sub
                        if ib < w:
                            cov[py, ib] += (b - ib) / sub
    return np.clip(cov[:, :w], 0.0, 1.0)


def rasterize_bold(edges, scale, ox, oy, w, h, embolden):
    """Comme rasterize, avec un epaississement (en pixels) : rendu x4,
    dilatation par un disque puis reduction."""
    if embolden <= 0:
        return rasterize(edges, scale, ox, oy, w, h)
    K = 4
    hi = rasterize(edges, scale * K, ox * K, oy * K, w * K, h * K, sub=4)
    r = embolden * K
    ir = int(np.ceil(r))
    out = hi.copy()
    for dy in range(-ir, ir + 1):
        for dx in range(-ir, ir + 1):
            if dx == 0 and dy == 0:
                continue
            dist = (dx * dx + dy * dy) ** 0.5
            if dist > r + 0.5:
                continue
            wgt = 1.0 if dist <= r - 0.5 else (r + 0.5 - dist)
            sh = np.zeros_like(hi)
            ys0, ys1 = max(0, dy), hi.shape[0] + min(0, dy)
            xs0, xs1 = max(0, dx), hi.shape[1] + min(0, dx)
            sh[ys0:ys1, xs0:xs1] = hi[ys0 - dy:ys1 - dy, xs0 - dx:xs1 - dx]
            out = np.maximum(out, sh * wgt)
    return out.reshape(h, K, w, K).mean(axis=(1, 3))
