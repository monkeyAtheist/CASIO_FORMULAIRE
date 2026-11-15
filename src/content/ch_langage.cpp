// Chapitre : langage mathematique (logique, ensembles, applications,
// symboles usuels et leur lecture).
extern const char CH_LANGAGE[];
const char CH_LANGAGE[] = R"TEX(
@page Logique
@card Quantificateurs
@tags pour tout quel que soit il existe unique quantificateur forall exists
@table[cc] \text{Symbole} & \text{Se lit}
@row \forall & \text{pour tout, quel que soit}
@row \exists & \text{il existe (au moins un)}
@row \exists! & \text{il existe un unique}
@row \nexists & \text{il n'existe pas}
@math \forall x\in\mathbb{R},\ x^2\geq 0\qquad\exists x\in\mathbb{R},\ x^2=2
@math \exists!\,x\in\mathbb{R},\ e^x=2\qquad\nexists x\in\mathbb{R},\ x^2=-1
@note La virgule se lit « on a » (ou « tel que » après $\exists$). L'ordre compte : $\forall x,\ \exists y,\ y>x$ est vrai dans $\mathbb{R}$, mais $\exists y,\ \forall x,\ y>x$ est faux.
@card Connecteurs logiques
@tags non et ou implique equivaut connecteur
@table[cc] \text{Symbole} & \text{Se lit}
@row \neg P & \text{non }P
@row P\land Q & P\text{ et }Q
@row P\lor Q & P\text{ ou }Q\ \text{(inclusif)}
@row P\implies Q & \text{si }P\text{, alors }Q
@row P\iff Q & P\text{ si et seulement si }Q
@note En français, on écrit plutôt « et », « ou » que $\land$, $\lor$ (attention : $\wedge$ est aussi le produit vectoriel). « $P$ ou $Q$ » est vrai dès que l'une au moins est vraie.
@card Table de vérité
@tags vrai faux verite
@table[cccccc] P & Q & \neg P & P\land Q & P\lor Q & P\implies Q
@row \text{V} & \text{V} & \text{F} & \text{V} & \text{V} & \text{V}
@row \text{V} & \text{F} & \text{F} & \text{F} & \text{V} & \text{F}
@row \text{F} & \text{V} & \text{V} & \text{F} & \text{V} & \text{V}
@row \text{F} & \text{F} & \text{V} & \text{F} & \text{F} & \text{V}
@note V : vrai, F : faux. Une implication dont l'hypothèse est fausse est vraie.
@card Implication, contraposée, réciproque
@tags contraposee reciproque condition necessaire suffisante il faut il suffit
@math (P\implies Q)\iff(\neg Q\implies\neg P)\quad\text{(contraposée)}
@math (P\iff Q)\iff\big((P\implies Q)\ \text{et}\ (Q\implies P)\big)
@math (P\implies Q)\iff(\neg P\ \text{ou}\ Q)
@li $P\implies Q$ : $P$ est une condition suffisante pour $Q$ (« il suffit que $P$ ») ; $Q$ est une condition nécessaire pour $P$ (« il faut que $Q$ »).
@li Réciproque de $P\implies Q$ : $Q\implies P$. Elle peut être fausse : $x=2\implies x^2=4$, mais $x^2=4$ n'implique pas $x=2$.
@card Négations
@tags negation de morgan contraire
@math \neg\big(\forall x,\ P(x)\big)\iff\exists x,\ \neg P(x)
@math \neg\big(\exists x,\ P(x)\big)\iff\forall x,\ \neg P(x)
@math \neg(P\ \text{et}\ Q)\iff\neg P\ \text{ou}\ \neg Q
@math \neg(P\ \text{ou}\ Q)\iff\neg P\ \text{et}\ \neg Q
@math \neg(P\implies Q)\iff P\ \text{et}\ \neg Q
@note Pour nier : échanger $\forall$ et $\exists$, « et » et « ou », puis nier la propriété finale ($<$ devient $\geq$). Lignes 3 et 4 : lois de De Morgan.
@card Exemple : nier une limite
@tags negation limite epsilon exemple
@text $u_n\to\ell$ :
@math \forall\varepsilon>0,\ \exists N\in\mathbb{N},\ \forall n\geq N,\ |u_n-\ell|<\varepsilon
@text $(u_n)$ ne tend pas vers $\ell$ :
@math \exists\varepsilon>0,\ \forall N\in\mathbb{N},\ \exists n\geq N,\ |u_n-\ell|\geq\varepsilon
@card Types de raisonnement
@tags raisonnement absurde contraposee contre exemple disjonction analyse synthese
@li Direct : on suppose $P$ et on en déduit $Q$.
@li Par contraposée : on suppose $\neg Q$ et on montre $\neg P$.
@li Par l'absurde : on suppose le contraire de ce qu'on veut montrer et on aboutit à une contradiction.
@li Contre-exemple : pour montrer qu'un « $\forall x$ » est faux, un seul $x$ qui ne convient pas suffit.
@li Disjonction de cas ; analyse-synthèse (existence et unicité).
@card Récurrence
@tags recurrence initialisation heredite
@li Initialisation : $P(n_0)$ est vraie.
@li Hérédité : pour tout $n\geq n_0$, $P(n)\implies P(n+1)$.
@li Conclusion : $P(n)$ est vraie pour tout $n\geq n_0$.
@note Récurrence forte : on suppose $P(n_0)$, …, $P(n)$ toutes vraies pour montrer $P(n+1)$.

@page Ensembles
@card Appartenance et inclusion
@tags appartient inclus contient sous ensemble vide
@table[cc] \text{Symbole} & \text{Se lit}
@row x\in A & x\text{ appartient à }A
@row x\notin A & x\text{ n'appartient pas à }A
@row A\subset B & A\text{ est inclus dans }B
@row A\subsetneq B & \text{inclus, mais }A\neq B
@row A\not\subset B & A\text{ n'est pas inclus dans }B
@row B\supset A & B\text{ contient }A
@row \emptyset & \text{l'ensemble vide}
@math A\subset B\iff\big(\forall x,\ x\in A\implies x\in B\big)
@math A=B\iff A\subset B\ \text{et}\ B\subset A
@note $A\subseteq B$ a le même sens que $A\subset B$ (notation anglo-saxonne). Pour montrer $A=B$ : double inclusion.
@card Opérations
@tags union intersection prive complementaire produit cartesien parties
@table[cc] \text{Symbole} & \text{Se lit}
@row A\cup B & A\text{ union }B
@row A\cap B & A\text{ inter }B
@row A\setminus B & A\text{ privé de }B
@row \overline{A},\ \complement_EA & \text{complémentaire de }A
@row A\times B & A\text{ croix }B
@row \mathcal{P}(E) & \text{parties de }E
@math x\in A\cup B\iff x\in A\ \text{ou}\ x\in B
@math x\in A\cap B\iff x\in A\ \text{et}\ x\in B
@math A\times B=\left\{(a,b)\mid a\in A,\ b\in B\right\}
@note $A\times B$ : produit cartésien (couples). $A$ et $B$ sont disjoints si $A\cap B=\emptyset$.
@card Propriétés
@tags de morgan distributivite cardinal
@math \overline{A\cup B}=\overline{A}\cap\overline{B}\qquad\overline{A\cap B}=\overline{A}\cup\overline{B}
@math A\cap(B\cup C)=(A\cap B)\cup(A\cap C)
@math A\cup(B\cap C)=(A\cup B)\cap(A\cup C)
@align \mathrm{card}(A\cup B) &= \mathrm{card}\,A+\mathrm{card}\,B
@align &\quad-\mathrm{card}(A\cap B)
@math \mathrm{card}(A\times B)=\mathrm{card}\,A\times\mathrm{card}\,B\qquad\mathrm{card}\,\mathcal{P}(E)=2^{\mathrm{card}\,E}
@note Le cardinal (nombre d'éléments) se note aussi $|A|$ ou $\#A$.
@card Écrire un ensemble
@tags accolades tel que intervalle entiers
@math \{1,2,3\}\qquad\{x\in\mathbb{R}\mid x^2<1\}=\,]-1,1[
@math \llbracket 1,n\rrbracket=\{1,2,\ldots,n\}\qquad\{2k\mid k\in\mathbb{Z}\}=2\mathbb{Z}
@note La barre $\mid$ (ou « : ») se lit « tel que ». $\llbracket a,b\rrbracket$ : entiers de $a$ à $b$.
@card Ensembles de nombres
@tags naturels relatifs decimaux rationnels reels complexes
@math \mathbb{N}\subset\mathbb{Z}\subset\mathbb{D}\subset\mathbb{Q}\subset\mathbb{R}\subset\mathbb{C}
@table[cc] \text{Ensemble} & \text{Nom}
@row \mathbb{N} & \text{entiers naturels}\ \{0,1,2,\ldots\}
@row \mathbb{Z} & \text{entiers relatifs}
@row \mathbb{D} & \text{décimaux}
@row \mathbb{Q} & \text{rationnels}\ \frac{p}{q}
@row \mathbb{R} & \text{réels}
@row \mathbb{C} & \text{complexes}
@note $\mathbb{R}^{\ast}=\mathbb{R}\setminus\{0\}$, $\mathbb{R}_+=[0,+\infty[$, $\mathbb{R}_+^{\ast}=\,]0,+\infty[$, $\mathbb{R}_-=\,]-\infty,0]$. Irrationnels : $\mathbb{R}\setminus\mathbb{Q}$ (par exemple $\sqrt{2}$, $\pi$).

@page Applications, symboles
@card Applications
@tags application fonction rond composition image antecedent mapsto
@math f:\ E\to F,\ \ x\mapsto f(x)
@note Se lit « $f$ de $E$ dans $F$, qui à $x$ associe $f(x)$ ». $f(x)$ : image de $x$ ; $x$ : un antécédent de $f(x)$.
@math (g\circ f)(x)=g\big(f(x)\big)
@note $g\circ f$ se lit « $g$ rond $f$ » : on applique $f$, puis $g$. En général $g\circ f\neq f\circ g$.
@math f(A)=\{f(x)\mid x\in A\}\qquad f^{-1}(B)=\{x\in E\mid f(x)\in B\}
@note Image directe de $A$, image réciproque de $B$. $\mathrm{Id}_E$ : identité, $x\mapsto x$.
@card Injection, surjection, bijection
@tags injective surjective bijective reciproque
@text Injective :
@math \forall x,x'\in E,\ f(x)=f(x')\implies x=x'
@text Surjective :
@math \forall y\in F,\ \exists x\in E,\ f(x)=y
@text Bijective :
@math \forall y\in F,\ \exists!\,x\in E,\ f(x)=y
@note Bijective = injective et surjective. Alors $f^{-1}\circ f=\mathrm{Id}_E$ et $f\circ f^{-1}=\mathrm{Id}_F$.
@card Comparer, approcher
@tags environ equivalent petit o grand o negligeable proportionnel congru defini
@table[cc] \text{Symbole} & \text{Se lit}
@row a\approx b & a\text{ environ égal à }b
@row f\sim g & f\text{ équivalente à }g
@row f=o(g) & f\text{ négligeable devant }g
@row f=O(g) & f\text{ dominée par }g
@row a\ll b & a\text{ très petit devant }b
@row y\propto x & y\text{ proportionnel à }x
@row a\equiv b\ [n] & a\text{ congru à }b\text{ modulo }n
@row a:=b & a\text{ est défini par }b
@note $o$ : « petit o », $\frac{f}{g}\to 0$ ; $O$ : « grand O », $\frac{f}{g}$ bornée (au voisinage du point considéré). $f\equiv 0$ : $f$ identiquement nulle.
@card Sommes, produits, réunions
@tags somme produit sigma indice muet reunion
@math \sum_{k=1}^{n}a_k=a_1+a_2+\cdots+a_n\qquad\prod_{k=1}^{n}a_k=a_1\,a_2\cdots a_n
@math \bigcup_{i\in I}A_i\qquad\bigcap_{i\in I}A_i
@note « Somme pour $k$ allant de 1 à $n$ des $a_k$ ». L'indice $k$ est muet : on peut le renommer.
@card Lettres grecques
@tags grec alphabet alpha beta lettre
@table[cccc] \text{Lettre} & \text{Nom} & \text{Lettre} & \text{Nom}
@row \alpha & \text{alpha} & \nu & \text{nu}
@row \beta & \text{bêta} & \xi,\ \Xi & \text{xi}
@row \gamma,\ \Gamma & \text{gamma} & \pi,\ \Pi & \text{pi}
@row \delta,\ \Delta & \text{delta} & \rho & \text{rhô}
@row \varepsilon,\ \epsilon & \text{epsilon} & \sigma,\ \Sigma & \text{sigma}
@row \zeta & \text{zêta} & \tau & \text{tau}
@row \eta & \text{êta} & \upsilon & \text{upsilon}
@row \theta,\ \Theta & \text{thêta} & \varphi,\ \phi,\ \Phi & \text{phi}
@row \iota & \text{iota} & \chi & \text{khi}
@row \kappa & \text{kappa} & \psi,\ \Psi & \text{psi}
@row \lambda,\ \Lambda & \text{lambda} & \omega,\ \Omega & \text{oméga}
@row \mu & \text{mu} & &
@card Abréviations
@tags abreviation ssi cqfd tq ie resp qed iff
@li ssi : si et seulement si ($\iff$) ; tq : tel que ; resp. : respectivement.
@li i.e. : c'est-à-dire ; cf. : se reporter à.
@li cqfd : ce qu'il fallait démontrer ; fin de preuve : $\blacksquare$ ou $\square$.
@li $\therefore$ : donc ; $\because$ : car (surtout en anglais).
@note En anglais : iff (if and only if), s.t. (such that), w.r.t. (with respect to), QED, WLOG (sans perte de généralité).
)TEX";
