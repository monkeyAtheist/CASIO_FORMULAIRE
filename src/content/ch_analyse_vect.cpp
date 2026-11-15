// Chapitre : analyse vectorielle (gradient, divergence, rotationnel...).
extern const char CH_ANALYSE_VECT[];
const char CH_ANALYSE_VECT[] = R"TEX(
@page Opérateurs (cartésiennes)
@card Nabla
@tags nabla operateur
@math \vec{\nabla}=\frac{\partial}{\partial x}\,\vec{e}_x+\frac{\partial}{\partial y}\,\vec{e}_y+\frac{\partial}{\partial z}\,\vec{e}_z
@card Gradient
@tags gradient grad
@align \overrightarrow{\mathrm{grad}}\,f &= \vec{\nabla}f
@align &= \frac{\partial f}{\partial x}\,\vec{e}_x+\frac{\partial f}{\partial y}\,\vec{e}_y+\frac{\partial f}{\partial z}\,\vec{e}_z
@math df=\overrightarrow{\mathrm{grad}}\,f\cdot d\overrightarrow{OM}
@note Le gradient est orthogonal aux surfaces $f=\text{cte}$, dirigé vers les $f$ croissants.
@note Notation anglo-saxonne : $\nabla f$ ou $\operatorname{grad}f$.
@card Divergence
@tags divergence div flux
@math \mathrm{div}\,\vec{A}=\vec{\nabla}\cdot\vec{A}=\frac{\partial A_x}{\partial x}+\frac{\partial A_y}{\partial y}+\frac{\partial A_z}{\partial z}
@note Notation anglo-saxonne : $\nabla\cdot\mathbf{A}$.
@card Rotationnel
@tags rotationnel rot curl
@math \overrightarrow{\mathrm{rot}}\,\vec{A}=\vec{\nabla}\wedge\vec{A}=\begin{pmatrix}\dfrac{\partial A_z}{\partial y}-\dfrac{\partial A_y}{\partial z}\\\dfrac{\partial A_x}{\partial z}-\dfrac{\partial A_z}{\partial x}\\\dfrac{\partial A_y}{\partial x}-\dfrac{\partial A_x}{\partial y}\end{pmatrix}
@note Notation anglo-saxonne : $\nabla\times\mathbf{A}=\operatorname{curl}\mathbf{A}$.
@card Laplacien
@tags laplacien
@math \Delta f=\frac{\partial^2f}{\partial x^2}+\frac{\partial^2f}{\partial y^2}+\frac{\partial^2f}{\partial z^2}=\mathrm{div}\left(\overrightarrow{\mathrm{grad}}\,f\right)
@math \Delta\vec{A}=\overrightarrow{\mathrm{grad}}\left(\mathrm{div}\,\vec{A}\right)-\overrightarrow{\mathrm{rot}}\left(\overrightarrow{\mathrm{rot}}\,\vec{A}\right)
@note $\Delta\vec{A}$ en cartésiennes : $\Delta$ appliqué à chaque composante.
@note Notation anglo-saxonne : $\nabla^2f$.

@page Cylindriques (r, θ, z)
@card Gradient
@tags gradient cylindrique
@math \vec{\nabla}f=\frac{\partial f}{\partial r}\,\vec{u}_r+\frac{1}{r}\frac{\partial f}{\partial\theta}\,\vec{u}_{\theta}+\frac{\partial f}{\partial z}\,\vec{u}_z
@card Divergence
@tags divergence cylindrique
@math \mathrm{div}\,\vec{A}=\frac{1}{r}\frac{\partial\left(rA_r\right)}{\partial r}+\frac{1}{r}\frac{\partial A_{\theta}}{\partial\theta}+\frac{\partial A_z}{\partial z}
@card Rotationnel
@tags rotationnel cylindrique
@align \left(\overrightarrow{\mathrm{rot}}\,\vec{A}\right)_r &= \frac{1}{r}\frac{\partial A_z}{\partial\theta}-\frac{\partial A_{\theta}}{\partial z}
@align \left(\overrightarrow{\mathrm{rot}}\,\vec{A}\right)_{\theta} &= \frac{\partial A_r}{\partial z}-\frac{\partial A_z}{\partial r}
@align \left(\overrightarrow{\mathrm{rot}}\,\vec{A}\right)_z &= \frac{1}{r}\left(\frac{\partial\left(rA_{\theta}\right)}{\partial r}-\frac{\partial A_r}{\partial\theta}\right)
@card Laplacien
@tags laplacien cylindrique
@math \Delta f=\frac{1}{r}\frac{\partial}{\partial r}\left(r\frac{\partial f}{\partial r}\right)+\frac{1}{r^2}\frac{\partial^2f}{\partial\theta^2}+\frac{\partial^2f}{\partial z^2}

@page Sphériques (r, θ, φ)
@card Gradient
@tags gradient spherique
@align \vec{\nabla}f &= \frac{\partial f}{\partial r}\,\vec{u}_r+\frac{1}{r}\frac{\partial f}{\partial\theta}\,\vec{u}_{\theta}
@align &\quad+\frac{1}{r\sin\theta}\frac{\partial f}{\partial\varphi}\,\vec{u}_{\varphi}
@card Divergence
@tags divergence spherique
@align \mathrm{div}\,\vec{A} &= \frac{1}{r^2}\frac{\partial\left(r^2A_r\right)}{\partial r}+\frac{1}{r\sin\theta}\frac{\partial\left(\sin\theta\,A_{\theta}\right)}{\partial\theta}
@align &\quad+\frac{1}{r\sin\theta}\frac{\partial A_{\varphi}}{\partial\varphi}
@card Rotationnel
@tags rotationnel spherique
@align \left(\overrightarrow{\mathrm{rot}}\,\vec{A}\right)_r &= \frac{1}{r\sin\theta}\left(\frac{\partial\left(\sin\theta\,A_{\varphi}\right)}{\partial\theta}-\frac{\partial A_{\theta}}{\partial\varphi}\right)
@align \left(\overrightarrow{\mathrm{rot}}\,\vec{A}\right)_{\theta} &= \frac{1}{r}\left(\frac{1}{\sin\theta}\frac{\partial A_r}{\partial\varphi}-\frac{\partial\left(rA_{\varphi}\right)}{\partial r}\right)
@align \left(\overrightarrow{\mathrm{rot}}\,\vec{A}\right)_{\varphi} &= \frac{1}{r}\left(\frac{\partial\left(rA_{\theta}\right)}{\partial r}-\frac{\partial A_r}{\partial\theta}\right)
@card Laplacien
@tags laplacien spherique
@align \Delta f &= \frac{1}{r^2}\frac{\partial}{\partial r}\left(r^2\frac{\partial f}{\partial r}\right)+\frac{1}{r^2\sin\theta}\frac{\partial}{\partial\theta}\left(\sin\theta\frac{\partial f}{\partial\theta}\right)
@align &\quad+\frac{1}{r^2\sin^2\theta}\frac{\partial^2f}{\partial\varphi^2}
@note Partie radiale : $\frac{1}{r^2}\frac{\partial}{\partial r}\left(r^2\frac{\partial f}{\partial r}\right)=\frac{1}{r}\frac{\partial^2(rf)}{\partial r^2}$.

@page Identités et théorèmes
@card Identités
@tags identite formule
@math \overrightarrow{\mathrm{rot}}\left(\overrightarrow{\mathrm{grad}}\,f\right)=\vec{0}\qquad\mathrm{div}\left(\overrightarrow{\mathrm{rot}}\,\vec{A}\right)=0
@math \overrightarrow{\mathrm{grad}}(fg)=f\,\overrightarrow{\mathrm{grad}}\,g+g\,\overrightarrow{\mathrm{grad}}\,f
@math \mathrm{div}\left(f\vec{A}\right)=f\,\mathrm{div}\,\vec{A}+\overrightarrow{\mathrm{grad}}\,f\cdot\vec{A}
@math \overrightarrow{\mathrm{rot}}\left(f\vec{A}\right)=f\,\overrightarrow{\mathrm{rot}}\,\vec{A}+\overrightarrow{\mathrm{grad}}\,f\wedge\vec{A}
@math \mathrm{div}\left(\vec{A}\wedge\vec{B}\right)=\vec{B}\cdot\overrightarrow{\mathrm{rot}}\,\vec{A}-\vec{A}\cdot\overrightarrow{\mathrm{rot}}\,\vec{B}
@card Ostrogradski (flux-divergence)
@tags ostrogradski green gauss flux
@math \oiint_{\Sigma}\vec{A}\cdot d\vec{S}=\iiint_{V}\mathrm{div}\,\vec{A}\,d\tau
@note $\Sigma$ : surface fermée limitant $V$, orientée vers l'extérieur.
@card Stokes (circulation-rotationnel)
@tags stokes ampere circulation
@math \oint_{\mathcal{C}}\vec{A}\cdot d\vec{\ell}=\iint_{S}\overrightarrow{\mathrm{rot}}\,\vec{A}\cdot d\vec{S}
@note $S$ s'appuie sur le contour $\mathcal{C}$ ; orientations liées par la règle de la main droite.
@card Gradient et circulation
@tags potentiel conservatif
@math \int_A^B\overrightarrow{\mathrm{grad}}\,f\cdot d\vec{\ell}=f(B)-f(A)
@math \overrightarrow{\mathrm{rot}}\,\vec{E}=\vec{0}\iff\vec{E}=-\overrightarrow{\mathrm{grad}}\,V\quad\text{(domaine étoilé)}
@math \mathrm{div}\,\vec{B}=0\iff\vec{B}=\overrightarrow{\mathrm{rot}}\,\vec{A}
@card Green-Riemann (plan)
@tags green riemann
@math \oint_{\partial D}P\,dx+Q\,dy=\iint_D\left(\frac{\partial Q}{\partial x}-\frac{\partial P}{\partial y}\right)dx\,dy
@card Équations de Maxwell
@tags maxwell electromagnetisme
@align \mathrm{div}\,\vec{E} &= \frac{\rho}{\varepsilon_0}
@align \mathrm{div}\,\vec{B} &= 0
@align \overrightarrow{\mathrm{rot}}\,\vec{E} &= -\frac{\partial\vec{B}}{\partial t}
@align \overrightarrow{\mathrm{rot}}\,\vec{B} &= \mu_0\,\vec{j}+\mu_0\varepsilon_0\frac{\partial\vec{E}}{\partial t}
)TEX";
