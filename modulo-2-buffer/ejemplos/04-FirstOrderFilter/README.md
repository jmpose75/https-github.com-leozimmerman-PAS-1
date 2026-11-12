# FirstOrderFilter

Filtro pasabajos y pasaaltos de primer orden sobre el reproductor del ejemplo 01.

El pasabajos es **la misma fórmula del suavizado del ejemplo 02**, aplicada a cada muestra:

```
y[i] = y[i-1] * n + x[i] * (1 - n)        n = e ^ (-2π · corte / sample rate)
```

En el 02, `n` se elegía con un slider; acá sale de la frecuencia de corte. El pasaaltos es la señal menos su pasabajos:
`x[i] - y[i]`. Cada canal guarda su propia salida anterior (el estado del filtro).

## Compilar

- **Projucer:** abrir `FirstOrderFilter.jucer`, elegir el exportador (Xcode, Visual Studio 2022 o Linux Makefile) y *Save and Open in IDE*.
- **CMake:** desde esta carpeta, `cmake -B build` y `cmake --build build`. La app queda en `build/FirstOrderFilter_artefacts/`.

Más detalle (dónde está JUCE, requisitos, cómo abrirlo en cada IDE) en el
[README de HelloJuce](../../../modulo-1-entorno/ejemplo/HelloJuce/README.md).
