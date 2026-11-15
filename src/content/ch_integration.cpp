// Chapitre : integration. Format : voir src/doc/Content.hpp.
extern const char CH_INTEGRATION[];
const char CH_INTEGRATION[] = R"TEX(
@page Propriétés de l'intégrale
@card Théorème fondamental
@tags primitive integrale
@math \int_a^b f(t)\,dt=\Big[F(t)\Big]_a^b=F(b)-F(a)
@math \frac{d}{dx}\int_a^x f(t)\,dt=f(x)
@math \frac{d}{dx}\int_{u(x)}^{v(x)}f=f\big(v(x)\big)\,v'(x)-f\big(u(x)\big)\,u'(x)
@note $f$ continue, $F$ une primitive de $f$.
@card Linéarité, relation de Chasles
@tags chasles
@math \int_a^b\left(\alpha f+\beta g\right)=\alpha\int_a^b f+\beta\int_a^b g
@math \int_a^b f=\int_a^c f+\int_c^b f\qquad\int_b^a f=-\int_a^b f
@card Inégalités
@tags positivite croissance
@math f\geq 0\ \text{et}\ a\leq b\implies\int_a^b f\geq 0
@math \left|\int_a^b f\right|\leq\int_a^b|f|\quad(a\leq b)
@math m\leq f\leq M\implies m(b-a)\leq\int_a^b f\leq M(b-a)
@math \left(\int_a^b fg\right)^2\leq\int_a^b f^2\int_a^b g^2
@note Dernière ligne : inégalité de Cauchy-Schwarz.
@card Valeur moyenne
@tags moyenne
@math \mu=\frac{1}{b-a}\int_a^b f(t)\,dt
@card Parité et périodicité
@tags paire impaire periodique
@math f\ \text{paire}:\ \int_{-a}^{a}f=2\int_0^a f
@math f\ \text{impaire}:\ \int_{-a}^{a}f=0
@math f\ T\text{-périodique}:\ \int_a^{a+T}f=\int_0^T f

@page Techniques de calcul
@card Intégration par parties
@tags ipp parties
@align \int_a^b u'(t)\,v(t)\,dt &= \Big[u(t)\,v(t)\Big]_a^b
@align &\quad -\int_a^b u(t)\,v'(t)\,dt
@math \int u'v=uv-\int uv'
@note $u$ et $v$ de classe $\mathcal{C}^1$. Dériver la fonction qui se simplifie ($x^n$, $\ln$, $\arctan$…).
@card Changement de variable
@tags changement variable substitution
@math \int_{\varphi(a)}^{\varphi(b)}f(x)\,dx=\int_a^b f\big(\varphi(t)\big)\,\varphi'(t)\,dt
@text On pose $x=\varphi(t)$, donc $dx=\varphi'(t)\,dt$, et on change les bornes.
@note $\varphi$ de classe $\mathcal{C}^1$ sur $[a,b]$, $f$ continue.
@card Changements de variable usuels
@tags changement variable racine
@table[cc] \text{Si l'on voit} & \text{On pose}
@row \sqrt{a^2-x^2} & x=a\sin t
@row \sqrt{a^2+x^2} & x=a\sh t\ \ \text{ou}\ \ x=a\tan t
@row \sqrt{x^2-a^2} & x=a\ch t
@row R\left(e^x\right) & t=e^x
@row R(\cos x,\sin x) & t=\tan\frac{x}{2}
@row \sqrt[n]{\frac{ax+b}{cx+d}} & t=\sqrt[n]{\frac{ax+b}{cx+d}}
@card Substitution t = tan(x/2)
@tags tangente moitie
@math \cos x=\frac{1-t^2}{1+t^2}\qquad\sin x=\frac{2t}{1+t^2}
@math \tan x=\frac{2t}{1-t^2}\qquad dx=\frac{2\,dt}{1+t^2}
@card Règles de Bioche
@tags bioche trigonometrique
@text Pour $\int R(\cos x,\sin x)\,dx$ : si $R(\cos x,\sin x)\,dx$ est invariant par
@li $x\mapsto -x$ : poser $t=\cos x$ ;
@li $x\mapsto\pi-x$ : poser $t=\sin x$ ;
@li $x\mapsto\pi+x$ : poser $t=\tan x$ ;
@li sinon : $t=\tan\frac{x}{2}$.
@card Puissances de sin et cos
@tags linearisation
@math \cos^2 x=\frac{1+\cos 2x}{2}\qquad\sin^2 x=\frac{1-\cos 2x}{2}
@text Puissance impaire : sortir un facteur, puis poser $t=\sin x$ (ou $t=\cos x$) avec $\cos^2 x+\sin^2 x=1$.

@page Sommes de Riemann, impropres
@card Sommes de Riemann
@tags riemann somme
@math \frac{b-a}{n}\sum_{k=0}^{n-1}f\left(a+k\,\frac{b-a}{n}\right)\xrightarrow[n\to+\infty]{}\int_a^b f
@math \frac{1}{n}\sum_{k=1}^{n}f\left(\frac{k}{n}\right)\xrightarrow[n\to+\infty]{}\int_0^1 f(t)\,dt
@note $f$ continue (par morceaux) sur $[a,b]$.
@card Intégrales de Riemann
@tags convergence impropre generalisee
@math \int_1^{+\infty}\frac{dt}{t^{\alpha}}\ \text{converge}\iff\alpha>1
@math \int_0^1\frac{dt}{t^{\alpha}}\ \text{converge}\iff\alpha<1
@card Intégrales de référence
@tags gauss
@math \int_0^{+\infty}e^{-at}\,dt=\frac{1}{a}\quad(a>0)
@math \int_{-\infty}^{+\infty}e^{-x^2}\,dx=\sqrt{\pi}\qquad\int_0^1\ln t\,dt=-1
@math \int_0^{+\infty}t^n\,e^{-t}\,dt=n!
@card Critères de convergence
@tags comparaison equivalent
@li $0\leq f\leq g$ : $\int g$ converge $\implies\int f$ converge.
@li $f\sim g$ (fonctions positives) : même nature.
@li $\int|f|$ converge $\implies\int f$ converge.

@page Intégrales multiples
@card Fubini
@tags double rectangle
@align \iint_{[a,b]\times[c,d]}f(x,y)\,dx\,dy
@align &\qquad=\int_a^b\left(\int_c^d f(x,y)\,dy\right)dx
@math \iint_D f=\int_a^b\left(\int_{\varphi_1(x)}^{\varphi_2(x)}f(x,y)\,dy\right)dx
@note Domaine $D=\{a\leq x\leq b,\ \varphi_1(x)\leq y\leq\varphi_2(x)\}$.
@card Changement de variables
@tags jacobien
@align \iint_D f(x,y)\,dx\,dy
@align &\qquad=\iint_{\Delta}f\big(x(u,v),y(u,v)\big)\,|J|\,du\,dv
@math J=\frac{\partial(x,y)}{\partial(u,v)}=\begin{vmatrix}\dfrac{\partial x}{\partial u}&\dfrac{\partial x}{\partial v}\\\dfrac{\partial y}{\partial u}&\dfrac{\partial y}{\partial v}\end{vmatrix}
@card Éléments de surface et de volume
@tags polaire cylindrique spherique
@table[cc] \text{Coordonnées} & \text{Élément}
@row \text{cartésiennes} & dx\,dy\,dz
@row \text{polaires}\ (r,\theta) & r\,dr\,d\theta
@row \text{cylindriques}\ (r,\theta,z) & r\,dr\,d\theta\,dz
@row \text{sphériques}\ (r,\theta,\varphi) & r^2\sin\theta\,dr\,d\theta\,d\varphi
@note Sphériques : $\theta\in[0,\pi]$ (colatitude), $\varphi\in[0,2\pi[$.
@card Aire, volume, centre de gravité
@tags aire volume barycentre
@math \mathcal{A}=\iint_D dx\,dy\qquad V=\iiint_{\Omega}dx\,dy\,dz
@math x_G=\frac{1}{\mathcal{A}}\iint_D x\,dx\,dy
@math y_G=\frac{1}{\mathcal{A}}\iint_D y\,dx\,dy
@card Intégrale de Gauss en polaires
@align \iint_{\mathbb{R}^2}e^{-(x^2+y^2)}\,dx\,dy
@align &\qquad=\int_0^{2\pi}\!\!\int_0^{+\infty}e^{-r^2}\,r\,dr\,d\theta=\pi
)TEX";
