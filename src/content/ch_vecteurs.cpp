// Chapitre : vecteurs (produit scalaire, vectoriel, mixte).
extern const char CH_VECTEURS[];
const char CH_VECTEURS[] = R"TEX(
@page Produit scalaire
@card Définitions
@tags scalaire norme dot product
@math \vec{u}\cdot\vec{v}=\|\vec{u}\|\,\|\vec{v}\|\cos\left(\vec{u},\vec{v}\right)
@math \vec{u}\cdot\vec{v}=u_xv_x+u_yv_y+u_zv_z
@math \|\vec{u}\|=\sqrt{\vec{u}\cdot\vec{u}}=\sqrt{u_x^2+u_y^2+u_z^2}
@note En base orthonormée.
@note Notation anglo-saxonne : $\mathbf{u}\cdot\mathbf{v}$ (« dot product »). Autres écritures : $(\vec{u}\,|\,\vec{v})$, $\langle\vec{u},\vec{v}\rangle$.
@card Propriétés
@tags orthogonal cauchy schwarz
@math \vec{u}\perp\vec{v}\iff\vec{u}\cdot\vec{v}=0
@math \|\vec{u}+\vec{v}\|^2=\|\vec{u}\|^2+2\,\vec{u}\cdot\vec{v}+\|\vec{v}\|^2
@math \left|\vec{u}\cdot\vec{v}\right|\leq\|\vec{u}\|\,\|\vec{v}\|
@math \vec{u}\cdot\vec{v}=\frac{1}{4}\left(\|\vec{u}+\vec{v}\|^2-\|\vec{u}-\vec{v}\|^2\right)
@card Angle et projection
@tags projection angle
@math \cos\left(\vec{u},\vec{v}\right)=\frac{\vec{u}\cdot\vec{v}}{\|\vec{u}\|\,\|\vec{v}\|}
@math \vec{u}_{\parallel}=\frac{\vec{u}\cdot\vec{v}}{\|\vec{v}\|^2}\,\vec{v}\qquad\vec{u}_{\perp}=\vec{u}-\vec{u}_{\parallel}
@card Al-Kashi
@tags triangle kashi pythagore
@math a^2=b^2+c^2-2bc\cos\hat{A}
@math \frac{a}{\sin\hat{A}}=\frac{b}{\sin\hat{B}}=\frac{c}{\sin\hat{C}}

@page Produit vectoriel
@card Composantes
@tags vectoriel wedge cross product
@math \vec{u}\wedge\vec{v}=\begin{pmatrix}u_x\\u_y\\u_z\end{pmatrix}\wedge\begin{pmatrix}v_x\\v_y\\v_z\end{pmatrix}=\begin{pmatrix}u_yv_z-u_zv_y\\u_zv_x-u_xv_z\\u_xv_y-u_yv_x\end{pmatrix}
@math \vec{u}\wedge\vec{v}=\begin{vmatrix}\vec{e}_x&\vec{e}_y&\vec{e}_z\\u_x&u_y&u_z\\v_x&v_y&v_z\end{vmatrix}
@note Base orthonormée directe. Déterminant « formel » développé selon la 1re ligne.
@note Notation anglo-saxonne : $\mathbf{u}\times\mathbf{v}$ (« cross product »), aussi courante en physique en France.
@card Norme et sens
@tags aire parallelogramme
@math \|\vec{u}\wedge\vec{v}\|=\|\vec{u}\|\,\|\vec{v}\|\,\left|\sin\left(\vec{u},\vec{v}\right)\right|
@text $\vec{u}\wedge\vec{v}$ est orthogonal à $\vec{u}$ et à $\vec{v}$, et $\left(\vec{u},\vec{v},\vec{u}\wedge\vec{v}\right)$ est direct. Sa norme est l'aire du parallélogramme.
@card Propriétés
@tags antisymetrie colineaire
@math \vec{v}\wedge\vec{u}=-\,\vec{u}\wedge\vec{v}\qquad\vec{u}\wedge\vec{u}=\vec{0}
@math \vec{u}\ \text{et}\ \vec{v}\ \text{colinéaires}\iff\vec{u}\wedge\vec{v}=\vec{0}
@math \vec{e}_x\wedge\vec{e}_y=\vec{e}_z\quad\vec{e}_y\wedge\vec{e}_z=\vec{e}_x\quad\vec{e}_z\wedge\vec{e}_x=\vec{e}_y
@card Double produit vectoriel
@tags lagrange identite
@math \vec{a}\wedge\left(\vec{b}\wedge\vec{c}\right)=(\vec{a}\cdot\vec{c})\,\vec{b}-(\vec{a}\cdot\vec{b})\,\vec{c}
@math \|\vec{u}\wedge\vec{v}\|^2+(\vec{u}\cdot\vec{v})^2=\|\vec{u}\|^2\,\|\vec{v}\|^2
@note Moyen mnémotechnique : « $\vec{b}$ fois ($\vec{a}\cdot\vec{c}$) moins $\vec{c}$ fois ($\vec{a}\cdot\vec{b}$) ».

@page Produit mixte, géométrie
@card Produit mixte
@tags mixte volume determinant
@math \left[\vec{u},\vec{v},\vec{w}\right]=\left(\vec{u}\wedge\vec{v}\right)\cdot\vec{w}=\det\left(\vec{u},\vec{v},\vec{w}\right)
@math \left[\vec{u},\vec{v},\vec{w}\right]=\left[\vec{v},\vec{w},\vec{u}\right]=-\left[\vec{v},\vec{u},\vec{w}\right]
@note $\left|\left[\vec{u},\vec{v},\vec{w}\right]\right|$ = volume du parallélépipède ; nul si les vecteurs sont coplanaires.
@note Notation anglo-saxonne : $\mathbf{u}\cdot(\mathbf{v}\times\mathbf{w})$ (« scalar triple product »).
@card Plans et droites
@tags plan droite normal distance
@math \mathcal{P}:\ ax+by+cz+d=0\qquad\vec{n}=(a,b,c)
@math d(M,\mathcal{P})=\frac{|ax_M+by_M+cz_M+d|}{\sqrt{a^2+b^2+c^2}}
@math d(M,\mathcal{D})=\frac{\left\|\overrightarrow{AM}\wedge\vec{u}\right\|}{\|\vec{u}\|}
@note $\mathcal{D}$ : droite passant par $A$, de vecteur directeur $\vec{u}$.
@card Aires et volumes
@tags triangle tetraedre
@math \mathcal{A}_{ABC}=\frac{1}{2}\left\|\overrightarrow{AB}\wedge\overrightarrow{AC}\right\|
@math V_{ABCD}=\frac{1}{6}\left|\left[\overrightarrow{AB},\overrightarrow{AC},\overrightarrow{AD}\right]\right|
@card Barycentre
@tags barycentre milieu
@math \overrightarrow{OG}=\frac{\sum\alpha_i\,\overrightarrow{OA_i}}{\sum\alpha_i}\qquad\sum\alpha_i\,\overrightarrow{GA_i}=\vec{0}
)TEX";
