# TP 1 · ToneApp: compilar, modificar y versionar

## Objetivo

Comprobar que tenés el entorno de trabajo completo: armar un proyecto JUCE con código que no escribiste,
compilarlo, modificarlo y llevar el historial de cambios con Git hasta publicarlo en GitHub.

No hace falta entender cómo funciona el audio de la app: eso lo vemos en el módulo 2.

## La app

**ToneApp** genera un seno que se controla con el mouse:

- **izquierda ↔ derecha:** frecuencia;
- **abajo ↔ arriba:** volumen;
- botón **Mute:** silencio.

Dibuja la onda en la ventana y muestra la frecuencia y el volumen.

> Bajá el volumen de tu computadora antes de probarla por primera vez.

## Recursos

En [`recursos/`](recursos):

| Archivo | Para qué |
|---|---|
| `MainComponent.h` | Declaración de la app. Al principio está el bloque **`Config`** con los valores para modificar |
| `MainComponent.cpp` | Implementación de la app |
| `gitignore.txt` | `.gitignore` para un proyecto JUCE |

Como referencia de un proyecto completo (con `.jucer` y `CMakeLists.txt`), mirá
[`../ejemplo/HelloJuce`](../ejemplo/HelloJuce).

## Consigna

### 1. Crear el proyecto
1. En Projucer: *New Project → Application → Audio*. Nombre: `ToneApp`. Elegí una carpeta **fuera** de este repo.
2. Reemplazá `Source/MainComponent.h` y `Source/MainComponent.cpp` por los de `recursos/`.
   **No toques `Main.cpp`.**
3. *Save and Open in IDE*, compilá y ejecutá. Tiene que sonar al mover el mouse.

*Alternativa con CMake:* armá una carpeta `ToneApp/` con el `CMakeLists.txt` y el `Source/Main.cpp` de `HelloJuce`,
más los dos archivos de `recursos/` en `Source/`. En el `CMakeLists.txt` cambiá `HelloJuce` por `ToneApp` y compilá:

```bash
cmake -B build -DJUCE_PATH=/ruta/a/JUCE
cmake --build build
```

### 2. Primer commit: la app original
1. Copiá `recursos/gitignore.txt` a la carpeta de tu proyecto y renombralo a **`.gitignore`**.
2. `git init`, y revisá con `git status` que **no** aparezcan `Builds/` ni `JuceLibraryCode/` (o `build/`).
3. Primer commit con la app **sin cambios**. Por ejemplo: `ToneApp original`.

### 3. Cambios: un commit por cada uno
Hacé **al menos tres** cambios de la lista, o cambios propios. Después de cada uno, compilá, probá,
revisá con `git diff` y hacé **un commit con un mensaje que diga qué cambió**.

Cambios sugeridos (casi todos están en el bloque `Config` de `MainComponent.h`):

- el rango de frecuencias (por ejemplo, de 200 a 2000 Hz);
- los colores del fondo, de la onda o del texto;
- el grosor de la línea;
- el tamaño inicial de la ventana;
- el texto del botón o el mensaje que aparece cuando no suena;
- invertir el eje: que arriba sea silencio y abajo el volumen máximo (en `updateFromMouse`, en `MainComponent.cpp`).

Ejemplos de mensajes de commit:

```
Rango de frecuencias de 200 a 2000 Hz
Onda en celeste y fondo negro
Volumen máximo arriba en lugar de abajo
```

Mensajes como `cambios`, `fix` o `asdf` no cuentan.

### 4. Publicar
1. Creá un repositorio en GitHub con el nombre `ToneApp`. Puede ser público o privado; si es privado, agregá al docente como colaborador.
2. Conectalo con `git remote add origin ...` y hacé `git push`.
3. Agregá un `README.md` corto: qué es la app, qué cambiaste y cómo se compila. Commit y push.

### Opcional: un cambio con IA
Pedile a una herramienta de IA uno de los cambios (o uno nuevo, por ejemplo que el color de la onda cambie con la
frecuencia). Revisalo con `git diff` antes de hacer el commit, y contá en el README qué herramienta usaste,
qué le pediste y si tuviste que corregir algo.

## Entrega

El **link al repositorio de GitHub**, antes del comienzo del módulo 3.

## Checklist de revisión

- [ ] El repo no tiene `Builds/`, `JuceLibraryCode/` ni `build/`.
- [ ] El primer commit es la app original, sin cambios.
- [ ] Hay al menos tres commits de cambios, uno por cambio, con mensajes que dicen qué cambió.
- [ ] El proyecto compila a partir de lo que está en el repo.
- [ ] Hay un `README.md` con qué es, qué cambió y cómo se compila.
