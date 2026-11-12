#pragma once

#include <JuceHeader.h>

// La ventana de la app. Es un AudioAppComponent: un componente gráfico que además produce audio.
class MainComponent : public juce::AudioAppComponent
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

    // ==== Filtro de primer orden ====

    // Parámetros: los cambia la interfaz y los usa el audio (std::atomic, ver ejemplo 02).
    std::atomic<float> cutoffHz { 1000.0f };  // frecuencia de corte
    std::atomic<bool>  highPass { false };    // false = pasabajos, true = pasaaltos

    double currentSampleRate = 44100.0;       // lo sabemos recién en prepareToPlay

    // El estado del filtro: la última salida de cada canal (izquierdo y derecho).
    // Es el "ultimoValor" de la fórmula de suavizado del ejemplo 02.
    float lastOutput[2] = { 0.0f, 0.0f };

    juce::Slider   cutoffSlider;
    juce::Label    cutoffLabel { {}, "Corte (Hz)" };
    juce::ComboBox typeBox;
    juce::Label    typeLabel   { {}, "Tipo" };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
