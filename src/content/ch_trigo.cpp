// Chapitre : trigonometrie.
extern const char CH_TRIGO[];
const char CH_TRIGO[] = R"TEX(
@page Bases
@card Relations fondamentales
@tags pythagore
@math \cos^2 x+\sin^2 x=1
@math \tan x=\frac{\sin x}{\cos x}\qquad1+\tan^2 x=\frac{1}{\cos^2 x}
@card Valeurs remarquables
@tags valeurs angles remarquables
@table[cccccc] x & 0 & \frac{\pi}{6} & \frac{\pi}{4} & \frac{\pi}{3} & \frac{\pi}{2}
@row \sin x & 0 & \frac{1}{2} & \frac{\sqrt{2}}{2} & \frac{\sqrt{3}}{2} & 1
@row \cos x & 1 & \frac{\sqrt{3}}{2} & \frac{\sqrt{2}}{2} & \frac{1}{2} & 0
@row \tan x & 0 & \frac{\sqrt{3}}{3} & 1 & \sqrt{3} & \text{—}
@card Angles associés
@tags associe symetrie
@table[ccc] \theta & \cos\theta & \sin\theta
@row -x & \cos x & -\sin x
@row \pi-x & -\cos x & \sin x
@row \pi+x & -\cos x & -\sin x
@row \frac{\pi}{2}-x & \sin x & \cos x
@row \frac{\pi}{2}+x & -\sin x & \cos x
@note Périodes : $2\pi$ pour $\cos$ et $\sin$, $\pi$ pour $\tan$.

@page Relations et conversions
@card Carrés et inverses
@tags tangente carre cosinus secante cotangente
@math 1+\tan^2 x=\frac{1}{\cos^2 x}\qquad 1+\cotan^2 x=\frac{1}{\sin^2 x}
@math \cos^2 x=\frac{1}{1+\tan^2 x}\qquad\sin^2 x=\frac{\tan^2 x}{1+\tan^2 x}
@math \cotan x=\frac{1}{\tan x}=\frac{\cos x}{\sin x}
@note Notations anglo-saxonnes : $\sec x=\frac{1}{\cos x}$, $\csc x=\frac{1}{\sin x}$, $\cot x=\cotan x$. Ainsi $1+\tan^2 x=\sec^2 x$.
@card Table de conversion
@tags conversion exprimer sin cos tan fonction
@text Ligne exprimée en fonction de la colonne :
@table[cccc] & s=\sin x & c=\cos x & t=\tan x
@row \sin x & s & \pm\sqrt{1-c^2} & \pm\frac{t}{\sqrt{1+t^2}}
@row \cos x & \pm\sqrt{1-s^2} & c & \pm\frac{1}{\sqrt{1+t^2}}
@row \tan x & \pm\frac{s}{\sqrt{1-s^2}} & \pm\frac{\sqrt{1-c^2}}{c} & t
@note Le signe $\pm$ dépend du quadrant où se trouve $x$.
@card Fonctions trigonométriques des réciproques
@tags arcsin arccos arctan composee reciproque
@table[cccc] & \arcsin x & \arccos x & \arctan x
@row \sin & x & \sqrt{1-x^2} & \frac{x}{\sqrt{1+x^2}}
@row \cos & \sqrt{1-x^2} & x & \frac{1}{\sqrt{1+x^2}}
@row \tan & \frac{x}{\sqrt{1-x^2}} & \frac{\sqrt{1-x^2}}{x} & x
@note Exemple : $\cos(\arctan x)=\frac{1}{\sqrt{1+x^2}}$. Pas de $\pm$ : les intervalles d'arrivée fixent les signes.
@card Angle moitié
@tags moitie demi angle
@math \cos^2\frac{x}{2}=\frac{1+\cos x}{2}\qquad\sin^2\frac{x}{2}=\frac{1-\cos x}{2}
@math \tan\frac{x}{2}=\frac{\sin x}{1+\cos x}=\frac{1-\cos x}{\sin x}
@math 1+\cos x=2\cos^2\frac{x}{2}\qquad 1-\cos x=2\sin^2\frac{x}{2}
@card Angle triple
@tags triple
@math \cos 3x=4\cos^3 x-3\cos x\qquad\sin 3x=3\sin x-4\sin^3 x
@math \tan 3x=\frac{3\tan x-\tan^3 x}{1-3\tan^2 x}
@card Dérivées et primitives liées
@tags derivee tangente primitive cotangente
@math (\tan x)'=1+\tan^2 x=\frac{1}{\cos^2 x}
@math (\cotan x)'=-1-\cotan^2 x=-\frac{1}{\sin^2 x}
@math \int\tan x\,dx=-\ln|\cos x|+C\qquad\int\frac{dx}{\cos^2 x}=\tan x+C
@math \int\tan^2 x\,dx=\tan x-x+C
@note Astuce : $\tan^2 x=\frac{1}{\cos^2 x}-1$.
@card Hyperboliques
@tags ch sh th hyperbolique osborn
@math \ch^2 x-\sh^2 x=1\qquad 1-\th^2 x=\frac{1}{\ch^2 x}
@math \ch^2 x=\frac{\ch 2x+1}{2}\qquad\sh^2 x=\frac{\ch 2x-1}{2}
@math \ch 2x=\ch^2 x+\sh^2 x\qquad\sh 2x=2\sh x\ch x
@note Règle d'Osborn : une formule trigonométrique devient hyperbolique en remplaçant $\cos$ par $\ch$, $\sin$ par $\sh$, et en changeant le signe de chaque produit de deux sinus.

@page Formules d'addition
@card Addition
@tags addition somme difference
@align \cos(a+b) &= \cos a\cos b-\sin a\sin b
@align \cos(a-b) &= \cos a\cos b+\sin a\sin b
@align \sin(a+b) &= \sin a\cos b+\cos a\sin b
@align \sin(a-b) &= \sin a\cos b-\cos a\sin b
@align \tan(a+b) &= \frac{\tan a+\tan b}{1-\tan a\tan b}
@card Duplication
@tags double angle
@align \cos 2a &= \cos^2 a-\sin^2 a
@align &= 2\cos^2 a-1=1-2\sin^2 a
@align \sin 2a &= 2\sin a\cos a
@align \tan 2a &= \frac{2\tan a}{1-\tan^2 a}
@card Linéarisation
@tags lineariser produit carre
@align \cos^2 a &= \frac{1+\cos 2a}{2}
@align \sin^2 a &= \frac{1-\cos 2a}{2}
@align \cos a\cos b &= \frac{1}{2}\big[\cos(a-b)+\cos(a+b)\big]
@align \sin a\sin b &= \frac{1}{2}\big[\cos(a-b)-\cos(a+b)\big]
@align \sin a\cos b &= \frac{1}{2}\big[\sin(a+b)+\sin(a-b)\big]
@card Somme en produit
@tags factorisation simpson
@align \cos p+\cos q &= 2\cos\frac{p+q}{2}\cos\frac{p-q}{2}
@align \cos p-\cos q &= -2\sin\frac{p+q}{2}\sin\frac{p-q}{2}
@align \sin p+\sin q &= 2\sin\frac{p+q}{2}\cos\frac{p-q}{2}
@align \sin p-\sin q &= 2\sin\frac{p-q}{2}\cos\frac{p+q}{2}
@card Avec t = tan(a/2)
@tags tangente moitie
@math \cos a=\frac{1-t^2}{1+t^2}\qquad\sin a=\frac{2t}{1+t^2}
@math \tan a=\frac{2t}{1-t^2}
@card a cos x + b sin x
@tags amplitude phase
@math a\cos x+b\sin x=R\cos(x-\varphi)
@math R=\sqrt{a^2+b^2},\quad\cos\varphi=\frac{a}{R},\quad\sin\varphi=\frac{b}{R}

@page Équations et réciproques
@card Équations trigonométriques
@tags equation resolution
@math \cos x=\cos a\iff x=\pm a+2k\pi
@math \sin x=\sin a\iff\begin{cases}x=a+2k\pi\\x=\pi-a+2k\pi\end{cases}
@math \tan x=\tan a\iff x=a+k\pi\qquad(k\in\mathbb{Z})
@card Fonctions réciproques
@tags arcsin arccos arctan asin acos atan
@table[ccc] f & \text{Départ} & \text{Arrivée}
@row \arcsin & [-1,1] & \left[-\frac{\pi}{2},\frac{\pi}{2}\right]
@row \arccos & [-1,1] & [0,\pi]
@row \arctan & \mathbb{R} & \left]-\frac{\pi}{2},\frac{\pi}{2}\right[
@math \arcsin x+\arccos x=\frac{\pi}{2}
@math \arctan x+\arctan\frac{1}{x}=\frac{\pi}{2}\ \ (x>0)
@math \cos(\arcsin x)=\sin(\arccos x)=\sqrt{1-x^2}
@note Notation anglo-saxonne et calculatrice : $\sin^{-1}x$, $\cos^{-1}x$, $\tan^{-1}x$ (SHIFT sin, cos, tan). À ne pas confondre avec $\frac{1}{\sin x}$.
@card Trigonométrie hyperbolique
@tags ch sh th cosh sinh tanh
@math \ch x=\frac{e^x+e^{-x}}{2}\qquad\sh x=\frac{e^x-e^{-x}}{2}
@math \ch^2 x-\sh^2 x=1\qquad\th x=\frac{\sh x}{\ch x}
@math \ch(a+b)=\ch a\ch b+\sh a\sh b
@math \sh(a+b)=\sh a\ch b+\ch a\sh b
@note Notation anglo-saxonne : $\sinh$, $\cosh$, $\tanh$.
@card Réciproques hyperboliques
@tags argsh argch argth
@math \argsh x=\ln\left(x+\sqrt{x^2+1}\right)
@math \argch x=\ln\left(x+\sqrt{x^2-1}\right)\quad(x\geq 1)
@math \argth x=\frac{1}{2}\ln\frac{1+x}{1-x}\quad(|x|<1)
@note Notation anglo-saxonne : $\sinh^{-1}$ ou $\operatorname{arsinh}$, etc.
)TEX";
