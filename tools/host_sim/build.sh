#!/bin/sh
# Compile le simulateur PC (g++, ASan/UBSan). Usage : tools/host_sim/build.sh
set -e
cd "$(dirname "$0")/../.."
SRC=$(find src -name '*.cpp' ! -name main.cpp)
OBJ=$(mktemp -d)
FLAGS="-std=c++17 -O1 -g -Wall -Wextra -Wno-missing-field-initializers -fsanitize=address,undefined -fno-exceptions -fno-rtti -Itools/host_sim"
g++ $FLAGS -Dmain=app_main -c src/main.cpp -o "$OBJ/main_app.o"
g++ $FLAGS -o tools/host_sim/formulaire_sim tools/host_sim/sim.cpp "$OBJ/main_app.o" $SRC
rm -rf "$OBJ"
echo "OK : tools/host_sim/formulaire_sim"
