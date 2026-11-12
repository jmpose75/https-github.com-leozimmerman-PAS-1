# GainPan

Ganancia y paneo sobre el reproductor del ejemplo 01.

- **Ganancia:** un slider de 0 a 1. Cada muestra se multiplica por ese número (0 = silencio, 1 = volumen original).
- **Paneo de potencia constante:** izquierda `cos (p * pi/2)`, derecha `sin (p * pi/2)`, con `p` entre 0 y 1.
- Si movés rápido un slider puede escucharse un ruidito (*zipper noise*): falta suavizar el valor.

## Compilar

Antes: tener JUCE al lado del repo (la carpeta `JUCE` junto a `PAS-1`) y lo instalado en [REQUISITOS.md](../../../REQUISITOS.md).

### Con Projucer
1. La primera vez, en Projucer: *Global Paths → Path to JUCE* con la carpeta de JUCE.
2. Abrir `GainPan.jucer`, elegir el exportador (Xcode, Visual Studio 2022, Visual Studio 2026 o Linux Makefile: el que coincida con tu versión de Visual Studio) y *Save and Open in IDE*.
3. Compilar y ejecutar desde el IDE.
   - **Xcode:** esquema *GainPan - App* y ▶.
   - **Visual Studio:** *Depurar → Iniciar depuración* (F5).
   - **Linux:** `cd Builds/LinuxMakefile && make`, y después `./build/GainPan`.

### Con CMake
Desde esta carpeta:

```bash
cmake -B build
cmake --build build
```

| Sistema | Dónde queda la app |
|---|---|
| macOS | `build/GainPan_artefacts/GainPan.app` (abrir con `open`) |
| Windows | `build\GainPan_artefacts\Debug\GainPan.exe` |
| Linux | `build/GainPan_artefacts/GainPan` |

En Windows también se puede abrir esta carpeta en Visual Studio (*Abrir una carpeta local*) y ejecutar *GainPan.exe*.
En Linux hacen falta las [dependencias de JUCE](https://github.com/juce-framework/JUCE/blob/master/docs/Linux%20Dependencies.md).

Si JUCE está en otro lugar: `cmake -B build -DJUCE_PATH=/ruta/a/JUCE`. Si no lo encuentra, lo descarga de GitHub.

Más detalle en el [README de HelloJuce](../../../modulo-1-entorno/ejemplo/HelloJuce/README.md).
