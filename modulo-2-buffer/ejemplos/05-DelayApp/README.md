# DelayApp

Delay estéreo sobre el reproductor del ejemplo 01.

- **Buffer circular:** guarda los últimos 2 segundos de audio. Se reserva en `prepareToPlay`, nunca en el hilo de audio.
- **Tiempo:** de milisegundos a muestras (`ms / 1000 * sample rate`). Se lee esa cantidad de muestras hacia atrás.
- **Feedback:** una parte del eco vuelve a entrar al buffer, así se repite varias veces.
- **Dry/Wet:** mezcla de la señal original y el eco.

## Compilar

Antes: tener JUCE al lado del repo (la carpeta `JUCE` junto a `PAS-1`) y lo instalado en [REQUISITOS.md](../../../REQUISITOS.md).

### Con Projucer
1. La primera vez, en Projucer: *Global Paths → Path to JUCE* con la carpeta de JUCE.
2. Abrir `DelayApp.jucer`, elegir el exportador (Xcode, Visual Studio 2022, Visual Studio 2026 o Linux Makefile: el que coincida con tu versión de Visual Studio) y *Save and Open in IDE*.
3. Compilar y ejecutar desde el IDE.
   - **Xcode:** esquema *DelayApp - App* y ▶.
   - **Visual Studio:** *Depurar → Iniciar depuración* (F5).
   - **Linux:** `cd Builds/LinuxMakefile && make`, y después `./build/DelayApp`.

### Con CMake
Desde esta carpeta:

```bash
cmake -B build
cmake --build build
```

| Sistema | Dónde queda la app |
|---|---|
| macOS | `build/DelayApp_artefacts/DelayApp.app` (abrir con `open`) |
| Windows | `build\DelayApp_artefacts\Debug\DelayApp.exe` |
| Linux | `build/DelayApp_artefacts/DelayApp` |

En Windows también se puede abrir esta carpeta en Visual Studio (*Abrir una carpeta local*) y ejecutar *DelayApp.exe*.
En Linux hacen falta las [dependencias de JUCE](https://github.com/juce-framework/JUCE/blob/master/docs/Linux%20Dependencies.md).

Si JUCE está en otro lugar: `cmake -B build -DJUCE_PATH=/ruta/a/JUCE`. Si no lo encuentra, lo descarga de GitHub.

Más detalle en el [README de HelloJuce](../../../modulo-1-entorno/ejemplo/HelloJuce/README.md).
