# DelayApp

Delay estéreo sobre el reproductor del ejemplo 01.

- **Buffer circular:** guarda los últimos 2 segundos de audio. Se reserva en `prepareToPlay`, nunca en el hilo de audio.
- **Tiempo:** de milisegundos a muestras (`ms / 1000 * sample rate`). Se lee esa cantidad de muestras hacia atrás.
- **Feedback:** una parte del eco vuelve a entrar al buffer, así se repite varias veces.
- **Dry/Wet:** mezcla de la señal original y el eco.

## Compilar

- **Projucer:** abrir `DelayApp.jucer`, elegir el exportador (Xcode, Visual Studio 2022 o Linux Makefile) y *Save and Open in IDE*.
- **CMake:** desde esta carpeta, `cmake -B build` y `cmake --build build`. La app queda en `build/DelayApp_artefacts/`.

Más detalle (dónde está JUCE, requisitos, cómo abrirlo en cada IDE) en el
[README de HelloJuce](../../../modulo-1-entorno/ejemplo/HelloJuce/README.md).
