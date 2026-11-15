// Declarations partagees entre l'analyseur et la mise en page.
#pragma once
#include "MathTypes.hpp"

namespace math {

Node *newNode(uint8_t type);
Box *newBox(uint8_t kind);
bool isDummy(const Node *n);
bool isDummy(const Box *b);
void reportError(const char *what, const char *where, int len);

// Analyse : renvoie un N_GROUP contenant la liste.
Node *parse(const char *src, int len);
Box *layoutRoot(Node *root, uint8_t style, bool zoom, bool leadingOrd);

}  // namespace math
