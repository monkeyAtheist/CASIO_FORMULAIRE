// Chapitre : transformees (Fourier, Laplace, poles et stabilite, Z),
// convolution et correlation.
// Notation : p pour Laplace (s en notation anglo-saxonne), i pour le complexe
// (comme dans les series de Fourier).
extern const char CH_TRANSFORMEES[];
const char CH_TRANSFORMEES[] = R"TEX(
@page Fourier : définition
@card Transformée de Fourier
@tags fourier transformee spectre
@math \hat{f}(\omega)=\mathcal{F}[f](\omega)=\int_{-\infty}^{+\infty}f(t)\,e^{-i\omega t}\,dt
@math f(t)=\frac{1}{2\pi}\int_{-\infty}^{+\infty}\hat{f}(\omega)\,e^{i\omega t}\,d\omega
@note En fréquence ($\omega=2\pi\nu$) : $\hat{f}(\nu)=\int f(t)\,e^{-2i\pi\nu t}\,dt$ et $f(t)=\int\hat{f}(\nu)\,e^{2i\pi\nu t}\,d\nu$. Il faut $\int|f|$ fini.
@card Propriétés
@tags linearite retard modulation derivation
@table[cc] f(t) & \hat{f}(\omega)
@row \alpha f+\beta g & \alpha\hat{f}+\beta\hat{g}
@row f(t-t_0) & e^{-i\omega t_0}\,\hat{f}(\omega)
@row e^{i\omega_0t}f(t) & \hat{f}(\omega-\omega_0)
@row f(at) & \frac{1}{|a|}\,\hat{f}\left(\frac{\omega}{a}\right)
@row f'(t) & i\omega\,\hat{f}(\omega)
@row f^{(n)}(t) & (i\omega)^n\,\hat{f}(\omega)
@row t\,f(t) & i\,\hat{f}\,'(\omega)
@row (f\ast g)(t) & \hat{f}(\omega)\,\hat{g}(\omega)
@row f(t)\,g(t) & \frac{1}{2\pi}\left(\hat{f}\ast\hat{g}\right)(\omega)
@card Parseval-Plancherel
@tags parseval plancherel energie
@math \int_{-\infty}^{+\infty}|f(t)|^2\,dt=\frac{1}{2\pi}\int_{-\infty}^{+\infty}\left|\hat{f}(\omega)\right|^2\,d\omega
@card Symétries
@tags parite reelle
@li $f$ réelle : $\hat{f}(-\omega)=\overline{\hat{f}(\omega)}$ (module pair, argument impair).
@li $f$ réelle paire : $\hat{f}$ réelle paire.
@li $f$ réelle impaire : $\hat{f}$ imaginaire pure impaire.

@page Fourier : transformées usuelles
@card Table
@tags table dirac porte sinus cardinal gaussienne
@table[cc] f(t) & \hat{f}(\omega)
@row \delta(t) & 1
@row 1 & 2\pi\,\delta(\omega)
@row \delta(t-t_0) & e^{-i\omega t_0}
@row e^{i\omega_0t} & 2\pi\,\delta(\omega-\omega_0)
@row \cos(\omega_0t) & \pi\big[\delta(\omega-\omega_0)+\delta(\omega+\omega_0)\big]
@row \sin(\omega_0t) & -i\pi\big[\delta(\omega-\omega_0)-\delta(\omega+\omega_0)\big]
@row \Pi_T(t) & T\,\frac{\sin(\omega T/2)}{\omega T/2}
@row e^{-a|t|} & \frac{2a}{a^2+\omega^2}
@row e^{-at}\,u(t) & \frac{1}{a+i\omega}
@row e^{-at^2} & \sqrt{\frac{\pi}{a}}\;e^{-\frac{\omega^2}{4a}}
@note $a>0$. $\delta$ : impulsion de Dirac ; $u$ : échelon unité (Heaviside) ; $\Pi_T$ : porte de largeur $T$ et de hauteur 1 centrée en 0 ; $\frac{\sin x}{x}=\operatorname{sinc}x$.
@note En anglais, $\operatorname{sinc}x$ désigne souvent $\frac{\sin\pi x}{\pi x}$.
@card Dualité
@tags dualite
@math \mathcal{F}\big[\hat{f}\big](\omega)=2\pi\,f(-\omega)
@note Une porte donne un sinus cardinal ; un sinus cardinal donne une porte. Une gaussienne donne une gaussienne.

@page Laplace : définition
@card Transformée de Laplace
@tags laplace transformee
@math F(p)=\mathcal{L}[f](p)=\int_0^{+\infty}f(t)\,e^{-pt}\,dt
@note $f$ causale ($f(t)=0$ pour $t<0$). $p$ complexe (noté $s$ en notation anglo-saxonne). Régime harmonique : $p=i\omega$ (noté $j\omega$ en électricité).
@card Propriétés
@tags derivation integration retard amortissement
@table[cc] f(t) & F(p)
@row \alpha f+\beta g & \alpha F+\beta G
@row f'(t) & p\,F(p)-f(0^+)
@row f''(t) & p^2F(p)-p\,f(0)-f'(0)
@row \int_0^t f(\tau)\,d\tau & \frac{F(p)}{p}
@row f(t-\tau)\,u(t-\tau) & e^{-\tau p}\,F(p)
@row e^{-at}f(t) & F(p+a)
@row t\,f(t) & -F'(p)
@row f(at) & \frac{1}{a}\,F\left(\frac{p}{a}\right)
@row (f\ast g)(t) & F(p)\,G(p)
@note $a>0$ ; $f(0)$ désigne $f(0^+)$. Conditions initiales nulles : dériver revient à multiplier par $p$, intégrer à diviser par $p$.
@card Valeurs initiale et finale
@tags valeur finale valeur initiale
@math \lim_{t\to 0^+}f(t)=\lim_{p\to+\infty}p\,F(p)
@math \lim_{t\to+\infty}f(t)=\lim_{p\to 0}p\,F(p)
@note Valeur finale : seulement si tous les pôles de $p\,F(p)$ sont à partie réelle strictement négative.
@card Résoudre une équation différentielle
@tags methode equation differentielle
@li Transformer l'équation ($f'\to pF-f(0^+)$…).
@li Résoudre l'équation algébrique obtenue en $F(p)$.
@li Décomposer $F(p)$ en éléments simples (voir Primitives).
@li Revenir à $f(t)$ avec la table.
@math \tau y'+y=E\,u(t),\quad y(0)=0
@math \implies Y(p)=\frac{E}{p\,(1+\tau p)}=\frac{E}{p}-\frac{E\tau}{1+\tau p}
@math \implies y(t)=E\left(1-e^{-t/\tau}\right)

@page Laplace : table
@card Transformées usuelles
@tags table laplace echelon rampe
@table[cc] f(t)\ \ (t\geq 0) & F(p)
@row \delta(t) & 1
@row u(t)\ \text{(échelon)} & \frac{1}{p}
@row t\ \text{(rampe)} & \frac{1}{p^2}
@row t^n & \frac{n!}{p^{n+1}}
@row e^{-at} & \frac{1}{p+a}
@row t\,e^{-at} & \frac{1}{(p+a)^2}
@row t^n\,e^{-at} & \frac{n!}{(p+a)^{n+1}}
@row \sin\omega t & \frac{\omega}{p^2+\omega^2}
@row \cos\omega t & \frac{p}{p^2+\omega^2}
@row e^{-at}\sin\omega t & \frac{\omega}{(p+a)^2+\omega^2}
@row e^{-at}\cos\omega t & \frac{p+a}{(p+a)^2+\omega^2}
@row \sh at & \frac{a}{p^2-a^2}
@row \ch at & \frac{p}{p^2-a^2}
@row t\sin\omega t & \frac{2\omega p}{\left(p^2+\omega^2\right)^2}
@row \frac{1}{\tau}\,e^{-t/\tau} & \frac{1}{1+\tau p}
@row 1-e^{-t/\tau} & \frac{1}{p\,(1+\tau p)}

@page Pôles et stabilité
@card Fonction de transfert
@tags transfert poles zeros
@math H(p)=\frac{S(p)}{E(p)}=\frac{N(p)}{D(p)}
@note Pôles : racines de $D(p)$ ; zéros : racines de $N(p)$. Réponse impulsionnelle $h=\mathcal{L}^{-1}[H]$ ; réponse indicielle (à un échelon) : $\mathcal{L}^{-1}\left[\frac{H(p)}{p}\right]$.
@card Pôle et comportement
@tags pole mode reponse
@text Chaque pôle $p_k$ donne un terme $e^{p_kt}$ dans la réponse :
@table[cl] \text{Pôle} & \text{Comportement}
@row p_k<0 & \text{décroissance},\ \tau=-\frac{1}{p_k}
@row p_k>0 & \text{divergence}
@row p_k=0 & \text{constante (intégrateur)}
@row \sigma\pm i\omega,\ \sigma<0 & \text{oscillation amortie}
@row \pm i\omega & \text{oscillation entretenue}
@row \sigma\pm i\omega,\ \sigma>0 & \text{oscillation divergente}
@note Une paire $\sigma\pm i\omega$ donne $e^{\sigma t}\cos(\omega t+\phi)$. Un pôle d'ordre $m$ donne des termes $t^je^{p_kt}$ avec $j<m$.
@card Stabilité
@tags stable instable demi plan
@math \text{Stable}\iff\Re(p_k)<0\ \text{pour tous les pôles}
@note Tous les pôles dans le demi-plan gauche. Plus un pôle est loin de l'axe imaginaire, plus son mode est rapide : les pôles les plus proches de l'axe (dominants) fixent la dynamique.
@card Premier ordre
@tags premier ordre constante temps
@math H(p)=\frac{K}{1+\tau p}\qquad p_1=-\frac{1}{\tau}
@math s(t)=K\left(1-e^{-t/\tau}\right)\quad\text{(échelon unité)}
@note Temps de réponse à 5 % : $t_{5\%}\approx 3\tau$.
@card Deuxième ordre
@tags second ordre amortissement xi depassement
@math H(p)=\frac{K\,\omega_0^2}{p^2+2\xi\omega_0\,p+\omega_0^2}=\frac{K}{1+\frac{2\xi}{\omega_0}p+\frac{p^2}{\omega_0^2}}
@table[cl] \xi & \text{Pôles}
@row \xi>1 & -\xi\omega_0\pm\omega_0\sqrt{\xi^2-1}\ \ \text{(réels)}
@row \xi=1 & -\omega_0\ \ \text{(double)}
@row 0<\xi<1 & -\xi\omega_0\pm i\,\omega_0\sqrt{1-\xi^2}
@row \xi=0 & \pm i\,\omega_0
@math \omega_p=\omega_0\sqrt{1-\xi^2}\qquad D_{\%}=100\,e^{-\frac{\pi\xi}{\sqrt{1-\xi^2}}}
@note $\xi=\frac{1}{2Q}$ (voir Équations différentielles). $\xi<1$ : pseudo-périodique (dépassement $D_{\%}$) ; $\xi=1$ : critique ; $\xi>1$ : apériodique.
@card Réponse fréquentielle
@tags bode gain phase harmonique
@math G_{\text{dB}}=20\log\left|H(i\omega)\right|\qquad\varphi=\arg H(i\omega)
@note Premier ordre : pulsation de coupure $\omega_c=\frac{1}{\tau}$ (gain $-3$ dB, $\varphi=-45°$), pente $-20$ dB par décade.

@page Transformée en Z
@card Définition
@tags z echantillonnage discret
@math X(z)=\mathcal{Z}[x_n](z)=\sum_{n=0}^{+\infty}x_n\,z^{-n}
@note Suite causale ($x_n=0$ si $n<0$). Échantillonnage : $x_n=x(nT_e)$, et $z=e^{pT_e}$.
@card Propriétés
@tags retard avance
@table[cc] x_n & X(z)
@row \alpha x_n+\beta y_n & \alpha X+\beta Y
@row x_{n-k}\ \text{(retard)} & z^{-k}X(z)
@row x_{n+1}\ \text{(avance)} & z\,X(z)-z\,x_0
@row x_{n+2} & z^2X(z)-z^2x_0-z\,x_1
@row a^n\,x_n & X\left(\frac{z}{a}\right)
@row n\,x_n & -z\,X'(z)
@row \sum_{k=0}^{n}x_ky_{n-k} & X(z)\,Y(z)
@card Valeurs initiale et finale
@tags valeur finale
@math x_0=\lim_{z\to+\infty}X(z)
@math \lim_{n\to+\infty}x_n=\lim_{z\to 1}(z-1)\,X(z)
@note Valeur finale : si les pôles de $(z-1)X(z)$ sont dans le disque unité.
@card Table
@tags table z echelon
@table[cc] x_n\ \ (n\geq 0) & X(z)
@row \delta_n\ \text{(impulsion)} & 1
@row 1\ \text{(échelon)} & \frac{z}{z-1}
@row n & \frac{z}{(z-1)^2}
@row a^n & \frac{z}{z-a}
@row n\,a^n & \frac{a\,z}{(z-a)^2}
@row e^{-anT} & \frac{z}{z-e^{-aT}}
@row \cos(n\omega) & \frac{z\,(z-\cos\omega)}{z^2-2z\cos\omega+1}
@row \sin(n\omega) & \frac{z\sin\omega}{z^2-2z\cos\omega+1}
@card Pôles et stabilité
@tags stable cercle unite
@math \text{Stable}\iff|z_k|<1\ \text{pour tous les pôles}
@note Pôles à l'intérieur du cercle unité. Un pôle $z_k$ donne un terme $z_k^n$ : décroissant si $|z_k|<1$, alterné si $z_k<0$, oscillant s'il est complexe. Avec $z=e^{pT_e}$, le demi-plan gauche en $p$ devient le disque unité en $z$.
@card Équations de récurrence
@tags recurrence suite
@math y_{n+1}-a\,y_n=x_n\implies Y(z)=\frac{X(z)+z\,y_0}{z-a}
@note Comme avec Laplace : transformer, décomposer $\frac{Y(z)}{z}$ en éléments simples, puis revenir avec la table.

@page Convolution
@card Produit de convolution
@tags convolution produit continu
@math (f\ast g)(t)=\int_{-\infty}^{+\infty}f(\tau)\,g(t-\tau)\,d\tau
@text Signaux causaux (nuls pour $t<0$) :
@math (f\ast g)(t)=\int_0^t f(\tau)\,g(t-\tau)\,d\tau
@note Lecture graphique : on retourne $g$, on le décale de $t$, on multiplie par $f$ et on intègre (aire du produit).
@card Convolution discrète
@tags convolution discrete somme suite numerique polynome
@math (x\ast y)_n=\sum_{k=-\infty}^{+\infty}x_k\,y_{n-k}
@text Suites causales (nulles pour $n<0$) :
@math (x\ast y)_n=\sum_{k=0}^{n}x_k\,y_{n-k}
@note Suites de $N$ et $M$ termes : le résultat a $N+M-1$ termes. Même règle que le produit de polynômes : $(1,2,3)\ast(1,1)=(1,3,5,3)$, comme $\left(1+2X+3X^2\right)(1+X)=1+3X+5X^2+3X^3$.
@card Propriétés
@tags commutative associative neutre dirac decalage
@math f\ast g=g\ast f\qquad(f\ast g)\ast h=f\ast(g\ast h)
@math f\ast(g+h)=f\ast g+f\ast h\qquad f\ast\delta=f
@math f(t)\ast\delta(t-t_0)=f(t-t_0)\qquad(f\ast g)'=f'\ast g=f\ast g'
@math \int_{-\infty}^{+\infty}(f\ast g)=\int_{-\infty}^{+\infty}f\times\int_{-\infty}^{+\infty}g
@note Discret : $\delta_n$ (1 en $n=0$, 0 ailleurs) est neutre et $x_n\ast\delta_{n-k}=x_{n-k}$. Durées : si $f$ dure $T_1$ et $g$ dure $T_2$, $f\ast g$ dure $T_1+T_2$.
@card Convolution et transformées
@tags theoreme convolution fourier laplace z produit
@math \mathcal{F}[f\ast g]=\hat{f}\,\hat{g}\qquad\mathcal{F}[f\,g]=\frac{1}{2\pi}\,\hat{f}\ast\hat{g}
@math \mathcal{L}[f\ast g]=F(p)\,G(p)\qquad\mathcal{Z}[x\ast y]=X(z)\,Y(z)
@note La convolution devient un produit : c'est ce qui rend les transformées si utiles.
@card Systèmes linéaires invariants
@tags filtre reponse impulsionnelle systeme lti transfert
@math s(t)=(h\ast e)(t)=\int_{-\infty}^{+\infty}h(\tau)\,e(t-\tau)\,d\tau
@math s_n=\sum_{k}h_k\,e_{n-k}\qquad S=H\times E
@note $h$ : réponse impulsionnelle (sortie quand l'entrée est $\delta$). $H$ : fonction de transfert, transformée de $h$. Stable $\iff\int|h|<+\infty$ (ou $\sum|h_k|<+\infty$). Causal $\iff h=0$ pour $t<0$.
@card Exemple : porte et triangle
@tags porte triangle exemple
@math \Pi_T\ast\Pi_T=T\,\Lambda_T\qquad\Lambda_T(t)=\begin{cases}1-\frac{|t|}{T} & \text{si }|t|\leq T\\0 & \text{sinon}\end{cases}
@note Triangle de base $2T$. Côté Fourier : $\left(T\operatorname{sinc}\frac{\omega T}{2}\right)^2$.
@card Somme de variables aléatoires
@tags probabilite somme independantes densite loi
@math f_{X+Y}=f_X\ast f_Y\qquad P(X+Y=n)=\sum_{k}P(X=k)\,P(Y=n-k)
@note $X$ et $Y$ indépendantes. Exemple : la somme de deux lois uniformes sur $[0,1]$ a une densité en triangle.

@page Corrélation
@card Intercorrélation
@tags intercorrelation correlation croisee retard
@math R_{xy}(\tau)=\int_{-\infty}^{+\infty}x(t)\,y(t-\tau)\,dt
@math R_{xy}(\tau)=\big(x(t)\ast y(-t)\big)(\tau)
@note Signaux réels d'énergie finie (complexes : remplacer $y$ par $\overline{y}$). Ressemblance entre $x$ et $y$ décalé de $\tau$. Si $y(t)=x(t-d)$ (écho, retard $d$), $R_{yx}$ est maximale en $\tau=d$.
@card Autocorrélation
@tags autocorrelation energie
@math R_x(\tau)=R_{xx}(\tau)=\int_{-\infty}^{+\infty}x(t)\,x(t-\tau)\,dt
@math R_x(0)=E=\int_{-\infty}^{+\infty}x^2(t)\,dt\qquad\left|R_x(\tau)\right|\leq R_x(0)
@math R_x(-\tau)=R_x(\tau)\qquad R_{xy}(-\tau)=R_{yx}(\tau)
@note $R_x$ est paire et maximale en $\tau=0$ : un signal ressemble le plus à lui-même sans décalage.
@card Signaux de puissance
@tags puissance moyenne periodique sinusoide
@math R_x(\tau)=\lim_{T\to+\infty}\frac{1}{T}\int_{-T/2}^{T/2}x(t)\,x(t-\tau)\,dt
@math R_x(0)=P\ \ \text{(puissance moyenne)}
@text Sinusoïde :
@align x(t) &= A\cos(\omega_0t+\varphi)
@align R_x(\tau) &= \frac{A^2}{2}\cos(\omega_0\tau)
@note Signal périodique : moyenne sur une période ; $R_x$ a la même période et la phase $\varphi$ disparaît.
@card Wiener-Khintchine
@tags densite spectrale energie puissance spectre
@math \mathcal{F}[R_x](\omega)=\left|\hat{x}(\omega)\right|^2\qquad\mathcal{F}[R_{xy}](\omega)=\hat{x}(\omega)\,\overline{\hat{y}(\omega)}
@note $\left|\hat{x}\right|^2$ : densité spectrale d'énergie. Signal de puissance : $S_x=\mathcal{F}[R_x]$ est la densité spectrale de puissance et $P=\frac{1}{2\pi}\int S_x(\omega)\,d\omega$.
@card Corrélation discrète
@tags discrete echantillons coefficient bruit blanc
@math R_{xy}[k]=\sum_{n}x_n\,y_{n-k}\qquad R_x[0]=\sum_n x_n^2
@math \rho_{xy}[k]=\frac{R_{xy}[k]}{\sqrt{R_x[0]\,R_y[0]}}\in[-1,1]
@note $\rho=\pm 1$ : signaux proportionnels (pour ce décalage). Bruit blanc centré : $R_x[k]=\sigma^2\,\delta_k$ (aucune ressemblance dès qu'on décale).
)TEX";
