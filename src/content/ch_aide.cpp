// Chapitre cache : aide (touche F5 de l'accueil).
extern const char CH_AIDE[];
const char CH_AIDE[] = R"TEX(
@page Utilisation
@card Se déplacer
@li **Accueil** : flèches puis **EXE** pour ouvrir un thème. La grille défile : **↓** sous la quatrième rangée montre les thèmes suivants.
@li **↑ ↓** : carte précédente / suivante. Une longue carte défile d'abord.
@li **← →** (ou **F2 F3**) : page précédente / suivante du thème.
@li **SHIFT** puis **↑ ↓** : défilement rapide d'un écran.
@li **F1** : sommaire du thème (pages et cartes).
@li **EXIT** : retour à l'accueil.
@card Agrandir une carte
@li **EXE** (ou **F5**) : la carte choisie s'affiche en grand.
@li **↑ ↓** : défiler. **← →** : défiler à l'horizontale si la carte est large, sinon carte précédente / suivante.
@li **EXE** ou **EXIT** : retour à la page.
@card Rechercher
@li **F4** : recherche dans les titres et les mots clés.
@li Tapez directement les lettres rouges des touches (sans ALPHA) ; la touche [.] fait une espace.
@li **DEL** efface une lettre, **AC/ON** ou **F5** efface tout, **EXE** ouvre la carte.
@card Touche MENU
@text **MENU** revient au menu de la calculatrice. En rouvrant Formulaire, on retrouve l'écran quitté.
@card Ajouter ses formules
@text Les fiches sont dans **src/content/** (un fichier par thème) : une directive par ligne (@page, @card, @math, @align, @table, @row, @text, @note, @li). Les formules s'écrivent en LaTeX. Voir le README.
@card À propos
@text Formulaire 1.2 pour Graph 90+E (gint). Moteur de formules maison inspiré de TeX : tailles de symboles, bornes et délimiteurs extensibles de la police DejaVu Math TeX Gyre.
@note Polices DejaVu et DejaVu Math TeX Gyre : licences libres (voir README).
)TEX";
