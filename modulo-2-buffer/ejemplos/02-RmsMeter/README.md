# RmsMeter

Un medidor de nivel RMS sobre el reproductor del ejemplo 01.

- **RMS a mano:** en `getNextAudioBlock`, un loop recorre las muestras del buffer, las eleva al cuadrado, las promedia
  y saca la raíz cuadrada.
- **Pasar el valor a la interfaz:** el hilo de audio lo guarda en un `std::atomic` y un `Timer` lo lee 30 veces por segundo.
- **Medidor:** una barra dibujada en `paint()`, y un `Label` con el valor del RMS.
- **Suavizado:** el slider elige `n` en `nuevoValor = ultimoValor * n + nuevoValor * (1 - n)`.
  Esta fórmula es un filtro pasabajos de un polo: la volvemos a usar en el ejemplo 04.

## Compilar

- **Projucer:** abrir `RmsMeter.jucer`, elegir el exportador (Xcode, Visual Studio 2022 o Linux Makefile) y *Save and Open in IDE*.
- **CMake:** desde esta carpeta, `cmake -B build` y `cmake --build build`. La app queda en `build/RmsMeter_artefacts/`.

Más detalle (dónde está JUCE, requisitos, cómo abrirlo en cada IDE) en el
[README de HelloJuce](../../../modulo-1-entorno/ejemplo/HelloJuce/README.md).
