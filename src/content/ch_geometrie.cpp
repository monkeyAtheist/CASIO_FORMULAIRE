// Chapitre : geometrie (triangles, aires et perimetres, volumes, barycentre,
// droites et coniques).
extern const char CH_GEOMETRIE[];
const char CH_GEOMETRIE[] = R"TEX(
@page Triangles
@card Angles, Pythagore
@tags triangle angles somme pythagore rectangle
@math \widehat{A}+\widehat{B}+\widehat{C}=\pi\quad(180°)
@math \text{Rectangle en }A\iff a^2=b^2+c^2
@note Notations : $a=BC$, $b=CA$, $c=AB$ (côté opposé à chaque sommet). Pythagore et sa réciproque.
@card Triangle rectangle
@tags cosinus sinus tangente soh cah toa hypotenuse
@math \cos\widehat{B}=\frac{AB}{BC}\qquad\sin\widehat{B}=\frac{AC}{BC}\qquad\tan\widehat{B}=\frac{AC}{AB}
@note Rectangle en $A$ : adjacent sur hypoténuse, opposé sur hypoténuse, opposé sur adjacent (« CAH SOH TOA »).
@card Al-Kashi, loi des sinus
@tags al kashi cosinus loi sinus circonscrit
@math a^2=b^2+c^2-2bc\cos\widehat{A}
@math \frac{a}{\sin\widehat{A}}=\frac{b}{\sin\widehat{B}}=\frac{c}{\sin\widehat{C}}=2R
@note $R$ : rayon du cercle circonscrit. Al-Kashi généralise Pythagore ($\widehat{A}=\frac{\pi}{2}$).
@card Aire d'un triangle
@tags aire heron surface
@math \mathcal{A}=\frac{\text{base}\times\text{hauteur}}{2}=\frac{1}{2}\,bc\sin\widehat{A}
@math \mathcal{A}=\sqrt{p(p-a)(p-b)(p-c)}\quad\text{(Héron)}
@math \mathcal{A}=\frac{abc}{4R}=r\,p
@note $p=\frac{a+b+c}{2}$ : demi-périmètre ; $R$, $r$ : rayons des cercles circonscrit et inscrit.
@card Points remarquables
@tags mediane hauteur mediatrice bissectrice centre gravite orthocentre euler
@li Médianes : centre de gravité $G$, aux $\frac{2}{3}$ de chaque médiane en partant du sommet.
@li Médiatrices : centre $O$ du cercle circonscrit (équidistant des sommets).
@li Bissectrices : centre $I$ du cercle inscrit (équidistant des côtés).
@li Hauteurs : orthocentre $H$.
@li $O$, $G$, $H$ alignés (droite d'Euler) : $\overrightarrow{OH}=3\,\overrightarrow{OG}$.
@card Thalès, similitude
@tags thales proportionnalite semblables agrandissement reduction
@math (MN)\parallel(BC)\implies\frac{AM}{AB}=\frac{AN}{AC}=\frac{MN}{BC}
@note $M\in(AB)$, $N\in(AC)$. Figures semblables de rapport $k$ : longueurs $\times k$, aires $\times k^2$, volumes $\times k^3$.
@card Triangles particuliers
@tags equilateral isocele rectangle demi carre
@li Équilatéral de côté $a$ : hauteur $\frac{\sqrt{3}}{2}\,a$, aire $\frac{\sqrt{3}}{4}\,a^2$, $R=\frac{a}{\sqrt{3}}$, $r=\frac{a}{2\sqrt{3}}$.
@li Rectangle : le centre du cercle circonscrit est le milieu de l'hypoténuse.
@li Demi-carré : côtés $1$, $1$, $\sqrt{2}$. Demi-équilatéral : $1$, $\sqrt{3}$, $2$ (angles $30°$, $60°$, $90°$).

@page Périmètres et aires
@card Figures usuelles
@tags perimetre aire carre rectangle parallelogramme losange trapeze disque
@table[ccc] \text{Figure} & \text{Périmètre} & \text{Aire}
@row \text{Carré} & 4a & a^2
@row \text{Rectangle} & 2(L+\ell) & L\,\ell
@row \text{Parallélogramme} & 2(a+b) & b\,h
@row \text{Losange} & 4a & \frac{D\,d}{2}
@row \text{Trapèze} & B+b+c+d & \frac{(B+b)\,h}{2}
@row \text{Triangle} & a+b+c & \frac{b\,h}{2}
@row \text{Disque} & 2\pi R & \pi R^2
@row \text{Ellipse} & \text{voir plus bas} & \pi\,a\,b
@note $h$ : hauteur ; $D$, $d$ : diagonales du losange ; $B$, $b$ : bases du trapèze, $c$, $d$ : ses deux autres côtés.
@card Cercle, arc, secteur
@tags cercle arc secteur angulaire radian corde couronne
@math \mathcal{P}=2\pi R=\pi D\qquad\mathcal{A}=\pi R^2=\frac{\pi D^2}{4}
@math \text{Arc : }L=R\,\theta\qquad\text{Secteur : }\mathcal{A}=\frac{1}{2}\,R^2\,\theta
@math \text{Corde}=2R\sin\frac{\theta}{2}\qquad\text{Couronne : }\mathcal{A}=\pi\left(R^2-r^2\right)
@note $\theta$ en radians ($\pi\ \text{rad}=180°$). En degrés : $L=\frac{\pi R\alpha}{180}$ et $\mathcal{A}=\frac{\pi R^2\alpha}{360}$.
@card Ellipse (« ovale »)
@tags ellipse ovale perimetre ramanujan aire
@math \frac{x^2}{a^2}+\frac{y^2}{b^2}=1\qquad\mathcal{A}=\pi\,a\,b
@math \mathcal{P}\approx\pi\left[3(a+b)-\sqrt{(3a+b)(a+3b)}\right]
@math \mathcal{P}=4a\int_0^{\pi/2}\sqrt{1-e^2\sin^2t}\,dt\qquad e=\sqrt{1-\frac{b^2}{a^2}}
@note $a\geq b$ : demi-axes. Pas de formule simple pour le périmètre ; celle de Ramanujan est exacte pour un cercle ; son erreur reste sous $0{,}5$ % (sous $0{,}03$ % si $b\geq\frac{a}{5}$). Un « ovale » quelconque n'a pas de formule : on l'assimile à une ellipse.
@card Polygones réguliers
@tags polygone regulier hexagone pentagone angles
@math \text{Somme des angles}=(n-2)\,\pi\qquad c=2R\sin\frac{\pi}{n}
@math \mathcal{A}=\frac{n}{2}\,R^2\sin\frac{2\pi}{n}
@note $n$ côtés de longueur $c$, inscrit dans un cercle de rayon $R$. Hexagone : $c=R$ et $\mathcal{A}=\frac{3\sqrt{3}}{2}\,c^2$.
@card Angles inscrits
@tags angle inscrit centre arc cocyclique
@li Un angle inscrit vaut la moitié de l'angle au centre qui intercepte le même arc.
@li Deux angles inscrits qui interceptent le même arc sont égaux.
@li Un triangle inscrit dans un cercle, dont un côté est un diamètre, est rectangle.

@page Volumes et surfaces
@card Solides usuels
@tags volume aire cube pave cylindre cone sphere boule pyramide prisme
@table[ccc] \text{Solide} & \text{Aire totale} & \text{Volume}
@row \text{Cube} & 6a^2 & a^3
@row \text{Pavé droit} & 2(ab+bc+ca) & abc
@row \text{Prisme droit} & 2\mathcal{B}+\mathcal{P}h & \mathcal{B}\,h
@row \text{Cylindre} & 2\pi R(R+h) & \pi R^2h
@row \text{Pyramide} & \mathcal{B}+\text{faces} & \frac{1}{3}\,\mathcal{B}\,h
@row \text{Cône} & \pi R(R+g) & \frac{1}{3}\,\pi R^2h
@row \text{Sphère, boule} & 4\pi R^2 & \frac{4}{3}\,\pi R^3
@note $\mathcal{B}$ : aire de la base ; $\mathcal{P}$ : périmètre de la base ; $h$ : hauteur ; $g=\sqrt{R^2+h^2}$ : génératrice du cône. Aires latérales : cylindre $2\pi Rh$, cône $\pi Rg$.
@card Autres solides
@tags tronc cone calotte tore ellipsoide tetraedre
@table[ccc] \text{Solide} & \text{Aire} & \text{Volume}
@row \text{Calotte} & 2\pi Rh & \frac{\pi h^2}{3}\,(3R-h)
@row \text{Tore} & 4\pi^2Rr & 2\pi^2Rr^2
@row \text{Ellipsoïde} & \text{—} & \frac{4}{3}\,\pi\,abc
@row \text{Tétraèdre} & \sqrt{3}\,a^2 & \frac{\sqrt{2}}{12}\,a^3
@note Calotte sphérique de hauteur $h$ sur une sphère de rayon $R$ (sans le disque de base). Tétraèdre régulier d'arête $a$. Tore : $R$ = distance du centre au centre du tube, $r$ = rayon du tube.
@text Tronc de cône (rayons $R$ et $r$, hauteur $h$) :
@math V=\frac{\pi h}{3}\left(R^2+Rr+r^2\right)\qquad S_{\text{lat}}=\pi(R+r)\,g
@note $g=\sqrt{h^2+(R-r)^2}$ ; $r=0$ redonne le cône.
@card Théorèmes de Guldin
@tags guldin pappus revolution
@math S=2\pi\,r_G\,L\qquad V=2\pi\,r_G\,\mathcal{A}
@note Surface (resp. volume) engendrée par la rotation d'une courbe de longueur $L$ (resp. d'une surface d'aire $\mathcal{A}$) autour d'un axe qui ne la traverse pas ; $r_G$ : distance de son centre de gravité à l'axe. Tore : $V=2\pi R\times\pi r^2$.
@card Solides de révolution
@tags revolution volume longueur arc integrale
@math V=\pi\int_a^b f(x)^2\,dx\qquad S=2\pi\int_a^b f(x)\sqrt{1+f'(x)^2}\,dx
@math L=\int_a^b\sqrt{1+f'(x)^2}\,dx
@note Rotation de la courbe $y=f(x)\geq 0$ autour de l'axe $(Ox)$ ; $L$ : longueur de la courbe.

@page Barycentre
@card Définition
@tags barycentre poids pondere moyenne
@math \sum_{i=1}^{n}\alpha_i\,\overrightarrow{GA_i}=\vec{0}\qquad\left(\sum_{i=1}^{n}\alpha_i\neq 0\right)
@math \overrightarrow{OG}=\frac{1}{\sum\alpha_i}\sum_{i=1}^{n}\alpha_i\,\overrightarrow{OA_i}
@math x_G=\frac{\sum\alpha_i\,x_i}{\sum\alpha_i}\qquad y_G=\frac{\sum\alpha_i\,y_i}{\sum\alpha_i}
@note $G$ : barycentre des points pondérés $(A_i,\alpha_i)$ ; il existe et il est unique si $\sum\alpha_i\neq 0$. $O$ : point quelconque.
@card Deux points
@tags segment milieu
@math \alpha\,\overrightarrow{GA}+\beta\,\overrightarrow{GB}=\vec{0}\iff\overrightarrow{AG}=\frac{\beta}{\alpha+\beta}\,\overrightarrow{AB}
@note $G\in(AB)$ ; $G\in[AB]$ si $\alpha$ et $\beta$ ont le même signe, plus près du point de plus grand poids. Milieu : $\alpha=\beta$.
@card Propriétés
@tags homogeneite associativite isobarycentre reduction
@li Homogénéité : multiplier tous les poids par $k\neq 0$ ne change pas $G$.
@li Associativité : on peut remplacer des points par leur barycentre partiel, affecté de la somme de leurs poids.
@li Isobarycentre (poids égaux) : milieu de deux points, centre de gravité d'un triangle.
@math \sum\alpha_i\,\overrightarrow{MA_i}=\left(\sum\alpha_i\right)\overrightarrow{MG}\quad\text{pour tout }M
@card Centre de gravité d'un triangle
@tags isobarycentre mediane triangle
@math x_G=\frac{x_A+x_B+x_C}{3}\qquad y_G=\frac{y_A+y_B+y_C}{3}
@math \overrightarrow{GA}+\overrightarrow{GB}+\overrightarrow{GC}=\vec{0}
@note Intersection des médianes, aux $\frac{2}{3}$ de chacune en partant du sommet.
@card Relation de Leibniz
@tags leibniz huygens inertie distance carre
@math \sum\alpha_i\,MA_i^2=\left(\sum\alpha_i\right)MG^2+\sum\alpha_i\,GA_i^2
@note Pour tout point $M$. En mécanique, c'est le théorème de Huygens : $J_{\Delta}=J_G+m\,d^2$.
@card Centres de masse usuels
@tags centre masse gravite solide plaque demi disque cone
@math \overrightarrow{OG}=\frac{1}{m}\int\overrightarrow{OM}\,dm
@table[cc] \text{Corps homogène} & \text{Distance de }G
@row \text{Demi-disque} & \frac{4R}{3\pi}\ \text{du diamètre}
@row \text{Demi-cercle (fil)} & \frac{2R}{\pi}\ \text{du diamètre}
@row \text{Triangle (plaque)} & \frac{h}{3}\ \text{de la base}
@row \text{Cône plein} & \frac{h}{4}\ \text{de la base}
@row \text{Demi-boule} & \frac{3R}{8}\ \text{du plan de coupe}
@row \text{Demi-sphère (coque)} & \frac{R}{2}\ \text{du plan de coupe}

@page Droites et coniques
@card Droites du plan
@tags droite equation pente coefficient directeur parallele perpendiculaire
@math y=mx+p\qquad m=\frac{y_B-y_A}{x_B-x_A}
@math ax+by+c=0\qquad\vec{u}=(-b,a)\qquad\vec{n}=(a,b)
@math d(M,\mathcal{D})=\frac{\left|ax_M+by_M+c\right|}{\sqrt{a^2+b^2}}
@note Parallèles : même pente $m=m'$ ; perpendiculaires : $m\,m'=-1$. $\vec{u}$ : vecteur directeur ; $\vec{n}$ : vecteur normal.
@card Distance, milieu
@tags distance milieu longueur
@math AB=\sqrt{\left(x_B-x_A\right)^2+\left(y_B-y_A\right)^2}
@math I=\left(\frac{x_A+x_B}{2},\frac{y_A+y_B}{2}\right)
@card Cercle
@tags cercle equation parametrique
@math (x-x_{\Omega})^2+(y-y_{\Omega})^2=R^2
@math x=x_{\Omega}+R\cos t\qquad y=y_{\Omega}+R\sin t
@note Centre $\Omega$, rayon $R$. $M$ est sur le cercle de diamètre $[AB]$ si et seulement si $\overrightarrow{MA}\cdot\overrightarrow{MB}=0$.
@card Ellipse
@tags ellipse foyer excentricite
@math \frac{x^2}{a^2}+\frac{y^2}{b^2}=1\qquad c^2=a^2-b^2\qquad e=\frac{c}{a}<1
@math x=a\cos t\qquad y=b\sin t\qquad MF+MF'=2a
@note $a\geq b>0$ ; foyers $F(c,0)$ et $F'(-c,0)$.
@card Hyperbole
@tags hyperbole asymptote foyer
@math \frac{x^2}{a^2}-\frac{y^2}{b^2}=1\qquad c^2=a^2+b^2\qquad e=\frac{c}{a}>1
@math \left|MF-MF'\right|=2a\qquad\text{asymptotes : }y=\pm\frac{b}{a}\,x
@card Parabole
@tags parabole foyer directrice
@math y^2=2px\qquad F\left(\frac{p}{2},0\right)\qquad\text{directrice : }x=-\frac{p}{2}
@note Chaque point est à égale distance du foyer et de la directrice ($e=1$). Pour $y=ax^2$ : foyer $\left(0,\frac{1}{4a}\right)$.
@card Coniques en polaires
@tags polaire foyer kepler orbite conique
@math r=\frac{p}{1+e\cos\theta}
@note Origine en un foyer. $e=0$ : cercle ; $0<e<1$ : ellipse ; $e=1$ : parabole ; $e>1$ : hyperbole (orbites de Kepler).
)TEX";
