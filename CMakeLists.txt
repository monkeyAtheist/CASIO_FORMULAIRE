# Formulaire de mathematiques pour Casio Graph 90+E (fxSDK / gint).
# Compilation : fxsdk build-cg   ->   Formulaire.g3a
cmake_minimum_required(VERSION 3.15)
project(Formulaire VERSION 1.2 LANGUAGES C CXX)

include(GenerateG3A)
include(Fxconv)
find_package(Gint 2.9 REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Toutes les sources de src/ (le contenu des fiches est dans src/content/).
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_executable(formulaire ${SOURCES})
# Pas d'exceptions, pas de RTTI, pas de garde sur les statiques locales :
# l'application n'utilise pas la bibliotheque C++ (ni new/delete, ni std::).
target_compile_options(formulaire PRIVATE
    -Wall -Wextra -Wno-missing-field-initializers -O2
    -fno-exceptions -fno-rtti -fno-threadsafe-statics -fno-use-cxa-atexit)
target_link_libraries(formulaire Gint::Gint)

if("${FXSDK_PLATFORM_LONG}" STREQUAL fxCG50)
  generate_g3a(TARGET formulaire OUTPUT "Formulaire.g3a"
    NAME "Formulaire" ICONS assets-cg/icon-uns.png assets-cg/icon-sel.png)
endif()
