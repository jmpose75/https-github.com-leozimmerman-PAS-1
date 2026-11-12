# GainPan

Ganancia y paneo sobre el reproductor del ejemplo 01.

- **Ganancia:** un slider de 0 a 1. Cada muestra se multiplica por ese número (0 = silencio, 1 = volumen original).
- **Paneo de potencia constante:** izquierda `cos (p * pi/2)`, derecha `sin (p * pi/2)`, con `p` entre 0 y 1.
- Si movés rápido un slider puede escucharse un ruidito (*zipper noise*): falta suavizar el valor.

## Compilar

- **Projucer:** abrir `GainPan.jucer`, elegir el exportador (Xcode, Visual Studio 2022 o Linux Makefile) y *Save and Open in IDE*.
- **CMake:** desde esta carpeta, `cmake -B build` y `cmake --build build`. La app queda en `build/GainPan_artefacts/`.

Más detalle (dónde está JUCE, requisitos, cómo abrirlo en cada IDE) en el
[README de HelloJuce](../../../modulo-1-entorno/ejemplo/HelloJuce/README.md).
