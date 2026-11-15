#!/bin/sh
# Verifie le contenu (et produit les images des pages avec -png DIR).
set -e
cd "$(dirname "$0")/../.."
SRC=$(find src -name '*.cpp' ! -name main.cpp ! -path 'src/ui/*')
g++ -std=c++17 -O1 -g -Wall -Wextra -Wno-missing-field-initializers -fsanitize=address,undefined \
    -fno-exceptions -fno-rtti -Itools/host_sim -o tools/tests/check_content tools/tests/check_content.cpp $SRC
tools/tests/check_content "$@"
