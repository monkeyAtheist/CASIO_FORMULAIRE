// Chapitre : nombres complexes.
extern const char CH_COMPLEXES[];
const char CH_COMPLEXES[] = R"TEX(
@page Formes d'un complexe
@card Forme algébrique
@tags conjugue module partie reelle imaginaire
@math z=a+ib\qquad a=\Re(z),\ b=\Im(z)
@math i^2=-1
@math \bar{z}=a-ib\qquad|z|=\sqrt{a^2+b^2}=\sqrt{z\,\bar{z}}
@math \Re(z)=\frac{z+\bar{z}}{2}\qquad\Im(z)=\frac{z-\bar{z}}{2i}
@note En électricité, l'unité imaginaire se note $j$. Notation anglo-saxonne du conjugué : $z^{\ast}$.
@card Forme exponentielle
@tags module argument trigonometrique
@math z=r\,e^{i\theta}=r\,(\cos\theta+i\sin\theta)
@math r=|z|\qquad\theta=\arg z\ [2\pi]
@math \cos\theta=\frac{a}{r}\qquad\sin\theta=\frac{b}{r}
@note Si $a>0$ : $\theta=\arctan\frac{b}{a}$ ; si $a<0$ : $\theta=\arctan\frac{b}{a}+\pi$.
@card Calculs
@tags produit quotient inverse
@math z\,z'=r\,r'\,e^{i(\theta+\theta')}\qquad\frac{z}{z'}=\frac{r}{r'}\,e^{i(\theta-\theta')}
@math \overline{z+z'}=\bar{z}+\bar{z}'\qquad\overline{z\,z'}=\bar{z}\,\bar{z}'\qquad|z\,z'|=|z|\,|z'|
@math \frac{1}{z}=\frac{\bar{z}}{|z|^2}\qquad\frac{1}{a+ib}=\frac{a-ib}{a^2+b^2}
@math \big||z|-|z'|\big|\leq|z+z'|\leq|z|+|z'|

@page Euler, Moivre, racines
@card Formules d'Euler
@tags euler exponentielle
@math e^{i\theta}=\cos\theta+i\sin\theta
@math \cos\theta=\frac{e^{i\theta}+e^{-i\theta}}{2}\qquad\sin\theta=\frac{e^{i\theta}-e^{-i\theta}}{2i}
@card Formule de Moivre
@tags moivre
@math (\cos\theta+i\sin\theta)^n=\cos n\theta+i\sin n\theta
@card Angle moitié
@tags factorisation
@math e^{ia}+e^{ib}=2\cos\frac{a-b}{2}\,e^{i\frac{a+b}{2}}
@math 1+e^{i\theta}=2\cos\frac{\theta}{2}\,e^{i\frac{\theta}{2}}
@math 1-e^{i\theta}=-2i\sin\frac{\theta}{2}\,e^{i\frac{\theta}{2}}
@card Racines n-ièmes
@tags racine unite
@math z^n=1\iff z=e^{\frac{2ik\pi}{n}},\ \ k\in\{0,\dots,n-1\}
@math z^n=r\,e^{i\theta}\iff z=r^{\frac{1}{n}}\,e^{i\frac{\theta+2k\pi}{n}}
@math \sum_{k=0}^{n-1}e^{\frac{2ik\pi}{n}}=0\quad(n\geq 2)
@card Géométrie
@tags distance angle rotation similitude
@math AB=|z_B-z_A|
@math \left(\overrightarrow{AB},\overrightarrow{AC}\right)=\arg\frac{z_C-z_A}{z_B-z_A}
@text Rotation de centre $\Omega(\omega)$ et d'angle $\theta$ :
@math z'-\omega=e^{i\theta}(z-\omega)
@math \text{A, B, C alignés}\iff\frac{z_C-z_A}{z_B-z_A}\in\mathbb{R}
)TEX";
