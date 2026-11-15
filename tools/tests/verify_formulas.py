#!/usr/bin/env python3
"""Verification numerique des formules du formulaire (numpy seulement).

Chaque test reprend une formule des fiches (src/content) et la compare a un
calcul numerique independant (derivees par differences finies, sommes,
integrales, champs de vecteurs aleatoires...). Usage :
    python3 tools/tests/verify_formulas.py
"""
import math
import cmath
import numpy as np

rng = np.random.default_rng(1)
FAILS = []
COUNT = [0]


def check(name, ok):
    COUNT[0] += 1
    if not ok:
        FAILS.append(name)
        print('ECHEC :', name)


def close(a, b, tol=1e-6):
    a, b = np.asarray(a, dtype=complex), np.asarray(b, dtype=complex)
    return bool(np.all(np.abs(a - b) <= tol * (1 + np.abs(b))))


def d(f, x, h=1e-5):
    return (f(x + h) - f(x - h)) / (2 * h)


def d2(f, x, h=1e-4):
    return (f(x + h) - 2 * f(x) + f(x - h)) / (h * h)


def deriv_ok(name, f, fp, xs):
    check('derivee ' + name, all(close(d(f, x), fp(x), 1e-5) for x in xs))


def prim_ok(name, F, f, xs):
    check('primitive ' + name, all(close(d(F, x), f(x), 1e-5) for x in xs))


sh, ch, th = np.sinh, np.cosh, np.tanh
argsh, argch, argth = np.arcsinh, np.arccosh, np.arctanh
X = [0.3, 0.7, 1.3, 2.1]
XS = [0.2, -0.4, 0.55]      # dans ]-1,1[
XN = [-1.7, -0.6, 0.4, 1.9]  # reels non nuls

# ---------------------------------------------------------------- derivees
for n in (1, 2, 5):
    deriv_ok('x^%d' % n, lambda x: x**n, lambda x: n * x**(n - 1), X)
    deriv_ok('1/x^%d' % n, lambda x: 1 / x**n, lambda x: -n / x**(n + 1), XN)
deriv_ok('1/x', lambda x: 1 / x, lambda x: -1 / x**2, XN)
deriv_ok('x^a', lambda x: x**1.7, lambda x: 1.7 * x**0.7, X)
deriv_ok('sqrt', np.sqrt, lambda x: 1 / (2 * np.sqrt(x)), X)
deriv_ok('racine 3', lambda x: x**(1 / 3), lambda x: 1 / (3 * (x**2)**(1 / 3)), X)
deriv_ok('a^x', lambda x: 2.5**x, lambda x: 2.5**x * math.log(2.5), X)
deriv_ok('ln|x|', lambda x: np.log(abs(x)), lambda x: 1 / x, XN)
deriv_ok('log_a', lambda x: np.log(x) / np.log(3), lambda x: 1 / (x * np.log(3)), X)
deriv_ok('tan', np.tan, lambda x: 1 + np.tan(x)**2, X[:2])
deriv_ok('tan bis', np.tan, lambda x: 1 / np.cos(x)**2, X[:2])
cot = lambda x: 1 / np.tan(x)
deriv_ok('cotan', cot, lambda x: -1 - cot(x)**2, X)
deriv_ok('cotan bis', cot, lambda x: -1 / np.sin(x)**2, X)
deriv_ok('arcsin', np.arcsin, lambda x: 1 / np.sqrt(1 - x**2), XS)
deriv_ok('arccos', np.arccos, lambda x: -1 / np.sqrt(1 - x**2), XS)
deriv_ok('arctan', np.arctan, lambda x: 1 / (1 + x**2), XN)
deriv_ok('th', th, lambda x: 1 - th(x)**2, XN)
deriv_ok('th bis', th, lambda x: 1 / ch(x)**2, XN)
deriv_ok('argsh', argsh, lambda x: 1 / np.sqrt(x**2 + 1), XN)
deriv_ok('argch', argch, lambda x: 1 / np.sqrt(x**2 - 1), [1.3, 2.1, 4.0])
deriv_ok('argth', argth, lambda x: 1 / (1 - x**2), XS)
# formes composees, u = 1 + x^2 / 3 + sin x
u = lambda x: 1.2 + x**2 / 3 + 0.3 * np.sin(x)
up = lambda x: 2 * x / 3 + 0.3 * np.cos(x)
vv = lambda x: 0.5 + 0.2 * x
vp = lambda x: 0.2
deriv_ok('u^n', lambda x: u(x)**4, lambda x: 4 * up(x) * u(x)**3, X)
deriv_ok('1/u', lambda x: 1 / u(x), lambda x: -up(x) / u(x)**2, X)
deriv_ok('sqrt u', lambda x: np.sqrt(u(x)), lambda x: up(x) / (2 * np.sqrt(u(x))), X)
deriv_ok('e^u', lambda x: np.exp(u(x)), lambda x: up(x) * np.exp(u(x)), X)
deriv_ok('ln u', lambda x: np.log(u(x)), lambda x: up(x) / u(x), X)
deriv_ok('tan u', lambda x: np.tan(0.3 * u(x)), lambda x: 0.3 * up(x) / np.cos(0.3 * u(x))**2, X[:2])
deriv_ok('arctan u', lambda x: np.arctan(u(x)), lambda x: up(x) / (1 + u(x)**2), X)
w = lambda x: 0.4 * np.sin(x)
wp = lambda x: 0.4 * np.cos(x)
deriv_ok('arcsin u', lambda x: np.arcsin(w(x)), lambda x: wp(x) / np.sqrt(1 - w(x)**2), X)
deriv_ok('a^u', lambda x: 3.0**u(x), lambda x: up(x) * 3.0**u(x) * math.log(3), X)
deriv_ok('u^v', lambda x: u(x)**vv(x),
         lambda x: (vp(x) * np.log(u(x)) + vv(x) * up(x) / u(x)) * u(x)**vv(x), X)
deriv_ok('u/v', lambda x: u(x) / vv(x), lambda x: (up(x) * vv(x) - u(x) * vp(x)) / vv(x)**2, X)
deriv_ok('uvw', lambda x: u(x) * vv(x) * w(x),
         lambda x: up(x) * vv(x) * w(x) + u(x) * vp(x) * w(x) + u(x) * vv(x) * wp(x), X)
# reciproque : f = x^3 + x, (f^-1)'(y) = 1 / f'(f^-1(y))
f3 = lambda x: x**3 + x
def f3inv(y):
    x = 0.0
    for _ in range(60):
        x -= (f3(x) - y) / (3 * x * x + 1)
    return x
deriv_ok('reciproque', f3inv, lambda y: 1 / (3 * f3inv(y)**2 + 1), [0.5, 2.0, 5.0])
# Leibniz n = 3 avec u = e^{2x}, v = x^4
U = [lambda x, k=k: 2**k * np.exp(2 * x) for k in range(4)]
V = [lambda x: x**4, lambda x: 4 * x**3, lambda x: 12 * x**2, lambda x: 24 * x]
x0 = 0.7
lhs = sum(math.comb(3, k) * U[k](x0) * V[3 - k](x0) for k in range(4))
# derivee 3e exacte de e^{2x} x^4
exact = np.exp(2 * x0) * (8 * x0**4 + 48 * x0**3 + 72 * x0**2 + 24 * x0)
check('Leibniz', close(lhs, exact))

# ---------------------------------------------------------------- primitives
prim_ok('1/x^n', lambda x: -1 / ((4 - 1) * x**3), lambda x: 1 / x**4, XN)
prim_ok('sqrt', lambda x: 2 / 3 * x * np.sqrt(x), np.sqrt, X)
prim_ok('a^x', lambda x: 2.5**x / math.log(2.5), lambda x: 2.5**x, X)
prim_ok('ln', lambda x: x * np.log(x) - x, np.log, X)
prim_ok('x e^ax', lambda x: (x / 1.5 - 1 / 1.5**2) * np.exp(1.5 * x), lambda x: x * np.exp(1.5 * x), X)
prim_ok('tan', lambda x: -np.log(abs(np.cos(x))), np.tan, X)
prim_ok('cotan', lambda x: np.log(abs(np.sin(x))), cot, X)
prim_ok('1/sin^2', lambda x: -cot(x), lambda x: 1 / np.sin(x)**2, X)
prim_ok('sin(ax+b)', lambda x: -np.cos(2 * x + 1) / 2, lambda x: np.sin(2 * x + 1), X)
prim_ok('sin^2', lambda x: x / 2 - np.sin(2 * x) / 4, lambda x: np.sin(x)**2, X)
prim_ok('cos^2', lambda x: x / 2 + np.sin(2 * x) / 4, lambda x: np.cos(x)**2, X)
prim_ok('1/cos', lambda x: np.log(abs(np.tan(x / 2 + np.pi / 4))), lambda x: 1 / np.cos(x), X[:3])
prim_ok('1/sin', lambda x: np.log(abs(np.tan(x / 2))), lambda x: 1 / np.sin(x), X)
prim_ok('1/(a2+x2)', lambda x: np.arctan(x / 2) / 2, lambda x: 1 / (4 + x**2), XN)
prim_ok('1/sqrt(a2-x2)', lambda x: np.arcsin(x / 2), lambda x: 1 / np.sqrt(4 - x**2), [0.3, -1.2, 1.5])
prim_ok('1/sqrt(x2+1)', lambda x: np.log(x + np.sqrt(x**2 + 1)), lambda x: 1 / np.sqrt(x**2 + 1), XN)
prim_ok('1/sqrt(x2-1)', lambda x: np.log(abs(x + np.sqrt(x**2 - 1))), lambda x: 1 / np.sqrt(x**2 - 1), [1.3, -2.0, 3.1])
prim_ok('1/sqrt(x2+k)', lambda x: np.log(abs(x + np.sqrt(x**2 - 2))), lambda x: 1 / np.sqrt(x**2 - 2), [1.6, -2.0, 3.1])
prim_ok('1/(1-x2)', lambda x: 0.5 * np.log(abs((1 + x) / (1 - x))), lambda x: 1 / (1 - x**2), [0.3, 2.0, -3.0])
prim_ok('1/(x2-a2)', lambda x: np.log(abs((x - 2) / (x + 2))) / 4, lambda x: 1 / (x**2 - 4), [0.3, 3.0, -5.0])
prim_ok('th', lambda x: np.log(ch(x)), th, XN)
prim_ok('u^a u', lambda x: u(x)**2.5 / 2.5, lambda x: up(x) * u(x)**1.5, X)
prim_ok('u/u^n', lambda x: -1 / (2 * u(x)**2), lambda x: up(x) / u(x)**3, X)
prim_ok('u/sqrt u', lambda x: 2 * np.sqrt(u(x)), lambda x: up(x) / np.sqrt(u(x)), X)
prim_ok('u/cos2 u', lambda x: np.tan(0.3 * u(x)), lambda x: 0.3 * up(x) / np.cos(0.3 * u(x))**2, X[:2])
a, b = 1.3, -0.4
check('elements simples', close(1 / ((2.0 - a) * (2.0 - b)), (1 / (a - b)) * (1 / (2.0 - a) - 1 / (2.0 - b))))
A2, B2, C2 = 2.0, 1.0, 3.0
D = B2**2 - 4 * A2 * C2
prim_ok('trinome', lambda x: 2 / math.sqrt(-D) * np.arctan((2 * A2 * x + B2) / math.sqrt(-D)),
        lambda x: 1 / (A2 * x * x + B2 * x + C2), XN)
prim_ok('(x+p)^2+q^2', lambda x: np.arctan((x + 1) / 2) / 2, lambda x: 1 / ((x + 1)**2 + 4), XN)
prim_ok('Delta=0', lambda x: -1 / (2 * (x - 1)), lambda x: 1 / (2 * (x - 1)**2), [2.0, -1.0])

# ---------------------------------------------------------------- integrales
def integ(f, a, b, n=20000):
    t = np.linspace(a, b, n + 1)
    y = f(t)
    return (b - a) / n * (y.sum() - (y[0] + y[-1]) / 2)
check('Gauss', close(integ(lambda x: np.exp(-x * x), -10, 10), math.sqrt(math.pi)))
check('int ln', close(integ(np.log, 1e-12, 1, 400000), -1, 1e-4))
check('int t^n e^-t', close(integ(lambda t: t**4 * np.exp(-t), 0, 60), 24))
check('d/dx int_u^v', close(d(lambda x: integ(np.cos, x**2, 3 * x, 2000), 0.8),
                            np.cos(3 * 0.8) * 3 - np.cos(0.8**2) * 2 * 0.8, 1e-5))
# t = tan(x/2)
for x in (0.4, 1.1, 2.5):
    t = math.tan(x / 2)
    check('t=tan(x/2)', close([math.cos(x), math.sin(x), math.tan(x)],
                              [(1 - t * t) / (1 + t * t), 2 * t / (1 + t * t), 2 * t / (1 - t * t)]))

# ---------------------------------------------------------------- DL
xs = 1e-2
def dl(name, f, poly, order):
    err = abs(f(xs) - poly(xs))
    check('DL ' + name, err < 50 * xs**order)
dl('exp', np.exp, lambda x: 1 + x + x**2 / 2 + x**3 / 6, 4)
dl('ch', ch, lambda x: 1 + x**2 / 2 + x**4 / 24, 6)
dl('sh', sh, lambda x: x + x**3 / 6 + x**5 / 120, 7)
dl('cos', np.cos, lambda x: 1 - x**2 / 2 + x**4 / 24, 6)
dl('sin', np.sin, lambda x: x - x**3 / 6 + x**5 / 120, 7)
dl('tan', np.tan, lambda x: x + x**3 / 3 + 2 * x**5 / 15, 7)
dl('ln', lambda x: np.log(1 + x), lambda x: x - x**2 / 2 + x**3 / 3, 4)
dl('(1+x)^a', lambda x: (1 + x)**0.7, lambda x: 1 + 0.7 * x + 0.7 * (-0.3) / 2 * x**2, 3)
dl('sqrt', lambda x: np.sqrt(1 + x), lambda x: 1 + x / 2 - x**2 / 8 + x**3 / 16, 4)
dl('1/sqrt', lambda x: 1 / np.sqrt(1 + x), lambda x: 1 - x / 2 + 3 * x**2 / 8, 3)
dl('arctan', np.arctan, lambda x: x - x**3 / 3 + x**5 / 5, 7)
dl('arcsin', np.arcsin, lambda x: x + x**3 / 6 + 3 * x**5 / 40, 7)
dl('argth', argth, lambda x: x + x**3 / 3 + x**5 / 5, 7)
check('Stirling', close(math.factorial(60) / (math.sqrt(2 * math.pi * 60) * (60 / math.e)**60), 1, 2e-3))
check('harmonique', close(sum(1 / k for k in range(1, 100001)) - math.log(100000), 0.5772156649, 1e-5))

# ---------------------------------------------------------------- suites, sommes, series
n = 23
check('somme k', sum(range(1, n + 1)) == n * (n + 1) // 2)
check('somme k^2', sum(k * k for k in range(1, n + 1)) == n * (n + 1) * (2 * n + 1) // 6)
check('somme k^3', sum(k**3 for k in range(1, n + 1)) == (n * (n + 1) // 2)**2)
q = 0.73
check('geometrique', close(sum(q**k for k in range(n + 1)), (1 - q**(n + 1)) / (1 - q)))
check('geometrique p..n', close(sum(2 * q**k for k in range(4, n + 1)), 2 * q**4 * (1 - q**(n - 4 + 1)) / (1 - q)))
check('arith somme', close(sum(3 + 2 * k for k in range(n + 1)), (n + 1) * (3 + (3 + 2 * n)) / 2))
aa, bb, u0 = 0.6, 2.0, 7.0
l = bb / (1 - aa)
uu = u0
for k in range(10):
    uu = aa * uu + bb
check('arith-geom', close(uu - l, aa**10 * (u0 - l)))
phi, psi = (1 + 5**0.5) / 2, (1 - 5**0.5) / 2
F = [0, 1]
for k in range(30):
    F.append(F[-1] + F[-2])
check('Fibonacci', all(close(F[k], (phi**k - psi**k) / 5**0.5) for k in range(30)))
check('telescopique', close(sum(1 / (k * (k + 1)) for k in range(1, n + 1)), 1 - 1 / (n + 1)))
check('a^n-b^n', close(3.0**7 - 2.0**7, (3 - 2) * sum(3.0**k * 2.0**(6 - k) for k in range(7))))
check('binome', close((1.3 + 0.4)**9, sum(math.comb(9, k) * 1.3**k * 0.4**(9 - k) for k in range(10))))
check('Pascal', math.comb(9, 4) + math.comb(9, 5) == math.comb(10, 5))
check('zeta(2)', close(sum(1 / k**2 for k in range(1, 200001)), math.pi**2 / 6, 1e-5))
check('ln 2', close(sum((-1)**(k + 1) / k for k in range(1, 200001)), math.log(2), 1e-5))
check('sum n q^(n-1)', close(sum(k * q**(k - 1) for k in range(1, 400)), 1 / (1 - q)**2))
# recurrence d'ordre 2, racines complexes : u(n+2) = 2 cos(t) r u(n+1) - r^2 u(n)
r, t = 0.9, 0.7
ua = [1.0, 0.5]
for k in range(20):
    ua.append(2 * r * math.cos(t) * ua[-1] - r * r * ua[-2])
lam = 1.0
mu = (0.5 / r - math.cos(t)) / math.sin(t)
check('recurrence complexe', all(close(ua[k], r**k * (lam * math.cos(k * t) + mu * math.sin(k * t))) for k in range(20)))
# Fourier : signal carre impair -> b_n = 4/(n pi) pour n impair ; Parseval
check('Parseval carre', close(0.5 * sum((4 / (k * math.pi))**2 for k in range(1, 200001, 2)), 1, 1e-4))

# ---------------------------------------------------------------- trigonometrie
for (A, B) in [(0.3, 1.1), (2.0, -0.7)]:
    s, c = math.sin, math.cos
    check('cos(a+b)', close(c(A + B), c(A) * c(B) - s(A) * s(B)))
    check('cos(a-b)', close(c(A - B), c(A) * c(B) + s(A) * s(B)))
    check('sin(a+b)', close(s(A + B), s(A) * c(B) + c(A) * s(B)))
    check('sin(a-b)', close(s(A - B), s(A) * c(B) - c(A) * s(B)))
    check('tan(a+b)', close(math.tan(A + B), (math.tan(A) + math.tan(B)) / (1 - math.tan(A) * math.tan(B))))
    check('cos2a', close(c(2 * A), [c(A)**2 - s(A)**2, 2 * c(A)**2 - 1, 1 - 2 * s(A)**2]))
    check('sin2a tan2a', close([s(2 * A), math.tan(2 * A)], [2 * s(A) * c(A), 2 * math.tan(A) / (1 - math.tan(A)**2)]))
    check('linearisation', close([c(A) * c(B), s(A) * s(B), s(A) * c(B)],
                                 [0.5 * (c(A - B) + c(A + B)), 0.5 * (c(A - B) - c(A + B)), 0.5 * (s(A + B) + s(A - B))]))
    p, qq = A, B
    check('somme-produit', close([c(p) + c(qq), c(p) - c(qq), s(p) + s(qq), s(p) - s(qq)],
                                 [2 * c((p + qq) / 2) * c((p - qq) / 2), -2 * s((p + qq) / 2) * s((p - qq) / 2),
                                  2 * s((p + qq) / 2) * c((p - qq) / 2), 2 * s((p - qq) / 2) * c((p + qq) / 2)]))
    check('ch(a+b) sh(a+b)', close([math.cosh(A + B), math.sinh(A + B)],
                                   [math.cosh(A) * math.cosh(B) + math.sinh(A) * math.sinh(B),
                                    math.sinh(A) * math.cosh(B) + math.cosh(A) * math.sinh(B)]))
xx = 0.8
for th_, (cc, ss) in [(-xx, (math.cos(xx), -math.sin(xx))), (math.pi - xx, (-math.cos(xx), math.sin(xx))),
                      (math.pi + xx, (-math.cos(xx), -math.sin(xx))), (math.pi / 2 - xx, (math.sin(xx), math.cos(xx))),
                      (math.pi / 2 + xx, (-math.sin(xx), math.cos(xx)))]:
    check('angles associes', close([math.cos(th_), math.sin(th_)], [cc, ss]))
vals = {0: (0, 1), math.pi / 6: (0.5, 3**0.5 / 2), math.pi / 4: (2**0.5 / 2, 2**0.5 / 2), math.pi / 3: (3**0.5 / 2, 0.5)}
for ang, (s_, c_) in vals.items():
    check('valeurs remarquables', close([math.sin(ang), math.cos(ang)], [s_, c_]))
check('tan pi/6', close(math.tan(math.pi / 6), 3**0.5 / 3))
aa, bb = 1.2, -0.7
R = math.hypot(aa, bb)
ph = math.atan2(bb, aa)
check('a cos + b sin', close(aa * math.cos(0.9) + bb * math.sin(0.9), R * math.cos(0.9 - ph)))
check('arcsin+arccos', close(math.asin(0.3) + math.acos(0.3), math.pi / 2))
check('arctan x + arctan 1/x', close(math.atan(2.3) + math.atan(1 / 2.3), math.pi / 2))
check('cos(arcsin)', close([math.cos(math.asin(0.4)), math.sin(math.acos(0.4))], [math.sqrt(1 - 0.16)] * 2))
check('argsh argch argth', close([math.asinh(1.3), math.acosh(1.3), math.atanh(0.4)],
                                 [math.log(1.3 + math.sqrt(1.3**2 + 1)), math.log(1.3 + math.sqrt(1.3**2 - 1)),
                                  0.5 * math.log(1.4 / 0.6)]))

# ---------------------------------------------------------------- complexes
for th_ in (0.4, 2.2):
    e = cmath.exp(1j * th_)
    check('Euler', close([math.cos(th_), math.sin(th_)], [(e + 1 / e) / 2, (e - 1 / e) / 2j]))
    check('1+e^it', close(1 + e, 2 * math.cos(th_ / 2) * cmath.exp(1j * th_ / 2)))
    check('1-e^it', close(1 - e, -2j * math.sin(th_ / 2) * cmath.exp(1j * th_ / 2)))
    check('e^ia+e^ib', close(cmath.exp(0.3j) + e, 2 * math.cos((0.3 - th_) / 2) * cmath.exp(1j * (0.3 + th_) / 2)))
    check('Moivre', close((math.cos(th_) + 1j * math.sin(th_))**5, math.cos(5 * th_) + 1j * math.sin(5 * th_)))
check('racines unite', close(sum(cmath.exp(2j * math.pi * k / 7) for k in range(7)), 0, 1e-12))
z = -1.5 + 0.8j
check('arg a<0', close(math.atan(z.imag / z.real) + math.pi, cmath.phase(z)))
check('1/z', close(1 / z, z.conjugate() / abs(z)**2))
zr = 2 * cmath.exp(0.9j)
check('racines n-iemes', all(close((2**(1 / 3) * cmath.exp(1j * (0.9 + 2 * k * math.pi) / 3))**3, zr) for k in range(3)))

# ---------------------------------------------------------------- vecteurs
for _ in range(5):
    a_, b_, c_ = rng.normal(size=3), rng.normal(size=3), rng.normal(size=3)
    check('double produit', close(np.cross(a_, np.cross(b_, c_)), np.dot(a_, c_) * b_ - np.dot(a_, b_) * c_))
    check('Lagrange', close(np.dot(np.cross(a_, b_), np.cross(a_, b_)) + np.dot(a_, b_)**2,
                            np.dot(a_, a_) * np.dot(b_, b_)))
    check('polarisation', close(np.dot(a_, b_), 0.25 * (np.sum((a_ + b_)**2) - np.sum((a_ - b_)**2))))
    m = np.linalg.det(np.array([a_, b_, c_]))
    check('produit mixte', close([np.dot(np.cross(a_, b_), c_), np.dot(np.cross(b_, c_), a_), -np.dot(np.cross(b_, a_), c_)], [m] * 3))
    comp = np.array([a_[1] * b_[2] - a_[2] * b_[1], a_[2] * b_[0] - a_[0] * b_[2], a_[0] * b_[1] - a_[1] * b_[0]])
    check('composantes produit vectoriel', close(comp, np.cross(a_, b_)))
    # distance point-plan et point-droite
    nrm = rng.normal(size=3)
    dd = 0.7
    M = rng.normal(size=3)
    P0 = -dd * nrm / np.dot(nrm, nrm)
    proj = M - np.dot(M - P0, nrm) / np.dot(nrm, nrm) * nrm
    check('distance plan', close(np.linalg.norm(M - proj), abs(np.dot(nrm, M) + dd) / np.linalg.norm(nrm)))
    A_, U_ = rng.normal(size=3), rng.normal(size=3)
    tt = np.dot(M - A_, U_) / np.dot(U_, U_)
    check('distance droite', close(np.linalg.norm(M - A_ - tt * U_), np.linalg.norm(np.cross(M - A_, U_)) / np.linalg.norm(U_)))
# Al-Kashi et sinus
Ap, Bp, Cp = np.array([0.0, 0]), np.array([3.0, 0.4]), np.array([1.0, 2.2])
aL, bL, cL = np.linalg.norm(Bp - Cp), np.linalg.norm(Ap - Cp), np.linalg.norm(Ap - Bp)
angA = math.acos(np.dot(Bp - Ap, Cp - Ap) / (bL * cL))
angB = math.acos(np.dot(Ap - Bp, Cp - Bp) / (aL * cL))
check('Al-Kashi', close(aL**2, bL**2 + cL**2 - 2 * bL * cL * math.cos(angA)))
check('loi des sinus', close(aL / math.sin(angA), bL / math.sin(angB)))

# ---------------------------------------------------------------- matrices
for _ in range(3):
    A = rng.normal(size=(3, 3))
    B = rng.normal(size=(3, 3))
    (a1, b1, c1), (d1, e1, f1), (g1, h1, i1) = A
    check('Sarrus', close(np.linalg.det(A), a1 * e1 * i1 + b1 * f1 * g1 + c1 * d1 * h1 - c1 * e1 * g1 - a1 * f1 * h1 - b1 * d1 * i1))
    check('det(AB)', close(np.linalg.det(A @ B), np.linalg.det(A) * np.linalg.det(B)))
    check('det(lambda A)', close(np.linalg.det(2.5 * A), 2.5**3 * np.linalg.det(A)))
    check('tr(AB)', close(np.trace(A @ B), np.trace(B @ A)))
    check('(AB)^T', close((A @ B).T, B.T @ A.T))
    check('(AB)^-1', close(np.linalg.inv(A @ B), np.linalg.inv(B) @ np.linalg.inv(A)))
    cof = np.array([[(-1)**(i + j) * np.linalg.det(np.delete(np.delete(A, i, 0), j, 1)) for j in range(3)] for i in range(3)])
    check('comatrice', close(np.linalg.inv(A), cof.T / np.linalg.det(A)))
    bvec = rng.normal(size=3)
    xsol = np.linalg.solve(A, bvec)
    for i in range(3):
        Ai = A.copy()
        Ai[:, i] = bvec
        check('Cramer', close(xsol[i], np.linalg.det(Ai) / np.linalg.det(A)))
    ev = np.linalg.eigvals(A)
    check('tr = somme vp, det = produit', close([np.sum(ev), np.prod(ev)], [np.trace(A), np.linalg.det(A)]))
A2m = np.array([[1.3, -0.4], [2.0, 0.7]])
(a1, b1), (c1, d1) = A2m
check('inverse 2x2', close(np.linalg.inv(A2m), np.array([[d1, -b1], [-c1, a1]]) / (a1 * d1 - b1 * c1)))
lam_ = np.linalg.eigvals(A2m)
check('chi_A 2x2', close([l_**2 - np.trace(A2m) * l_ + np.linalg.det(A2m) for l_ in lam_], [0, 0], 1e-9))
Rm = lambda t: np.array([[math.cos(t), -math.sin(t)], [math.sin(t), math.cos(t)]])
check('rotations', close(Rm(0.4) @ Rm(1.1), Rm(1.5)) and close(np.linalg.inv(Rm(0.4)), Rm(0.4).T))
# rotation dans le plan : changement de repere
x0_, y0_, al = 1.0, -2.0, 0.6
xp, yp = 0.7, 1.9
xy = np.array([x0_, y0_]) + Rm(al) @ np.array([xp, yp])
check('changement de repere', close([(xy[0] - x0_) * math.cos(al) + (xy[1] - y0_) * math.sin(al),
                                     -(xy[0] - x0_) * math.sin(al) + (xy[1] - y0_) * math.cos(al)], [xp, yp]))

# ---------------------------------------------------------------- coordonnees
def sph(r, t, p):
    return np.array([r * math.sin(t) * math.cos(p), r * math.sin(t) * math.sin(p), r * math.cos(t)])
r0, t0, p0 = 1.7, 0.9, 2.1
ur = np.array([math.sin(t0) * math.cos(p0), math.sin(t0) * math.sin(p0), math.cos(t0)])
ut = np.array([math.cos(t0) * math.cos(p0), math.cos(t0) * math.sin(p0), -math.sin(t0)])
up_ = np.array([-math.sin(p0), math.cos(p0), 0])
hh = 1e-6
check('base spherique', close((sph(r0 + hh, t0, p0) - sph(r0 - hh, t0, p0)) / (2 * hh), ur) and
      close((sph(r0, t0 + hh, p0) - sph(r0, t0 - hh, p0)) / (2 * hh), r0 * ut) and
      close((sph(r0, t0, p0 + hh) - sph(r0, t0, p0 - hh)) / (2 * hh), r0 * math.sin(t0) * up_))
check('base directe', close(np.cross(ur, ut), up_))
J = np.column_stack([(sph(r0 + hh, t0, p0) - sph(r0 - hh, t0, p0)) / (2 * hh),
                     (sph(r0, t0 + hh, p0) - sph(r0, t0 - hh, p0)) / (2 * hh),
                     (sph(r0, t0, p0 + hh) - sph(r0, t0, p0 - hh)) / (2 * hh)])
check('jacobien spherique', close(np.linalg.det(J), r0**2 * math.sin(t0), 1e-5))
# acceleration en polaires : trajectoire r(t), theta(t)
rt = lambda t: 1 + 0.3 * t**2
tht = lambda t: 0.5 * t + 0.2 * t**3
pos = lambda t: np.array([rt(t) * math.cos(tht(t)), rt(t) * math.sin(tht(t))])
T0 = 0.8
acc = (pos(T0 + 1e-4) - 2 * pos(T0) + pos(T0 - 1e-4)) / 1e-8
rd, rdd = 0.6 * T0, 0.6
td, tdd = 0.5 + 0.6 * T0**2, 1.2 * T0
urp = np.array([math.cos(tht(T0)), math.sin(tht(T0))])
utp = np.array([-math.sin(tht(T0)), math.cos(tht(T0))])
check('acceleration polaire', close(acc, (rdd - rt(T0) * td**2) * urp + (2 * rd * td + rt(T0) * tdd) * utp, 1e-5))

# ---------------------------------------------------------------- analyse vectorielle
def cart_field(P):
    x, y, z = P
    return np.array([x * y + math.sin(z), y * z**2 + x, math.exp(0.3 * x) * y + z * x])


def cart_scalar(P):
    x, y, z = P
    return x * x * y + math.sin(y * z) + math.exp(0.2 * x * z)


def num_grad(f, P, h=1e-5):
    return np.array([(f(P + h * e) - f(P - h * e)) / (2 * h) for e in np.eye(3)])


def num_div(F, P, h=1e-5):
    return sum((F(P + h * e)[i] - F(P - h * e)[i]) / (2 * h) for i, e in enumerate(np.eye(3)))


def num_rot(F, P, h=1e-5):
    Jm = np.array([(F(P + h * e) - F(P - h * e)) / (2 * h) for e in np.eye(3)]).T  # J[i][j] = dFi/dxj
    return np.array([Jm[2, 1] - Jm[1, 2], Jm[0, 2] - Jm[2, 0], Jm[1, 0] - Jm[0, 1]])


def num_lap(f, P, h=1e-4):
    return sum((f(P + h * e) - 2 * f(P) + f(P - h * e)) / h**2 for e in np.eye(3))


# cylindriques
def cyl2cart(r, t, z):
    return np.array([r * math.cos(t), r * math.sin(t), z])


def cyl_basis(t):
    return (np.array([math.cos(t), math.sin(t), 0]), np.array([-math.sin(t), math.cos(t), 0]), np.array([0, 0, 1.0]))


def comps_cyl(r, t, z):
    ur_, ut_, uz_ = cyl_basis(t)
    F = cart_field(cyl2cart(r, t, z))
    return np.array([F @ ur_, F @ ut_, F @ uz_])


def pd(f, args, i, h=1e-5):
    a1 = list(args); a2 = list(args)
    a1[i] += h; a2[i] -= h
    return (f(*a1) - f(*a2)) / (2 * h)


r0, t0, z0 = 1.3, 0.8, -0.4
P = cyl2cart(r0, t0, z0)
fc = lambda r, t, z: cart_scalar(cyl2cart(r, t, z))
ur_, ut_, uz_ = cyl_basis(t0)
g = pd(fc, (r0, t0, z0), 0) * ur_ + pd(fc, (r0, t0, z0), 1) / r0 * ut_ + pd(fc, (r0, t0, z0), 2) * uz_
check('grad cylindrique', close(g, num_grad(cart_scalar, P), 1e-5))
Ar = lambda r, t, z: comps_cyl(r, t, z)[0]
At = lambda r, t, z: comps_cyl(r, t, z)[1]
Az = lambda r, t, z: comps_cyl(r, t, z)[2]
div_c = pd(lambda r, t, z: r * Ar(r, t, z), (r0, t0, z0), 0) / r0 + pd(At, (r0, t0, z0), 1) / r0 + pd(Az, (r0, t0, z0), 2)
check('div cylindrique', close(div_c, num_div(cart_field, P), 1e-5))
rot_c = np.array([pd(Az, (r0, t0, z0), 1) / r0 - pd(At, (r0, t0, z0), 2),
                  pd(Ar, (r0, t0, z0), 2) - pd(Az, (r0, t0, z0), 0),
                  (pd(lambda r, t, z: r * At(r, t, z), (r0, t0, z0), 0) - pd(Ar, (r0, t0, z0), 1)) / r0])
check('rot cylindrique', close(rot_c[0] * ur_ + rot_c[1] * ut_ + rot_c[2] * uz_, num_rot(cart_field, P), 1e-5))
h4 = 1e-3
lap_c = ((r0 + h4 / 2) * (fc(r0 + h4, t0, z0) - fc(r0, t0, z0)) - (r0 - h4 / 2) * (fc(r0, t0, z0) - fc(r0 - h4, t0, z0))) / (r0 * h4 * h4) \
    + (fc(r0, t0 + h4, z0) - 2 * fc(r0, t0, z0) + fc(r0, t0 - h4, z0)) / (r0**2 * h4 * h4) \
    + (fc(r0, t0, z0 + h4) - 2 * fc(r0, t0, z0) + fc(r0, t0, z0 - h4)) / (h4 * h4)
check('laplacien cylindrique', close(lap_c, num_lap(cart_scalar, P), 1e-4))

# spheriques
r0, t0, p0 = 1.4, 1.1, 0.6
P = sph(r0, t0, p0)
fs = lambda r, t, p: cart_scalar(sph(r, t, p))
def sph_basis(t, p):
    return (np.array([math.sin(t) * math.cos(p), math.sin(t) * math.sin(p), math.cos(t)]),
            np.array([math.cos(t) * math.cos(p), math.cos(t) * math.sin(p), -math.sin(t)]),
            np.array([-math.sin(p), math.cos(p), 0]))
def comps_sph(r, t, p):
    a1, a2, a3 = sph_basis(t, p)
    F = cart_field(sph(r, t, p))
    return np.array([F @ a1, F @ a2, F @ a3])
er, et, ep = sph_basis(t0, p0)
args = (r0, t0, p0)
g = pd(fs, args, 0) * er + pd(fs, args, 1) / r0 * et + pd(fs, args, 2) / (r0 * math.sin(t0)) * ep
check('grad spherique', close(g, num_grad(cart_scalar, P), 1e-5))
Ar = lambda r, t, p: comps_sph(r, t, p)[0]
At = lambda r, t, p: comps_sph(r, t, p)[1]
Ap_ = lambda r, t, p: comps_sph(r, t, p)[2]
s0 = math.sin(t0)
div_s = pd(lambda r, t, p: r * r * Ar(r, t, p), args, 0) / r0**2 \
    + pd(lambda r, t, p: math.sin(t) * At(r, t, p), args, 1) / (r0 * s0) + pd(Ap_, args, 2) / (r0 * s0)
check('div spherique', close(div_s, num_div(cart_field, P), 1e-5))
rot_s = np.array([(pd(lambda r, t, p: math.sin(t) * Ap_(r, t, p), args, 1) - pd(At, args, 2)) / (r0 * s0),
                  (pd(Ar, args, 2) / s0 - pd(lambda r, t, p: r * Ap_(r, t, p), args, 0)) / r0,
                  (pd(lambda r, t, p: r * At(r, t, p), args, 0) - pd(Ar, args, 1)) / r0])
check('rot spherique', close(rot_s[0] * er + rot_s[1] * et + rot_s[2] * ep, num_rot(cart_field, P), 1e-5))
f0 = fs(r0, t0, p0)
lap_s = ((r0 + h4 / 2)**2 * (fs(r0 + h4, t0, p0) - f0) - (r0 - h4 / 2)**2 * (f0 - fs(r0 - h4, t0, p0))) / (r0**2 * h4 * h4) \
    + (math.sin(t0 + h4 / 2) * (fs(r0, t0 + h4, p0) - f0) - math.sin(t0 - h4 / 2) * (f0 - fs(r0, t0 - h4, p0))) / (r0**2 * s0 * h4 * h4) \
    + (fs(r0, t0, p0 + h4) - 2 * f0 + fs(r0, t0, p0 - h4)) / (r0**2 * s0**2 * h4 * h4)
check('laplacien spherique', close(lap_s, num_lap(cart_scalar, P), 1e-4))
radial = lambda r: r * fs(r, t0, p0)
check('partie radiale', close(((r0 + h4 / 2)**2 * (fs(r0 + h4, t0, p0) - f0) - (r0 - h4 / 2)**2 * (f0 - fs(r0 - h4, t0, p0))) / (r0**2 * h4 * h4),
                              (radial(r0 + h4) - 2 * radial(r0) + radial(r0 - h4)) / (h4 * h4) / r0, 1e-4))
# identites
P = np.array([0.3, -0.8, 1.1])
G = lambda Q: cart_scalar(Q)
Bf = lambda Q: np.array([Q[1] * Q[2], math.cos(Q[0]) + Q[2], Q[0] * Q[1]**2])
check('div(fA)', close(num_div(lambda Q: G(Q) * cart_field(Q), P), G(P) * num_div(cart_field, P) + num_grad(G, P) @ cart_field(P), 1e-5))
check('rot(fA)', close(num_rot(lambda Q: G(Q) * cart_field(Q), P),
                       G(P) * num_rot(cart_field, P) + np.cross(num_grad(G, P), cart_field(P)), 1e-5))
check('div(AxB)', close(num_div(lambda Q: np.cross(cart_field(Q), Bf(Q)), P),
                        Bf(P) @ num_rot(cart_field, P) - cart_field(P) @ num_rot(Bf, P), 1e-5))
check('rot grad = 0', close(num_rot(lambda Q: num_grad(G, Q, 1e-4), P, 1e-4), np.zeros(3), 1e-4))
# Green-Riemann sur le disque unite : P = -y^3, Q = x^3 -> int (3x^2+3y^2) = 3 pi / 2
ts = np.linspace(0, 2 * math.pi, 20001)
xs_, ys_ = np.cos(ts), np.sin(ts)
circ = np.sum((-ys_[:-1]**3) * np.diff(xs_) + (xs_[:-1]**3) * np.diff(ys_))
check('Green-Riemann', close(circ, 1.5 * math.pi, 1e-3))

# ---------------------------------------------------------------- equations differentielles
tau, E, y0 = 0.7, 3.0, -1.0
yt = lambda t: E + (y0 - E) * math.exp(-t / tau)
check('premier ordre tau', close(tau * d(yt, 0.9) + yt(0.9), E))
Q, w0 = 3.0, 2.0
rts = np.roots([1, w0 / Q, w0**2])
check('pseudo-pulsation', close(abs(rts[0].imag), w0 * math.sqrt(1 - 1 / (4 * Q * Q))))
# variation de la constante : y' + a(t) y = b(t) avec a = 2t, b = t
At_ = lambda t: t * t
yp = lambda t: math.exp(-At_(t)) * integ(lambda s: s * np.exp(s * s), 0, t, 4000)
check('variation de la constante', close(d(yp, 0.8) + 2 * 0.8 * yp(0.8), 0.8, 1e-5))
# racine double
rr = -1.5
ysol = lambda t: (2 + 3 * t) * math.exp(rr * t)
check('racine double', close(d2(ysol, 0.4) - 2 * rr * d(ysol, 0.4) + rr * rr * ysol(0.4), 0, 1e-4))


# ================================================================ v1.1
# ---------------------------------------------------------------- elements simples
check('division euclidienne', all(close((x**3 + 2 * x + 1) / (x * x + 1), x + (x + 1) / (x * x + 1)) for x in XN))
check('(x+1)/(x-1)', all(close((x + 1) / (x - 1), 1 + 2 / (x - 1)) for x in [0.3, 2.5, -4.0]))
check('exemple pole double', all(close((2 * x + 1) / (x * (x - 1)**2), 1 / x - 1 / (x - 1) + 3 / (x - 1)**2) for x in [0.4, 2.5, -1.7]))
Bc, Cc, pp, qq = 1.5, -0.7, 2.0, 5.0
for x in XN:
    lhs = (Bc * x + Cc) / (x * x + pp * x + qq)
    rhs = Bc / 2 * (2 * x + pp) / (x * x + pp * x + qq) + (Cc - Bc * pp / 2) / (x * x + pp * x + qq)
    check('seconde espece', close(lhs, rhs))
om = math.sqrt(qq - pp * pp / 4)
prim_ok('arctan canonique', lambda x: np.arctan((x + pp / 2) / om) / om, lambda x: 1 / (x * x + pp * x + qq), XN)
prim_ok('ln trinome', lambda x: np.log(x * x + pp * x + qq), lambda x: (2 * x + pp) / (x * x + pp * x + qq), XN)
# coefficient P(a)/Q'(a)
Pf = lambda x: 3 * x + 2
Qf = lambda x: (x - 1) * (x + 2) * (x - 3)
Qd = lambda x: d(Qf, x)
for a_ in (1, -2, 3):
    lim = (Pf(a_ + 1e-7) * 1e-7 / Qf(a_ + 1e-7))
    check('pole simple P/Q\'', close(lim, Pf(a_) / Qd(a_), 1e-5))

# ---------------------------------------------------------------- exp, log, factorielle
check('serie exp', close(sum(1.7**k / math.factorial(k) for k in range(40)), math.exp(1.7)))
check('e', close(sum(1 / math.factorial(k) for k in range(25)), math.e))
check('ln 1/a, ln sqrt', close([math.log(1 / 3.7), math.log(math.sqrt(3.7))], [-math.log(3.7), 0.5 * math.log(3.7)]))
check('log_a', close([math.log(12, 3), math.log(5 * 7, 3), math.log(5**2.5, 3)],
                     [math.log(12) / math.log(3), math.log(5, 3) + math.log(7, 3), 2.5 * math.log(5, 3)]))
check('changement de base', close([math.log(9, 5), math.log(5, 3)], [math.log(9, 7) / math.log(5, 7), 1 / math.log(3, 5)]))
check('valeurs log', close([math.log10(2), math.log(2), math.log(10)], [0.301, 0.693, 2.303], 2e-3))
deriv_ok('log_a', lambda x: np.log(x) / np.log(2), lambda x: 1 / (x * np.log(2)), X)
check('lim ln x / x', abs(math.log(1e8) / 1e8) < 1e-6 and abs(1e-8 * math.log(1e-8)) < 1e-6)
check('factorielles', [math.factorial(k) for k in (0, 1, 2, 3, 4, 5, 6, 10)] == [1, 1, 2, 6, 24, 120, 720, 3628800])
check('n!/k!', math.factorial(9) // math.factorial(5) == 6 * 7 * 8 * 9)
check('binomiaux', all(math.comb(n_, k_) == math.factorial(n_) // (math.factorial(k_) * math.factorial(n_ - k_)) and
                       math.comb(n_, k_) == math.comb(n_, n_ - k_) and
                       (k_ == 0 or k_ * math.comb(n_, k_) == n_ * math.comb(n_ - 1, k_ - 1))
                       for n_ in range(1, 12) for k_ in range(0, n_ + 1)))
check('binome 2', all(math.comb(n_, 2) == n_ * (n_ - 1) // 2 and sum(math.comb(n_, k_) for k_ in range(n_ + 1)) == 2**n_ for n_ in range(2, 15)))
check('triangle de Pascal', [[math.comb(n_, k_) for k_ in range(n_ + 1)] for n_ in range(6)] ==
      [[1], [1, 1], [1, 2, 1], [1, 3, 3, 1], [1, 4, 6, 4, 1], [1, 5, 10, 10, 5, 1]])
check('arrangements', math.perm(7, 3) == math.factorial(7) // math.factorial(4))

# ---------------------------------------------------------------- matrices
A = np.array([[1.0, -1, 1], [2, 1, 2], [4, 5, -3]])
cofA = np.array([[(-1)**(i + j) * np.linalg.det(np.delete(np.delete(A, i, 0), j, 1)) for j in range(3)] for i in range(3)])
check('exemple det -21', close(np.linalg.det(A), -21))
check('exemple comatrice', close(cofA, [[-13, 14, 6], [2, -7, -9], [-3, 0, 3]]))
check('exemple inverse', close(np.linalg.inv(A), np.array([[13, -2, 3], [-14, 7, 0], [-6, 9, -3]]) / 21))
check('A com^T = det I', close(A @ cofA.T, np.linalg.det(A) * np.eye(3)) and close(cofA.T @ A, np.linalg.det(A) * np.eye(3)))
check('exemple Laplace 18', close(np.linalg.det(np.array([[2.0, 0, 1], [1, 3, -1], [4, 0, 5]])), 18))
M4 = rng.normal(size=(4, 4))
for i in range(4):
    dev = sum(M4[i, j] * (-1)**(i + j) * np.linalg.det(np.delete(np.delete(M4, i, 0), j, 1)) for j in range(4))
    check('Laplace ligne %d' % i, close(dev, np.linalg.det(M4)))
    devc = sum(M4[j, i] * (-1)**(i + j) * np.linalg.det(np.delete(np.delete(M4, j, 0), i, 1)) for j in range(4))
    check('Laplace colonne %d' % i, close(devc, np.linalg.det(M4)))
check('damier', all((-1)**(i + j) == (1 if (i + j) % 2 == 0 else -1) for i in range(5) for j in range(5)))
check('produit 2x2', close(np.array([[1, 2], [3, 4]]) @ np.array([[5, 6], [7, 8]]), [[1 * 5 + 2 * 7, 1 * 6 + 2 * 8], [3 * 5 + 4 * 7, 3 * 6 + 4 * 8]]))
E12 = np.array([[0, 1], [0, 0]]); E11 = np.array([[1, 0], [0, 0]])
check('non commutativite', close(E11 @ E12, E12) and close(E12 @ E11, np.zeros((2, 2))))
check('somme exemple', close(np.array([[1, 2], [3, 4]]) + np.array([[5, 0], [-1, 2]]), [[6, 2], [2, 6]]))
def Eij(i, j, n=3, p=3):
    m = np.zeros((n, p)); m[i, j] = 1; return m
check('E_ij E_kl', all(close(Eij(i, j) @ Eij(k, l), Eij(i, l) if j == k else np.zeros((3, 3)))
                       for i in range(3) for j in range(3) for k in range(3) for l in range(3)))
check('base canonique', close(sum(A[i, j] * Eij(i, j) for i in range(3) for j in range(3)), A))
S_ = (A + A.T) / 2; K_ = (A - A.T) / 2
check('sym + antisym', close(S_ + K_, A) and close(S_.T, S_) and close(K_.T, -K_) and close(np.diag(K_), 0))
check('transposee exemple', close(np.array([[1, 2, 3], [4, 5, 6]]).T, [[1, 4], [2, 5], [3, 6]]))
check('(A^-1)^T', close(np.linalg.inv(A).T, np.linalg.inv(A.T)))
check('(lambda A)^-1, (A^k)^-1', close(np.linalg.inv(2.5 * A), np.linalg.inv(A) / 2.5) and
      close(np.linalg.inv(A @ A @ A), np.linalg.matrix_power(np.linalg.inv(A), 3)))
N_ = np.array([[0.0, 2, 3], [0, 0, 4], [0, 0, 0]])
check('nilpotente', close(np.linalg.matrix_power(N_, 3), 0))
lam0 = 1.7
check('(lambda I + N)^n', close(np.linalg.matrix_power(lam0 * np.eye(3) + N_, 6),
                                sum(math.comb(6, k) * lam0**(6 - k) * np.linalg.matrix_power(N_, k) for k in range(3))))
check('Gauss-Jordan 2x2', close(np.linalg.inv(np.array([[1.0, 2], [3, 4]])), [[-2, 1], [1.5, -0.5]]))
T_ = np.eye(3); T_[1, 0] = 2.5
check('transvection', close((T_ @ A)[1], A[1] + 2.5 * A[0]))
P12 = np.array([[0.0, 1, 0], [1, 0, 0], [0, 0, 1]])
check('permutation', close((P12 @ A)[[1, 0, 2]], A))
check('triangulaire det', close(np.linalg.det(np.triu(A)), np.prod(np.diag(np.triu(A)))))
aa_, bb_, cc_, dd_, ee_, ff_ = 2.0, -1.0, 3.0, 4.0, 1.5, -2.0
xs_ = np.linalg.solve(np.array([[aa_, bb_], [cc_, dd_]]), np.array([ee_, ff_]))
check('Cramer 2x2', close(xs_, [(ee_ * dd_ - bb_ * ff_) / (aa_ * dd_ - bb_ * cc_), (aa_ * ff_ - cc_ * ee_) / (aa_ * dd_ - bb_ * cc_)]))
check('rang', np.linalg.matrix_rank(np.array([[1, 1, 2], [2, 4, 5], [3, 5, 7]])) == 2)
# suites de matrices
Am = np.array([[0.5, 0.2], [0.1, 0.3]]); Bm = np.array([1.0, 2.0]); U0 = np.array([3.0, -1.0])
Cm = np.linalg.solve(np.eye(2) - Am, Bm)
U = U0.copy()
for k in range(12):
    U = Am @ U + Bm
check('U_n = A^n(U0-C)+C', close(U, np.linalg.matrix_power(Am, 12) @ (U0 - Cm) + Cm))
aR, bR = 1.0, 2.0
seq = [1.0, 3.0]
for k in range(10):
    seq.append(aR * seq[-1] + bR * seq[-2])
Xn = np.linalg.matrix_power(np.array([[aR, bR], [1, 0]]), 10) @ np.array([seq[1], seq[0]])
check('matrice compagnon', close(Xn, [seq[11], seq[10]]))
check('vp compagnon', close(sorted(np.linalg.eigvals(np.array([[aR, bR], [1, 0]])).real), sorted(np.roots([1, -aR, -bR]).real)))
Mk = np.array([[0.85, 0.15], [0.45, 0.55]])
check('Markov etat stable', close(np.array([0.75, 0.25]) @ Mk, [0.75, 0.25]) and
      close(np.array([0.6, 0.4]) @ np.linalg.matrix_power(Mk, 200), [0.75, 0.25]))
Ad = np.array([[0.5, 0.3], [0.2, 0.4]])
check('A^n -> 0', np.max(np.abs(np.linalg.eigvals(Ad))) < 1 and close(np.linalg.matrix_power(Ad, 200), 0, 1e-9))
Ms = np.array([[6.25, -9], [4.5, -6.5]])
ev = np.linalg.eigvals(Ms)
check('methode 2x2', close(sorted(ev), sorted(np.roots([1, -np.trace(Ms), np.linalg.det(Ms)]))))

# ---------------------------------------------------------------- jacobiennes
r0, t0, p0 = 1.6, 0.7, 1.9
def polar(r, t): return np.array([r * math.cos(t), r * math.sin(t)])
Jp = np.column_stack([(polar(r0 + hh, t0) - polar(r0 - hh, t0)) / (2 * hh), (polar(r0, t0 + hh) - polar(r0, t0 - hh)) / (2 * hh)])
check('jacobienne polaire', close(Jp, [[math.cos(t0), -r0 * math.sin(t0)], [math.sin(t0), r0 * math.cos(t0)]], 1e-6) and close(np.linalg.det(Jp), r0, 1e-6))
def cyl(r, t, z): return np.array([r * math.cos(t), r * math.sin(t), z])
Jc = np.column_stack([(cyl(r0 + hh, t0, 0.3) - cyl(r0 - hh, t0, 0.3)) / (2 * hh), (cyl(r0, t0 + hh, 0.3) - cyl(r0, t0 - hh, 0.3)) / (2 * hh),
                      (cyl(r0, t0, 0.3 + hh) - cyl(r0, t0, 0.3 - hh)) / (2 * hh)])
check('jacobienne cylindrique', close(Jc, [[math.cos(t0), -r0 * math.sin(t0), 0], [math.sin(t0), r0 * math.cos(t0), 0], [0, 0, 1]], 1e-6))
Js = np.column_stack([(sph(r0 + hh, t0, p0) - sph(r0 - hh, t0, p0)) / (2 * hh), (sph(r0, t0 + hh, p0) - sph(r0, t0 - hh, p0)) / (2 * hh),
                      (sph(r0, t0, p0 + hh) - sph(r0, t0, p0 - hh)) / (2 * hh)])
st, ct, sp, cp = math.sin(t0), math.cos(t0), math.sin(p0), math.cos(p0)
check('jacobienne spherique', close(Js, [[st * cp, r0 * ct * cp, -r0 * st * sp], [st * sp, r0 * ct * sp, r0 * st * cp], [ct, -r0 * st, 0]], 1e-6)
      and close(np.linalg.det(Js), r0**2 * st, 1e-6))
fA = lambda v: np.array([v[0] * v[1], math.sin(v[0]) + v[1]**2])
gA = lambda w: np.array([w[0] + w[1]**2, w[0] * w[1], math.exp(w[0])])
def jac(f, v, m):
    return np.column_stack([(f(v + hh * e) - f(v - hh * e)) / (2 * hh) for e in np.eye(m)])
v0 = np.array([0.4, 1.3])
check('jacobienne composee', close(jac(lambda v: gA(fA(v)), v0, 2), jac(gA, fA(v0), 2) @ jac(fA, v0, 2), 1e-5))

# ---------------------------------------------------------------- Fourier
def fourier(f, w, T=60.0, n=600001):
    t = np.linspace(-T, T, n)
    y = f(t) * np.exp(-1j * w * t)
    return (2 * T / (n - 1)) * (y.sum() - (y[0] + y[-1]) / 2)
aF = 1.3
for w in (0.0, 0.7, 2.1):
    check('Fourier e^-a|t|', close(fourier(lambda t: np.exp(-aF * abs(t)), w), 2 * aF / (aF**2 + w**2), 1e-5))
    check('Fourier e^-at u(t)', close(fourier(lambda t: np.where(t >= 0, np.exp(-aF * t), 0.0), w, n=2000001), 1 / (aF + 1j * w), 1e-3))
    check('Fourier gaussienne', close(fourier(lambda t: np.exp(-aF * t * t), w), math.sqrt(math.pi / aF) * math.exp(-w * w / (4 * aF)), 1e-6))
    Tp = 2.0
    ex = Tp if w == 0 else Tp * math.sin(w * Tp / 2) / (w * Tp / 2)
    check('Fourier porte', close(fourier(lambda t: np.where(abs(t) < Tp / 2, 1.0, 0.0), w, T=2, n=2000001), ex, 1e-4))
g = lambda t: np.exp(-aF * t * t)
w = 0.9; t0_ = 0.7
check('Fourier retard', close(fourier(lambda t: g(t - t0_), w), np.exp(-1j * w * t0_) * fourier(g, w)))
check('Fourier modulation', close(fourier(lambda t: np.exp(1j * 0.5 * t) * g(t), w), fourier(g, w - 0.5)))
check('Fourier dilatation', close(fourier(lambda t: g(2.0 * t), w), 0.5 * fourier(g, w / 2.0)))
check('Fourier derivee', close(fourier(lambda t: -2 * aF * t * g(t), w), 1j * w * fourier(g, w)))
check('Fourier t f', close(fourier(lambda t: t * g(t), w), 1j * (fourier(g, w + 1e-4) - fourier(g, w - 1e-4)) / 2e-4, 1e-5))
energy = integ(lambda t: g(t)**2, -40, 40, 400000)
ws = np.linspace(-40, 40, 40001)
Fw = math.sqrt(math.pi / aF) * np.exp(-ws**2 / (4 * aF))
check('Parseval-Plancherel', close(energy, (np.sum(np.abs(Fw)**2) * (ws[1] - ws[0])) / (2 * math.pi), 1e-5))
check('dualite', close(fourier(lambda t: math.sqrt(math.pi / aF) * np.exp(-t * t / (4 * aF)), w), 2 * math.pi * g(-w), 1e-6))
# convolution : gaussiennes
g2 = lambda t: np.exp(-0.7 * t * t)
conv = lambda t: math.sqrt(math.pi / (aF + 0.7)) * np.exp(-aF * 0.7 / (aF + 0.7) * t * t)
check('Fourier convolution', close(fourier(conv, w), fourier(g, w) * fourier(g2, w), 1e-6))

# ---------------------------------------------------------------- Laplace
def laplace(f, p, T=80.0, n=800001):
    t = np.linspace(0, T, n)
    y = f(t) * np.exp(-p * t)
    return (T / (n - 1)) * (y.sum() - (y[0] + y[-1]) / 2)
pL, aL, wL, tauL = 1.3, 0.6, 2.0, 0.8
table = [
    ('echelon', lambda t: np.ones_like(t), 1 / pL),
    ('rampe', lambda t: t, 1 / pL**2),
    ('t^3', lambda t: t**3, 6 / pL**4),
    ('e^-at', lambda t: np.exp(-aL * t), 1 / (pL + aL)),
    ('t e^-at', lambda t: t * np.exp(-aL * t), 1 / (pL + aL)**2),
    ('t^2 e^-at', lambda t: t**2 * np.exp(-aL * t), 2 / (pL + aL)**3),
    ('sin', lambda t: np.sin(wL * t), wL / (pL**2 + wL**2)),
    ('cos', lambda t: np.cos(wL * t), pL / (pL**2 + wL**2)),
    ('e^-at sin', lambda t: np.exp(-aL * t) * np.sin(wL * t), wL / ((pL + aL)**2 + wL**2)),
    ('e^-at cos', lambda t: np.exp(-aL * t) * np.cos(wL * t), (pL + aL) / ((pL + aL)**2 + wL**2)),
    ('sh', lambda t: np.sinh(aL * t), aL / (pL**2 - aL**2)),
    ('ch', lambda t: np.cosh(aL * t), pL / (pL**2 - aL**2)),
    ('t sin', lambda t: t * np.sin(wL * t), 2 * wL * pL / (pL**2 + wL**2)**2),
    ('1/tau e^-t/tau', lambda t: np.exp(-t / tauL) / tauL, 1 / (1 + tauL * pL)),
    ('1-e^-t/tau', lambda t: 1 - np.exp(-t / tauL), 1 / (pL * (1 + tauL * pL))),
]
for name, f, F in table:
    check('Laplace ' + name, close(laplace(f, pL), F, 1e-6))
fL = lambda t: np.sin(1.5 * t) * np.exp(-0.3 * t) + 0.4
FL = lambda p: laplace(fL, p)
check('Laplace derivee', close(laplace(lambda t: d(fL, t), pL), pL * FL(pL) - fL(0.0), 1e-5))
check('Laplace derivee 2', close(laplace(lambda t: d2(fL, t), pL), pL**2 * FL(pL) - pL * fL(0.0) - d(fL, 0.0), 1e-4))
Fint = laplace(lambda t: (1 - np.exp(-aL * t)) / aL, pL)
check('Laplace integrale', close(Fint, laplace(lambda t: np.exp(-aL * t), pL) / pL, 1e-6))
check('Laplace retard', close(laplace(lambda t: np.where(t >= 0.5, fL(t - 0.5), 0.0), pL, n=2000001), np.exp(-0.5 * pL) * FL(pL), 1e-4))
check('Laplace amortissement', close(laplace(lambda t: np.exp(-aL * t) * fL(t), pL), FL(pL + aL), 1e-6))
check('Laplace t f', close(laplace(lambda t: t * fL(t), pL), -(FL(pL + 1e-4) - FL(pL - 1e-4)) / 2e-4, 1e-5))
check('Laplace dilatation', close(laplace(lambda t: fL(2.0 * t), pL), FL(pL / 2.0) / 2.0, 1e-5))
check('valeur finale', close(1e-6 * (0.5 / (1e-6 * (1 + 0.8 * 1e-6))), 0.5, 1e-5))
check('valeur initiale', close(1e8 * (wL / (1e16 + wL**2)), 0, 1e-6))
check('exemple tau y + y = E', close((3.0 / pL) - 3.0 * tauL / (1 + tauL * pL), 3.0 / (pL * (1 + tauL * pL))))
# deuxieme ordre : poles et depassement
xi, w0 = 0.35, 3.0
r_ = np.roots([1, 2 * xi * w0, w0**2])
check('poles 2e ordre', close(sorted(r_, key=lambda z: z.imag), [-xi * w0 - 1j * w0 * math.sqrt(1 - xi**2), -xi * w0 + 1j * w0 * math.sqrt(1 - xi**2)]))
for xi2 in (1.5,):
    r2 = sorted(np.roots([1, 2 * xi2 * w0, w0**2]).real)
    check('poles reels', close(r2, [-xi2 * w0 - w0 * math.sqrt(xi2**2 - 1), -xi2 * w0 + w0 * math.sqrt(xi2**2 - 1)]))
wp = w0 * math.sqrt(1 - xi**2)
tpk = math.pi / wp
s_peak = 1 - math.exp(-xi * w0 * tpk) * (math.cos(wp * tpk) + xi / math.sqrt(1 - xi**2) * math.sin(wp * tpk))
check('depassement', close(100 * (s_peak - 1), 100 * math.exp(-math.pi * xi / math.sqrt(1 - xi**2))))
check('formes 2e ordre', close(1 / (1 + 2 * xi / w0 * pL + pL**2 / w0**2), w0**2 / (pL**2 + 2 * xi * w0 * pL + w0**2)))
check('t5% 1er ordre', close(1 - math.exp(-3), 0.95, 2e-3))
check('-3 dB', close(20 * math.log10(abs(1 / (1 + 1j))), -3.01, 1e-3) and close(math.degrees(cmath.phase(1 / (1 + 1j))), -45))

# ---------------------------------------------------------------- Z
def ztr(x, z, N=4000):
    return sum(x(n) * z**(-n) for n in range(N))
zz, aZ, wZ = 1.7 + 0.4j, 0.6, 0.9
ztab = [
    ('echelon', lambda n: 1.0, zz / (zz - 1)),
    ('n', lambda n: n, zz / (zz - 1)**2),
    ('a^n', lambda n: aZ**n, zz / (zz - aZ)),
    ('n a^n', lambda n: n * aZ**n, aZ * zz / (zz - aZ)**2),
    ('e^-anT', lambda n: math.exp(-0.4 * n * 0.5), zz / (zz - math.exp(-0.4 * 0.5))),
    ('cos', lambda n: math.cos(n * wZ), zz * (zz - math.cos(wZ)) / (zz**2 - 2 * zz * math.cos(wZ) + 1)),
    ('sin', lambda n: math.sin(n * wZ), zz * math.sin(wZ) / (zz**2 - 2 * zz * math.cos(wZ) + 1)),
]
for name, x, X_ in ztab:
    check('Z ' + name, close(ztr(x, zz), X_, 1e-8))
xs_seq = lambda n: 0.5**n + 0.2 * n * 0.3**n
Xz = lambda z: ztr(xs_seq, z)
check('Z retard', close(ztr(lambda n: xs_seq(n - 2) if n >= 2 else 0.0, zz), zz**-2 * Xz(zz)))
check('Z avance', close(ztr(lambda n: xs_seq(n + 1), zz), zz * Xz(zz) - zz * xs_seq(0)))
check('Z avance 2', close(ztr(lambda n: xs_seq(n + 2), zz), zz**2 * Xz(zz) - zz**2 * xs_seq(0) - zz * xs_seq(1)))
check('Z a^n x_n', close(ztr(lambda n: 1.3**n * xs_seq(n), zz + 1.0, 400), Xz((zz + 1.0) / 1.3)))
check('Z n x_n', close(ztr(lambda n: n * xs_seq(n), zz), -zz * (Xz(zz + 1e-5) - Xz(zz - 1e-5)) / 2e-5, 1e-6))
ys_seq = lambda n: 0.7**n
convz = lambda n: sum(xs_seq(k) * ys_seq(n - k) for k in range(n + 1))
check('Z convolution', close(ztr(convz, zz, 300), Xz(zz) * ztr(ys_seq, zz), 1e-8))
zb = 1e7
check('Z valeur initiale', close(Xz(zb), xs_seq(0), 1e-5))
X_fin = lambda z: z / (z - 1) - z / (z - 0.5)    # x_n = 1 - 0.5^n -> 1
check('Z valeur finale', close((1.000001 - 1) * X_fin(1.000001), 1.0, 1e-5))
y0r, aR2 = 2.0, 0.6
yv = [y0r]
for n in range(200):
    yv.append(aR2 * yv[-1] + xs_seq(n))
check('Z recurrence', close(ztr(lambda n: yv[n], zz, 200), (Xz(zz) + zz * y0r) / (zz - aR2), 1e-8))

# ---------------------------------------------------------------- probabilites
pB, nB, lamP = 0.3, 12, 2.4
ks = np.arange(0, nB + 1)
pmf = np.array([math.comb(nB, k) * pB**k * (1 - pB)**(nB - k) for k in ks])
check('binomiale somme/E/V', close([pmf.sum(), (ks * pmf).sum(), (ks**2 * pmf).sum() - (ks * pmf).sum()**2], [1, nB * pB, nB * pB * (1 - pB)]))
kg = np.arange(1, 2000)
pg = (1 - pB)**(kg - 1) * pB
check('geometrique', close([pg.sum(), (kg * pg).sum(), (kg**2 * pg).sum() - (kg * pg).sum()**2, pg[5:].sum()],
                           [1, 1 / pB, (1 - pB) / pB**2, (1 - pB)**5]))
kp = np.arange(0, 120)
pp_ = np.array([math.exp(-lamP) * lamP**k / math.factorial(k) for k in kp])
check('Poisson', close([pp_.sum(), (kp * pp_).sum(), (kp**2 * pp_).sum() - (kp * pp_).sum()**2], [1, lamP, lamP]))
conv = np.convolve(pp_[:60], np.array([math.exp(-1.1) * 1.1**k / math.factorial(k) for k in range(60)]))[:60]
check('somme de Poisson', close(conv, [math.exp(-(lamP + 1.1)) * (lamP + 1.1)**k / math.factorial(k) for k in range(60)]))
nU = 6; ku = np.arange(1, nU + 1)
check('uniforme discrete', close([ku.mean(), (ku**2).mean() - ku.mean()**2], [(nU + 1) / 2, (nU**2 - 1) / 12]))
check('Bernoulli', close([pB, pB - pB**2], [pB, pB * (1 - pB)]))
aU, bU = -1.0, 3.0
check('uniforme continue', close([integ(lambda x: x / (bU - aU), aU, bU), integ(lambda x: x * x / (bU - aU), aU, bU) - 1.0], [(aU + bU) / 2, (bU - aU)**2 / 12]))
lE = 0.8
fE = lambda x: lE * np.exp(-lE * x)
check('exponentielle', close([integ(fE, 0, 80), integ(lambda x: x * fE(x), 0, 80), integ(lambda x: x * x * fE(x), 0, 80) - (1 / lE)**2,
                              integ(fE, 0, 1.5)], [1, 1 / lE, 1 / lE**2, 1 - math.exp(-lE * 1.5)], 1e-6))
check('sans memoire', close(math.exp(-lE * (2 + 1.5)) / math.exp(-lE * 2), math.exp(-lE * 1.5)))
mu, sg = 1.5, 0.7
fN = lambda x: np.exp(-(x - mu)**2 / (2 * sg * sg)) / (sg * math.sqrt(2 * math.pi))
check('normale', close([integ(fN, -10, 12), integ(lambda x: x * fN(x), -10, 12), integ(lambda x: (x - mu)**2 * fN(x), -10, 12)], [1, mu, sg * sg]))
Phi = lambda z: 0.5 * (1 + math.erf(z / math.sqrt(2)))
check('valeurs Phi', close([Phi(1), Phi(1.645), Phi(1.96), Phi(2.576)], [0.841, 0.95, 0.975, 0.995], 1e-3))
check('P(|Z|<=z)', close([2 * Phi(z) - 1 for z in (1, 1.645, 1.96, 2.576)], [0.683, 0.90, 0.95, 0.99], 1e-3))
check('68-95-99,7', close([2 * Phi(1) - 1, 2 * Phi(2) - 1, 2 * Phi(3) - 1], [0.683, 0.954, 0.997], 1e-3) and close(Phi(-0.8), 1 - Phi(0.8)))
# Bayes
PA = [0.2, 0.5, 0.3]; PBA = [0.1, 0.4, 0.7]
PBt = sum(a * b for a, b in zip(PA, PBA))
check('probabilites totales / Bayes', close(PBt, 0.43) and close(PA[1] * PBA[1] / PBt, 0.2 / 0.43))
# Tchebychev (binomiale)
eps = 2.0
mB, vB = nB * pB, nB * pB * (1 - pB)
check('Tchebychev', sum(pmf[k] for k in ks if abs(k - mB) >= eps) <= vB / eps**2)
# statistiques
xs_d = rng.normal(size=50) * 2 + 3
ys_d = 1.5 * xs_d + rng.normal(size=50)
cov = np.mean((xs_d - xs_d.mean()) * (ys_d - ys_d.mean()))
check('variance (Koenig)', close(np.mean((xs_d - xs_d.mean())**2), np.mean(xs_d**2) - xs_d.mean()**2))
check('covariance', close(cov, np.mean(xs_d * ys_d) - xs_d.mean() * ys_d.mean()))
check('variance corrigee', close(np.sum((xs_d - xs_d.mean())**2) / 49, np.var(xs_d, ddof=1)))
a_reg = cov / np.var(xs_d)
b_reg = ys_d.mean() - a_reg * xs_d.mean()
check('regression', close([a_reg, b_reg], np.polyfit(xs_d, ys_d, 1)))
check('correlation', close(cov / (np.std(xs_d) * np.std(ys_d)), np.corrcoef(xs_d, ys_d)[0, 1]))
check('V(X+Y) cov', close(np.var(xs_d + ys_d), np.var(xs_d) + np.var(ys_d) + 2 * cov))
check('V(aX+b)', close(np.var(3 * xs_d + 2), 9 * np.var(xs_d)))


# ================================================================ v1.2
# ---------------------------------------------------------------- Taylor
def taylor_poly(derivs, a, x):
    return sum(dk * (x - a)**k / math.factorial(k) for k, dk in enumerate(derivs))
# Taylor-Lagrange (egalite) : exp, a = 0, n = 3 -> c dans ]0, x[
xT = 0.7
RT = math.exp(xT) - taylor_poly([1, 1, 1, 1], 0.0, xT)
cT = math.log(RT * 24 / xT**4)
check('Taylor-Lagrange c entre a et x', 0 < cT < xT)
# reste integral : sin, a = 0.2, x = 1.1, n = 4
aT, xT2, nT = 0.2, 1.1, 4
sd = [math.sin(aT), math.cos(aT), -math.sin(aT), -math.cos(aT), math.sin(aT)]
Rn = math.sin(xT2) - taylor_poly(sd, aT, xT2)
tt = np.linspace(aT, xT2, 200001)
ig = (xT2 - tt)**nT / math.factorial(nT) * np.cos(tt)      # f^(5) = cos
check('Taylor reste integral', close(Rn, np.trapezoid(ig, tt) if hasattr(np, 'trapezoid') else np.trapz(ig, tt), 1e-8))
# inegalite de Taylor-Lagrange : cos et ln(1+x)
for n in range(0, 6):
    for xx in (0.3, 1.0, 2.5):
        Tn = sum((math.cos(k * math.pi / 2)) * xx**k / math.factorial(k) for k in range(n + 1))
        check('Taylor-Lagrange cos n=%d' % n, abs(math.cos(xx) - Tn) <= xx**(n + 1) / math.factorial(n + 1) + 1e-15)
for n in range(1, 6):
    xx = 0.4
    Tn = sum((-1)**(k - 1) * xx**k / k for k in range(1, n + 1))
    M = math.factorial(n)          # |f^(n+1)(t)| = n!/(1+t)^(n+1) <= n! sur [0, x]
    check('Taylor-Lagrange ln n=%d' % n, abs(math.log(1 + xx) - Tn) <= M * xx**(n + 1) / math.factorial(n + 1))
# Taylor-Young : (f - T_n)/h^n -> 0 (ln en a = 0.5)
aY = 0.5
dY = [math.log(1 + aY)] + [(-1)**(k - 1) * math.factorial(k - 1) / (1 + aY)**k for k in range(1, 4)]
r1 = abs(math.log(1 + aY + 1e-2) - taylor_poly(dY, aY, aY + 1e-2)) / 1e-6
r2 = abs(math.log(1 + aY + 1e-3) - taylor_poly(dY, aY, aY + 1e-3)) / 1e-9
check('Taylor-Young', r2 < r1 / 5 and r2 < 1e-2)
# exemple e^0,1
T2 = 1 + 0.1 + 0.1**2 / 2
check('exemple e^0,1 : T2', close(T2, 1.105))
check('exemple e^0,1 : majoration', abs(math.exp(0.1) - T2) <= math.exp(0.1) * 0.1**3 / 6 < 2e-4)
check('exemple e^0,1 : erreur reelle', abs(abs(math.exp(0.1) - T2) - 1.7e-4) < 0.05e-4)
check('exemple e^0,1 : valeur', abs(math.exp(0.1) - 1.10517) < 1e-5)
h = 1e-3
check('DL sqrt(1+h)', abs(math.sqrt(1 + h) - (1 + h / 2 - h * h / 8)) / h**2 < 1e-3)
# asymptote : f(x) = sqrt(x^2 + x) = x + 1/2 - 1/(8x) + o(1/x)
xx = 1e3
check('developpement asymptotique', abs(math.sqrt(xx * xx + xx) - (xx + 0.5 - 1 / (8 * xx))) * xx < 1e-3)

# ---------------------------------------------------------------- inegalites
ok = True
for _ in range(2000):
    a_, b_ = np.sort(rng.normal(size=2) * 3)
    c_, d_ = np.sort(rng.normal(size=2) * 3)
    k = -abs(rng.normal()) - 0.01
    ok &= (k * a_ >= k * b_) and (a_ / k >= b_ / k) and (-a_ >= -b_)
    ok &= (a_ + c_ <= b_ + d_) and (a_ - d_ <= b_ - c_)
    if a_ * b_ > 0:
        ok &= 1 / a_ >= 1 / b_
    if a_ < 0 < b_:
        ok &= 1 / a_ < 0 < 1 / b_
    A, B, C, D = abs(a_), abs(a_) + abs(b_), abs(c_) + 0.01, abs(c_) + abs(d_) + 0.02
    ok &= A * C <= B * D and A / D <= B / C
    if b_ <= 0:
        ok &= a_**2 >= b_**2
    if a_ >= 0:
        ok &= a_**2 <= b_**2 and math.sqrt(a_) <= math.sqrt(b_)
check('regles des inegalites', bool(ok))
xs_i = np.linspace(-5, 5, 100001)
xs_i = xs_i[np.abs(xs_i) > 1e-9]
check('inequation 1/x < 2', bool(np.all((1 / xs_i < 2) == ((xs_i < 0) | (xs_i > 0.5)))))
check('|x| >= r', bool(np.all((np.abs(xs_i) >= 1.3) == ((xs_i <= -1.3) | (xs_i >= 1.3)))))
xb = np.linspace(-1, 4, 1001)
check('Bernoulli', all(bool(np.all((1 + xb)**n >= 1 + n * xb - 1e-12)) for n in range(1, 8)))
check('|sin x| <= |x|', bool(np.all(np.abs(np.sin(xs_i)) <= np.abs(xs_i))))
x2 = np.linspace(0, math.pi / 2, 1001)
check('2x/pi <= sin x', bool(np.all(2 * x2 / math.pi <= np.sin(x2) + 1e-15)))
u_, v_ = rng.normal(size=6), rng.normal(size=6)
check('Cauchy-Schwarz', (u_ @ v_)**2 <= (u_ @ u_) * (v_ @ v_))
check('fractions', close(2 / 3 + 5 / 7, (2 * 7 + 3 * 5) / 21) and close((2 / 3) / (5 / 7), 14 / 15))

# ---------------------------------------------------------------- trigo (relations)
xt = np.array([0.3, 1.1, 2.0, -0.8, 4.0])
check('1+cotan^2', close(1 + 1 / np.tan(xt)**2, 1 / np.sin(xt)**2))
check('cos^2 = 1/(1+tan^2)', close(np.cos(xt)**2, 1 / (1 + np.tan(xt)**2)))
check('sin^2 = tan^2/(1+tan^2)', close(np.sin(xt)**2, np.tan(xt)**2 / (1 + np.tan(xt)**2)))
s_, c_, t_ = np.sin(xt), np.cos(xt), np.tan(xt)
conv_tab = [
    (s_, np.sqrt(1 - c_**2)), (s_, t_ / np.sqrt(1 + t_**2)),
    (c_, np.sqrt(1 - s_**2)), (c_, 1 / np.sqrt(1 + t_**2)),
    (t_, s_ / np.sqrt(1 - s_**2)), (t_, np.sqrt(1 - c_**2) / c_)]
check('table de conversion (au signe pres)', all(close(np.abs(a), np.abs(b)) for a, b in conv_tab))
xa = np.array([0.2, 0.55, 0.9])
xr = np.array([-2.0, -0.3, 0.7, 3.0])
arc_tab = [
    (np.sin(np.arccos(xa)), np.sqrt(1 - xa**2)), (np.sin(np.arctan(xr)), xr / np.sqrt(1 + xr**2)),
    (np.cos(np.arcsin(xa)), np.sqrt(1 - xa**2)), (np.cos(np.arctan(xr)), 1 / np.sqrt(1 + xr**2)),
    (np.tan(np.arcsin(xa)), xa / np.sqrt(1 - xa**2)), (np.tan(np.arccos(xa)), np.sqrt(1 - xa**2) / xa),
    (np.sin(np.arcsin(-xa)), -xa), (np.cos(np.arcsin(-xa)), np.sqrt(1 - xa**2))]
check('trigo des reciproques', all(close(a, b) for a, b in arc_tab))
check('angle moitie cos^2', close(np.cos(xt / 2)**2, (1 + np.cos(xt)) / 2))
check('angle moitie sin^2', close(np.sin(xt / 2)**2, (1 - np.cos(xt)) / 2))
check('tan moitie', close(np.tan(xt / 2), np.sin(xt) / (1 + np.cos(xt))) and close(np.tan(xt / 2), (1 - np.cos(xt)) / np.sin(xt)))
check('cos 3x', close(np.cos(3 * xt), 4 * np.cos(xt)**3 - 3 * np.cos(xt)))
check('sin 3x', close(np.sin(3 * xt), 3 * np.sin(xt) - 4 * np.sin(xt)**3))
check('tan 3x', close(np.tan(3 * xt), (3 * t_ - t_**3) / (1 - 3 * t_**2)))
deriv_ok('cotan', lambda x: 1 / np.tan(x), lambda x: -1 - 1 / np.tan(x)**2, [0.4, 1.2, 2.5])
deriv_ok('cotan (2)', lambda x: 1 / np.tan(x), lambda x: -1 / np.sin(x)**2, [0.4, 1.2, 2.5])
prim_ok('tan', lambda x: -np.log(np.abs(np.cos(x))), np.tan, [0.3, 1.0, 2.2, -1.2])
prim_ok('1/cos^2', np.tan, lambda x: 1 / np.cos(x)**2, [0.3, 1.0, -1.2])
prim_ok('tan^2', lambda x: np.tan(x) - x, lambda x: np.tan(x)**2, [0.3, 1.0, -1.2])
xh = np.array([-1.5, 0.2, 0.9, 2.3])
check('1 - th^2', close(1 - th(xh)**2, 1 / ch(xh)**2))
check('ch^2 lineaire', close(ch(xh)**2, (ch(2 * xh) + 1) / 2))
check('sh^2 lineaire', close(sh(xh)**2, (ch(2 * xh) - 1) / 2))
check('ch 2x', close(ch(2 * xh), ch(xh)**2 + sh(xh)**2))
check('sh 2x', close(sh(2 * xh), 2 * sh(xh) * ch(xh)))

# ---------------------------------------------------------------- convolution
check('convolution (1,2,3)*(1,1)', list(np.convolve([1, 2, 3], [1, 1])) == [1, 3, 5, 3])
xa_, ya_, za_ = rng.normal(size=7), rng.normal(size=5), rng.normal(size=4)
check('convolution commutative', close(np.convolve(xa_, ya_), np.convolve(ya_, xa_)))
check('convolution associative', close(np.convolve(np.convolve(xa_, ya_), za_), np.convolve(xa_, np.convolve(ya_, za_))))
check('convolution longueur N+M-1', len(np.convolve(xa_, ya_)) == 7 + 5 - 1)
causal = [sum(xa_[k] * ya_[n - k] for k in range(0, n + 1) if k < 7 and n - k < 5) for n in range(11)]
check('convolution causale (somme de 0 a n)', close(causal, np.convolve(xa_, ya_)))
delta_k = np.zeros(4); delta_k[2] = 1
check('convolution par delta decale', close(np.convolve(xa_, delta_k)[2:9], xa_))
zz = 1.7 + 0.4j
Zof = lambda x: sum(x[n] * zz**(-n) for n in range(len(x)))
check('Z de la convolution', close(Zof(np.convolve(xa_, ya_)), Zof(xa_) * Zof(ya_)))
# porte * porte = T triangle
Tc = 1.5
dtc = 1e-3
tc = np.arange(-3, 3 + dtc / 2, dtc)
porte = np.where(np.abs(tc) < Tc / 2, 1.0, 0.0)
pp = np.convolve(porte, porte) * dtc
tpp = np.arange(len(pp)) * dtc - 6
tri = np.where(np.abs(tpp) <= Tc, Tc * (1 - np.abs(tpp) / Tc), 0.0)
check('porte * porte = T triangle', float(np.max(np.abs(pp - tri))) < 5e-3)
for w in (0.0, 1.1, 2.7):
    ex = Tc**2 if w == 0 else (Tc * math.sin(w * Tc / 2) / (w * Tc / 2))**2
    check('Fourier du triangle', close(fourier(lambda t: np.where(np.abs(t) <= Tc, Tc - np.abs(t), 0.0), w, T=2, n=400001), ex, 1e-5))
# Laplace : (e^-t * sin)(t) = (sin t - cos t + e^-t)/2
fc = lambda t: (np.sin(t) - np.cos(t) + np.exp(-t)) / 2
tq = 1.3
tau_ = np.linspace(0, tq, 200001)
num = np.exp(-tau_) * np.sin(tq - tau_)
check('convolution causale continue', close(np.sum((num[1:] + num[:-1]) / 2) * (tq / 200000), fc(tq), 1e-8))
check('Laplace convolution', close(laplace(fc, pL), 1 / (pL + 1) / (pL**2 + 1), 1e-6))
# integrale du produit de convolution (gaussiennes a = 1.3, b = 0.7)
cgg = lambda t: math.sqrt(math.pi / (aF + 0.7)) * np.exp(-aF * 0.7 / (aF + 0.7) * t * t)
check('integrale de f*g', close(integ(cgg, -40, 40, 400000), math.sqrt(math.pi / aF) * math.sqrt(math.pi / 0.7), 1e-6))
# derivee (f*g)' = f'*g (gaussiennes)
check("(f*g)' = f'*g", close(d(cgg, 0.6), integ(lambda s: -2 * aF * s * g(s) * g2(0.6 - s), -30, 30, 300000), 1e-6))
# somme de variables aleatoires : deux des, deux uniformes
de = np.ones(6) / 6
check('somme de deux des', close(np.convolve(de, de)[5], 6 / 36))
du = 1e-3
uni = np.ones(1000)
tri_u = np.convolve(uni, uni) * du
check('uniforme + uniforme = triangle', abs(tri_u[999] - 1.0) < 2e-3 and abs(tri_u[499] - 0.5) < 2e-3)

# ---------------------------------------------------------------- correlation
x_c, y_c = rng.normal(size=40), rng.normal(size=30)
Rxy = np.correlate(x_c, y_c, 'full')            # indice k + len(y) - 1
def R_def(x, y, k):
    return sum(x[n] * y[n - k] for n in range(len(x)) if 0 <= n - k < len(y))
check('intercorrelation (definition)', close([Rxy[k + 29] for k in (-5, 0, 3, 12)], [R_def(x_c, y_c, k) for k in (-5, 0, 3, 12)]))
check('intercorrelation = x * y(-t)', close(Rxy, np.convolve(x_c, y_c[::-1])))
Rx = np.correlate(x_c, x_c, 'full')
check('autocorrelation R(0) = energie', close(Rx[39], np.sum(x_c**2)))
check('autocorrelation paire', close(Rx, Rx[::-1]))
check('autocorrelation maximale en 0', bool(np.all(np.abs(Rx) <= Rx[39] + 1e-12)))
Ryx = np.correlate(y_c, x_c, 'full')
check('Rxy(-k) = Ryx(k)', close([R_def(x_c, y_c, -k) for k in (2, 7)], [R_def(y_c, x_c, k) for k in (2, 7)]))
# echo : y(t) = x(t - d) -> Ryx max en d
x_e = rng.normal(size=300)
dd = 37
y_e = np.concatenate([np.zeros(dd), x_e])[:300] + 0.1 * rng.normal(size=300)
lags = range(-100, 101)
check('echo : Ryx max en tau = d', max(lags, key=lambda k: R_def(y_e, x_e, k)) == dd)
# sinusoide
A_, w0_, ph_ = 1.7, 2.3, 0.6
Tper = 2 * math.pi / w0_
ts = np.linspace(0, Tper, 200001)[:-1]
for tau in (0.0, 0.4, 1.9):
    m = np.mean(A_ * np.cos(w0_ * ts + ph_) * A_ * np.cos(w0_ * (ts - tau) + ph_))
    check('autocorrelation sinusoide', close(m, A_**2 / 2 * math.cos(w0_ * tau), 1e-6))
# Wiener-Khintchine (discret, avec zeros)
Nf = 128
X_ = np.fft.fft(x_c, Nf); Y_ = np.fft.fft(y_c, Nf)
Rx_c = np.zeros(Nf); Rx_c[:40] = Rx[39:]; Rx_c[-39:] = Rx[:39]
check('Wiener-Khintchine discret', close(np.fft.fft(Rx_c), np.abs(X_)**2, 1e-9))
Rxy_c = np.zeros(Nf); Rxy_c[:40] = Rxy[29:]; Rxy_c[-29:] = Rxy[:29]
check('Wiener-Khintchine croise', close(np.fft.fft(Rxy_c), X_ * np.conj(Y_), 1e-9))
# Wiener-Khintchine continu : gaussienne
Rg = lambda tau: math.sqrt(math.pi / (2 * aF)) * np.exp(-aF * tau * tau / 2)
check('autocorrelation gaussienne', close(integ(lambda t: g(t) * g(t - 0.8), -30, 30, 300000), Rg(0.8), 1e-6))
for w in (0.0, 1.2):
    check('Wiener-Khintchine continu', close(fourier(Rg, w), abs(math.sqrt(math.pi / aF) * math.exp(-w * w / (4 * aF)))**2, 1e-6))
rho = Rxy / math.sqrt(Rx[39] * np.sum(y_c**2))
check('coefficient de correlation dans [-1,1]', bool(np.all(np.abs(rho) <= 1 + 1e-12)))
check('rho = -1 si proportionnels', close(np.sum(x_c * (-2.5 * x_c)) / math.sqrt(np.sum(x_c**2) * np.sum((2.5 * x_c)**2)), -1))
bb = rng.normal(size=200000) * 1.4
check('bruit blanc', abs(np.mean(bb[:-5] * bb[5:])) < 0.03 and abs(np.mean(bb * bb) - 1.96) < 0.03)

# ---------------------------------------------------------------- geometrie
def tri_data(A, B, C):
    a = np.linalg.norm(B - C); b = np.linalg.norm(C - A); c = np.linalg.norm(A - B)
    angA = math.acos(np.dot(B - A, C - A) / (b * c))
    angB = math.acos(np.dot(A - B, C - B) / (a * c))
    angC = math.acos(np.dot(A - C, B - C) / (a * b))
    area = abs((B - A)[0] * (C - A)[1] - (B - A)[1] * (C - A)[0]) / 2
    # centre du cercle circonscrit
    M = np.array([2 * (B - A), 2 * (C - A)])
    O = np.linalg.solve(M, np.array([B @ B - A @ A, C @ C - A @ A]))
    return a, b, c, angA, angB, angC, area, O
for _ in range(5):
    A, B, C = rng.normal(size=(3, 2)) * 3
    a, b, c, angA, angB, angC, area, O = tri_data(A, B, C)
    R = np.linalg.norm(A - O)
    p = (a + b + c) / 2
    check('somme des angles', close(angA + angB + angC, math.pi))
    check('Al-Kashi', close(a * a, b * b + c * c - 2 * b * c * math.cos(angA)))
    check('loi des sinus = 2R', close([a / math.sin(angA), b / math.sin(angB), c / math.sin(angC)], [2 * R] * 3))
    check('aire 1/2 bc sin A', close(area, 0.5 * b * c * math.sin(angA)))
    check('Heron', close(area, math.sqrt(p * (p - a) * (p - b) * (p - c))))
    check('aire abc/4R', close(area, a * b * c / (4 * R)))
    I = (a * A + b * B + c * C) / (a + b + c)       # centre du cercle inscrit
    u = (C - B) / a
    r_in = abs((I - B)[0] * u[1] - (I - B)[1] * u[0])
    check('aire r p', close(area, r_in * p))
    G = (A + B + C) / 3
    check('G aux 2/3 de la mediane', close(G, A + 2 / 3 * ((B + C) / 2 - A)))
    H = A + B + C - 2 * O                          # orthocentre
    check('orthocentre', abs(np.dot(H - A, C - B)) < 1e-9 * (1 + a * a) and abs(np.dot(H - B, C - A)) < 1e-9 * (1 + b * b))
    check('droite d Euler OH = 3 OG', close(H - O, 3 * (G - O)))
    # angle inscrit : point D sur le cercle, du meme cote que A
    check('angle inscrit = moitie de l angle au centre',
          close(angA, math.acos(np.dot(B - O, C - O) / R**2) / 2) or close(angA, math.pi - math.acos(np.dot(B - O, C - O) / R**2) / 2))
# equilateral
aE = 2.3
A, B, C = np.array([0, 0.0]), np.array([aE, 0.0]), np.array([aE / 2, aE * math.sqrt(3) / 2])
_, _, _, _, _, _, area, O = tri_data(A, B, C)
check('equilateral aire', close(area, math.sqrt(3) / 4 * aE**2))
check('equilateral R', close(np.linalg.norm(A - O), aE / math.sqrt(3)))
check('equilateral r', close(O[1], aE / (2 * math.sqrt(3))))
# triangle rectangle : centre du cercle circonscrit au milieu de l'hypotenuse
A, B, C = np.array([0, 0.0]), np.array([3.0, 0]), np.array([0, 2.0])
check('rectangle : milieu de l hypotenuse', close(tri_data(A, B, C)[7], (B + C) / 2))
# Thales (cercle) : M sur le cercle de diametre [AB] -> MA.MB = 0
th_ = 1.1
Mc = np.array([math.cos(th_), math.sin(th_)])
check('cercle de diametre [AB]', abs(np.dot(np.array([-1, 0]) - Mc, np.array([1, 0]) - Mc)) < 1e-12)
# cercle : corde, segment
Rc, thc = 1.8, 1.2
check('corde', close(np.linalg.norm(Rc * np.array([1, 0]) - Rc * np.array([math.cos(thc), math.sin(thc)])), 2 * Rc * math.sin(thc / 2)))
yy = np.linspace(-Rc, Rc, 400001)
xd = Rc * math.cos(thc / 2)
seg = np.trapezoid(np.clip(2 * (np.sqrt(np.maximum(Rc * Rc - yy * yy, 0)) - 0), 0, None) * 0 + np.where(np.sqrt(np.maximum(Rc * Rc - yy * yy, 0)) > xd, np.sqrt(np.maximum(Rc * Rc - yy * yy, 0)) - xd, 0), yy) if hasattr(np, 'trapezoid') else None
check('segment circulaire', close(seg, Rc * Rc / 2 * (thc - math.sin(thc)), 1e-6))
# polygones reguliers
for n in (3, 5, 6, 9):
    Rp = 1.4
    P = np.array([[Rp * math.cos(2 * math.pi * k / n), Rp * math.sin(2 * math.pi * k / n)] for k in range(n)])
    shoe = 0.5 * abs(sum(P[k][0] * P[(k + 1) % n][1] - P[(k + 1) % n][0] * P[k][1] for k in range(n)))
    check('polygone regulier aire n=%d' % n, close(shoe, n / 2 * Rp**2 * math.sin(2 * math.pi / n)))
    check('polygone regulier cote n=%d' % n, close(np.linalg.norm(P[1] - P[0]), 2 * Rp * math.sin(math.pi / n)))
    ang = math.acos(np.dot(P[0] - P[1], P[2] - P[1]) / np.linalg.norm(P[0] - P[1])**2)
    check('somme des angles polygone n=%d' % n, close(n * ang, (n - 2) * math.pi))
check('hexagone', close(6 / 2 * 1.0 * math.sin(math.pi / 3), 3 * math.sqrt(3) / 2))
# ellipse
def ell_perim(a, b, n=200001):
    t = np.linspace(0, 2 * math.pi, n)
    return np.trapezoid(np.sqrt((a * np.sin(t))**2 + (b * np.cos(t))**2), t)
def ell_int(a, b, n=200001):
    e2 = 1 - b * b / (a * a)
    t = np.linspace(0, math.pi / 2, n)
    return 4 * a * np.trapezoid(np.sqrt(1 - e2 * np.sin(t)**2), t)
worst = 0.0
for ratio in (1.0, 0.8, 0.5, 0.2, 0.05, 0.0):
    a_e, b_e = 2.0, 2.0 * ratio
    exact = ell_int(a_e, b_e)
    if ratio > 0:
        check('perimetre ellipse (integrale)', close(exact, ell_perim(a_e, b_e), 1e-7))
    ram = math.pi * (3 * (a_e + b_e) - math.sqrt((3 * a_e + b_e) * (a_e + 3 * b_e)))
    worst = max(worst, abs(ram - exact) / exact)
    if ratio >= 0.2:
        check('Ramanujan a moins de 0,03 %% (b/a=%g)' % ratio, abs(ram - exact) / exact < 3e-4)
    if ratio == 1.0:
        check('Ramanujan exact pour le cercle', close(ram, 2 * math.pi * a_e, 1e-12))
check('Ramanujan a moins de 0,5 %', worst < 0.005)
xe = np.linspace(-2, 2, 400001)
check('aire ellipse', close(np.trapezoid(2 * 1.2 * np.sqrt(np.maximum(1 - xe**2 / 4, 0)), xe), math.pi * 2 * 1.2, 1e-6))
# solides de revolution
def vol_rev(f, a, b, n=400001):
    x = np.linspace(a, b, n)
    return math.pi * np.trapezoid(f(x)**2, x)
def surf_rev(f, a, b, n=400001):
    x = np.linspace(a, b, n)
    fp = np.gradient(f(x), x)
    return 2 * math.pi * np.trapezoid(f(x) * np.sqrt(1 + fp**2), x)
Rs, hs, rs = 1.3, 2.1, 0.5
gs = math.sqrt(Rs**2 + hs**2)
check('volume cone', close(vol_rev(lambda x: Rs * (1 - x / hs), 0, hs), math.pi * Rs**2 * hs / 3, 1e-8))
check('aire laterale cone', close(surf_rev(lambda x: Rs * (1 - x / hs), 0, hs), math.pi * Rs * gs, 1e-6))
check('volume cylindre', close(vol_rev(lambda x: Rs + 0 * x, 0, hs), math.pi * Rs**2 * hs))
check('volume boule', close(vol_rev(lambda x: np.sqrt(np.maximum(Rs**2 - x**2, 0)), -Rs, Rs), 4 / 3 * math.pi * Rs**3, 1e-6))
check('aire sphere', close(surf_rev(lambda x: np.sqrt(np.maximum(Rs**2 - x**2, 0)), -Rs + 1e-9, Rs - 1e-9, 2000001), 4 * math.pi * Rs**2, 1e-3))
check('volume tronc de cone', close(vol_rev(lambda x: Rs + (rs - Rs) * x / hs, 0, hs), math.pi * hs / 3 * (Rs**2 + Rs * rs + rs**2), 1e-8))
check('aire tronc de cone', close(surf_rev(lambda x: Rs + (rs - Rs) * x / hs, 0, hs), math.pi * (Rs + rs) * math.sqrt(hs**2 + (Rs - rs)**2), 1e-6))
hc = 0.7
check('volume calotte', close(vol_rev(lambda x: np.sqrt(np.maximum(Rs**2 - x**2, 0)), Rs - hc, Rs), math.pi * hc**2 / 3 * (3 * Rs - hc), 1e-6))
check('aire calotte', close(surf_rev(lambda x: np.sqrt(np.maximum(Rs**2 - x**2, 0)), Rs - hc, Rs - 1e-9, 2000001), 2 * math.pi * Rs * hc, 1e-3))
check('volume ellipsoide', close(np.trapezoid(math.pi * 1.1 * 0.6 * (1 - np.linspace(-1.7, 1.7, 400001)**2 / 1.7**2), np.linspace(-1.7, 1.7, 400001)), 4 / 3 * math.pi * 1.7 * 1.1 * 0.6, 1e-8))
# tore (R, r) : volume et aire par integrales, puis Guldin
Rt, rt = 2.0, 0.6
yv = np.linspace(-rt, rt, 400001)
sq = np.sqrt(np.maximum(rt**2 - yv**2, 0))
Vt = math.pi * np.trapezoid((Rt + sq)**2 - (Rt - sq)**2, yv)
check('volume tore', close(Vt, 2 * math.pi**2 * Rt * rt**2, 1e-6))
uu = np.linspace(0, 2 * math.pi, 4001)
St = 2 * math.pi * np.trapezoid(rt * (Rt + rt * np.cos(uu)), uu)
check('aire tore', close(St, 4 * math.pi**2 * Rt * rt, 1e-8))
check('Guldin (tore)', close(2 * math.pi * Rt * math.pi * rt**2, Vt, 1e-6) and close(2 * math.pi * Rt * 2 * math.pi * rt, St, 1e-8))
# tetraedre regulier
aT_ = 1.6
P4 = np.array([[1, 1, 1], [1, -1, -1], [-1, 1, -1], [-1, -1, 1]], dtype=float) * aT_ / (2 * math.sqrt(2))
check('tetraedre arete', close(np.linalg.norm(P4[0] - P4[1]), aT_))
check('tetraedre volume', close(abs(np.linalg.det(np.array([P4[1] - P4[0], P4[2] - P4[0], P4[3] - P4[0]]))) / 6, math.sqrt(2) / 12 * aT_**3))
check('tetraedre aire', close(4 * np.linalg.norm(np.cross(P4[1] - P4[0], P4[2] - P4[0])) / 2, math.sqrt(3) * aT_**2))
check('pave droit aire', close(2 * (1 * 2 + 2 * 3 + 3 * 1), 22))
check('longueur d arc (cercle)', close(np.trapezoid(np.sqrt(1 + np.gradient(np.sqrt(4 - np.linspace(-1, 1, 400001)**2), np.linspace(-1, 1, 400001))**2), np.linspace(-1, 1, 400001)), 2 * 2 * math.asin(0.5), 1e-6))
# barycentre
Pts = rng.normal(size=(5, 2)); al = np.array([1.0, 2.5, -0.7, 3.0, 0.4])
Gb = (al[:, None] * Pts).sum(0) / al.sum()
check('barycentre : somme ponderee nulle', close((al[:, None] * (Pts - Gb)).sum(0), [0, 0]))
Mb = np.array([0.3, -1.2])
check('reduction somme alpha MA = (somme alpha) MG', close((al[:, None] * (Pts - Mb)).sum(0), al.sum() * (Gb - Mb)))
check('Leibniz', close((al * ((Pts - Mb)**2).sum(1)).sum(), al.sum() * ((Gb - Mb)**2).sum() + (al * ((Pts - Gb)**2).sum(1)).sum()))
G2 = (2.0 * Pts[0] + 3.0 * Pts[1]) / 5.0
check('barycentre de deux points', close(G2 - Pts[0], 3.0 / 5.0 * (Pts[1] - Pts[0])))
G_assoc = ((al[:2].sum()) * ((al[:2, None] * Pts[:2]).sum(0) / al[:2].sum()) + (al[2:, None] * Pts[2:]).sum(0)) / al.sum()
check('associativite', close(G_assoc, Gb))
check('homogeneite', close(((3 * al)[:, None] * Pts).sum(0) / (3 * al).sum(), Gb))
# centres de masse
Rm = 1.5
yy = np.linspace(0, Rm, 400001)
check('demi-disque 4R/3pi', close(np.trapezoid(yy * 2 * np.sqrt(Rm**2 - yy**2), yy) / (math.pi * Rm**2 / 2), 4 * Rm / (3 * math.pi), 1e-6))
tt = np.linspace(0, math.pi, 400001)
check('demi-cercle 2R/pi', close(np.trapezoid(Rm * np.sin(tt), tt) / math.pi, 2 * Rm / math.pi, 1e-8))
hm = 2.0
zz_ = np.linspace(0, hm, 400001)
check('triangle h/3', close(np.trapezoid(zz_ * (1 - zz_ / hm), zz_) / np.trapezoid(1 - zz_ / hm, zz_), hm / 3, 1e-8))
check('cone h/4', close(np.trapezoid(zz_ * (1 - zz_ / hm)**2, zz_) / np.trapezoid((1 - zz_ / hm)**2, zz_), hm / 4, 1e-8))
zr = np.linspace(0, Rm, 400001)
check('demi-boule 3R/8', close(np.trapezoid(zr * (Rm**2 - zr**2), zr) / np.trapezoid(Rm**2 - zr**2, zr), 3 * Rm / 8, 1e-8))
check('demi-sphere R/2', close(np.trapezoid(zr * 2 * math.pi * Rm, zr) / (2 * math.pi * Rm**2), Rm / 2, 1e-8))
# droites et coniques
aD, bD, cD = 2.0, -3.0, 1.5
Mp = np.array([1.2, 2.7])
nD = np.array([aD, bD]); P0 = np.array([0.0, cD / 3.0])     # point de la droite
check('distance point-droite', close(abs(np.dot(Mp - P0, nD)) / np.linalg.norm(nD), abs(aD * Mp[0] + bD * Mp[1] + cD) / math.hypot(aD, bD)))
check('vecteur directeur (-b, a)', close(np.dot(np.array([-bD, aD]), nD), 0))
check('perpendiculaires m m\' = -1', close(np.dot([1, 0.8], [1, -1 / 0.8]), 0))
ae, be = 2.5, 1.5
ce = math.sqrt(ae**2 - be**2)
ok = True
for t in np.linspace(0, 6, 13):
    Mq = np.array([ae * math.cos(t), be * math.sin(t)])
    ok &= abs(np.linalg.norm(Mq - [ce, 0]) + np.linalg.norm(Mq + [ce, 0]) - 2 * ae) < 1e-12
check('ellipse MF + MF\' = 2a', bool(ok))
ah, bh = 1.2, 0.9
chh = math.sqrt(ah**2 + bh**2)
ok = True
for t in np.linspace(-2, 2, 9):
    Mq = np.array([ah * math.cosh(t), bh * math.sinh(t)])
    ok &= abs(abs(np.linalg.norm(Mq - [chh, 0]) - np.linalg.norm(Mq + [chh, 0])) - 2 * ah) < 1e-9
check('hyperbole |MF - MF\'| = 2a', bool(ok))
check('asymptote hyperbole', abs(bh * math.sinh(8) / (ah * math.cosh(8)) - bh / ah) < 1e-6)
pp_ = 1.3
ok = True
for y in (-2, -0.5, 0.3, 1.7):
    x = y * y / (2 * pp_)
    ok &= abs(math.hypot(x - pp_ / 2, y) - (x + pp_ / 2)) < 1e-12
check('parabole foyer-directrice', bool(ok))
aq = 0.8
check('y = ax^2 : foyer (0, 1/4a)', all(abs(math.hypot(x, aq * x * x - 1 / (4 * aq)) - (aq * x * x + 1 / (4 * aq))) < 1e-12 for x in (-1.0, 0.4, 2.0)))
for e in (0.0, 0.5, 1.0, 1.7):
    pc = 1.1
    ok = True
    for th_ in (0.2, 1.0, 2.0):
        if 1 + e * math.cos(th_) <= 0:
            continue
        r_ = pc / (1 + e * math.cos(th_))
        if e > 0:
            ok &= abs(r_ - e * (pc / e - r_ * math.cos(th_))) < 1e-12
        else:
            ok &= abs(r_ - pc) < 1e-12
    check('conique polaire e=%g' % e, bool(ok))


# ---------------------------------------------------------------- langage mathematique
from itertools import product, combinations
imp = lambda a, b: (not a) or b
table_v = [(True, True, False, True, True, True), (True, False, False, False, True, False),
           (False, True, True, False, True, True), (False, False, True, False, False, True)]
check('table de verite', all((not P_) == nP and (P_ and Q_) == et and (P_ or Q_) == ou and imp(P_, Q_) == im
                             for (P_, Q_, nP, et, ou, im) in table_v))
TF = (True, False)
check('contraposee', all(imp(a, b) == imp(not b, not a) for a, b in product(TF, TF)))
check('equivalence = double implication', all((a == b) == (imp(a, b) and imp(b, a)) for a, b in product(TF, TF)))
check('De Morgan (logique)', all((not (a and b)) == ((not a) or (not b)) and (not (a or b)) == ((not a) and (not b)) for a, b in product(TF, TF)))
check('negation implication', all((not imp(a, b)) == (a and not b) for a, b in product(TF, TF)))
dom = range(-5, 6)
Pp = lambda x: x * x > 3
check('negation quantificateurs', (not all(Pp(x) for x in dom)) == any(not Pp(x) for x in dom)
      and (not any(x * x == 7 for x in dom)) == all(x * x != 7 for x in dom))
E_ = set(range(12))
ok = True
for _ in range(200):
    A_ = set(x for x in E_ if rng.random() < 0.5)
    B_ = set(x for x in E_ if rng.random() < 0.5)
    C_ = set(x for x in E_ if rng.random() < 0.5)
    comp = lambda X: E_ - X
    ok &= comp(A_ | B_) == comp(A_) & comp(B_) and comp(A_ & B_) == comp(A_) | comp(B_)
    ok &= A_ & (B_ | C_) == (A_ & B_) | (A_ & C_) and A_ | (B_ & C_) == (A_ | B_) & (A_ | C_)
    ok &= len(A_ | B_) == len(A_) + len(B_) - len(A_ & B_)
    ok &= len(set(product(A_, B_))) == len(A_) * len(B_)
check('ensembles : De Morgan, distributivite, cardinaux', bool(ok))
check('card P(E) = 2^n', sum(1 for k in range(6) for _ in combinations(range(5), k)) == 2**5)
check('{x | x^2 < 1} = ]-1,1[', all(((x * x < 1) == (-1 < x < 1)) for x in np.linspace(-3, 3, 601)))

print('%d verifications, %d echec(s)' % (COUNT[0], len(FAILS)))
raise SystemExit(1 if FAILS else 0)
