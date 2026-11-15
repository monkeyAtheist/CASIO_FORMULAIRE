// Chapitre : coordonnees et changements de repere.
extern const char CH_REPERES[];
const char CH_REPERES[] = R"TEX(
@page Polaires (plan)
@card Coordonnées polaires
@tags polaire
@math x=r\cos\theta\qquad y=r\sin\theta
@math r=\sqrt{x^2+y^2}\qquad\cos\theta=\frac{x}{r},\ \sin\theta=\frac{y}{r}
@card Base locale
@tags base mobile
@align \vec{u}_r &= \cos\theta\,\vec{e}_x+\sin\theta\,\vec{e}_y
@align \vec{u}_{\theta} &= -\sin\theta\,\vec{e}_x+\cos\theta\,\vec{e}_y
@math \frac{d\vec{u}_r}{d\theta}=\vec{u}_{\theta}\qquad\frac{d\vec{u}_{\theta}}{d\theta}=-\vec{u}_r
@card Cinématique
@tags vitesse acceleration
@math \overrightarrow{OM}=r\,\vec{u}_r
@math \vec{v}=\dot{r}\,\vec{u}_r+r\dot{\theta}\,\vec{u}_{\theta}
@math \vec{a}=\left(\ddot{r}-r\dot{\theta}^2\right)\vec{u}_r+\left(2\dot{r}\dot{\theta}+r\ddot{\theta}\right)\vec{u}_{\theta}
@card Éléments
@tags element longueur surface
@math d\overrightarrow{OM}=dr\,\vec{u}_r+r\,d\theta\,\vec{u}_{\theta}\qquad dS=r\,dr\,d\theta

@page Cylindriques
@card Coordonnées (r, θ, z)
@tags cylindrique
@math x=r\cos\theta\quad y=r\sin\theta\quad z=z
@math r\geq 0,\quad\theta\in[0,2\pi[
@card Base locale
@tags base
@align \vec{u}_r &= \cos\theta\,\vec{e}_x+\sin\theta\,\vec{e}_y
@align \vec{u}_{\theta} &= -\sin\theta\,\vec{e}_x+\cos\theta\,\vec{e}_y
@align \vec{u}_z &= \vec{e}_z
@card Éléments différentiels
@tags element volume surface
@math d\overrightarrow{OM}=dr\,\vec{u}_r+r\,d\theta\,\vec{u}_{\theta}+dz\,\vec{u}_z
@math d\tau=r\,dr\,d\theta\,dz
@math dS_r=r\,d\theta\,dz\qquad dS_{\theta}=dr\,dz
@math dS_z=r\,dr\,d\theta
@card Vitesse et accélération
@tags cinematique
@math \vec{v}=\dot{r}\,\vec{u}_r+r\dot{\theta}\,\vec{u}_{\theta}+\dot{z}\,\vec{u}_z
@math \vec{a}=\left(\ddot{r}-r\dot{\theta}^2\right)\vec{u}_r+\left(2\dot{r}\dot{\theta}+r\ddot{\theta}\right)\vec{u}_{\theta}+\ddot{z}\,\vec{u}_z

@page Sphériques
@card Coordonnées (r, θ, φ)
@tags spherique colatitude azimut
@math x=r\sin\theta\cos\varphi\qquad y=r\sin\theta\sin\varphi
@math z=r\cos\theta
@math r\geq 0,\quad\theta\in[0,\pi],\quad\varphi\in[0,2\pi[
@math r=\sqrt{x^2+y^2+z^2}\qquad\cos\theta=\frac{z}{r}
@note $\theta$ : angle avec $Oz$ (colatitude) ; $\varphi$ : azimut, angle dans le plan $xOy$.
@note Attention : beaucoup de livres de mathématiques américains notent $(\rho,\theta,\varphi)$ avec $\theta$ l'azimut et $\varphi$ la colatitude, rôles échangés.
@card Base locale
@tags base
@math \vec{u}_r=\begin{pmatrix}\sin\theta\cos\varphi\\\sin\theta\sin\varphi\\\cos\theta\end{pmatrix}\quad\vec{u}_{\theta}=\begin{pmatrix}\cos\theta\cos\varphi\\\cos\theta\sin\varphi\\-\sin\theta\end{pmatrix}
@math \vec{u}_{\varphi}=\begin{pmatrix}-\sin\varphi\\\cos\varphi\\0\end{pmatrix}
@note Composantes dans $\left(\vec{e}_x,\vec{e}_y,\vec{e}_z\right)$.
@card Éléments différentiels
@tags element volume surface
@math d\overrightarrow{OM}=dr\,\vec{u}_r+r\,d\theta\,\vec{u}_{\theta}+r\sin\theta\,d\varphi\,\vec{u}_{\varphi}
@math d\tau=r^2\sin\theta\,dr\,d\theta\,d\varphi
@math dS_r=r^2\sin\theta\,d\theta\,d\varphi
@card Vitesse
@tags cinematique
@math \vec{v}=\dot{r}\,\vec{u}_r+r\dot{\theta}\,\vec{u}_{\theta}+r\sin\theta\,\dot{\varphi}\,\vec{u}_{\varphi}

@page Changement de repère
@card Repère translaté et tourné (plan)
@tags translation rotation repere
@math \begin{pmatrix}x\\y\end{pmatrix}=\begin{pmatrix}x_0\\y_0\end{pmatrix}+\begin{pmatrix}\cos\alpha&-\sin\alpha\\\sin\alpha&\cos\alpha\end{pmatrix}\begin{pmatrix}x'\\y'\end{pmatrix}
@align x' &= (x-x_0)\cos\alpha+(y-y_0)\sin\alpha
@align y' &= -(x-x_0)\sin\alpha+(y-y_0)\cos\alpha
@note Nouveau repère d'origine $O'(x_0,y_0)$, axes tournés de $\alpha$.
@card Rotations dans l'espace
@tags rotation axe
@math R_z(\alpha)=\begin{pmatrix}\cos\alpha&-\sin\alpha&0\\\sin\alpha&\cos\alpha&0\\0&0&1\end{pmatrix}
@math R_x(\alpha)=\begin{pmatrix}1&0&0\\0&\cos\alpha&-\sin\alpha\\0&\sin\alpha&\cos\alpha\end{pmatrix}
@card Changement de base
@tags passage base orthonormee
@math X=P\,X'\qquad X'=P^{-1}X\qquad A'=P^{-1}AP
@note Colonnes de $P$ : nouveaux vecteurs de base dans l'ancienne base. Bases orthonormées : $P^{-1}=P^T$.
@card Changement de référentiel
@tags referentiel vitesse entrainement coriolis
@math \left(\frac{d\vec{A}}{dt}\right)_{\mathcal{R}}=\left(\frac{d\vec{A}}{dt}\right)_{\mathcal{R}'}+\vec{\Omega}\wedge\vec{A}
@math \vec{v}_{/\mathcal{R}}=\vec{v}_{/\mathcal{R}'}+\vec{v}_{O'}+\vec{\Omega}\wedge\overrightarrow{O'M}
@math \vec{a}_{/\mathcal{R}}=\vec{a}_{/\mathcal{R}'}+\vec{a}_e+2\,\vec{\Omega}\wedge\vec{v}_{/\mathcal{R}'}
@note $\vec{\Omega}$ : vecteur rotation de $\mathcal{R}'$ par rapport à $\mathcal{R}$ ; $2\,\vec{\Omega}\wedge\vec{v}$ : terme de Coriolis.

@page Jacobiens
@card Matrice jacobienne
@tags jacobienne jacobien differentielle
@math J_f=\left(\frac{\partial f_i}{\partial x_j}\right)=\begin{pmatrix}\dfrac{\partial f_1}{\partial x_1}&\cdots&\dfrac{\partial f_1}{\partial x_n}\\\vdots&&\vdots\\\dfrac{\partial f_m}{\partial x_1}&\cdots&\dfrac{\partial f_m}{\partial x_n}\end{pmatrix}
@math df=J_f\,dX\qquad J_{g\circ f}(X)=J_g\big(f(X)\big)\,J_f(X)
@note Ligne $i$ : gradient de $f_i$. Le jacobien est $\det J_f$ (si $m=n$).
@card Jacobienne des polaires
@tags jacobienne polaire
@math J=\begin{pmatrix}\dfrac{\partial x}{\partial r}&\dfrac{\partial x}{\partial\theta}\\[0pt]\dfrac{\partial y}{\partial r}&\dfrac{\partial y}{\partial\theta}\end{pmatrix}=\begin{pmatrix}\cos\theta&-r\sin\theta\\\sin\theta&r\cos\theta\end{pmatrix}
@math \det J=r\cos^2\theta+r\sin^2\theta=r
@card Jacobienne des cylindriques
@tags jacobienne cylindrique
@math J=\begin{pmatrix}\cos\theta&-r\sin\theta&0\\\sin\theta&r\cos\theta&0\\0&0&1\end{pmatrix}\qquad\det J=r
@card Jacobienne des sphériques
@tags jacobienne spherique
@math J=\begin{pmatrix}\sin\theta\cos\varphi&r\cos\theta\cos\varphi&-r\sin\theta\sin\varphi\\\sin\theta\sin\varphi&r\cos\theta\sin\varphi&r\sin\theta\cos\varphi\\\cos\theta&-r\sin\theta&0\end{pmatrix}
@math \det J=\frac{\partial(x,y,z)}{\partial(r,\theta,\varphi)}=r^2\sin\theta
@note Colonnes : dérivées par rapport à $r$, $\theta$, $\varphi$.
@card Jacobiens (résumé)
@tags jacobien
@math \frac{\partial(x,y)}{\partial(r,\theta)}=r\qquad\frac{\partial(x,y,z)}{\partial(r,\theta,z)}=r
@math \frac{\partial(x,y,z)}{\partial(r,\theta,\varphi)}=r^2\sin\theta
@note $dx\,dy=r\,dr\,d\theta$ ; $dx\,dy\,dz=r^2\sin\theta\,dr\,d\theta\,d\varphi$ (valeur absolue du jacobien).
)TEX";
