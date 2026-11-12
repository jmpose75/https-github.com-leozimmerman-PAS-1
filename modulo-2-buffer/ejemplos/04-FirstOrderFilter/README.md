# FirstOrderFilter

Filtro pasabajos y pasaaltos de primer orden sobre el reproductor del ejemplo 01.

El pasabajos es **la misma fórmula del suavizado del ejemplo 02**, aplicada a cada muestra:

```
y[i] = y[i-1] * n + x[i] * (1 - n)        n = e ^ (-2π · corte / sample rate)
```

En el 02, `n` se elegía con un slider; acá sale de la frecuencia de corte. El pasaaltos es la señal menos su pasabajos:
`x[i] - y[i]`. Cada canal guarda su propia salida anterior (el estado del filtro).

## Compilar

Antes: tener JUCE al lado del repo (la carpeta `JUCE` junto a `PAS-1`) y lo instalado en [REQUISITOS.md](../../../REQUISITOS.md).

### Con Projucer
1. La primera vez, en Projucer: *Global Paths → Path to JUCE* con la carpeta de JUCE.
2. Abrir `FirstOrderFilter.jucer`, elegir el exportador (Xcode, Visual Studio 2022, Visual Studio 2026 o Linux Makefile: el que coincida con tu versión de Visual Studio) y *Save and Open in IDE*.
3. Compilar y ejecutar desde el IDE.
   - **Xcode:** esquema *FirstOrderFilter - App* y ▶.
   - **Visual Studio:** *Depurar → Iniciar depuración* (F5).
   - **Linux:** `cd Builds/LinuxMakefile && make`, y después `./build/FirstOrderFilter`.

### Con CMake
Desde esta carpeta:

```bash
cmake -B build
cmake --build build
```

| Sistema | Dónde queda la app |
|---|---|
| macOS | `build/FirstOrderFilter_artefacts/FirstOrderFilter.app` (abrir con `open`) |
| Windows | `build\FirstOrderFilter_artefacts\Debug\FirstOrderFilter.exe` |
| Linux | `build/FirstOrderFilter_artefacts/FirstOrderFilter` |

En Windows también se puede abrir esta carpeta en Visual Studio (*Abrir una carpeta local*) y ejecutar *FirstOrderFilter.exe*.
En Linux hacen falta las [dependencias de JUCE](https://github.com/juce-framework/JUCE/blob/master/docs/Linux%20Dependencies.md).

Si JUCE está en otro lugar: `cmake -B build -DJUCE_PATH=/ruta/a/JUCE`. Si no lo encuentra, lo descarga de GitHub.

Más detalle en el [README de HelloJuce](../../../modulo-1-entorno/ejemplo/HelloJuce/README.md).
