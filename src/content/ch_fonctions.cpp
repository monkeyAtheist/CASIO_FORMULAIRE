// Chapitre : exponentielle, logarithme, puissances, algebre.
extern const char CH_FONCTIONS[];
const char CH_FONCTIONS[] = R"TEX(
@page Exponentielle
@card Définition par une série
@tags exp serie somme factorielle definition
@math e^x=\exp(x)=\sum_{k=0}^{+\infty}\frac{x^k}{k!}=1+x+\frac{x^2}{2!}+\frac{x^3}{3!}+\cdots
@math e=\sum_{k=0}^{+\infty}\frac{1}{k!}\approx 2{,}718\,28\qquad e^x=\lim_{n\to+\infty}\left(1+\frac{x}{n}\right)^n
@note Converge pour tout $x$ réel (ou complexe). $\exp$ est l'unique fonction telle que $f'=f$ et $f(0)=1$.
@card Propriétés
@tags exp proprietes
@math e^{a+b}=e^a\,e^b\qquad e^{a-b}=\frac{e^a}{e^b}\qquad e^{-a}=\frac{1}{e^a}
@math \left(e^a\right)^n=e^{na}\qquad e^0=1\qquad e^x>0
@math \left(e^x\right)'=e^x\qquad\left(e^{u}\right)'=u'\,e^{u}
@card Limites
@tags limite exp
@math \lim_{x\to-\infty}e^x=0\qquad\lim_{x\to+\infty}e^x=+\infty
@math \lim_{x\to+\infty}\frac{e^x}{x^n}=+\infty\qquad\lim_{x\to-\infty}x^n\,e^x=0
@card Puissances réelles
@tags puissance exposant reel
@math a^b=e^{b\ln a}\ \ (a>0)
@math a^{b+c}=a^b\,a^c\qquad\left(a^b\right)^c=a^{bc}\qquad a^{-b}=\frac{1}{a^b}
@math (ab)^c=a^c\,b^c\qquad a^{\frac{1}{n}}=\sqrt[n]{a}\qquad a^{\frac{p}{q}}=\sqrt[q]{a^p}

@page Logarithmes
@card Logarithme népérien
@tags ln neperien proprietes
@math \ln x=\int_1^x\frac{dt}{t}\qquad\left(\ln x\right)'=\frac{1}{x}\quad(x>0)
@math y=\ln x\iff x=e^y\qquad\ln 1=0\qquad\ln e=1
@note $\ln$ est la réciproque de $\exp$ : $e^{\ln x}=x$ ($x>0$) et $\ln\left(e^x\right)=x$.
@card Propriétés de ln
@tags ln produit quotient puissance
@math \ln(ab)=\ln a+\ln b\qquad\ln\frac{a}{b}=\ln a-\ln b
@math \ln\frac{1}{a}=-\ln a\qquad\ln\left(a^r\right)=r\ln a\qquad\ln\sqrt{a}=\frac{1}{2}\ln a
@note Pour $a,b>0$. Avec des valeurs négatives : $\ln|ab|=\ln|a|+\ln|b|$.
@card Limites de ln
@tags limite ln
@math \lim_{x\to 0^+}\ln x=-\infty\qquad\lim_{x\to+\infty}\ln x=+\infty
@math \lim_{x\to+\infty}\frac{\ln x}{x}=0\qquad\lim_{x\to 0^+}x\ln x=0\qquad\lim_{x\to 0}\frac{\ln(1+x)}{x}=1
@card Logarithme en base a
@tags log base decimal binaire log10 log2
@math \log_a x=\frac{\ln x}{\ln a}\qquad(a>0,\ a\neq 1)
@math y=\log_a x\iff x=a^y\qquad\log_a a=1\qquad\log_a 1=0
@math \log_a(xy)=\log_a x+\log_a y\qquad\log_a\left(x^r\right)=r\log_a x
@math a^{\log_a x}=x\qquad\log_a\left(a^x\right)=x\qquad\left(\log_a x\right)'=\frac{1}{x\ln a}
@card Changement de base, bases usuelles
@tags changement base decibel
@math \log_b x=\frac{\log_a x}{\log_a b}\qquad\log_a b=\frac{1}{\log_b a}
@math \log x=\log_{10}x\qquad\log\left(10^n\right)=n\qquad\log 2\approx 0{,}301
@math \log_2 x=\frac{\ln x}{\ln 2}\qquad\ln 2\approx 0{,}693\qquad\ln 10\approx 2{,}303
@note Décibels : $G_{\text{dB}}=20\log|H|$ (gain en amplitude), $10\log\frac{P_2}{P_1}$ (puissances).
@note En anglais, $\log x$ désigne souvent $\ln x$ (mathématiques) ; $\lg x=\log_{10}x$ (norme ISO), mais $\log_2x$ en informatique.

@page Factorielle, coefficients binomiaux
@card Factorielle
@tags factorielle
@math n!=1\times 2\times 3\times\cdots\times n=\prod_{k=1}^{n}k\qquad 0!=1
@math (n+1)!=(n+1)\times n!\qquad\frac{n!}{k!}=(k+1)(k+2)\cdots n
@math n!\sim\sqrt{2\pi n}\left(\frac{n}{e}\right)^n\quad\text{(Stirling)}
@note $1!=1$, $2!=2$, $3!=6$, $4!=24$, $5!=120$, $6!=720$, $10!=3\,628\,800$.
@card Coefficients binomiaux
@tags binomial combinaison k parmi n
@math \binom{n}{k}=\frac{n!}{k!\,(n-k)!}=\frac{n(n-1)\cdots(n-k+1)}{k!}
@note « $k$ parmi $n$ » : nombre de façons de choisir $k$ éléments parmi $n$ (sans ordre). Ancienne notation : $C_n^k$. Vaut 0 si $k>n$.
@note En anglais : $C(n,k)$ ou ${}_nC_k$. Calculatrice : nCr.
@card Propriétés
@tags pascal symetrie
@math \binom{n}{0}=\binom{n}{n}=1\qquad\binom{n}{1}=n\qquad\binom{n}{2}=\frac{n(n-1)}{2}
@math \binom{n}{k}=\binom{n}{n-k}\qquad\binom{n}{k}+\binom{n}{k+1}=\binom{n+1}{k+1}
@math k\binom{n}{k}=n\binom{n-1}{k-1}\qquad\sum_{k=0}^{n}\binom{n}{k}=2^n
@card Triangle de Pascal
@tags pascal triangle
@table[cccccc] n\backslash k & 0 & 1 & 2 & 3 & 4
@row 0 & 1 & & & &
@row 1 & 1 & 1 & & &
@row 2 & 1 & 2 & 1 & &
@row 3 & 1 & 3 & 3 & 1 &
@row 4 & 1 & 4 & 6 & 4 & 1
@row 5 & 1 & 5 & 10 & 10 & 5
@note Chaque terme est la somme des deux termes au-dessus (ligne précédente, colonnes $k-1$ et $k$).
@card Binôme de Newton
@tags binome newton
@math (a+b)^n=\sum_{k=0}^{n}\binom{n}{k}\,a^k\,b^{n-k}
@math (1+x)^n=1+nx+\frac{n(n-1)}{2}x^2+\cdots+x^n
@card Dénombrement
@tags arrangement permutation liste
@li $p$-listes (ordre, avec répétition) : $n^p$.
@li Arrangements (ordre, sans répétition) : $A_n^p=\frac{n!}{(n-p)!}$.
@li Permutations de $n$ éléments : $n!$.
@li Combinaisons (sans ordre) : $\binom{n}{p}$.

@page Second degré
@card Racines
@tags discriminant delta trinome
@math ax^2+bx+c=0\qquad\Delta=b^2-4ac
@math \Delta>0:\ x_{1,2}=\frac{-b\pm\sqrt{\Delta}}{2a}
@math \Delta=0:\ x_0=-\frac{b}{2a}
@math \Delta<0:\ x_{1,2}=\frac{-b\pm i\sqrt{-\Delta}}{2a}
@card Somme et produit des racines
@tags vieta
@math x_1+x_2=-\frac{b}{a}\qquad x_1\,x_2=\frac{c}{a}
@math ax^2+bx+c=a(x-x_1)(x-x_2)
@card Forme canonique
@tags canonique sommet
@math ax^2+bx+c=a\left[\left(x+\frac{b}{2a}\right)^2-\frac{\Delta}{4a^2}\right]
@note Sommet de la parabole : $\left(-\frac{b}{2a},-\frac{\Delta}{4a}\right)$.

@page Algèbre
@card Identités remarquables
@tags identite developper factoriser
@math (a\pm b)^2=a^2\pm 2ab+b^2
@math (a-b)(a+b)=a^2-b^2
@math (a+b)^3=a^3+3a^2b+3ab^2+b^3
@math a^3-b^3=(a-b)\left(a^2+ab+b^2\right)
@math a^3+b^3=(a+b)\left(a^2-ab+b^2\right)
@card Fractions
@tags fraction quotient denominateur
@math \frac{a}{b}+\frac{c}{d}=\frac{ad+bc}{bd}\qquad\frac{a}{b}\times\frac{c}{d}=\frac{ac}{bd}
@math \frac{\;\frac{a}{b}\;}{\;\frac{c}{d}\;}=\frac{a}{b}\times\frac{d}{c}=\frac{ad}{bc}
@note $b,c,d\neq 0$. Diviser par une fraction revient à multiplier par son inverse.
@card Partie entière
@tags floor entier
@math \lfloor x\rfloor\leq x<\lfloor x\rfloor+1\qquad x-1<\lfloor x\rfloor\leq x
@note Aussi notée $E(x)$ ; en anglais « floor ». $\lceil x\rceil$ : partie entière supérieure (« ceiling »).

@page Inégalités
@card Opérations qui conservent le sens
@tags inegalite ajouter multiplier positif
@math a\leq b\implies a+c\leq b+c\quad\text{et}\quad a-c\leq b-c
@math a\leq b\ \text{et}\ k>0\implies ka\leq kb\ \text{et}\ \frac{a}{k}\leq\frac{b}{k}
@note On ajoute ou retranche le même nombre, on multiplie ou divise par un nombre strictement positif.
@card Opérations qui changent le sens
@tags inegalite negatif oppose inverse changement sens
@math a\leq b\implies -a\geq -b
@math a\leq b\ \text{et}\ k<0\implies ka\geq kb\ \text{et}\ \frac{a}{k}\geq\frac{b}{k}
@math a\leq b\ \text{et}\ ab>0\implies\frac{1}{a}\geq\frac{1}{b}
@note Multiplier ou diviser par un négatif change le sens : $-2x<6\iff x>-3$. Passer à l'inverse change le sens si $a$ et $b$ ont le même signe ; si $a<0<b$, alors $\frac{1}{a}<0<\frac{1}{b}$.
@card Carrés, racines, fonctions monotones
@tags carre racine croissante decroissante fonction
@math 0\leq a\leq b\implies a^2\leq b^2\ \text{et}\ \sqrt{a}\leq\sqrt{b}
@math a\leq b\leq 0\implies a^2\geq b^2
@li $f$ croissante : $a\leq b\implies f(a)\leq f(b)$. Sens conservé : $\exp$, $\ln$, $\sqrt{\ }$, $x^3$, $\arctan$…
@li $f$ décroissante : $a\leq b\implies f(a)\geq f(b)$. Sens changé : $\frac{1}{x}$ sur $]0,+\infty[$, $e^{-x}$, $q^x$ et $\log_q$ si $0<q<1$…
@card Sommes et produits d'inégalités
@tags somme produit membre a membre
@math a\leq b\ \text{et}\ c\leq d\implies a+c\leq b+d
@math 0\leq a\leq b\ \text{et}\ 0\leq c\leq d\implies ac\leq bd
@math a\leq b\ \text{et}\ c\leq d\implies a-d\leq b-c
@note On ne soustrait pas et on ne divise pas deux inégalités membre à membre. Quotient : $0<a\leq b$ et $0<c\leq d$ donnent $\frac{a}{d}\leq\frac{b}{c}$.
@card Inéquations et tableau de signes
@tags signe produit quotient inequation tableau
@li Tout ramener à « $\ldots\geq 0$ », factoriser, puis faire un tableau de signes.
@li Ne jamais multiplier par une expression de signe inconnu (comme $x$ dans $\frac{1}{x}<2$).
@li $ax+b$ est du signe de $a$ après sa racine $-\frac{b}{a}$ ; $ax^2+bx+c$ est du signe de $a$, sauf entre ses racines.
@math \frac{1}{x}<2\iff\frac{1-2x}{x}<0\iff x<0\ \text{ou}\ x>\frac{1}{2}
@card Valeur absolue
@tags absolue triangulaire distance
@math |ab|=|a|\,|b|\qquad|a+b|\leq|a|+|b|\qquad\big||a|-|b|\big|\leq|a-b|
@math |x|\leq r\iff -r\leq x\leq r\qquad|x-a|\leq r\iff a-r\leq x\leq a+r
@math |x|\geq r\iff x\leq -r\ \text{ou}\ x\geq r
@card Inégalités classiques
@tags moyenne arithmetique geometrique cauchy schwarz bernoulli
@math \sqrt{ab}\leq\frac{a+b}{2}\quad(a,b\geq 0)\qquad ab\leq\frac{a^2+b^2}{2}
@math \left(\sum a_kb_k\right)^2\leq\sum a_k^2\,\sum b_k^2\quad\text{(Cauchy-Schwarz)}
@math (1+x)^n\geq 1+nx\quad(x\geq -1)\quad\text{(Bernoulli)}
@card Inégalités de convexité
@tags convexite inegalite
@math e^x\geq 1+x\qquad\ln(1+x)\leq x\quad(x>-1)
@math |\sin x|\leq|x|\qquad\sin x\leq x\quad(x\geq 0)\qquad\frac{2}{\pi}\,x\leq\sin x\ \ \text{sur}\ \left[0,\frac{\pi}{2}\right]
)TEX";
