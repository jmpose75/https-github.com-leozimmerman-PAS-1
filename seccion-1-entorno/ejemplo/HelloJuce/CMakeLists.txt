# ------------------------------------------------------------------------------
# HelloJuce: el mismo proyecto que HelloJuce.jucer, descrito con CMake.
#
#   cmake -B build                          (busca JUCE al lado del repo: ../JUCE)
#   cmake -B build -DJUCE_PATH=/ruta/a/JUCE (o indicá dónde está)
#   cmake --build build
#
# Si no encuentra JUCE, lo descarga de GitHub (hace falta internet la 1ra vez).
# ------------------------------------------------------------------------------
cmake_minimum_required (VERSION 3.22)
project (HelloJuce VERSION 1.0.0)

set (CMAKE_CXX_STANDARD 17)

# --- 1. Encontrar JUCE --------------------------------------------------------
set (JUCE_PATH "${CMAKE_CURRENT_LIST_DIR}/../../../../JUCE" CACHE PATH "Carpeta de JUCE")

if (EXISTS "${JUCE_PATH}/CMakeLists.txt")
    add_subdirectory ("${JUCE_PATH}" JUCE)
else()
    include (FetchContent)
    FetchContent_Declare (JUCE
        GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
        GIT_TAG        9.0.5
        GIT_SHALLOW    ON)
    FetchContent_MakeAvailable (JUCE)
endif()

# --- 2. Declarar la aplicación ------------------------------------------------
juce_add_gui_app (HelloJuce
    PRODUCT_NAME "HelloJuce"
    COMPANY_NAME "PAS1")

# Genera JuceHeader.h (lo que en Projucer está en JuceLibraryCode/)
juce_generate_juce_header (HelloJuce)

# --- 3. Archivos fuente (los mismos que usa el .jucer) ------------------------
target_sources (HelloJuce PRIVATE
    Source/Main.cpp
    Source/MainComponent.cpp)

target_compile_definitions (HelloJuce PRIVATE
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_APPLICATION_NAME_STRING="$<TARGET_PROPERTY:HelloJuce,JUCE_PRODUCT_NAME>"
    JUCE_APPLICATION_VERSION_STRING="$<TARGET_PROPERTY:HelloJuce,JUCE_VERSION>")

# --- 4. Módulos de JUCE (los mismos que en la pestaña "Modules" de Projucer) ---
target_link_libraries (HelloJuce
    PRIVATE
        juce::juce_audio_utils
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags)
