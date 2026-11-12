# TP 2 · Una app que analiza o procesa el buffer

## Objetivo

Diseñar y programar **tu propia app** que trabaje con las muestras del buffer de audio: que las **analice**
(mida algo de la señal y lo muestre), que las **procese** (las modifique para cambiar el sonido), o las dos cosas.

La app es libre. Lo único que pedimos es que quede dentro de lo que vimos en el módulo.

## Punto de partida

Copiá la carpeta [`../ejemplos/01-AudioFilePlayer`](../ejemplos/01-AudioFilePlayer) a tu carpeta de entrega
(ver *Entrega*) y cambiale el nombre al proyecto:

- **Projucer:** abrí el `.jucer` y cambiá el *Project Name*.
- **CMake:** en el `CMakeLists.txt`, reemplazá `AudioFilePlayer` por el nombre de tu app.

El reproductor ya te llena el buffer con muestras. A partir de ahí, lo que hagas en `getNextAudioBlock` es tuyo.
Los ejemplos 02 a 05 te sirven de referencia. Para probar, hay archivos de audio en [`audiofiles/`](../../audiofiles).

## Reglas

- Trabajá **muestra por muestra** sobre el buffer, escribiendo vos los cálculos.
- **Sin FFT** ni análisis de frecuencia.
- **Sin síntesis:** el sonido sale del archivo, no de osciladores (eso viene en el módulo 3).
- Una interfaz con al menos un control (slider, botón, combo) que cambie algo de lo que hace la app,
  o una visualización de lo que mide.

## Algunas ideas

Son solo sugerencias: podés tomar una, combinar varias o hacer otra cosa.

**Análisis**
- Medidor de **peak** con indicador de *clipping* (se prende cuando una muestra llega a 1).
- **Osciloscopio:** dibujar la forma de onda del último bloque.
- Medidor **por canal** (izquierdo y derecho) o de correlación entre canales.
- Contar los **cruces por cero** del bloque: da una idea de qué tan aguda o ruidosa es la señal.

**Procesamiento**
- **Distorsión:** recortar las muestras (*clipping*) o pasarlas por una curva como `std::tanh`.
- **Bitcrusher:** reducir la resolución de las muestras (redondear) o repetir muestras para bajar el sample rate.
- **Ancho estéreo:** pasar a mono, invertir canales o ensanchar con mid/side.
- **Delay multi-tap** o **ping-pong** (el eco salta de un canal al otro), a partir del ejemplo 05.

**Análisis + procesamiento**
- **Noise gate:** silenciar la señal cuando el RMS baja de un umbral.
- **Compresor simple:** bajar la ganancia cuando el nivel supera un umbral.
- **Filtro controlado por el nivel:** que la frecuencia de corte del filtro del ejemplo 04 siga al RMS.

