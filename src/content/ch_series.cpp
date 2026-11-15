// Chapitre : series.
extern const char CH_SERIES[];
const char CH_SERIES[] = R"TEX(
@page Séries numériques
@card Définitions
@tags serie somme partielle reste
@math S_n=\sum_{k=0}^{n}u_k
@math \sum u_n\ \text{converge}\iff(S_n)\ \text{converge}
@math R_n=\sum_{k=n+1}^{+\infty}u_k=S-S_n
@note Condition nécessaire : $u_n\to 0$ (sinon divergence grossière).
@card Séries de référence
@tags geometrique riemann exponentielle
@math \sum_{n=0}^{+\infty}q^n=\frac{1}{1-q}\quad(|q|<1)
@math \sum\frac{1}{n^{\alpha}}\ \text{converge}\iff\alpha>1\quad\text{(Riemann)}
@math \sum_{n=0}^{+\infty}\frac{x^n}{n!}=e^x\qquad\sum_{n=1}^{+\infty}\frac{1}{n^2}=\frac{\pi^2}{6}
@math \sum_{n=1}^{+\infty}\frac{(-1)^{n+1}}{n}=\ln 2\qquad\sum_{n=0}^{+\infty}n\,q^{n-1}=\frac{1}{(1-q)^2}
@card Séries à termes positifs
@tags comparaison alembert critere
@li **Comparaison** : $0\leq u_n\leq v_n$ et $\sum v_n$ converge $\implies\sum u_n$ converge.
@li **Équivalents** : $u_n\sim v_n$, termes positifs : même nature.
@li **D'Alembert** : $\frac{u_{n+1}}{u_n}\to\ell$ ; $\ell<1$ : converge, $\ell>1$ : diverge.
@li **Série-intégrale** : $f$ positive décroissante : $\sum f(n)$ et $\int_1^{+\infty}f$ ont même nature.
@card Séries alternées
@tags alternee leibniz
@text Si $(u_n)$ décroît et tend vers 0, $\sum(-1)^n u_n$ converge et
@math |R_n|\leq u_{n+1}
@card Convergence absolue
@tags absolue
@math \sum|u_n|\ \text{converge}\implies\sum u_n\ \text{converge}

@page Séries entières
@card Rayon de convergence
@tags rayon alembert
@math \sum a_n x^n\ :\quad\left|\frac{a_{n+1}}{a_n}\right|\to\ell\implies R=\frac{1}{\ell}
@text Convergence absolue si $|x|<R$, divergence grossière si $|x|>R$.
@card Développements usuels
@tags exp cos sin ln arctan
@math e^x=\sum_{n=0}^{+\infty}\frac{x^n}{n!}\quad(R=+\infty)
@math \cos x=\sum_{n=0}^{+\infty}\frac{(-1)^n x^{2n}}{(2n)!}\quad(R=+\infty)
@math \sin x=\sum_{n=0}^{+\infty}\frac{(-1)^n x^{2n+1}}{(2n+1)!}\quad(R=+\infty)
@math \frac{1}{1-x}=\sum_{n=0}^{+\infty}x^n\quad(R=1)
@math \ln(1+x)=\sum_{n=1}^{+\infty}\frac{(-1)^{n+1}}{n}\,x^n\quad(R=1)
@math \arctan x=\sum_{n=0}^{+\infty}\frac{(-1)^n x^{2n+1}}{2n+1}\quad(R=1)
@math (1+x)^{\alpha}=\sum_{n=0}^{+\infty}\binom{\alpha}{n}x^n\quad(R=1)
@note $\binom{\alpha}{n}=\frac{\alpha(\alpha-1)\cdots(\alpha-n+1)}{n!}$, pour $\alpha\notin\mathbb{N}$.
@card Dérivation, intégration
@tags terme a terme
@math f(x)=\sum_{n=0}^{+\infty}a_nx^n\implies f'(x)=\sum_{n=1}^{+\infty}n\,a_nx^{n-1}
@math \int_0^x f=\sum_{n=0}^{+\infty}\frac{a_n}{n+1}\,x^{n+1}
@note Même rayon de convergence ; valable pour $|x|<R$.

@page Séries de Fourier
@card Coefficients réels
@tags fourier coefficient periode
@math a_0=\frac{1}{T}\int_0^T f(t)\,dt\qquad\omega=\frac{2\pi}{T}
@math a_n=\frac{2}{T}\int_0^T f(t)\cos(n\omega t)\,dt
@math b_n=\frac{2}{T}\int_0^T f(t)\sin(n\omega t)\,dt
@math f(t)=a_0+\sum_{n=1}^{+\infty}\Big(a_n\cos(n\omega t)+b_n\sin(n\omega t)\Big)
@note $f$ paire : $b_n=0$ ; $f$ impaire : $a_n=0$.
@card Forme complexe
@tags complexe exponentielle
@math c_n=\frac{1}{T}\int_0^T f(t)\,e^{-in\omega t}\,dt
@math f(t)=\sum_{n=-\infty}^{+\infty}c_n\,e^{in\omega t}\qquad a_n=c_n+c_{-n}
@card Dirichlet et Parseval
@tags parseval dirichlet energie
@text Si $f$ est $\mathcal{C}^1$ par morceaux, la série converge vers $\frac{f(t^+)+f(t^-)}{2}$.
@align \frac{1}{T}\int_0^T f^2(t)\,dt &= a_0^2+\frac{1}{2}\sum_{n=1}^{+\infty}\left(a_n^2+b_n^2\right)
@align &= \sum_{n\in\mathbb{Z}}|c_n|^2
)TEX";
