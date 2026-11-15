// Chapitre : suites.
extern const char CH_SUITES[];
const char CH_SUITES[] = R"TEX(
@page Arithmétiques, géométriques
@card Suite arithmétique (raison r)
@tags arithmetique raison
@math u_{n+1}=u_n+r
@math u_n=u_0+n\,r=u_p+(n-p)\,r
@math \sum_{k=0}^{n}u_k=(n+1)\,\frac{u_0+u_n}{2}
@note Somme = nombre de termes $\times$ (premier + dernier) / 2.
@card Suite géométrique (raison q)
@tags geometrique raison
@math u_{n+1}=q\,u_n\qquad u_n=u_0\,q^n=u_p\,q^{n-p}
@math \sum_{k=0}^{n}q^k=\frac{1-q^{n+1}}{1-q}\quad(q\neq 1)
@math \sum_{k=p}^{n}u_k=u_p\,\frac{1-q^{n-p+1}}{1-q}
@note Somme = premier terme $\times\frac{1-q^{\text{nb de termes}}}{1-q}$.
@card Limite de q^n
@tags limite puissance
@table[cc] q & \lim\limits_{n\to+\infty}q^n
@row q>1 & +\infty
@row q=1 & 1
@row -1<q<1 & 0
@row q\leq -1 & \text{pas de limite}
@card Arithmético-géométrique
@tags point fixe
@math u_{n+1}=a\,u_n+b\quad(a\neq 1)
@math \ell=\frac{b}{1-a}\qquad u_n-\ell=a^n\,(u_0-\ell)
@note $\ell$ est le point fixe : $\ell=a\ell+b$.

@page Récurrence d'ordre 2
@card u(n+2) = a u(n+1) + b u(n)
@tags recurrence lineaire caracteristique
@math u_{n+2}=a\,u_{n+1}+b\,u_n\quad\longrightarrow\quad r^2=a\,r+b
@li Deux racines $r_1\neq r_2$ : $u_n=\lambda\,r_1^n+\mu\,r_2^n$
@li Racine double $r$ : $u_n=(\lambda+\mu\,n)\,r^n$
@li Racines complexes $\rho\,e^{\pm i\theta}$ : $u_n=\rho^n\left(\lambda\cos n\theta+\mu\sin n\theta\right)$
@note $\lambda$ et $\mu$ se déterminent avec $u_0$ et $u_1$.
@card Exemple : Fibonacci
@tags fibonacci nombre or
@math F_{n+2}=F_{n+1}+F_n\qquad F_n=\frac{\varphi^n-\psi^n}{\sqrt{5}}
@math \varphi=\frac{1+\sqrt{5}}{2},\quad\psi=\frac{1-\sqrt{5}}{2}
@note Avec $F_0=0$, $F_1=1$.

@page Convergence
@card Définition
@tags limite epsilon
@math u_n\xrightarrow[n\to+\infty]{}\ell\iff
@math \forall\varepsilon>0,\ \exists N\in\mathbb{N},\ \forall n\geq N,\ |u_n-\ell|\leq\varepsilon
@card Théorèmes
@tags monotone gendarmes adjacentes bolzano
@li **Limite monotone** : croissante et majorée $\implies$ converge ; croissante non majorée $\implies$ tend vers $+\infty$.
@li **Gendarmes** : $v_n\leq u_n\leq w_n$ et $v_n,w_n\to\ell$ $\implies u_n\to\ell$.
@li **Adjacentes** : $(u_n)$ croissante, $(v_n)$ décroissante, $v_n-u_n\to 0$ $\implies$ même limite.
@li **Bolzano-Weierstrass** : une suite bornée a une sous-suite convergente.
@card Suites récurrentes u(n+1) = f(u(n))
@tags point fixe recurrente
@li Si $u_n\to\ell$ et $f$ continue en $\ell$ : $f(\ell)=\ell$.
@li $f$ croissante : $(u_n)$ est monotone (sens de $u_1-u_0$).
@li $f$ décroissante : $(u_{2n})$ et $(u_{2n+1})$ sont monotones, de sens contraires.
@li $|f'|\leq k<1$ sur un intervalle stable : $|u_n-\ell|\leq k^n\,|u_0-\ell|$.
@card Cesàro
@tags cesaro moyenne
@math u_n\to\ell\implies\frac{u_1+u_2+\cdots+u_n}{n}\to\ell

@page Sommes usuelles
@card Sommes d'entiers
@tags somme entiers carres cubes
@math \sum_{k=1}^{n}k=\frac{n(n+1)}{2}
@math \sum_{k=1}^{n}k^2=\frac{n(n+1)(2n+1)}{6}
@math \sum_{k=1}^{n}k^3=\left(\frac{n(n+1)}{2}\right)^2
@card Binôme de Newton
@tags binome coefficient pascal
@math (a+b)^n=\sum_{k=0}^{n}\binom{n}{k}\,a^k\,b^{n-k}
@math \binom{n}{k}=\frac{n!}{k!\,(n-k)!}\qquad\binom{n}{k}+\binom{n}{k+1}=\binom{n+1}{k+1}
@math \binom{n}{k}=\binom{n}{n-k}\qquad\sum_{k=0}^{n}\binom{n}{k}=2^n
@card Factorisation
@tags identite
@math a^n-b^n=(a-b)\sum_{k=0}^{n-1}a^k\,b^{n-1-k}
@card Télescopage
@tags telescopique
@math \sum_{k=0}^{n-1}\left(u_{k+1}-u_k\right)=u_n-u_0
@math \sum_{k=1}^{n}\frac{1}{k(k+1)}=1-\frac{1}{n+1}
@card Formule de Stirling
@tags factorielle stirling
@math n!\sim\sqrt{2\pi n}\left(\frac{n}{e}\right)^n
@card Série harmonique
@tags harmonique euler
@math H_n=\sum_{k=1}^{n}\frac{1}{k}=\ln n+\gamma+o(1)
@note $\gamma\approx 0{,}5772$ (constante d'Euler).
)TEX";
