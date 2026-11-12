# Recursos

Links útiles para el curso, ordenados por tipo de recurso. Entre corchetes, el módulo en el que se usa cada uno.

---

## 1. Instalación y herramientas

| Recurso | Para qué | Módulo |
|---|---|---|
| [JUCE · descarga](https://juce.com/get-juce/) | Descargar JUCE (usamos la versión 9) | [1] |
| [JUCE · releases en GitHub](https://github.com/juce-framework/JUCE/releases) | Todas las versiones, con las notas de cada una | [1] |
| [Xcode](https://developer.apple.com/xcode/) | IDE y compilador en macOS | [1] |
| [Visual Studio Community](https://visualstudio.microsoft.com/es/vs/community/) (2022 o 2026) | IDE y compilador en Windows. Instalar con *Desarrollo para el escritorio con C++* | [1] |
| [Dependencias de JUCE en Linux](https://github.com/juce-framework/JUCE/blob/master/docs/Linux%20Dependencies.md) | Paquetes que hay que instalar en Linux | [1] |
| [CMake](https://cmake.org/download/) | Compilar proyectos con `CMakeLists.txt` (en macOS también con `brew install cmake`) | [1] |
| [Git](https://git-scm.com/downloads) | Control de versiones. En Windows trae Git Bash | [1] |
| [SourceTree](https://www.sourcetreeapp.com/) | Interfaz gráfica de Git (macOS y Windows). La que se usa en clase | [1] |
| [GitHub Desktop](https://desktop.github.com/) | Otra interfaz gráfica de Git, más simple | [1] |
| [VS Code](https://code.visualstudio.com/) | Editor de código para cualquier sistema. Con la extensión *CMake Tools* compila proyectos CMake | [1] |
| [GitHub Education](https://education.github.com/pack) | Beneficios gratis para estudiantes, entre ellos GitHub Copilot | [1] |

## 2. JUCE

| Recurso | Para qué | Módulo |
|---|---|---|
| [Documentación](https://juce.com/learn/documentation/) | Punto de entrada a la documentación oficial | todos |
| [Referencia de clases](https://docs.juce.com/) | Qué hace cada clase y cada función | todos |
| [Tutoriales](https://juce.com/learn/tutorials/) | Todos los tutoriales oficiales | todos |
| [Foro](https://forum.juce.com/) | Preguntas y respuestas. Casi cualquier error ya lo tuvo alguien | todos |
| [Coding standards](https://juce.com/blog/coding-standards/) | Cómo escribe código el equipo de JUCE | todos |
| [Made with JUCE](https://juce.com/made-with-juce/) | Productos hechos con JUCE | [1] |
| [Repositorio de JUCE](https://github.com/juce-framework/JUCE) | El código fuente | todos |
| [Ejemplos de JUCE](https://github.com/juce-framework/JUCE/tree/master/examples) | Los ejemplos del DemoRunner, para leer el código | todos |
| [JUCE con CMake](https://github.com/juce-framework/JUCE/blob/master/docs/CMake%20API.md) | Todas las funciones de JUCE para `CMakeLists.txt` | [1] [5] |
| [awesome-juce](https://github.com/sudara/awesome-juce) | Lista de librerías, plugins open source y plantillas hechas con JUCE | [5] |
| [Pamplejuce](https://github.com/sudara/pamplejuce) | Plantilla de plugin con CMake, tests e integración continua | [5] |

### Tutoriales oficiales por módulo

| Tutorial | Módulo |
|---|---|
| [The main component](https://juce.com/tutorials/tutorial_main_component/) | [1] [2] |
| [Build a white noise generator](https://juce.com/tutorials/tutorial_simple_synth_noise/) | [2] |
| [The Slider class](https://juce.com/tutorials/tutorial_slider_values/) | [2] |
| [Build an audio player](https://juce.com/tutorials/tutorial_playing_sound_files/) | [2] |
| [Implement the OSC protocol in your app](https://juce.com/tutorials/tutorial_osc_sender_receiver/) | [2] |
| [Introduction to DSP](https://juce.com/tutorials/tutorial_dsp_introduction/) | [2] [5] |
| [Build a sine wave synthesiser](https://juce.com/tutorials/tutorial_sine_synth/) | [3] |
| [Wavetable synthesis](https://juce.com/tutorials/tutorial_wavetable_synth/) | [3] |
| [Build a MIDI synthesiser](https://juce.com/tutorials/tutorial_synth_using_midi_input/) | [3] |
| [Looping audio using the AudioSampleBuffer class](https://juce.com/tutorials/tutorial_looping_audio_sample_buffer/) | [4] |
| [Draw audio waveforms](https://juce.com/tutorials/tutorial_audio_thumbnail/) | [4] |
| [Create a basic Audio/MIDI plugin, part 1](https://juce.com/tutorials/tutorial_create_projucer_basic_plugin/) y [part 2](https://juce.com/tutorials/tutorial_code_basic_plugin/) | [4] |
| [Adding plug-in parameters](https://juce.com/tutorials/tutorial_audio_parameter/) | [4] |
| [Saving and loading your plug-in state](https://juce.com/tutorials/tutorial_audio_processor_value_tree_state/) | [4] |

Algunos tutoriales usan Projucer y otros CMake. El código de `Source/` sirve igual en los dos casos.

## 3. C++

| Recurso | Para qué | Módulo |
|---|---|---|
| [cppreference](https://en.cppreference.com/) | Referencia del lenguaje y de la librería estándar | todos |
| [learncpp.com](https://www.learncpp.com/) | Curso de C++ completo y gratuito, para repasar desde cero | [2] |

## 4. Audio en tiempo real

| Recurso | Para qué | Módulo |
|---|---|---|
| [Real-time audio programming 101: time waits for nothing](http://www.rossbencina.com/code/real-time-audio-programming-101-time-waits-for-nothing) (Ross Bencina) | Por qué en el hilo de audio no se reserva memoria, no se usan locks ni se hace nada que pueda esperar. Lectura obligatoria | [2] |
| [ADC en YouTube](https://www.youtube.com/@audiodevcon) | Charlas de la Audio Developer Conference. Buscar las de tiempo real, por ejemplo las de Timur Doumler | [2] |

## 5. DSP y teoría

**Bibliografía del programa**
- Pirkle, W. (2019). *Designing Audio Effect Plugins in C++: For AAX, AU, and VST3 with DSP Theory*. Focal Press. [2] [4]
- Pirkle, W. (2014). *Designing Software Synthesizer Plugins in C++: For RackAFX, VST3, and Audio Units*. Focal Press. [3]
- Zölzer, U. (Ed.) (2011). *DAFX: Digital Audio Effects* (2nd ed.). Wiley. [2]

**Online y gratis**

| Recurso | Para qué | Módulo |
|---|---|---|
| [Julius O. Smith · Introduction to Digital Filters](https://ccrma.stanford.edu/~jos/filters/) | Filtros digitales, desde la ecuación en diferencias | [2] |
| [Julius O. Smith · libros online](https://ccrma.stanford.edu/~jos/) | Sus otros libros: DFT, audio espectral y modelado físico | [2] [3] |
| [The Scientist and Engineer's Guide to DSP](https://www.dspguide.com/) | Libro de DSP general, muy claro | [2] |
| [EarLevel Engineering](https://www.earlevel.com/main/) | Artículos prácticos sobre osciladores wavetable, aliasing y filtros biquad | [2] [3] |
| [Audio EQ Cookbook](https://www.w3.org/TR/audio-eq-cookbook/) | Fórmulas de los filtros más usados (pasabajos, pasaaltos, peaking, shelving) | [2] |
| [musicdsp.org](https://www.musicdsp.org/) | Archivo de algoritmos de audio publicados por la comunidad | [2] [3] |
| [Will Pirkle](https://www.willpirkle.com/) | Sitio del autor de la bibliografía, con material complementario | [2] [3] |

## 6. OSC y MIDI

| Recurso | Para qué | Módulo |
|---|---|---|
| [Open Sound Control](https://opensoundcontrol.stanford.edu/) | Especificación de OSC | [2] |
| [Pure Data](https://puredata.info/) | Recibir OSC (gratis) | [2] |
| [Max](https://cycling74.com/products/max) | Recibir OSC | [2] |
| [TouchDesigner](https://derivative.ca/) | Recibir OSC y visualizar | [2] |
| [Resumen de mensajes MIDI 1.0](https://midi.org/summary-of-midi-1-0-messages) | Qué significa cada byte de un mensaje MIDI | [3] |
| [MIDI Monitor](https://www.snoize.com/midimonitor/) | Ver los mensajes MIDI que llegan (macOS) | [3] |
| [loopMIDI](https://www.tobias-erichsen.de/software/loopmidi.html) | Puertos MIDI virtuales en Windows. En macOS se usa el driver IAC, que ya viene en *Configuración de Audio MIDI* | [3] |

## 7. Plugins

| Recurso | Para qué | Módulo |
|---|---|---|
| AudioPluginHost | Host simple para probar plugins. Viene con JUCE, en `extras/AudioPluginHost` | [4] |
| [pluginval](https://github.com/Tracktion/pluginval) | Prueba que un plugin no falle en un DAW. Usarlo antes de entregar | [4] |
| [Reaper](https://www.reaper.fm/) | DAW con período de prueba gratuito | [4] |
| [Tracktion Waveform Free](https://www.tracktion.com/products/waveform-free) | DAW gratuito | [4] |
| [Ableton Live (prueba)](https://www.ableton.com/en/trial/) | DAW con versión de prueba | [4] |

## 8. Librerías externas

| Recurso | Para qué | Módulo |
|---|---|---|
| [CMake FetchContent](https://cmake.org/cmake/help/latest/module/FetchContent.html) | Descargar e integrar una librería desde `CMakeLists.txt` | [5] |
| [Signalsmith Stretch](https://github.com/Signalsmith-Audio/signalsmith-stretch) | Pitch shifting y time stretching (licencia MIT) | [5] |
| [SoundTouch](https://www.surina.net/soundtouch/) | Pitch shifting y cambio de tempo (licencia LGPL) | [5] |
| [Rubber Band](https://breakfastquay.com/rubberband/) | Pitch shifting y time stretching de alta calidad. Licencia GPL: obliga a publicar el código del proyecto que la use | [5] |
| [Faust](https://faust.grame.fr/) | Lenguaje para DSP que exporta código a JUCE | [5] |
| [libpd](https://github.com/libpd/libpd) | Pure Data como librería, para usar patches dentro de una app | [5] |
| [Catch2](https://github.com/catchorg/Catch2) | Tests en C++ | [5] |
| [GoogleTest](https://github.com/google/googletest) | Tests en C++ | [5] |

## 9. Comunidad

| Recurso | Para qué |
|---|---|
| [The Audio Programmer](https://www.theaudioprogrammer.com/) | Comunidad de programación de audio, con Discord |
| [The Audio Programmer en YouTube](https://www.youtube.com/@TheAudioProgrammer) | Tutoriales de JUCE y entrevistas |
| [ADC · Audio Developer Conference](https://audio.dev/) | La conferencia anual de desarrollo de audio. Las charlas quedan en YouTube |

## 10. Material de audio

| Recurso | Para qué | Módulo |
|---|---|---|
| [Freesound](https://freesound.org/) | Muestras de audio con licencias libres. Revisar la licencia de cada una | [2] [4] |
| [Audacity](https://www.audacityteam.org/) | Abrir, inspeccionar y convertir archivos de audio | [2] [4] |
| `audiofiles/` | Archivos de audio de este repo para las prácticas | [2] [4] |
