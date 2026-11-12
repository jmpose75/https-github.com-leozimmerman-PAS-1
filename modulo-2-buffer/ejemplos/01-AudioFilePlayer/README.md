# AudioFilePlayer

Un reproductor de archivos de audio: **Cargar...**, **Play** y **Stop**. El archivo se repite en loop.

Es la base de los demás ejemplos del módulo: una forma simple de llenar el buffer de audio con muestras.
Lo importante está en `getNextAudioBlock` (`Source/MainComponent.cpp`): el reproductor llena el buffer y, a partir
de ahí, se pueden leer las muestras (análisis) o modificarlas (efectos).

## Compilar

Antes: tener JUCE al lado del repo (la carpeta `JUCE` junto a `PAS-1`) y lo instalado en [REQUISITOS.md](../../../REQUISITOS.md).

### Con Projucer
1. La primera vez, en Projucer: *Global Paths → Path to JUCE* con la carpeta de JUCE.
2. Abrir `AudioFilePlayer.jucer`, elegir el exportador (Xcode, Visual Studio 2022, Visual Studio 2026 o Linux Makefile: el que coincida con tu versión de Visual Studio) y *Save and Open in IDE*.
3. Compilar y ejecutar desde el IDE.
   - **Xcode:** esquema *AudioFilePlayer - App* y ▶.
   - **Visual Studio:** *Depurar → Iniciar depuración* (F5).
   - **Linux:** `cd Builds/LinuxMakefile && make`, y después `./build/AudioFilePlayer`.

### Con CMake
Desde esta carpeta:

```bash
cmake -B build
cmake --build build
```

| Sistema | Dónde queda la app |
|---|---|
| macOS | `build/AudioFilePlayer_artefacts/AudioFilePlayer.app` (abrir con `open`) |
| Windows | `build\AudioFilePlayer_artefacts\Debug\AudioFilePlayer.exe` |
| Linux | `build/AudioFilePlayer_artefacts/AudioFilePlayer` |

En Windows también se puede abrir esta carpeta en Visual Studio (*Abrir una carpeta local*) y ejecutar *AudioFilePlayer.exe*.
En Linux hacen falta las [dependencias de JUCE](https://github.com/juce-framework/JUCE/blob/master/docs/Linux%20Dependencies.md).

Si JUCE está en otro lugar: `cmake -B build -DJUCE_PATH=/ruta/a/JUCE`. Si no lo encuentra, lo descarga de GitHub.

Más detalle en el [README de HelloJuce](../../../modulo-1-entorno/ejemplo/HelloJuce/README.md).
