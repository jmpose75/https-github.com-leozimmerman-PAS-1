#pragma once

#include <JuceHeader.h>

// La ventana de la app. Es un AudioAppComponent: un componente gráfico que además produce audio.
// NUEVO: también es un Timer, para actualizar el medidor varias veces por segundo.
class MainComponent : public juce::AudioAppComponent,
                      private juce::Timer
{
public:
    MainComponent();
    ~MainComponent() override;

    // Audio (se llaman desde el hilo de audio)
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    // Interfaz gráfica (se llaman desde el hilo de la interfaz)
    void paint (juce::Graphics& g) override;
    void resized() override;

private:
    // ---- Reproductor de archivos (igual en todos los ejemplos del módulo) ----
    juce::TextButton loadButton { "Cargar..." };
    juce::TextButton playButton { "Play" };
    juce::TextButton stopButton { "Stop" };

    juce::AudioFormatManager formatManager;                       // sabe leer wav, aiff, mp3, flac...
    juce::AudioTransportSource transport;                         // play, stop y posición
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;  // el archivo abierto
    std::unique_ptr<juce::FileChooser> chooser;                   // la ventana para elegir el archivo

    void chooseFile();
    void openFile (const juce::File& file);
    // ---- Fin del reproductor ----

    // ==== NUEVO en este ejemplo: medidor de RMS ====

    // El hilo de audio escribe el RMS de cada bloque y el hilo de la interfaz lo lee.
    //
    // ¿Por qué std::atomic? La app tiene (al menos) dos hilos que corren AL MISMO TIEMPO:
    //   - el hilo de audio, que llama a getNextAudioBlock() cientos de veces por segundo;
    //   - el hilo de la interfaz (message thread), que llama a timerCallback() y paint().
    // Si los dos usan una variable común sin protección, puede pasar que uno lea justo
    // mientras el otro está escribiendo y obtenga un valor a medio escribir (un "data race").
    // En C++ eso es comportamiento indefinido: puede andar bien casi siempre y fallar de vez en cuando.
    //
    // std::atomic<float> garantiza que cada lectura y cada escritura ocurren "de una sola vez":
    // el que lee siempre obtiene el valor viejo completo o el nuevo completo, nunca una mezcla.
    // Y lo hace sin bloquear al hilo de audio (no usa locks), algo importante porque el
    // hilo de audio nunca debe quedarse esperando.
    //
    // Se usa igual que un float común:   blockRms = 0.5f;   float x = blockRms;
    // Sirve para valores simples (float, int, bool). Para datos más grandes hacen falta otras técnicas.
    std::atomic<float> blockRms { 0.0f };

    float smoothedRms = 0.0f; // el valor que se dibuja (suavizado). Solo lo usa el hilo de la interfaz

    juce::Slider smoothingSlider;                      // controla cuánto se suaviza el medidor
    juce::Label  smoothingLabel { {}, "Suavizado" };
    juce::Label  valueLabel;                           // muestra el valor del RMS

    juce::Rectangle<int> meterArea;                    // dónde se dibuja el medidor (se calcula en resized)

    void timerCallback() override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
