// Chapitre : probabilites et statistiques.
extern const char CH_PROBAS[];
const char CH_PROBAS[] = R"TEX(
@page Probabilités
@card Règles de calcul
@tags probabilite union contraire
@math P(\bar{A})=1-P(A)\qquad P(\emptyset)=0\qquad 0\leq P(A)\leq 1
@math P(A\cup B)=P(A)+P(B)-P(A\cap B)
@card Équiprobabilité
@tags equiprobable denombrement
@math P(A)=\frac{\card A}{\card\Omega}=\frac{\text{nb de cas favorables}}{\text{nb de cas possibles}}
@card Probabilité conditionnelle
@tags conditionnelle sachant
@math P_B(A)=P(A\mid B)=\frac{P(A\cap B)}{P(B)}
@math P(A\cap B)=P(B)\,P_B(A)=P(A)\,P_A(B)
@card Indépendance
@tags independant
@math A,B\ \text{indépendants}\iff P(A\cap B)=P(A)\,P(B)
@note Alors $P_B(A)=P(A)$. Ne pas confondre avec incompatibles ($A\cap B=\emptyset$).
@card Probabilités totales, Bayes
@tags bayes totales partition arbre
@math P(B)=\sum_iP(A_i)\,P_{A_i}(B)
@math P_B(A_k)=\frac{P(A_k)\,P_{A_k}(B)}{\sum_iP(A_i)\,P_{A_i}(B)}
@note $(A_i)$ : système complet d'événements (ils partagent l'univers).

@page Variables aléatoires
@card Espérance
@tags esperance moyenne
@math E(X)=\sum_ix_i\,P(X=x_i)\qquad E(X)=\int_{-\infty}^{+\infty}x\,f(x)\,dx
@math E(aX+b)=a\,E(X)+b\qquad E(X+Y)=E(X)+E(Y)
@math E\big(g(X)\big)=\sum_ig(x_i)\,P(X=x_i)
@card Variance et écart-type
@tags variance ecart type koenig
@math V(X)=E\Big(\big(X-E(X)\big)^2\Big)=E\left(X^2\right)-E(X)^2
@math V(aX+b)=a^2\,V(X)\qquad\sigma(X)=\sqrt{V(X)}
@math V(X+Y)=V(X)+V(Y)
@note Dernière formule : seulement si $X$ et $Y$ sont indépendantes.
@note Notation anglo-saxonne : $\operatorname{Var}(X)$, et $E[X]$ ou $\mathbb{E}[X]$ pour l'espérance.
@card Densité et fonction de répartition
@tags densite repartition continue
@math f\geq 0\qquad\int_{-\infty}^{+\infty}f(x)\,dx=1\qquad P(a\leq X\leq b)=\int_a^bf(x)\,dx
@math F(x)=P(X\leq x)\qquad F'=f\qquad P(a<X\leq b)=F(b)-F(a)
@card Covariance
@tags covariance correlation
@math \mathrm{cov}(X,Y)=E(XY)-E(X)\,E(Y)
@math V(X+Y)=V(X)+V(Y)+2\,\mathrm{cov}(X,Y)
@math \rho=\frac{\mathrm{cov}(X,Y)}{\sigma_X\,\sigma_Y}\in[-1,1]
@card Tchebychev, grands nombres
@tags bienayme tchebychev grands nombres moyenne
@math P\big(|X-E(X)|\geq\varepsilon\big)\leq\frac{V(X)}{\varepsilon^2}
@math \bar{X}_n=\frac{X_1+\cdots+X_n}{n}\qquad E\left(\bar{X}_n\right)=\mu\qquad V\left(\bar{X}_n\right)=\frac{\sigma^2}{n}
@note Loi des grands nombres : $\bar{X}_n\to\mu$ (variables indépendantes de même loi).

@page Lois discrètes
@card Épreuve et loi de Bernoulli
@tags bernoulli epreuve succes
@text Épreuve à deux issues : succès (probabilité $p$) ou échec ($1-p$). $X\sim\mathcal{B}(p)$ :
@math P(X=1)=p\qquad P(X=0)=1-p
@math E(X)=p\qquad V(X)=p\,(1-p)
@card Loi binomiale B(n, p)
@tags binomiale schema bernoulli
@text Nombre de succès dans $n$ épreuves de Bernoulli identiques et indépendantes (schéma de Bernoulli) :
@math P(X=k)=\binom{n}{k}\,p^k\,(1-p)^{n-k},\ \ 0\leq k\leq n
@math E(X)=np\qquad V(X)=np\,(1-p)
@card Loi géométrique G(p)
@tags geometrique premier succes
@text Rang du premier succès :
@math P(X=k)=(1-p)^{k-1}\,p,\quad k\geq 1
@math E(X)=\frac{1}{p}\qquad V(X)=\frac{1-p}{p^2}\qquad P(X>k)=(1-p)^k
@card Loi de Poisson P(λ)
@tags poisson evenements rares
@text Nombre d'événements rares pendant une durée donnée (pannes, appels, désintégrations…) :
@math P(X=k)=e^{-\lambda}\,\frac{\lambda^k}{k!},\quad k\in\mathbb{N}
@math E(X)=\lambda\qquad V(X)=\lambda
@note Si $X\sim\mathcal{P}(\lambda)$ et $Y\sim\mathcal{P}(\mu)$ indépendantes : $X+Y\sim\mathcal{P}(\lambda+\mu)$.
@card Loi uniforme discrète
@tags uniforme de
@math P(X=k)=\frac{1}{n},\quad k\in\{1,\dots,n\}
@math E(X)=\frac{n+1}{2}\qquad V(X)=\frac{n^2-1}{12}

@page Lois continues
@card Loi uniforme U([a, b])
@tags uniforme continue
@math f(x)=\frac{1}{b-a}\ \text{sur}\ [a,b]
@math E(X)=\frac{a+b}{2}\qquad V(X)=\frac{(b-a)^2}{12}
@card Loi exponentielle E(λ)
@tags exponentielle duree vie memoire
@math f(x)=\lambda\,e^{-\lambda x}\quad(x\geq 0)
@math P(X\leq t)=1-e^{-\lambda t}\qquad P(X>t)=e^{-\lambda t}
@math E(X)=\frac{1}{\lambda}\qquad V(X)=\frac{1}{\lambda^2}
@text Absence de mémoire :
@math P_{X>s}(X>s+t)=P(X>t)
@note Durée de vie sans vieillissement ; $\frac{1}{\lambda}$ : durée de vie moyenne.
@card Loi normale N(μ, σ²)
@tags normale gauss laplace cloche
@math f(x)=\frac{1}{\sigma\sqrt{2\pi}}\,e^{-\frac{(x-\mu)^2}{2\sigma^2}}
@math E(X)=\mu\qquad V(X)=\sigma^2
@note Courbe en cloche symétrique autour de $\mu$. $aX+b\sim\mathcal{N}\left(a\mu+b,a^2\sigma^2\right)$ ; une somme de normales indépendantes est normale.
@card Loi normale centrée réduite N(0, 1)
@tags centree reduite phi
@math Z=\frac{X-\mu}{\sigma}\sim\mathcal{N}(0,1)\qquad\varphi(z)=\frac{1}{\sqrt{2\pi}}\,e^{-\frac{z^2}{2}}
@math \Phi(z)=P(Z\leq z)\qquad\Phi(-z)=1-\Phi(z)
@math P(-z\leq Z\leq z)=2\,\Phi(z)-1
@card Valeurs à connaître
@tags table valeurs 1,96
@table[ccccc] z & 1 & 1{,}645 & 1{,}96 & 2{,}576
@row \Phi(z) & 0{,}841 & 0{,}95 & 0{,}975 & 0{,}995
@row P(|Z|\leq z) & 0{,}683 & 0{,}90 & 0{,}95 & 0{,}99
@note $P(\mu-\sigma\leq X\leq\mu+\sigma)\approx 68\,\%$ ; à $\pm 2\sigma$ : 95 % ; à $\pm 3\sigma$ : 99,7 %.

@page Approximations, estimation
@card Approximations
@tags approximation binomiale poisson normale
@li Binomiale par Poisson ($n\geq 30$, $p\leq 0{,}1$) : $\mathcal{B}(n,p)\approx\mathcal{P}(np)$.
@li Binomiale par normale ($n\geq 30$, $np\geq 5$, $n(1-p)\geq 5$) : $\mathcal{B}(n,p)\approx\mathcal{N}\big(np,\,np(1-p)\big)$.
@li Poisson par normale ($\lambda\geq 20$) : $\mathcal{P}(\lambda)\approx\mathcal{N}(\lambda,\lambda)$.
@note Correction de continuité : $P(X=k)\approx P(k-0{,}5\leq Y\leq k+0{,}5)$.
@card Théorème central limite
@tags tcl central limite
@math \frac{X_1+\cdots+X_n-n\mu}{\sigma\sqrt{n}}\xrightarrow[n\to+\infty]{}\mathcal{N}(0,1)
@note Variables indépendantes de même loi (espérance $\mu$, écart-type $\sigma$). En pratique dès $n\geq 30$.
@card Intervalles de confiance à 95 %
@tags confiance estimation proportion moyenne
@math \text{Moyenne}:\ \bar{x}\pm 1{,}96\,\frac{\sigma}{\sqrt{n}}
@math \text{Proportion}:\ f\pm 1{,}96\,\sqrt{\frac{f(1-f)}{n}}
@note $\sigma$ inconnu : le remplacer par $s$ (écart-type corrigé). À 99 % : 2,576 au lieu de 1,96.

@page Statistique descriptive
@card Moyenne, variance, écart-type
@tags moyenne variance ecart type
@math \bar{x}=\frac{1}{n}\sum_{i=1}^{n}x_i\qquad\bar{x}=\frac{1}{N}\sum_in_i\,x_i\ \ \text{(effectifs)}
@math V=\frac{1}{n}\sum_{i=1}^{n}\left(x_i-\bar{x}\right)^2=\overline{x^2}-\bar{x}^2\qquad\sigma=\sqrt{V}
@math s^2=\frac{1}{n-1}\sum_{i=1}^{n}\left(x_i-\bar{x}\right)^2\quad\text{(variance corrigée)}
@card Médiane, quartiles
@tags mediane quartile
@text Médiane : valeur qui partage la série ordonnée en deux moitiés. $Q_1$, $Q_3$ : 1er et 3e quartiles ; écart interquartile $Q_3-Q_1$.
@card Covariance, corrélation
@tags covariance correlation
@math \mathrm{cov}(x,y)=\frac{1}{n}\sum_{i=1}^{n}\left(x_i-\bar{x}\right)\left(y_i-\bar{y}\right)=\overline{xy}-\bar{x}\,\bar{y}
@math r=\frac{\mathrm{cov}(x,y)}{\sigma_x\,\sigma_y}\in[-1,1]
@card Régression linéaire (moindres carrés)
@tags regression ajustement droite moindres carres
@math y=a\,x+b\qquad a=\frac{\mathrm{cov}(x,y)}{V(x)}\qquad b=\bar{y}-a\,\bar{x}
@note La droite passe par le point moyen $G\left(\bar{x},\bar{y}\right)$. $|r|$ proche de 1 : bon ajustement.
)TEX";
