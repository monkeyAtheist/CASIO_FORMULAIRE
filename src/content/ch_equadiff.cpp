// Chapitre : equations differentielles.
extern const char CH_EQUADIFF[];
const char CH_EQUADIFF[] = R"TEX(
@page Premier ordre
@card y' + a y = 0 (a constante)
@tags homogene exponentielle
@math y(t)=C\,e^{-at}
@card y' + a(t) y = b(t)
@tags variation constante lineaire
@math y_h(t)=C\,e^{-A(t)}\qquad A'=a
@text Solution générale : $y=y_h+y_p$. Variation de la constante :
@math y_p=C(t)\,e^{-A(t)}\qquad C'(t)=b(t)\,e^{A(t)}
@note Une condition initiale $y(t_0)=y_0$ donne une solution unique (Cauchy).
@card Régime transitoire
@tags constante de temps circuit rc
@math \tau\,y'+y=E\implies y(t)=E+\left(y_0-E\right)e^{-t/\tau}
@note Après $5\tau$, $y$ est à moins de 1 % de $E$.
@card Variables séparables
@tags separable
@math y'=f(t)\,g(y)\implies\int\frac{dy}{g(y)}=\int f(t)\,dt

@page Second ordre (coefficients constants)
@card ay'' + by' + cy = 0
@tags caracteristique homogene
@text Équation caractéristique $ar^2+br+c=0$, $\Delta=b^2-4ac$ :
@li $\Delta>0$ : $y=\lambda\,e^{r_1t}+\mu\,e^{r_2t}$
@li $\Delta=0$ : $y=(\lambda+\mu\,t)\,e^{rt}$
@li $\Delta<0$, $r=\alpha\pm i\beta$ : $y=e^{\alpha t}\left(\lambda\cos\beta t+\mu\sin\beta t\right)$
@card Solution particulière
@tags second membre particuliere
@table[cc] \text{Second membre} & \text{On cherche}
@row P(t),\ \deg P=n & Q(t),\ \deg Q=n
@row A\,e^{mt} & B\,e^{mt}
@row A\cos\omega t+B\sin\omega t & C\cos\omega t+D\sin\omega t
@note Multiplier par $t$ (ou $t^2$) si $m$ (ou $i\omega$) est racine simple (ou double) de l'équation caractéristique. En complexe : $\Re\left(Ke^{i\omega t}\right)$.
@card Oscillateur amorti
@tags oscillateur amortissement facteur qualite
@math \ddot{x}+\frac{\omega_0}{Q}\,\dot{x}+\omega_0^2\,x=0
@li $Q>\frac{1}{2}$ : pseudo-périodique, $\omega=\omega_0\sqrt{1-\frac{1}{4Q^2}}$
@li $Q=\frac{1}{2}$ : critique ; $Q<\frac{1}{2}$ : apériodique
@note Autre forme : $\ddot{x}+2\xi\omega_0\,\dot{x}+\omega_0^2\,x=0$ avec $\xi=\frac{1}{2Q}$.

@page Méthodes générales
@card Wronskien
@tags wronskien independance
@math W=\begin{vmatrix}y_1&y_2\\y_1'&y_2'\end{vmatrix}=y_1y_2'-y_1'y_2
@text $y_1$ et $y_2$ solutions de $y''+a\,y'+b\,y=0$ sont indépendantes si et seulement si $W\neq 0$.
@card Systèmes linéaires
@tags systeme exponentielle matrice
@math X'=AX\implies X(t)=e^{tA}X(0)
@text Si $A=PDP^{-1}$ : poser $X=PY$, alors $Y'=DY$ (équations découplées) et $y_i=c_i\,e^{\lambda_it}$.
@card Superposition
@tags lineaire superposition
@text Pour une équation linéaire, si $y_1$ résout le second membre $b_1$ et $y_2$ le second membre $b_2$, alors $y_1+y_2$ résout $b_1+b_2$.
)TEX";
