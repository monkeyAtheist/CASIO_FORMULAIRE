#pragma once
#include <stdint.h>
#include "MathTypes.hpp"

namespace math {

struct SymDef {
    const char *name;
    uint32_t cp;
    uint8_t cls;
};

struct NamedOp {
    const char *name;   // nom de la commande
    const char *text;   // texte affiche
    uint8_t limits;     // bornes dessus/dessous en style D
};

const SymDef *findSymbol(const char *name, int len);
const NamedOp *findNamedOp(const char *name, int len);
uint8_t charClass(uint32_t cp);
bool isBigOpLimits(uint32_t cp);
bool isIntegral(uint32_t cp);

}  // namespace math
