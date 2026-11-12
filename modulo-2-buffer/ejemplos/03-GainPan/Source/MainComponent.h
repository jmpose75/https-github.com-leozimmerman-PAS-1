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

    // ==== Ganancia y paneo ====

    // Parámetros: los cambia la interfaz y los usa el audio (std::atomic, ver ejemplo 02).
    std::atomic<float> gain { 1.0f }; // de 0 a 1: cada muestra se multiplica por este número
    std::atomic<float> pan  { 0.0f }; // -1 = izquierda, 0 = centro, 1 = derecha

    juce::Slider gainSlider;
    juce::Label  gainLabel { {}, "Ganancia" };
    juce::Slider panSlider;
    juce::Label  panLabel  { {}, "Paneo" };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
