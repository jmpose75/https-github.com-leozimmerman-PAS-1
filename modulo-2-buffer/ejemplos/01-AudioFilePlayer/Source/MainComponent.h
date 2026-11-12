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

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
