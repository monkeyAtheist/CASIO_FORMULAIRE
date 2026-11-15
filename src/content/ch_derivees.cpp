// Chapitre : derivees. Format : voir src/doc/Content.hpp.
extern const char CH_DERIVEES[];
const char CH_DERIVEES[] = R"TEX(
@page Dérivées usuelles
@card Puissances et racines
@tags derivee puissance racine inverse
@table[ccc] f(x) & f'(x) & \text{Validité}
@row k & 0 & \mathbb{R}
@row x & 1 & \mathbb{R}
@row x^n & n\,x^{n-1} & \mathbb{R},\ n\in\mathbb{N}^*
@row \frac{1}{x} & -\frac{1}{x^2} & \mathbb{R}^*
@row \frac{1}{x^n} & -\frac{n}{x^{n+1}} & \mathbb{R}^*,\ n\in\mathbb{N}^*
@row x^{\alpha} & \alpha\,x^{\alpha-1} & ]0,+\infty[,\ \alpha\in\mathbb{R}
@row \sqrt{x} & \frac{1}{2\sqrt{x}} & ]0,+\infty[
@row \sqrt[n]{x} & \frac{1}{n\sqrt[n]{x^{n-1}}} & ]0,+\infty[
@card Exponentielle et logarithme
@tags exp ln log exponentielle logarithme
@table[ccc] f(x) & f'(x) & \text{Validité}
@row e^x & e^x & \mathbb{R}
@row a^x & a^x\ln a & \mathbb{R},\ a>0
@row \ln x & \frac{1}{x} & ]0,+\infty[
@row \ln|x| & \frac{1}{x} & \mathbb{R}^*
@row \log_a x & \frac{1}{x\ln a} & ]0,+\infty[
@card Trigonométrie
@tags sin cos tan arcsin arccos arctan cotan
@table[cc] f(x) & f'(x)
@row \sin x & \cos x
@row \cos x & -\sin x
@row \tan x & 1+\tan^2 x=\frac{1}{\cos^2 x}
@row \cotan x & -1-\cotan^2 x=-\frac{1}{\sin^2 x}
@row \arcsin x & \frac{1}{\sqrt{1-x^2}}
@row \arccos x & -\frac{1}{\sqrt{1-x^2}}
@row \arctan x & \frac{1}{1+x^2}
@note $\tan$ : $x\neq\frac{\pi}{2}+k\pi$ ; $\cotan$ : $x\neq k\pi$ ; $\arcsin$ et $\arccos$ sont dérivables sur $]-1,1[$.
@card Hyperboliques
@tags sh ch th sinh cosh tanh argsh argch argth
@table[ccc] f(x) & f'(x) & \text{Validité}
@row \sh x & \ch x & \mathbb{R}
@row \ch x & \sh x & \mathbb{R}
@row \th x & 1-\th^2 x=\frac{1}{\ch^2 x} & \mathbb{R}
@row \argsh x & \frac{1}{\sqrt{x^2+1}} & \mathbb{R}
@row \argch x & \frac{1}{\sqrt{x^2-1}} & ]1,+\infty[
@row \argth x & \frac{1}{1-x^2} & ]-1,1[
@note $\sh=\sinh$, $\ch=\cosh$, $\th=\tanh$.

@page Opérations
@card Somme, produit, quotient
@tags uv produit quotient somme
@align (u+v)' &= u'+v'
@align (\lambda u)' &= \lambda\,u'
@align (uv)' &= u'v+uv'
@align \left(\frac{1}{v}\right)' &= -\frac{v'}{v^2}
@align \left(\frac{u}{v}\right)' &= \frac{u'v-uv'}{v^2}
@note $v$ ne s'annule pas sur l'intervalle.
@card Produit de trois fonctions
@math (uvw)' = u'vw+uv'w+uvw'
@card Composée
@tags chaine compose
@math (v\circ u)' = (v'\circ u)\times u'
@math \bigl(v(u(x))\bigr)' = v'\bigl(u(x)\bigr)\,u'(x)
@card Bijection réciproque
@tags reciproque inverse
@math \left(f^{-1}\right)'(y) = \frac{1}{f'\left(f^{-1}(y)\right)}
@note si $f'\left(f^{-1}(y)\right)\neq 0$.
@card Formule de Leibniz
@tags derivee n-ieme
@math (uv)^{(n)} = \sum_{k=0}^{n}\binom{n}{k}\,u^{(k)}\,v^{(n-k)}

@page Formes composées
@card Dérivée de f(u)
@tags compose u
@table[cc] \text{Fonction} & \text{Dérivée}
@row u^n & n\,u'\,u^{n-1}
@row \frac{1}{u} & -\frac{u'}{u^2}
@row \sqrt{u} & \frac{u'}{2\sqrt{u}}
@row u^{\alpha} & \alpha\,u'\,u^{\alpha-1}
@row e^u & u'\,e^u
@row \ln|u| & \frac{u'}{u}
@row \sin u & u'\cos u
@row \cos u & -u'\sin u
@row \tan u & u'\left(1+\tan^2 u\right)=\frac{u'}{\cos^2 u}
@row \arctan u & \frac{u'}{1+u^2}
@row \arcsin u & \frac{u'}{\sqrt{1-u^2}}
@row a^u & u'\,a^u\ln a
@row u^v=e^{v\ln u} & \left(v'\ln u+\frac{v\,u'}{u}\right)u^v
@card Fonction affine composée
@math \bigl(f(ax+b)\bigr)' = a\,f'(ax+b)

@page Théorèmes
@card Accroissements finis
@tags taf rolle
@text Si $f$ est continue sur $[a,b]$ et dérivable sur $]a,b[$ :
@math \exists\,c\in\,]a,b[,\quad f(b)-f(a)=f'(c)\,(b-a)
@note Rolle : si de plus $f(a)=f(b)$, il existe $c$ avec $f'(c)=0$.
@card Inégalité des accroissements finis
@tags iaf
@math |f'|\leq M\ \text{sur}\ I \implies |f(b)-f(a)|\leq M\,|b-a|
@card Variations et extremums
@li $f'\geq 0$ sur un intervalle $\iff f$ croissante.
@li $f'>0$ sauf en des points isolés $\implies f$ strictement croissante.
@li Extremum local en $a$ intérieur, $f$ dérivable en $a$ : $f'(a)=0$.
@card Tangente et approximation affine
@tags tangente
@math y=f(a)+f'(a)\,(x-a)
@math f(a+h)\approx f(a)+h\,f'(a)
)TEX";
