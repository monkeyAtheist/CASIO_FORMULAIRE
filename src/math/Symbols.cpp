// Table des commandes LaTeX reconnues (symboles, operateurs, fonctions).
#include "Symbols.hpp"
#include <string.h>

namespace math {

// Les lettres grecques minuscules sont en italique mathematique (U+1D6FC...),
// les majuscules droites, comme dans TeX.
static const SymDef s_syms[] = {
    // grec minuscule
    {"alpha", 0x1D6FC, C_ORD}, {"beta", 0x1D6FD, C_ORD}, {"gamma", 0x1D6FE, C_ORD},
    {"delta", 0x1D6FF, C_ORD}, {"epsilon", 0x1D716, C_ORD}, {"varepsilon", 0x1D700, C_ORD},
    {"zeta", 0x1D701, C_ORD}, {"eta", 0x1D702, C_ORD}, {"theta", 0x1D703, C_ORD},
    {"vartheta", 0x1D717, C_ORD}, {"iota", 0x1D704, C_ORD}, {"kappa", 0x1D705, C_ORD},
    {"lambda", 0x1D706, C_ORD}, {"mu", 0x1D707, C_ORD}, {"nu", 0x1D708, C_ORD},
    {"xi", 0x1D709, C_ORD}, {"pi", 0x1D70B, C_ORD}, {"varpi", 0x1D71B, C_ORD},
    {"rho", 0x1D70C, C_ORD}, {"varrho", 0x1D71A, C_ORD}, {"sigma", 0x1D70E, C_ORD},
    {"varsigma", 0x1D70D, C_ORD}, {"tau", 0x1D70F, C_ORD}, {"upsilon", 0x1D710, C_ORD},
    {"phi", 0x1D719, C_ORD}, {"varphi", 0x1D711, C_ORD}, {"chi", 0x1D712, C_ORD},
    {"psi", 0x1D713, C_ORD}, {"omega", 0x1D714, C_ORD},
    // grec majuscule
    {"Gamma", 0x393, C_ORD}, {"Delta", 0x394, C_ORD}, {"Theta", 0x398, C_ORD},
    {"Lambda", 0x39B, C_ORD}, {"Xi", 0x39E, C_ORD}, {"Pi", 0x3A0, C_ORD},
    {"Sigma", 0x3A3, C_ORD}, {"Upsilon", 0x3A5, C_ORD}, {"Phi", 0x3A6, C_ORD},
    {"Psi", 0x3A8, C_ORD}, {"Omega", 0x3A9, C_ORD},
    // operateurs binaires
    {"pm", 0xB1, C_BIN}, {"mp", 0x2213, C_BIN}, {"times", 0xD7, C_BIN},
    {"div", 0xF7, C_BIN}, {"cdot", 0x22C5, C_BIN}, {"circ", 0x2218, C_BIN},
    {"bullet", 0x2219, C_BIN}, {"ast", 0x2217, C_BIN}, {"otimes", 0x2297, C_BIN},
    {"oplus", 0x2295, C_BIN}, {"odot", 0x2299, C_BIN}, {"wedge", 0x2227, C_BIN},
    {"land", 0x2227, C_BIN}, {"vee", 0x2228, C_BIN}, {"lor", 0x2228, C_BIN},
    {"cap", 0x2229, C_BIN}, {"cup", 0x222A, C_BIN}, {"setminus", 0x2216, C_BIN},
    // relations
    {"leq", 0x2264, C_REL}, {"le", 0x2264, C_REL}, {"geq", 0x2265, C_REL},
    {"ge", 0x2265, C_REL}, {"leqslant", 0x2A7D, C_REL}, {"geqslant", 0x2A7E, C_REL},
    {"neq", 0x2260, C_REL}, {"ne", 0x2260, C_REL}, {"approx", 0x2248, C_REL},
    {"equiv", 0x2261, C_REL}, {"simeq", 0x2243, C_REL}, {"sim", 0x223C, C_REL},
    {"cong", 0x2245, C_REL}, {"propto", 0x221D, C_REL}, {"ll", 0x226A, C_REL},
    {"gg", 0x226B, C_REL}, {"subset", 0x2282, C_REL}, {"supset", 0x2283, C_REL},
    {"subseteq", 0x2286, C_REL}, {"supseteq", 0x2287, C_REL}, {"in", 0x2208, C_REL},
    {"notin", 0x2209, C_REL}, {"ni", 0x220B, C_REL}, {"perp", 0x22A5, C_REL},
    {"parallel", 0x2225, C_REL}, {"mid", 0x2223, C_REL}, {"nmid", 0x2224, C_REL},
    {"prec", 0x227A, C_REL}, {"succ", 0x227B, C_REL},
    {"subsetneq", 0x228A, C_REL}, {"supsetneq", 0x228B, C_REL}, {"nsubset", 0x2284, C_REL},
    {"nsupset", 0x2285, C_REL}, {"nsubseteq", 0x2288, C_REL}, {"nsupseteq", 0x2289, C_REL},
    {"to", 0x2192, C_REL}, {"rightarrow", 0x2192, C_REL}, {"gets", 0x2190, C_REL},
    {"leftarrow", 0x2190, C_REL}, {"leftrightarrow", 0x2194, C_REL},
    {"Rightarrow", 0x21D2, C_REL}, {"Leftarrow", 0x21D0, C_REL},
    {"Leftrightarrow", 0x21D4, C_REL}, {"implies", 0x27F9, C_REL},
    {"impliedby", 0x27F8, C_REL}, {"iff", 0x27FA, C_REL}, {"mapsto", 0x21A6, C_REL},
    {"longrightarrow", 0x27F6, C_REL}, {"longleftarrow", 0x27F5, C_REL},
    {"longleftrightarrow", 0x27F7, C_REL}, {"Longrightarrow", 0x27F9, C_REL},
    {"Longleftrightarrow", 0x27FA, C_REL}, {"uparrow", 0x2191, C_REL},
    {"downarrow", 0x2193, C_REL}, {"nearrow", 0x2197, C_REL}, {"searrow", 0x2198, C_REL},
    {"rightharpoonup", 0x21C0, C_REL},
    // symboles ordinaires
    {"infty", 0x221E, C_ORD}, {"partial", 0x2202, C_ORD}, {"nabla", 0x2207, C_ORD},
    {"forall", 0x2200, C_ORD}, {"exists", 0x2203, C_ORD}, {"nexists", 0x2204, C_ORD},
    {"neg", 0xAC, C_ORD}, {"lnot", 0xAC, C_ORD}, {"emptyset", 0x2205, C_ORD},
    {"varnothing", 0x2205, C_ORD}, {"ell", 0x2113, C_ORD}, {"hbar", 0x210F, C_ORD},
    {"angle", 0x2220, C_ORD}, {"prime", 0x2032, C_ORD}, {"degree", 0xB0, C_ORD},
    {"aleph", 0x2135, C_ORD}, {"wp", 0x2118, C_ORD}, {"therefore", 0x2234, C_REL},
    {"because", 0x2235, C_REL}, {"bot", 0x22A5, C_ORD},
    {"top", 0x22A4, C_ORD}, {"complement", 0x2201, C_ORD}, {"blacksquare", 0x220E, C_ORD},
    {"qed", 0x220E, C_ORD}, {"square", 0x25A1, C_ORD}, {"Box", 0x25A1, C_ORD},
    {"llbracket", 0x27E6, C_OPEN}, {"rrbracket", 0x27E7, C_CLOSE},
    {"ldots", 0x2026, C_INNER}, {"dots", 0x2026, C_INNER}, {"cdots", 0x22EF, C_INNER},
    {"vdots", 0x22EE, C_ORD}, {"ddots", 0x22F1, C_INNER},
    {"|", 0x2016, C_ORD}, {"Vert", 0x2016, C_ORD}, {"vert", 0x7C, C_ORD},
    {"{", 0x7B, C_OPEN}, {"}", 0x7D, C_CLOSE}, {"lbrace", 0x7B, C_OPEN},
    {"rbrace", 0x7D, C_CLOSE}, {"langle", 0x27E8, C_OPEN}, {"rangle", 0x27E9, C_CLOSE},
    {"lfloor", 0x230A, C_OPEN}, {"rfloor", 0x230B, C_CLOSE}, {"lceil", 0x2308, C_OPEN},
    {"rceil", 0x2309, C_CLOSE}, {"%", '%', C_ORD}, {"#", '#', C_ORD}, {"&", '&', C_ORD},
    {"_", '_', C_ORD}, {"$", '$', C_ORD}, {"backslash", '\\', C_ORD},
    {"imath", 0x131, C_ORD},
    // grands operateurs (symboles)
    {"sum", 0x2211, C_OP}, {"prod", 0x220F, C_OP}, {"coprod", 0x2210, C_OP},
    {"int", 0x222B, C_OP}, {"iint", 0x222C, C_OP}, {"iiint", 0x222D, C_OP},
    {"oint", 0x222E, C_OP}, {"oiint", 0x222F, C_OP}, {"bigcup", 0x22C3, C_OP},
    {"bigcap", 0x22C2, C_OP},
};

const SymDef *findSymbol(const char *name, int len)
{
    for (unsigned i = 0; i < sizeof(s_syms) / sizeof(s_syms[0]); i++) {
        const SymDef &s = s_syms[i];
        if (s.name[0] == name[0] && (int)strlen(s.name) == len && !strncmp(s.name, name, len))
            return &s;
    }
    return nullptr;
}

// Fonctions en caracteres droits. limits = 1 : bornes dessus/dessous en style D.
static const NamedOp s_ops[] = {
    {"sin", "sin", 0}, {"cos", "cos", 0}, {"tan", "tan", 0}, {"cot", "cot", 0},
    {"cotan", "cotan", 0}, {"sec", "sec", 0}, {"csc", "csc", 0},
    {"arcsin", "arcsin", 0}, {"arccos", "arccos", 0}, {"arctan", "arctan", 0},
    {"sinh", "sinh", 0}, {"cosh", "cosh", 0}, {"tanh", "tanh", 0}, {"coth", "coth", 0},
    {"sh", "sh", 0}, {"ch", "ch", 0}, {"th", "th", 0},
    {"argsh", "argsh", 0}, {"argch", "argch", 0}, {"argth", "argth", 0},
    {"arsinh", "arsinh", 0}, {"arcosh", "arcosh", 0}, {"artanh", "artanh", 0},
    {"ln", "ln", 0}, {"log", "log", 0}, {"lg", "lg", 0}, {"exp", "exp", 0},
    {"det", "det", 0}, {"dim", "dim", 0}, {"ker", "ker", 0}, {"Ker", "Ker", 0},
    {"deg", "deg", 0}, {"arg", "arg", 0}, {"Arg", "Arg", 0}, {"tr", "tr", 0},
    {"Tr", "Tr", 0}, {"rg", "rg", 0}, {"rang", "rang", 0}, {"Im", "Im", 0},
    {"Re", "Re", 0}, {"grad", "grad", 0}, {"rot", "rot", 0}, {"divg", "div", 0},
    {"Div", "div", 0}, {"card", "card", 0}, {"Card", "Card", 0}, {"id", "id", 0},
    {"Id", "Id", 0}, {"com", "com", 0}, {"Vect", "Vect", 0}, {"sgn", "sgn", 0},
    {"pgcd", "pgcd", 0}, {"ppcm", "ppcm", 0}, {"gcd", "gcd", 0}, {"lcm", "lcm", 0}, {"Pr", "Pr", 0}, {"mod", "mod", 0}, {"erf", "erf", 0},
    {"lim", "lim", 1}, {"limsup", "lim sup", 1}, {"liminf", "lim inf", 1},
    {"max", "max", 1}, {"min", "min", 1}, {"sup", "sup", 1}, {"inf", "inf", 1},
    {"Max", "Max", 1}, {"Min", "Min", 1},
};

const NamedOp *findNamedOp(const char *name, int len)
{
    for (unsigned i = 0; i < sizeof(s_ops) / sizeof(s_ops[0]); i++) {
        const NamedOp &s = s_ops[i];
        if (s.name[0] == name[0] && (int)strlen(s.name) == len && !strncmp(s.name, name, len))
            return &s;
    }
    return nullptr;
}

uint8_t charClass(uint32_t cp)
{
    switch (cp) {
    case '+': case 0x2212: case 0xB1: case 0x2213: case 0xD7: case 0xF7: case 0x22C5:
    case 0xB7: case 0x2218: case 0x2219: case 0x2217: case 0x2297: case 0x2295:
    case 0x2299: case 0x2227: case 0x2228: case 0x2229: case 0x222A: case 0x2216:
    case '*':
        return C_BIN;
    case '=': case '<': case '>': case ':': case 0x2264: case 0x2265: case 0x2260:
    case 0x2248: case 0x2261: case 0x2243: case 0x223C: case 0x2245: case 0x221D:
    case 0x226A: case 0x226B: case 0x2282: case 0x2283: case 0x2286: case 0x2287:
    case 0x2208: case 0x2209: case 0x220B: case 0x22A5: case 0x2225: case 0x2223:
    case 0x2192: case 0x2190: case 0x2194: case 0x21D2: case 0x21D0: case 0x21D4:
    case 0x27F9: case 0x27F8: case 0x27FA: case 0x21A6: case 0x27F6: case 0x27F5:
    case 0x27F7: case 0x2191: case 0x2193: case 0x2A7D: case 0x2A7E: case 0x2224:
        return C_REL;
    case '(': case '[': case 0x27E8: case 0x230A: case 0x2308:
        return C_OPEN;
    case ')': case ']': case 0x27E9: case 0x230B: case 0x2309: case '!': case '?':
        return C_CLOSE;
    case ',': case ';':
        return C_PUNCT;
    case 0x2211: case 0x220F: case 0x222B: case 0x222C: case 0x222D: case 0x222E:
    case 0x222F: case 0x22C3: case 0x22C2: case 0x2210:
        return C_OP;
    default:
        return C_ORD;
    }
}

bool isBigOpLimits(uint32_t cp)
{
    return cp == 0x2211 || cp == 0x220F || cp == 0x2210 || cp == 0x22C3 || cp == 0x22C2;
}

bool isIntegral(uint32_t cp)
{
    return cp >= 0x222B && cp <= 0x222F;
}

}  // namespace math
