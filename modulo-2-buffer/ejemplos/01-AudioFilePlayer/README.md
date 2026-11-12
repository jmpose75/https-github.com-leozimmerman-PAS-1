# AudioFilePlayer

Un reproductor de archivos de audio: **Cargar...**, **Play** y **Stop**. El archivo se repite en loop.

Es la base de los demás ejemplos del módulo: una forma simple de llenar el buffer de audio con muestras.
Lo importante está en `getNextAudioBlock` (`Source/MainComponent.cpp`): el reproductor llena el buffer y, a partir
de ahí, se pueden leer las muestras (análisis) o modificarlas (efectos).

## Compilar

- **Projucer:** abrir `AudioFilePlayer.jucer`, elegir el exportador (Xcode, Visual Studio 2022 o Linux Makefile) y *Save and Open in IDE*.
- **CMake:** desde esta carpeta, `cmake -B build` y `cmake --build build`. La app queda en `build/AudioFilePlayer_artefacts/`.

Más detalle (dónde está JUCE, requisitos, cómo abrirlo en cada IDE) en el
[README de HelloJuce](../../../modulo-1-entorno/ejemplo/HelloJuce/README.md).
