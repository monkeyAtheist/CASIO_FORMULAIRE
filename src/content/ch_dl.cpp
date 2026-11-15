// Chapitre : developpements limites, equivalents, limites.
extern const char CH_DL[];
const char CH_DL[] = R"TEX(
@page Formules de Taylor
@card Polynôme de Taylor et reste
@tags taylor polynome reste formule
@math f(x)=\sum_{k=0}^{n}\frac{f^{(k)}(a)}{k!}\,(x-a)^k+R_n(x)
@align T_n(x) &= f(a)+f'(a)(x-a)+\frac{f''(a)}{2!}(x-a)^2
@align &\quad+\cdots+\frac{f^{(n)}(a)}{n!}(x-a)^n
@note $T_n$ : polynôme de Taylor de $f$ d'ordre $n$ en $a$. $R_n(x)=f(x)-T_n(x)$ : reste. Les formules suivantes précisent ce reste.
@card Taylor-Young
@tags young petit o local
@align f(a+h) &\underset{h\to 0}{=} f(a)+f'(a)\,h+\frac{f''(a)}{2!}\,h^2+\cdots
@align &\quad+\frac{f^{(n)}(a)}{n!}\,h^n+o(h^n)
@math R_n(x)=o((x-a)^n)\quad\text{quand }x\to a
@note Il suffit que $f$ soit $n$ fois dérivable en $a$. Formule locale : elle donne un DL, mais aucune majoration de l'erreur.
@card Taylor-Lagrange
@tags lagrange reste egalite
@math R_n(x)=\frac{f^{(n+1)}(c)}{(n+1)!}\,(x-a)^{n+1}
@note Pour un certain $c$ strictement entre $a$ et $x$ ; $f$ de classe $\mathcal{C}^n$ et $n+1$ fois dérivable entre $a$ et $x$. Pour $n=0$ : accroissements finis.
@card Inégalité de Taylor-Lagrange
@tags lagrange majoration erreur
@math \left|f(x)-T_n(x)\right|\leq M\,\frac{|x-a|^{n+1}}{(n+1)!}
@note $M=\sup\left|f^{(n+1)}\right|$ entre $a$ et $x$ ($f$ de classe $\mathcal{C}^{n+1}$). Formule globale : majore l'erreur commise en remplaçant $f$ par $T_n$.
@card Taylor avec reste intégral
@tags taylor reste integral
@math R_n(x)=\int_a^x\frac{(x-t)^n}{n!}\,f^{(n+1)}(t)\,dt
@note $f$ de classe $\mathcal{C}^{n+1}$. Formule exacte, dont on déduit les deux précédentes.
@card Mac-Laurin (a = 0)
@tags maclaurin zero
@align f(x) &= f(0)+f'(0)\,x+\frac{f''(0)}{2!}\,x^2+\cdots
@align &\quad+\frac{f^{(n)}(0)}{n!}\,x^n+o(x^n)
@note Les DL de la page « DL usuels en 0 » s'obtiennent ainsi.
@card Exemple : valeur approchée
@tags exemple approximation erreur
@math e^{0{,}1}\approx T_2(0{,}1)=1+0{,}1+\frac{0{,}1^2}{2}=1{,}105
@math \left|R_2\right|\leq e^{0{,}1}\,\frac{0{,}1^3}{3!}<2\times 10^{-4}
@note Valeur exacte : $e^{0{,}1}=1{,}105\,17\ldots$ (erreur réelle $1{,}7\times 10^{-4}$).

@page Développements limités
@card Définition
@tags dl developpement limite definition
@math f(a+h)\underset{h\to 0}{=}\sum_{k=0}^{n}a_k\,h^k+o(h^n)
@li Unicité : si le DL existe, ses coefficients sont uniques.
@li Si $f$ est de classe $\mathcal{C}^n$ : $a_k=\frac{f^{(k)}(a)}{k!}$ (Taylor-Young).
@li Troncature : un DL d'ordre $n$ donne les DL d'ordre $p\leq n$.
@li $f$ paire : que des puissances paires ; $f$ impaire : que des impaires.
@li DL d'ordre 0 : $f$ continue en $a$ ; d'ordre 1 : $f$ dérivable, $a_1=f'(a)$.
@card Se ramener en 0
@tags changement variable infini asymptotique
@li En $a$ : poser $x=a+h$, avec $h\to 0$.
@li En $\pm\infty$ : poser $x=\frac{1}{h}$, avec $h\to 0^{\pm}$ (développement asymptotique).
@text Exemple, $\sqrt{x}$ en 1 : on pose $x=1+h$.
@math \sqrt{1+h}\underset{h\to 0}{=}1+\frac{h}{2}-\frac{h^2}{8}+o(h^2)
@card Applications
@tags tangente asymptote extremum position
@li Tangente en $a$ : $y=a_0+a_1(x-a)$. Position de la courbe : signe du premier terme non nul suivant, $a_k(x-a)^k$ ($k\geq 2$).
@li Asymptote : $f(x)=\alpha x+\beta+\frac{\gamma}{x}+o\left(\frac{1}{x}\right)$ donne $y=\alpha x+\beta$ ; position : signe de $\frac{\gamma}{x}$.
@li Extremum : $f'(a)=0$ et $f''(a)>0$ : minimum local ; $f''(a)<0$ : maximum local.
@li Limite : remplacer chaque terme par son DL et garder le terme dominant.

@page DL usuels en 0
@card Exponentielle et hyperboliques
@tags exp ch sh
@math e^x=1+x+\frac{x^2}{2!}+\frac{x^3}{3!}+\cdots+\frac{x^n}{n!}+o(x^n)
@math \ch x=1+\frac{x^2}{2!}+\frac{x^4}{4!}+\cdots+\frac{x^{2n}}{(2n)!}+o(x^{2n+1})
@math \sh x=x+\frac{x^3}{3!}+\cdots+\frac{x^{2n+1}}{(2n+1)!}+o(x^{2n+2})
@card Cosinus, sinus, tangente
@tags cos sin tan
@align \cos x &= 1-\frac{x^2}{2!}+\frac{x^4}{4!}-\cdots
@align &\quad +(-1)^n\frac{x^{2n}}{(2n)!}+o(x^{2n+1})
@align \sin x &= x-\frac{x^3}{3!}+\frac{x^5}{5!}-\cdots
@align &\quad +(-1)^n\frac{x^{2n+1}}{(2n+1)!}+o(x^{2n+2})
@align \tan x &= x+\frac{x^3}{3}+\frac{2x^5}{15}+o(x^6)
@card Logarithme, inverse
@tags ln log inverse geometrique
@align \ln(1+x) &= x-\frac{x^2}{2}+\frac{x^3}{3}-\cdots
@align &\quad+(-1)^{n-1}\frac{x^n}{n}+o(x^n)
@math \frac{1}{1-x}=1+x+x^2+\cdots+x^n+o(x^n)
@math \frac{1}{1+x}=1-x+x^2-\cdots+(-1)^nx^n+o(x^n)
@card Puissances
@tags racine puissance binome
@align (1+x)^{\alpha} &= 1+\alpha x+\frac{\alpha(\alpha-1)}{2}x^2+\cdots
@align &\quad+\binom{\alpha}{n}x^n+o(x^n)
@math \binom{\alpha}{n}=\frac{\alpha(\alpha-1)\cdots(\alpha-n+1)}{n!}
@math \sqrt{1+x}=1+\frac{x}{2}-\frac{x^2}{8}+\frac{x^3}{16}+o(x^3)
@math \frac{1}{\sqrt{1+x}}=1-\frac{x}{2}+\frac{3x^2}{8}+o(x^2)
@card Fonctions réciproques
@tags arctan arcsin argth
@align \arctan x &= x-\frac{x^3}{3}+\frac{x^5}{5}-\cdots
@align &\quad +(-1)^n\frac{x^{2n+1}}{2n+1}+o(x^{2n+2})
@math \arcsin x=x+\frac{x^3}{6}+\frac{3x^5}{40}+o(x^6)
@math \argth x=x+\frac{x^3}{3}+\frac{x^5}{5}+o(x^6)

@page Opérations et équivalents
@card Calculs avec les DL
@tags operations produit quotient composee
@li Somme, produit : on tronque au plus petit ordre utile.
@li Quotient : $\frac{1}{1-u}=1+u+u^2+\cdots$ avec $u\to 0$.
@li Composée $g\circ f$ : il faut $f(x)\to 0$, puis on remplace.
@li Primitive : on intègre terme à terme, sans oublier $F(0)$.
@card Équivalents usuels en 0
@tags equivalent
@table[cc] \text{En }0 & \text{En }0
@row \sin x\sim x & \tan x\sim x
@row 1-\cos x\sim\frac{x^2}{2} & \ln(1+x)\sim x
@row e^x-1\sim x & (1+x)^{\alpha}-1\sim\alpha x
@row \arctan x\sim x & \arcsin x\sim x
@row \sh x\sim x & \ch x-1\sim\frac{x^2}{2}
@card Équivalence et négligeabilité
@tags petit o notation landau
@math f\underset{a}{\sim}g\iff f=g+o(g)\iff\frac{f}{g}\xrightarrow[x\to a]{}1
@math f=o(g)\iff\frac{f}{g}\xrightarrow[x\to a]{}0
@note On peut multiplier, diviser, élever à une puissance des équivalents ; pas les additionner ni les composer.

@page Limites usuelles
@card Limites remarquables
@tags limite usuelle
@math \lim_{x\to 0}\frac{\sin x}{x}=1\qquad\lim_{x\to 0}\frac{1-\cos x}{x^2}=\frac{1}{2}
@math \lim_{x\to 0}\frac{e^x-1}{x}=1\qquad\lim_{x\to 0}\frac{\ln(1+x)}{x}=1
@math \lim_{n\to+\infty}\left(1+\frac{x}{n}\right)^n=e^x
@card Croissances comparées
@tags croissance comparee
@math \lim_{x\to+\infty}\frac{(\ln x)^{\alpha}}{x^{\beta}}=0\qquad\lim_{x\to+\infty}\frac{x^{\beta}}{e^{\gamma x}}=0
@math \lim_{x\to 0^+}x^{\beta}\,|\ln x|^{\alpha}=0\qquad\lim_{n\to+\infty}\frac{a^n}{n!}=0
@note $\alpha,\beta,\gamma>0$. Au voisinage de $+\infty$ : $\ln x\ll x^{\beta}\ll e^{\gamma x}$.
@card Formes indéterminées
@tags indetermination
@text $\infty-\infty$, $0\times\infty$, $\frac{0}{0}$, $\frac{\infty}{\infty}$, $1^{\infty}$, $0^0$, $\infty^0$ : factoriser le terme dominant, utiliser un DL ou écrire $u^v=e^{v\ln u}$.
)TEX";
