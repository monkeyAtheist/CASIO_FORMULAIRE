// Chapitre : matrices (definitions, operations, determinant, Gauss, inverse,
// systemes, reduction, suites de matrices).
extern const char CH_MATRICES[];
const char CH_MATRICES[] = R"TEX(
@page Définitions
@card Notation
@tags matrice coefficient taille format
@math A=\left(a_{ij}\right)_{1\leq i\leq n,\ 1\leq j\leq p}\in\mathcal{M}_{n,p}(\mathbb{R})
@math A=\begin{pmatrix}a_{11}&a_{12}&\cdots&a_{1p}\\a_{21}&a_{22}&\cdots&a_{2p}\\\vdots&\vdots&\ddots&\vdots\\a_{n1}&a_{n2}&\cdots&a_{np}\end{pmatrix}
@note $a_{ij}$ : ligne $i$, colonne $j$. Taille $n\times p$ ($n$ lignes, $p$ colonnes). Carrée si $n=p$ : $\mathcal{M}_n(\mathbb{R})$.
@card Égalité
@tags egalite
@math A=B\iff a_{ij}=b_{ij}\ \text{pour tous}\ i,j
@note Deux matrices de tailles différentes ne sont jamais égales.
@card Matrice identité, matrice nulle
@tags identite nulle neutre
@math I_3=\begin{pmatrix}1&0&0\\0&1&0\\0&0&1\end{pmatrix}\qquad0_{2,3}=\begin{pmatrix}0&0&0\\0&0&0\end{pmatrix}
@math A\,I_p=I_n\,A=A\qquad A+0=A\qquad A\,0=0
@note $I_n$ : 1 sur la diagonale, 0 ailleurs. C'est l'élément neutre du produit.
@card Matrices particulières
@tags diagonale triangulaire ligne colonne scalaire
@math D=\begin{pmatrix}d_1&0&0\\0&d_2&0\\0&0&d_3\end{pmatrix}\quad T=\begin{pmatrix}a&b&c\\0&d&e\\0&0&f\end{pmatrix}
@li **Ligne** : $1\times p$ ; **colonne** : $n\times 1$.
@li **Diagonale** : $a_{ij}=0$ si $i\neq j$. **Scalaire** : $\lambda I_n$.
@li **Triangulaire supérieure** : $a_{ij}=0$ si $i>j$ ; **inférieure** : si $i<j$.
@li **Strictement triangulaire** : diagonale nulle aussi.
@card Symétrique, antisymétrique
@tags symetrique antisymetrique transposee
@math A\ \text{symétrique}\iff A^T=A\iff a_{ji}=a_{ij}
@math A\ \text{antisymétrique}\iff A^T=-A
@math \begin{pmatrix}1&4&5\\4&2&6\\5&6&3\end{pmatrix}\qquad\begin{pmatrix}0&2&-1\\-2&0&3\\1&-3&0\end{pmatrix}
@math A=\frac{A+A^T}{2}+\frac{A-A^T}{2}
@note Antisymétrique : la diagonale est nulle. Toute matrice carrée = partie symétrique + partie antisymétrique (formule ci-dessus).
@card Matrices élémentaires E(k,l)
@tags elementaire base canonique
@text $E_{k,l}$ : 1 en position $(k,l)$, 0 ailleurs.
@math E_{1,2}=\begin{pmatrix}0&1\\0&0\end{pmatrix}\qquad E_{2,1}=\begin{pmatrix}0&0\\1&0\end{pmatrix}
@math A=\sum_{i,j}a_{ij}\,E_{i,j}\qquad E_{i,j}\,E_{k,l}=\begin{cases}E_{i,l}&\text{si }j=k\\0&\text{sinon}\end{cases}
@note $\mathcal{M}_{n,p}$ contient $n\times p$ matrices élémentaires (base canonique).

@page Opérations
@card Somme
@tags somme addition
@math A+B=\left(a_{ij}+b_{ij}\right)\qquad\text{(même taille)}
@math \begin{pmatrix}1&2\\3&4\end{pmatrix}+\begin{pmatrix}5&0\\-1&2\end{pmatrix}=\begin{pmatrix}6&2\\2&6\end{pmatrix}
@math A+B=B+A\quad(A+B)+C=A+(B+C)
@math A+0=A\qquad A+(-A)=0
@card Multiplication par un réel
@tags scalaire reel multiplication
@math \lambda A=\left(\lambda\,a_{ij}\right)
@math \lambda(A+B)=\lambda A+\lambda B
@math (\lambda+\mu)A=\lambda A+\mu A
@math \lambda(\mu A)=(\lambda\mu)A\qquad 1\cdot A=A
@card Produit
@tags produit multiplication ligne colonne
@math (AB)_{ij}=\sum_{k=1}^{p}a_{ik}\,b_{kj}=a_{i1}b_{1j}+\cdots+a_{ip}b_{pj}
@math \begin{pmatrix}a&b\\c&d\end{pmatrix}\begin{pmatrix}e&f\\g&h\end{pmatrix}=\begin{pmatrix}ae+bg&af+bh\\ce+dg&cf+dh\end{pmatrix}
@note Ligne $i$ de $A$ $\times$ colonne $j$ de $B$. $A$ : $n\times p$, $B$ : $p\times q$, $AB$ : $n\times q$. Il faut : nb de colonnes de $A$ = nb de lignes de $B$.
@card Propriétés du produit
@tags distributivite associativite
@math A(B+C)=AB+AC
@math (B+C)A=BA+CA
@math A(BC)=(AB)C
@math \lambda(AB)=(\lambda A)B=A(\lambda B)
@math I_nA=A\,I_p=A
@card Non-commutativité
@tags commutatif commutent
@text En général $AB\neq BA$ (parfois l'un des deux n'est même pas défini) :
@math \begin{pmatrix}1&0\\0&0\end{pmatrix}\begin{pmatrix}0&1\\0&0\end{pmatrix}=\begin{pmatrix}0&1\\0&0\end{pmatrix}
@math \begin{pmatrix}0&1\\0&0\end{pmatrix}\begin{pmatrix}1&0\\0&0\end{pmatrix}=\begin{pmatrix}0&0\\0&0\end{pmatrix}
@li $AB=0$ n'implique pas $A=0$ ou $B=0$.
@li $AB=AC$ n'implique pas $B=C$ (sauf si $A$ est inversible).
@li $(A+B)^2=A^2+AB+BA+B^2$ : identités remarquables seulement si $AB=BA$.
@note $A$ et $B$ commutent si $AB=BA$ (ex. : $I_n$, $\lambda I_n$, les puissances de $A$).

@page Transposée, trace, puissances
@card Transposée
@tags transposee
@math \left(A^T\right)_{ij}=a_{ji}\qquad\begin{pmatrix}1&2&3\\4&5&6\end{pmatrix}^T=\begin{pmatrix}1&4\\2&5\\3&6\end{pmatrix}
@math (A+B)^T=A^T+B^T\qquad(\lambda A)^T=\lambda A^T
@math \left(A^T\right)^T=A
@math (AB)^T=B^T\!A^T\qquad\left(A^{-1}\right)^T=\left(A^T\right)^{-1}
@note Les lignes deviennent les colonnes ; taille $n\times p\to p\times n$.
@note Aussi notée ${}^t\!A$ (France) ou $A'$.
@card Trace
@tags trace
@math \tr A=\sum_{i=1}^{n}a_{ii}\qquad\tr(A+B)=\tr A+\tr B
@math \tr(\lambda A)=\lambda\tr A\qquad\tr\left(A^T\right)=\tr A
@math \tr(AB)=\tr(BA)
@card Puissances
@tags puissance diagonale nilpotente
@math A^0=I_n\qquad A^k=A\times A\times\cdots\times A\ \ (k\ \text{fois})
@math A^pA^q=A^{p+q}\qquad\left(A^p\right)^q=A^{pq}
@math D=\mathrm{diag}\left(d_1,\dots,d_n\right)
@math D^k=\mathrm{diag}\left(d_1^k,\dots,d_n^k\right)
@li Une puissance d'une matrice triangulaire supérieure (inférieure) l'est aussi.
@li Strictement triangulaire d'ordre $n$ : $N^n=0$ (nilpotente).
@card Binôme de Newton (si AB = BA)
@tags binome commutent
@math (A+B)^n=\sum_{k=0}^{n}\binom{n}{k}A^kB^{n-k}
@math (\lambda I+N)^n=\sum_{k=0}^{n}\binom{n}{k}\lambda^{n-k}N^k
@note Utile avec $N$ nilpotente : la somme s'arrête dès que $N^k=0$.

@page Déterminant
@card Ordre 2
@tags determinant
@math \begin{vmatrix}a&b\\c&d\end{vmatrix}=ad-bc
@card Ordre 3 (règle de Sarrus)
@tags sarrus
@align \begin{vmatrix}a&b&c\\d&e&f\\g&h&i\end{vmatrix} &= aei+bfg+cdh
@align &\quad-ceg-afh-bdi
@note Sarrus : seulement pour l'ordre 3. Diagonales descendantes $(+)$, montantes $(-)$.
@card Mineurs et cofacteurs
@tags mineur cofacteur
@text Mineur $\Delta_{ij}$ : déterminant de la matrice obtenue en supprimant la ligne $i$ et la colonne $j$ de $A$.
@math C_{ij}=(-1)^{i+j}\,\Delta_{ij}\quad\text{(cofacteur)}
@card Signes des cofacteurs : le damier
@tags signe damier echiquier
@math \left((-1)^{i+j}\right)=\begin{pmatrix}+&-&+&-&\cdots\\-&+&-&+&\cdots\\+&-&+&-&\cdots\\-&+&-&+&\cdots\\\vdots&\vdots&\vdots&\vdots&\ddots\end{pmatrix}
@note $+$ en haut à gauche, puis on alterne le long des lignes et des colonnes.
@card Développement de Laplace
@tags laplace developpement ligne colonne cofacteur
@math \det A=\sum_{j=1}^{n}a_{ij}\,C_{ij}\quad\text{(selon la ligne }i\text{)}
@math \det A=\sum_{i=1}^{n}a_{ij}\,C_{ij}\quad\text{(selon la colonne }j\text{)}
@sub Exemple : selon la 1re ligne
@align \begin{vmatrix}a_{11}&a_{12}&a_{13}\\a_{21}&a_{22}&a_{23}\\a_{31}&a_{32}&a_{33}\end{vmatrix} &= +\,a_{11}\begin{vmatrix}a_{22}&a_{23}\\a_{32}&a_{33}\end{vmatrix}
@align &\quad-a_{12}\begin{vmatrix}a_{21}&a_{23}\\a_{31}&a_{33}\end{vmatrix}
@align &\quad+a_{13}\begin{vmatrix}a_{21}&a_{22}\\a_{31}&a_{32}\end{vmatrix}
@card Méthode de Laplace pas à pas
@tags laplace methode
@li Choisir la ligne ou la colonne qui a le plus de zéros.
@li Pour chaque $a_{ij}\neq 0$ : rayer la ligne $i$ et la colonne $j$, calculer le mineur $\Delta_{ij}$.
@li Prendre le signe du damier $(-1)^{i+j}$, multiplier par $a_{ij}$, puis additionner.
@math \begin{vmatrix}2&0&1\\1&3&-1\\4&0&5\end{vmatrix}\underset{C_2}{=}+3\begin{vmatrix}2&1\\4&5\end{vmatrix}=3\times 6=18
@note Exemple : développement selon la colonne 2 ; seul $a_{22}=3$ est non nul, signe $+$.
@card Propriétés
@tags inversible operations lignes
@math \det(AB)=\det A\,\det B\qquad\det\left(A^T\right)=\det A
@math \det(\lambda A)=\lambda^n\det A\qquad\det\left(A^{-1}\right)=\frac{1}{\det A}
@li Matrice triangulaire : produit des termes diagonaux.
@li $L_i\leftrightarrow L_j$ : change le signe. $L_i\leftarrow\lambda L_i$ : multiplie par $\lambda$.
@li $L_i\leftarrow L_i+\mu L_j$ : ne change rien (idem sur les colonnes).
@li Une ligne nulle ou deux lignes proportionnelles : $\det=0$.
@li $A$ inversible $\iff\det A\neq 0$.

@page Pivot de Gauss
@card Opérations élémentaires
@tags gauss operation elementaire ligne
@li **Échange** : $L_i\leftrightarrow L_j$.
@li **Dilatation** : $L_i\leftarrow\lambda L_i$, avec $\lambda\neq 0$.
@li **Transvection** : $L_i\leftarrow L_i+\mu L_j$, avec $j\neq i$.
@note Elles ne changent ni le rang, ni les solutions d'un système, ni le caractère inversible.
@card Matrices d'opérations
@tags transvection dilatation permutation
@text Faire une opération sur les lignes de $A$ revient à multiplier $A$ à gauche par la matrice obtenue en faisant cette opération sur $I_n$ :
@math T=\begin{pmatrix}1&0&0\\\mu&1&0\\0&0&1\end{pmatrix}:\ L_2\leftarrow L_2+\mu L_1
@math P_{12}=\begin{pmatrix}0&1&0\\1&0&0\\0&0&1\end{pmatrix}:\ L_1\leftrightarrow L_2
@note Transvection $I_n+\mu E_{i,j}$ ; dilatation $\mathrm{diag}(1,\dots,\lambda,\dots,1)$ ; permutation $P_{ij}$. Toutes sont inversibles.
@card Algorithme du pivot
@tags echelonner algorithme pivot
@li Choisir un pivot non nul dans la 1re colonne (échanger des lignes si besoin).
@li Annuler sous le pivot : $L_i\leftarrow L_i-\frac{a_{i1}}{a_{11}}L_1$.
@li Recommencer sur la sous-matrice : on obtient une matrice échelonnée.
@li Remonter (Gauss-Jordan) pour annuler au-dessus des pivots.
@math \begin{pmatrix}1&2\\3&4\end{pmatrix}\xrightarrow{L_2\leftarrow L_2-3L_1}\begin{pmatrix}1&2\\0&-2\end{pmatrix}
@card Inverse par Gauss-Jordan
@tags inverse gauss jordan
@math \left(\begin{array}{c|c}A&I_n\end{array}\right)\ \sim\ \cdots\ \sim\ \left(\begin{array}{c|c}I_n&A^{-1}\end{array}\right)
@math \left(\begin{array}{cc|cc}1&2&1&0\\3&4&0&1\end{array}\right)\sim\left(\begin{array}{cc|cc}1&0&-2&1\\0&1&\frac{3}{2}&-\frac{1}{2}\end{array}\right)
@note Mêmes opérations des deux côtés. Si une ligne de zéros apparaît à gauche : $A$ n'est pas inversible.
@card Rang
@tags rang echelonnee pivots
@text Rang = nombre de pivots (lignes non nulles) après échelonnement.
@math \rg A\leq\min(n,p)\qquad\rg A=\rg A^T
@math \rg A+\dim\Ker A=p\quad\text{(théorème du rang)}
@math A\in\mathcal{M}_n\ \text{inversible}\iff\rg A=n

@page Inverse
@card Définition
@tags inversible inverse
@math A\,B=B\,A=I_n\implies B=A^{-1}\ \text{(unique)}
@li $A$ inversible $\iff$ $\det A\neq 0$ $\iff$ $\rg A=n$ $\iff$ $\left(AX=0\implies X=0\right)$.
@li Si $A$ est inversible : $AB=AC\implies B=C$.
@card Inverse 2 × 2
@tags inverse deux
@math \begin{pmatrix}a&b\\c&d\end{pmatrix}^{-1}=\frac{1}{ad-bc}\begin{pmatrix}d&-b\\-c&a\end{pmatrix}
@note On échange $a$ et $d$, on change le signe de $b$ et $c$, on divise par le déterminant.
@card Comatrice
@tags comatrice cofacteur adjugate adj cofactor
@math \com(A)=\left(C_{ij}\right)=\left((-1)^{i+j}\Delta_{ij}\right)
@math A\,\com(A)^T=\com(A)^T\!A=\det(A)\,I_n
@math A^{-1}=\frac{1}{\det A}\,\com(A)^T
@note Ne pas oublier de transposer la comatrice.
@note Notation anglo-saxonne : « cofactor matrix » ; $\com(A)^T=\operatorname{adj}A$ (« adjugate »).
@card Exemple 3 × 3
@tags exemple inverse
@math A=\begin{pmatrix}1&-1&1\\2&1&2\\4&5&-3\end{pmatrix}\qquad\det A=-21
@math \com(A)=\begin{pmatrix}-13&14&6\\2&-7&-9\\-3&0&3\end{pmatrix}
@math A^{-1}=\frac{1}{21}\begin{pmatrix}13&-2&3\\-14&7&0\\-6&9&-3\end{pmatrix}
@card Propriétés
@tags produit inverse
@math (AB)^{-1}=B^{-1}A^{-1}\qquad\left(A^T\right)^{-1}=\left(A^{-1}\right)^T
@math (\lambda A)^{-1}=\frac{1}{\lambda}A^{-1}\qquad\left(A^k\right)^{-1}=\left(A^{-1}\right)^k
@math \mathrm{diag}(d_1,\dots,d_n)^{-1}=\mathrm{diag}\left(\frac{1}{d_1},\dots,\frac{1}{d_n}\right)

@page Systèmes linéaires
@card Écriture matricielle
@tags systeme ecriture matricielle
@math \begin{cases}a_{11}x+a_{12}y+a_{13}z=b_1\\a_{21}x+a_{22}y+a_{23}z=b_2\\a_{31}x+a_{32}y+a_{33}z=b_3\end{cases}\iff AX=B
@math A=\left(a_{ij}\right),\quad X=\begin{pmatrix}x\\y\\z\end{pmatrix},\quad B=\begin{pmatrix}b_1\\b_2\\b_3\end{pmatrix}
@card Résolution par l'inverse
@tags resolution inverse
@math A\ \text{inversible}:\ AX=B\iff X=A^{-1}B
@note Solution unique (système de Cramer).
@card Formules de Cramer
@tags cramer
@math x_i=\frac{\det A_i}{\det A}
@math \begin{cases}ax+by=e\\cx+dy=f\end{cases}
@math x=\frac{ed-bf}{ad-bc}\qquad y=\frac{af-ce}{ad-bc}
@note $A_i$ : $A$ dont la colonne $i$ est remplacée par $B$.
@card Résolution par le pivot de Gauss
@tags gauss echelonne augmentee
@math \left(\begin{array}{ccc|c}a_{11}&a_{12}&a_{13}&b_1\\a_{21}&a_{22}&a_{23}&b_2\\a_{31}&a_{32}&a_{33}&b_3\end{array}\right)\sim\left(\begin{array}{ccc|c}\ast&\ast&\ast&\ast\\0&\ast&\ast&\ast\\0&0&\ast&\ast\end{array}\right)
@text On échelonne la matrice augmentée $(A\,|\,B)$, puis on remonte : $z$, puis $y$, puis $x$.
@card Nombre de solutions
@tags rouche existence unicite
@li $\rg A=\rg(A\,|\,B)=p$ : solution unique.
@li $\rg A=\rg(A\,|\,B)<p$ : infinité de solutions ($p-\rg A$ paramètres).
@li $\rg A<\rg(A\,|\,B)$ : aucune solution.
@li $AX=0$ a une solution non nulle $\iff\det A=0$ (cas carré).

@page Réduction
@card Valeurs et vecteurs propres
@tags valeur propre vecteur propre caracteristique
@math A\,V=\lambda\,V,\quad V\neq 0
@math \chi_A(\lambda)=\det\left(\lambda I_n-A\right)=0
@math \tr A=\sum_i\lambda_i\qquad\det A=\prod_i\lambda_i
@card Méthode en dimension 2
@tags diagonaliser deux
@math \lambda^2-(\tr A)\,\lambda+\det A=0
@li Pour chaque racine $\lambda$, résoudre $(A-\lambda I_2)V=0$.
@li $V$, $W$ vecteurs propres non proportionnels : $P=\left(V\ W\right)$.
@math A=P\begin{pmatrix}\lambda&0\\0&\mu\end{pmatrix}P^{-1}
@card Diagonalisation
@tags diagonalisable puissance
@math A=P\,D\,P^{-1}\qquad D=\mathrm{diag}\left(\lambda_1,\dots,\lambda_n\right)
@math A^k=P\,D^k\,P^{-1}
@note Diagonalisable si $n$ vecteurs propres indépendants : par exemple $n$ valeurs propres distinctes, ou $A$ symétrique réelle ($P$ orthogonale).
@card Changement de base
@tags passage base similitude
@math A'=P^{-1}A\,P\qquad X=P\,X'
@note Colonnes de $P$ : nouveaux vecteurs de base exprimés dans l'ancienne base.
@card Matrices orthogonales, rotations
@tags rotation orthogonale
@math P^TP=I_n\iff P^{-1}=P^T\qquad\det P=\pm 1
@math R_{\theta}=\begin{pmatrix}\cos\theta&-\sin\theta\\\sin\theta&\cos\theta\end{pmatrix}
@math R_{\theta}R_{\varphi}=R_{\theta+\varphi}\qquad R_{\theta}^{-1}=R_{-\theta}=R_{\theta}^T

@page Suites de matrices
@card Suite U(n+1) = A U(n)
@tags suite matrice recurrence
@math U_{n+1}=A\,U_n\implies U_n=A^n\,U_0
@note Pour calculer $A^n$ : diagonaliser ($A^n=PD^nP^{-1}$), écrire $A=\lambda I+N$ avec $N$ nilpotente (binôme), ou chercher une récurrence.
@card Suite U(n+1) = A U(n) + B
@tags etat stable point fixe
@math C=AC+B\iff(I_n-A)\,C=B
@math C=(I_n-A)^{-1}B
@math U_n=A^n\left(U_0-C\right)+C
@note $C$ : état stable (si $I_n-A$ est inversible).
@card Récurrence d'ordre 2
@tags recurrence lineaire companion
@math u_{n+2}=a\,u_{n+1}+b\,u_n\iff X_{n+1}=\begin{pmatrix}a&b\\1&0\end{pmatrix}X_n
@math X_n=\begin{pmatrix}u_{n+1}\\u_n\end{pmatrix}\qquad X_n=\begin{pmatrix}a&b\\1&0\end{pmatrix}^nX_0
@note Les valeurs propres de cette matrice sont les racines de $r^2=ar+b$.
@card Convergence
@tags limite convergence
@text Une suite de matrices converge si chaque coefficient converge.
@math A=PDP^{-1}\ \text{et}\ |\lambda_i|<1\ \text{pour tout}\ i
@math \implies A^n\xrightarrow[n\to+\infty]{}0
@card Graphe probabiliste (Markov)
@tags markov probabiliste transition stable
@math P_{n+1}=P_n\,M\implies P_n=P_0\,M^n
@math \text{État stable}:\ P=P\,M,\quad p_1+\cdots+p_k=1
@note $P_n$ : matrice ligne des probabilités ; $M$ : matrice de transition (lignes de somme 1). Si une puissance de $M$ a tous ses coefficients $>0$ : état stable unique, limite de $P_n$ quel que soit $P_0$.
@math M=\begin{pmatrix}0{,}85&0{,}15\\0{,}45&0{,}55\end{pmatrix}\implies P=\begin{pmatrix}0{,}75&0{,}25\end{pmatrix}
)TEX";
