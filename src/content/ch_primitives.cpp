// Chapitre : primitives. Format : voir src/doc/Content.hpp.
extern const char CH_PRIMITIVES[];
const char CH_PRIMITIVES[] = R"TEX(
@page Primitives usuelles
@card Puissances
@tags primitive puissance inverse racine
@table[ccc] f(x) & F(x) & \text{Intervalle}
@row a & a\,x & \mathbb{R}
@row x^n\ \ (n\in\mathbb{N}) & \frac{x^{n+1}}{n+1} & \mathbb{R}
@row \frac{1}{x} & \ln|x| & x\neq 0
@row \frac{1}{x^2} & -\frac{1}{x} & x\neq 0
@row \frac{1}{x^n}\ \ (n\geq 2) & -\frac{1}{(n-1)\,x^{n-1}} & x\neq 0
@row x^{\alpha}\ \ (\alpha\neq -1) & \frac{x^{\alpha+1}}{\alpha+1} & x>0
@row \frac{1}{\sqrt{x}} & 2\sqrt{x} & x>0
@row \sqrt{x} & \frac{2}{3}\,x\sqrt{x} & x\geq 0
@note Une primitive est définie à une constante près : $F(x)+C$, sur un intervalle.
@card Exponentielle et logarithme
@tags exp ln log
@table[ccc] f(x) & F(x) & \text{Intervalle}
@row e^x & e^x & \mathbb{R}
@row e^{ax} & \frac{1}{a}\,e^{ax} & a\neq 0
@row a^x & \frac{a^x}{\ln a} & a>0,\ a\neq 1
@row \ln x & x\ln x-x & x>0
@row x\,e^{ax} & \left(\frac{x}{a}-\frac{1}{a^2}\right)e^{ax} & a\neq 0
@card Trigonométrie
@tags sin cos tan cotan
@table[cc] f(x) & F(x)
@row \sin x & -\cos x
@row \cos x & \sin x
@row \tan x & -\ln|\cos x|
@row \cotan x & \ln|\sin x|
@row \frac{1}{\cos^2 x}=1+\tan^2 x & \tan x
@row \frac{1}{\sin^2 x} & -\cotan x
@row \sin(ax+b) & -\frac{1}{a}\cos(ax+b)
@row \cos(ax+b) & \frac{1}{a}\sin(ax+b)
@row \sin^2 x & \frac{x}{2}-\frac{\sin 2x}{4}
@row \cos^2 x & \frac{x}{2}+\frac{\sin 2x}{4}
@row \frac{1}{\cos x} & \ln\left|\tan\left(\frac{x}{2}+\frac{\pi}{4}\right)\right|
@row \frac{1}{\sin x} & \ln\left|\tan\frac{x}{2}\right|
@page Réciproques, hyperboliques
@card Fonctions réciproques
@tags arctan arcsin arccos argsh argch argth
@table[ccc] f(x) & F(x) & \text{Intervalle}
@row \frac{1}{1+x^2} & \arctan x & \mathbb{R}
@row \frac{1}{a^2+x^2} & \frac{1}{a}\arctan\frac{x}{a} & a\neq 0
@row \frac{1}{\sqrt{1-x^2}} & \arcsin x & ]-1,1[
@row \frac{1}{\sqrt{a^2-x^2}} & \arcsin\frac{x}{a} & |x|<a
@row -\frac{1}{\sqrt{1-x^2}} & \arccos x & ]-1,1[
@row \frac{1}{\sqrt{x^2+1}} & \ln\left(x+\sqrt{x^2+1}\right) & \mathbb{R}
@row \frac{1}{\sqrt{x^2-1}} & \ln\left|x+\sqrt{x^2-1}\right| & |x|>1
@row \frac{1}{\sqrt{x^2+k}} & \ln\left|x+\sqrt{x^2+k}\right| & x^2+k>0
@row \frac{1}{1-x^2} & \frac{1}{2}\ln\left|\frac{1+x}{1-x}\right| & x\neq\pm 1
@row \frac{1}{x^2-a^2} & \frac{1}{2a}\ln\left|\frac{x-a}{x+a}\right| & x\neq\pm a
@note $\ln\left(x+\sqrt{x^2+1}\right)=\argsh x$ ; sur $]-1,1[$, $\frac{1}{2}\ln\frac{1+x}{1-x}=\argth x$.
@card Hyperboliques
@tags sh ch th sinh cosh tanh
@table[cc] f(x) & F(x)
@row \sh x & \ch x
@row \ch x & \sh x
@row \th x & \ln(\ch x)
@row \frac{1}{\ch^2 x} & \th x

@page Formes composées
@card Reconnaître u' f(u)
@tags compose u forme
@table[ccc] f & F & \text{Condition}
@row u'\,u^n & \frac{u^{n+1}}{n+1} & n\neq -1
@row u'\,u^{\alpha} & \frac{u^{\alpha+1}}{\alpha+1} & u>0,\ \alpha\neq -1
@row \frac{u'}{u} & \ln|u| & u\neq 0
@row \frac{u'}{u^2} & -\frac{1}{u} & u\neq 0
@row \frac{u'}{u^n} & -\frac{1}{(n-1)\,u^{n-1}} & n\geq 2
@row \frac{u'}{\sqrt{u}} & 2\sqrt{u} & u>0
@row u'\,e^u & e^u &
@row u'\cos u & \sin u &
@row u'\sin u & -\cos u &
@row \frac{u'}{\cos^2 u} & \tan u &
@row \frac{u'}{1+u^2} & \arctan u &
@row \frac{u'}{\sqrt{1-u^2}} & \arcsin u & |u|<1
@card Fonction affine composée
@math \int f(ax+b)\,dx=\frac{1}{a}\,F(ax+b)+C
@card Linéarité
@math \int\left(\alpha f+\beta g\right)=\alpha\int f+\beta\int g

@page Éléments simples
@card 1. Partie entière (deg P ≥ deg Q)
@tags division euclidienne partie entiere degre
@text Si $\deg P\geq\deg Q$, division euclidienne : $P=Q\,E+R$ avec $\deg R<\deg Q$.
@math \frac{P}{Q}=E+\frac{R}{Q}
@math \frac{x^3+2x+1}{x^2+1}=x+\frac{x+1}{x^2+1}
@math \frac{x+1}{x-1}=1+\frac{2}{x-1}
@note $E$ (polynôme) est la partie entière. Si $\deg P<\deg Q$ : pas de partie entière, $E=0$.
@card 2. Factoriser le dénominateur
@tags factoriser poles racines
@math Q(x)=c\prod_i\left(x-a_i\right)^{m_i}\prod_j\left(x^2+p_jx+q_j\right)^{s_j}
@note Racines réelles $a_i$ (pôles) de multiplicité $m_i$ ; trinômes à $\Delta<0$ (irréductibles sur $\mathbb{R}$).
@card 3. Forme de la décomposition
@tags forme decomposition
@text Chaque facteur de $Q$ donne les termes :
@align x-a &\longrightarrow\frac{A}{x-a}
@align (x-a)^m &\longrightarrow\frac{A_1}{x-a}+\cdots+\frac{A_m}{(x-a)^m}
@align x^2+px+q &\longrightarrow\frac{Bx+C}{x^2+px+q}
@note Trinôme à $\Delta<0$. Puissance $k$ d'un trinôme : $\sum_{i=1}^{k}\frac{B_ix+C_i}{\left(x^2+px+q\right)^i}$.
@card 4. Calcul des coefficients
@tags coefficient cache multiplier limite identification
@li **Pôle simple** $a$ : multiplier par $(x-a)$ et faire $x=a$, ou $A=\frac{P(a)}{Q'(a)}$.
@li **Pôle multiple** : $A_m=\left[(x-a)^m\frac{P}{Q}\right]_{x=a}$ (plus haute puissance).
@li **Autres coefficients** : limite de $x\,F(x)$ en $+\infty$, valeurs particulières ($x=0$…), ou identification.
@li **Trinôme** : multiplier par $x^2+px+q$ et faire $x=$ racine complexe.
@card Exemple (pôle double)
@tags exemple
@math F(x)=\frac{2x+1}{x(x-1)^2}=\frac{a}{x}+\frac{b}{x-1}+\frac{c}{(x-1)^2}
@math a=\left[xF\right]_{x=0}=1\qquad c=\left[(x-1)^2F\right]_{x=1}=3
@math \lim_{x\to+\infty}xF(x)=0=a+b\implies b=-1
@math F(x)=\frac{1}{x}-\frac{1}{x-1}+\frac{3}{(x-1)^2}
@card Cas usuel
@tags deux poles
@math \frac{1}{(x-a)(x-b)}=\frac{1}{a-b}\left(\frac{1}{x-a}-\frac{1}{x-b}\right)

@page Intégrer une fraction rationnelle
@card Éléments de première espèce
@tags pole logarithme
@math \int\frac{dx}{x-a}=\ln|x-a|
@math \int\frac{dx}{(x-a)^n}=-\frac{1}{(n-1)(x-a)^{n-1}}\quad(n\geq 2)
@card Éléments de seconde espèce
@tags trinome arctan canonique
@math \frac{Bx+C}{x^2+px+q}=\frac{B}{2}\,\frac{2x+p}{x^2+px+q}+\frac{C-\frac{Bp}{2}}{x^2+px+q}
@math \int\frac{2x+p}{x^2+px+q}\,dx=\ln\left(x^2+px+q\right)
@math \int\frac{dx}{\left(x+\frac{p}{2}\right)^2+\omega^2}=\frac{1}{\omega}\arctan\frac{x+\frac{p}{2}}{\omega}
@note Forme canonique : $x^2+px+q=\left(x+\frac{p}{2}\right)^2+\omega^2$ avec $\omega^2=q-\frac{p^2}{4}>0$.
@card Trinôme ax² + bx + c
@tags trinome arctan discriminant
@math \int\frac{2ax+b}{ax^2+bx+c}\,dx=\ln\left|ax^2+bx+c\right|
@text Si $\Delta=b^2-4ac<0$ :
@math \int\frac{dx}{ax^2+bx+c}=\frac{2}{\sqrt{-\Delta}}\arctan\frac{2ax+b}{\sqrt{-\Delta}}
@note Si $\Delta>0$ : décomposer avec les deux racines. Si $\Delta=0$ : $\int\frac{dx}{a(x-x_0)^2}=-\frac{1}{a(x-x_0)}$.
)TEX";
