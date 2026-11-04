# HelloJuce

La Audio App que crea Projucer (*New Project → Application → Audio*), sin cambios.
El mismo código fuente (`Source/`) se puede compilar de dos maneras.

## Con Projucer

1. En Projucer, la primera vez: *Global Paths → Path to JUCE* con la carpeta de JUCE.
2. Abrir `HelloJuce.jucer`, elegir el exportador (Xcode, Visual Studio 2022 o Linux Makefile) y hacer clic en *Save and Open in IDE*.
3. Compilar y ejecutar desde el IDE.

Projucer genera las carpetas `Builds/` y `JuceLibraryCode/`. No se suben al repo.

## Con CMake

Desde esta carpeta:

```bash
cmake -B build -DJUCE_PATH=/ruta/a/JUCE
cmake --build build
```

Si JUCE está al lado del repo (`../JUCE`), alcanza con `cmake -B build`. Si no encuentra JUCE, lo descarga.
La app queda en `build/HelloJuce_artefacts/`.

## Projucer y CMake, lado a lado

| | Projucer (`HelloJuce.jucer`) | CMake (`CMakeLists.txt`) |
|---|---|---|
| Archivos fuente | Pestaña *File Explorer* | `target_sources (...)` |
| Módulos de JUCE | Pestaña *Modules* | `target_link_libraries (... juce::juce_audio_utils)` |
| Dónde está JUCE | *Global Paths* | `JUCE_PATH` |
| `JuceHeader.h` | Lo genera en `JuceLibraryCode/` | `juce_generate_juce_header (...)` |
| Proyecto del IDE | *Save and Open in IDE* | `cmake -G Xcode` / `-G "Visual Studio 17 2022"` |
