// Chapitre : notations (formulaire / francaises et anglo-saxonnes).
extern const char CH_NOTATIONS[];
const char CH_NOTATIONS[] = R"TEX(
@page Vecteurs et matrices
@card Le formulaire et les autres
@tags notation anglais anglo saxon convention
@text La colonne « Ici » donne la notation du formulaire (usage français) ; la colonne « Anglo-saxon » celle des livres et logiciels en anglais.
@note Les deux usages se mélangent : en physique et au lycée, on trouve aussi $\times$ pour le produit vectoriel en France.
@card Vecteurs
@tags vecteur produit vectoriel scalaire cross dot wedge
@table[ccc] \text{Notion} & \text{Ici} & \text{Anglo-saxon}
@row \text{Vecteur} & \vec{u},\ \overrightarrow{AB} & \mathbf{u},\ \vec{u}
@row \text{Produit vectoriel} & \vec{u}\wedge\vec{v} & \mathbf{u}\times\mathbf{v}
@row \text{Produit scalaire} & \vec{u}\cdot\vec{v} & \mathbf{u}\cdot\mathbf{v},\ \langle\mathbf{u},\mathbf{v}\rangle
@row \text{Produit mixte} & \left[\vec{u},\vec{v},\vec{w}\right] & \mathbf{u}\cdot(\mathbf{v}\times\mathbf{w})
@row \text{Base} & \vec{e}_x,\ \vec{e}_y,\ \vec{e}_z & \mathbf{i},\ \mathbf{j},\ \mathbf{k}
@note Produit scalaire, autres écritures françaises : $(\vec{u}\,|\,\vec{v})$, $\langle\vec{u},\vec{v}\rangle$. Base anglo-saxonne aussi notée $\hat{x}$, $\hat{y}$, $\hat{z}$ (« cross product », « dot product »).
@card Analyse vectorielle
@tags gradient divergence rotationnel curl laplacien nabla
@table[ccc] \text{Notion} & \text{Ici} & \text{Anglo-saxon}
@row \text{Gradient} & \overrightarrow{\mathrm{grad}}\,f,\ \vec{\nabla}f & \nabla f,\ \operatorname{grad}f
@row \text{Divergence} & \mathrm{div}\,\vec{A} & \nabla\cdot\mathbf{A}
@row \text{Rotationnel} & \overrightarrow{\mathrm{rot}}\,\vec{A} & \nabla\times\mathbf{A},\ \operatorname{curl}\mathbf{A}
@row \text{Laplacien} & \Delta f & \nabla^2 f,\ \Delta f
@card Matrices
@tags transposee comatrice adjugate rang noyau trace
@table[ccc] \text{Notion} & \text{Ici} & \text{Anglo-saxon}
@row \text{Transposée} & A^T\ \ ({}^t\!A) & A^T,\ A'
@row \text{Comatrice} & \com A & C
@row \text{Rang} & \operatorname{rg}A & \operatorname{rank}A
@row \text{Noyau} & \operatorname{Ker}A & \ker A,\ \operatorname{null}A
@row \text{Image} & \operatorname{Im}A & \operatorname{range}A
@row \text{Trace} & \operatorname{tr}A & \operatorname{tr}A,\ \operatorname{Tr}A
@row \text{Matrices }n\times p & \mathcal{M}_{n,p}(\mathbb{R}) & \mathbb{R}^{n\times p}
@note $C$ : « cofactor matrix ». La comatrice transposée $\com(A)^T$ (ou ${}^t\!\com A$) s'écrit $\operatorname{adj}A$ (« adjugate ») : $A^{-1}=\frac{\operatorname{adj}A}{\det A}$. Piège : « adjoint » désigne souvent la transconjuguée $A^{\ast}$.

@page Fonctions et analyse
@card Fonctions usuelles
@tags logarithme ln log arcsin sinus moins un hyperbolique cotangente secante
@table[ccc] \text{Fonction} & \text{Ici} & \text{Anglo-saxon}
@row \text{Log. népérien} & \ln x & \ln x,\ \log x
@row \text{Log. décimal} & \log x & \log_{10}x
@row \text{Cotangente} & \cotan x & \cot x
@row \text{Inverses} & \frac{1}{\cos x},\ \frac{1}{\sin x} & \sec x,\ \csc x
@row \text{Réciproques} & \arcsin x & \sin^{-1}x
@row \text{Hyperboliques} & \sh x,\ \ch x & \sinh x,\ \cosh x
@row \text{Récip. hyperb.} & \argsh x & \sinh^{-1}x
@row \text{Partie entière} & \lfloor x\rfloor,\ E(x) & \lfloor x\rfloor\ \text{(floor)}
@note Pièges : $\sin^{-1}x=\arcsin x$, pas $\frac{1}{\sin x}$, alors que $\sin^2x=(\sin x)^2$. En anglais, $\log x$ désigne $\ln x$ en mathématiques mais souvent $\log_{10}x$ en ingénierie. Autres écritures : $\operatorname{asin}$ (logiciels), $\operatorname{arsinh}$, $\tanh$ pour $\th$. Sur la calculatrice, SHIFT sin donne $\sin^{-1}$, c'est-à-dire $\arcsin$.
@card Dérivées et intégrales
@tags derivee partielle primitive classe point
@table[ccc] \text{Notion} & \text{Ici} & \text{Anglo-saxon}
@row \text{Dérivée} & f',\ \frac{df}{dx} & f',\ \frac{df}{dx},\ Df
@row \text{Dérivée en }t & \dot{x}=\frac{dx}{dt} & \dot{x}
@row \text{Dérivée partielle} & \frac{\partial f}{\partial x} & \frac{\partial f}{\partial x},\ f_x,\ \partial_xf
@row \text{Classe} & \mathcal{C}^n & C^n
@note « Primitive » se dit « antiderivative » ; « intégrale impropre » : « improper integral ».
@card Intervalles et ensembles
@tags intervalle ouvert ferme crochet parenthese ensemble positif naturel inclusion
@table[ccc] \text{Notion} & \text{Ici} & \text{Anglo-saxon}
@row \text{Ouvert} & ]a,b[ & (a,b)
@row \text{Semi-ouvert} & [a,b[ & [a,b)
@row \text{Réels }\geq 0 & \mathbb{R}_+ & [0,\infty),\ \mathbb{R}_{\geq 0}
@row \text{Réels }\neq 0 & \mathbb{R}^{\ast} & \mathbb{R}\setminus\{0\}
@row \text{Naturels} & \mathbb{N}\ni 0 & \mathbb{N}\ \text{(0 souvent exclu)}
@row \text{Inclusion} & A\subset B & A\subseteq B
@row \text{Complément} & \overline{A} & A^c,\ A'
@note Vocabulaire : « positif » veut dire $\geq 0$, mais « positive » veut dire $>0$ (« non-negative » : $\geq 0$). De même, « croissante » se dit « non-decreasing » ; « increasing » est souvent strict.
@card Coordonnées sphériques et cylindriques
@tags spherique cylindrique colatitude azimut theta phi convention
@table[ccc] \text{Repère} & \text{Ici} & \text{Anglo-saxon}
@row \text{Cylindrique} & (r,\theta,z) & (\rho,\varphi,z),\ (r,\theta,z)
@row \text{Sphérique} & (r,\theta,\varphi) & (r,\theta,\varphi)\ \text{ou}\ (\rho,\varphi,\theta)
@note Le formulaire suit la convention des physiciens (norme ISO 80000-2) : $\theta\in[0,\pi]$ est la colatitude (angle depuis $Oz$) et $\varphi$ l'azimut. Beaucoup de manuels de mathématiques américains échangent $\theta$ et $\varphi$ : vérifier avant d'utiliser un jacobien ou un laplacien.

@page Nombres, probas, signaux
@card Écriture des nombres
@tags virgule point decimal milliers
@table[ccc] \text{Notion} & \text{Ici} & \text{Anglo-saxon}
@row \text{Virgule décimale} & 3{,}14 & 3.14
@row \text{Milliers} & 1\,000\,000 & 1{,}000{,}000
@row \text{Pourcentage} & 5\ \% & 5\%
@note Attention : en anglais, la virgule sépare les milliers. La calculatrice utilise le point décimal.
@card Complexes
@tags imaginaire j conjugue partie reelle
@table[ccc] \text{Notion} & \text{Ici} & \text{Anglo-saxon}
@row \text{Unité imaginaire} & i & i,\ j\ \text{(électricité)}
@row \text{Conjugué} & \overline{z} & \overline{z},\ z^{\ast}
@row \text{Partie réelle} & \operatorname{Re}z & \operatorname{Re}z,\ ℜz
@row \text{Partie imaginaire} & \operatorname{Im}z & \operatorname{Im}z,\ ℑz
@note En électricité, on écrit $j$ (aussi en France) pour ne pas confondre avec l'intensité $i$ ; le formulaire garde $i$.
@card Probabilités
@tags probabilite esperance variance conditionnelle combinaison arrangement loi
@table[ccc] \text{Notion} & \text{Ici} & \text{Anglo-saxon}
@row \text{Probabilité} & P(A) & \Pr(A),\ \mathbb{P}(A)
@row \text{Conditionnelle} & P_B(A) & P(A\mid B)
@row \text{Espérance} & E(X) & E[X],\ \mathbb{E}[X]
@row \text{Variance} & V(X) & \operatorname{Var}(X)
@row \text{Combinaisons} & \binom{n}{k},\ C_n^k & C(n,k),\ {}_nC_k
@row \text{Arrangements} & A_n^k & P(n,k),\ {}_nP_k
@row \text{Lois} & \mathcal{B},\ \mathcal{P},\ \mathcal{E} & \operatorname{Bin},\ \operatorname{Pois},\ \operatorname{Exp}
@note $P(A\mid B)$ s'écrit aussi en France ; $\binom{n}{k}$ est commun aux deux. Lois : binomiale, Poisson, exponentielle. Sur la calculatrice : nCr et nPr (menu PROB de OPTN). L'ancienne notation française $C_n^k$ place $n$ en bas, à l'inverse de certains livres anglais.
@card Transformées et signaux
@tags laplace p s jomega echelon heaviside sinc frequence
@table[ccc] \text{Notion} & \text{Ici} & \text{Anglo-saxon}
@row \text{Laplace} & p & s
@row \text{Harmonique} & H(i\omega) & H(j\omega)
@row \text{Échelon unité} & u(t) & u(t),\ H(t)
@row \text{Transformée} & \hat{f},\ \mathcal{F}[f] & \hat{f},\ \mathcal{F}\{f\},\ F(\omega)
@row \text{Sinus cardinal} & \frac{\sin x}{x} & \frac{\sin\pi x}{\pi x}
@note Vérifier la définition de $\operatorname{sinc}$ utilisée : la version normalisée $\frac{\sin\pi x}{\pi x}$ est courante en traitement du signal.
@card Arithmétique
@tags pgcd ppcm congruence modulo
@table[ccc] \text{Notion} & \text{Ici} & \text{Anglo-saxon}
@row \text{PGCD} & \pgcd(a,b) & \gcd(a,b)
@row \text{PPCM} & \ppcm(a,b) & \operatorname{lcm}(a,b)
@row \text{Congruence} & a\equiv b\ [n] & a\equiv b\ (\mathrm{mod}\ n)
)TEX";
