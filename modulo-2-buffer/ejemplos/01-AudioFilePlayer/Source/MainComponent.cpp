#include "MainComponent.h"

MainComponent::MainComponent()
{
    // ---- Reproductor ----
    addAndMakeVisible (loadButton);
    addAndMakeVisible (playButton);
    addAndMakeVisible (stopButton);

    // Qué hace cada botón al hacer clic (una "lambda": una función escrita en el lugar)
    loadButton.onClick = [this] { chooseFile(); };
    playButton.onClick = [this] { transport.start(); };
    stopButton.onClick = [this] { transport.stop(); };

    formatManager.registerBasicFormats();

    setSize (600, 400);
    setAudioChannels (0, 2); // 0 entradas, 2 salidas (estéreo)
}

MainComponent::~MainComponent()
{
    shutdownAudio();
    transport.setSource (nullptr);
}

//==============================================================================
// Audio

// Se llama antes de empezar a sonar: acá conocemos el sample rate y el tamaño del bloque.
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    transport.prepareToPlay (samplesPerBlockExpected, sampleRate);
}

// Se llama una y otra vez desde el hilo de audio: cada vez hay que llenar un bloque de muestras.
void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    // El reproductor llena el buffer con las muestras del archivo (o con silencio si está en stop).
    transport.getNextAudioBlock (bufferToFill);

    // A partir de acá, bufferToFill.buffer tiene las muestras: en los ejemplos que siguen
    // las vamos a leer (análisis) o a modificar (efectos).
}

void MainComponent::releaseResources()
{
    transport.releaseResources();
}

//==============================================================================
// Interfaz gráfica

void MainComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void MainComponent::resized()
{
    auto area = getLocalBounds().reduced (20);

    // Fila de botones del reproductor
    auto row = area.removeFromTop (30);
    loadButton.setBounds (row.removeFromLeft (100));
    row.removeFromLeft (10);
    playButton.setBounds (row.removeFromLeft (100));
    row.removeFromLeft (10);
    stopButton.setBounds (row.removeFromLeft (100));
}

//==============================================================================
// Reproductor: elegir y abrir un archivo

void MainComponent::chooseFile()
{
    chooser = std::make_unique<juce::FileChooser> ("Elegir un archivo de audio", juce::File(),
                                                   formatManager.getWildcardForAllFormats());

    // launchAsync abre la ventana sin bloquear la app y llama a la lambda cuando el usuario elige.
    chooser->launchAsync (juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
                          [this] (const juce::FileChooser& fc)
                          {
                              if (fc.getResult().existsAsFile())
                                  openFile (fc.getResult());
                          });
}

void MainComponent::openFile (const juce::File& file)
{
    auto* reader = formatManager.createReaderFor (file);
    if (reader == nullptr)
        return; // no es un archivo de audio que se pueda leer

    transport.stop();
    transport.setSource (nullptr);

    readerSource = std::make_unique<juce::AudioFormatReaderSource> (reader, true);
    readerSource->setLooping (true); // al llegar al final vuelve a empezar

    transport.setSource (readerSource.get(), 0, nullptr, reader->sampleRate);
}
