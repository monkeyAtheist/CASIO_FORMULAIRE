// Liste des chapitres (ordre de l'accueil). Ajouter un chapitre : creer
// src/content/ch_xxx.cpp sur le modele des autres, puis l'ajouter ici.
#include "../doc/Content.hpp"
#include "../gfx/Color.hpp"

extern const char CH_DERIVEES[];
extern const char CH_PRIMITIVES[];
extern const char CH_INTEGRATION[];
extern const char CH_DL[];
extern const char CH_SUITES[];
extern const char CH_SERIES[];
extern const char CH_TRIGO[];
extern const char CH_FONCTIONS[];
extern const char CH_COMPLEXES[];
extern const char CH_VECTEURS[];
extern const char CH_MATRICES[];
extern const char CH_REPERES[];
extern const char CH_GEOMETRIE[];
extern const char CH_ANALYSE_VECT[];
extern const char CH_EQUADIFF[];
extern const char CH_TRANSFORMEES[];
extern const char CH_PROBAS[];
extern const char CH_LANGAGE[];
extern const char CH_NOTATIONS[];
extern const char CH_AIDE[];

const Chapter g_chapters[] = {
    {"Dérivées", "Dérivées", "f'(x)", rgb(30, 90, 190), CH_DERIVEES, 0},
    {"Primitives", "Primitives", "\\int f(x)\\,dx", rgb(0, 120, 125), CH_PRIMITIVES, 0},
    {"Intégration", "Intégration", "[F]_a^b", rgb(25, 125, 60), CH_INTEGRATION, 0},
    {"Taylor, DL, limites", "DL, limites", "o(x^n)", rgb(185, 100, 0), CH_DL, 0},
    {"Suites", "Suites", "u_{n+1}", rgb(115, 60, 175), CH_SUITES, 0},
    {"Séries", "Séries", "\\sum u_n", rgb(170, 40, 120), CH_SERIES, 0},
    {"Trigonométrie", "Trigo", "\\cos\\theta", rgb(200, 45, 40), CH_TRIGO, 0},
    {"Fonctions usuelles et algèbre", "Fonctions", "e^x,\\ n!", rgb(160, 85, 30), CH_FONCTIONS, 0},
    {"Nombres complexes", "Complexes", "e^{i\\theta}", rgb(75, 60, 180), CH_COMPLEXES, 0},
    {"Vecteurs", "Vecteurs", "\\vec{u}\\wedge\\vec{v}", rgb(0, 105, 165), CH_VECTEURS, 0},
    {"Matrices", "Matrices", "\\det A", rgb(85, 95, 125), CH_MATRICES, 0},
    {"Coordonnées et repères", "Repères", "(r,\\theta,z)", rgb(20, 120, 100), CH_REPERES, 0},
    {"Géométrie", "Géométrie", "\\pi R^2", rgb(150, 105, 20), CH_GEOMETRIE, 0},
    {"Analyse vectorielle", "Analyse vect.", "\\vec{\\nabla}", rgb(160, 30, 70), CH_ANALYSE_VECT, 0},
    {"Équations différentielles", "Équa. diff.", "y'=ay", rgb(0, 100, 130), CH_EQUADIFF, 0},
    {"Transformées et convolution", "Transformées", "\\mathcal{L},\\ \\mathcal{Z}", rgb(100, 112, 20), CH_TRANSFORMEES, 0},
    {"Probabilités et statistiques", "Probas, stats", "\\mathcal{N}(\\mu,\\sigma)", rgb(200, 55, 105), CH_PROBAS, 0},
    {"Langage mathématique", "Langage", "\\forall\\,\\exists", rgb(80, 75, 150), CH_LANGAGE, 0},
    {"Notations françaises et anglo-saxonnes", "Notations", "\\wedge\\leftrightarrow\\times", rgb(110, 80, 65), CH_NOTATIONS, 0},
    {"Aide", "Aide", "?", rgb(60, 70, 90), CH_AIDE, 1},
};
const int g_chapterCount = sizeof(g_chapters) / sizeof(g_chapters[0]);
