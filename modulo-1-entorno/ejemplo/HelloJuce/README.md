# HelloJuce

La Audio App que crea Projucer (*New Project → Application → Audio*), con un solo cambio: en `paint()`
(`Source/MainComponent.cpp`) pinta el fondo de azul y muestra el texto "Hola, JUCE". Es lo primero para modificar.

El mismo código fuente (`Source/`) se puede compilar de tres maneras.

## 1. Con Projucer

1. En Projucer, la primera vez: *Global Paths → Path to JUCE* con la carpeta de JUCE.
2. Abrir `HelloJuce.jucer`, elegir el exportador (Xcode, Visual Studio 2022, Visual Studio 2026 o Linux Makefile: el que coincida con tu versión de Visual Studio) y hacer clic en *Save and Open in IDE*.
3. Compilar y ejecutar desde el IDE (en Xcode, esquema *HelloJuce - App*).

Projucer genera las carpetas `Builds/` y `JuceLibraryCode/`. No se suben al repo.

## 2. Con CMake + un IDE

CMake genera el proyecto del IDE, en lugar de Projucer. Desde esta carpeta:

**macOS (Xcode)**
```bash
cmake -B build-xcode -G Xcode
open build-xcode/HelloJuce.xcodeproj
```
En Xcode, esquema *HelloJuce* y ▶.

**Windows (Visual Studio)**
Abrir Visual Studio → *Abrir una carpeta local* → esta carpeta. Visual Studio reconoce el `CMakeLists.txt` y configura el proyecto solo.
Elegir *HelloJuce.exe* como elemento de inicio y ejecutar.

**Linux, o cualquier sistema con VS Code o CLion**
Abrir esta carpeta. Con VS Code hace falta la extensión *CMake Tools*.

Si se cambia `CMakeLists.txt` o se agregan archivos, CMake vuelve a configurar el proyecto.

## 3. Solo CMake, desde la terminal

Desde esta carpeta:

```bash
cmake -B build
cmake --build build
```

La primera vez tarda más: compila una herramienta de JUCE (`juceaide`).
Después de cambiar código, alcanza con `cmake --build build`.

Dónde queda la app:

| Sistema | App |
|---|---|
| macOS | `build/HelloJuce_artefacts/HelloJuce.app` (abrir con `open`) |
| Windows | `build\HelloJuce_artefacts\Debug\HelloJuce.exe` |
| Linux | `build/HelloJuce_artefacts/HelloJuce` |

### Dónde está JUCE
El `CMakeLists.txt` busca JUCE al lado del repo (`../JUCE`, junto a la carpeta `PAS-1`). Si está en otro lugar:

```bash
cmake -B build -DJUCE_PATH=/ruta/a/JUCE
```

Si no lo encuentra, lo descarga de GitHub (hace falta internet la primera vez).

### Requisitos
- **CMake 3.22 o más nuevo:** <https://cmake.org/download/> (en macOS también con `brew install cmake`).
  En Windows, Visual Studio ya trae CMake.
- **macOS:** Xcode, y que la terminal lo use: `xcode-select -p` tiene que mostrar la carpeta de Xcode.
  Si muestra `/Library/Developer/CommandLineTools` y aparece el error `'algorithm' file not found`:
  `sudo xcode-select -s /Applications/Xcode.app` y borrar la carpeta `build/`.
- **Linux:** las dependencias de JUCE, en <https://github.com/juce-framework/JUCE/blob/master/docs/Linux%20Dependencies.md>.

## Limpiar

Todo lo generado (`Builds/`, `JuceLibraryCode/`, `build/`, `build-xcode/`) se puede borrar y se vuelve a crear.
Para borrar todo lo que Git ignora en esta carpeta:

```bash
git clean -ndX     # lista lo que se va a borrar
git clean -fdX     # lo borra
```

## Projucer y CMake, lado a lado

| | Projucer (`HelloJuce.jucer`) | CMake (`CMakeLists.txt`) |
|---|---|---|
| Archivos fuente | Pestaña *File Explorer* | `target_sources (...)` |
| Módulos de JUCE | Pestaña *Modules* | `target_link_libraries (... juce::juce_audio_utils)` |
| Dónde está JUCE | *Global Paths* | `JUCE_PATH` |
| `JuceHeader.h` | Lo genera en `JuceLibraryCode/` | `juce_generate_juce_header (...)` |
| Proyecto del IDE | *Save and Open in IDE* | `cmake -G Xcode`, o abrir la carpeta en Visual Studio, VS Code o CLion |
