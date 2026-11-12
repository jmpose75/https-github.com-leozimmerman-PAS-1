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

    // ==== Sliders de ganancia y paneo ====
    gainSlider.setRange (0.0, 1.0, 0.01);   // 0 = silencio, 1 = volumen original
    gainSlider.setValue (1.0);
    gainSlider.setTextBoxStyle (juce::Slider::TextBoxRight, false, 60, 20);
    gainSlider.onValueChange = [this] { gain = (float) gainSlider.getValue(); };
    addAndMakeVisible (gainSlider);
    addAndMakeVisible (gainLabel);

    panSlider.setRange (-1.0, 1.0, 0.01);
    panSlider.setValue (0.0);
    panSlider.setTextBoxStyle (juce::Slider::TextBoxRight, false, 60, 20);
    panSlider.onValueChange = [this] { pan = (float) panSlider.getValue(); };
    addAndMakeVisible (panSlider);
    addAndMakeVisible (panLabel);

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

    // ==== Ganancia y paneo, muestra por muestra ====
    auto* buffer = bufferToFill.buffer;
    if (buffer->getNumChannels() < 2)
        return; // este ejemplo necesita estéreo

    // Leemos y escribimos cada muestra con getSample / setSample (como en el ejemplo 02).

    // ---- 1. Ganancia: todas las muestras de todos los canales se multiplican por el mismo número ----
    float g = gain;

    for (int channel = 0; channel < buffer->getNumChannels(); ++channel)
    {
        for (int i = 0; i < bufferToFill.numSamples; ++i)
        {
            int index = bufferToFill.startSample + i;
            float sample = buffer->getSample (channel, index);
            buffer->setSample (channel, index, sample * g);
        }
    }

    // ---- 2. Paneo: cada canal se multiplica por un número distinto ----
    // Paneo de potencia constante. Primero pasamos pan de [-1, 1] a p en [0, 1]:
    //   izquierda = cos (p * pi/2),  derecha = sin (p * pi/2)
    //   p = 0   -> izquierda = 1,     derecha = 0       (todo a la izquierda)
    //   p = 0.5 -> izquierda ≈ 0.707, derecha ≈ 0.707   (centro)
    //   p = 1   -> izquierda = 0,     derecha = 1       (todo a la derecha)
    // Como cos² + sin² = 1, la potencia total se mantiene igual en cualquier posición.
    // Un paneo lineal (izquierda = 1 - p, derecha = p) deja cada lado a la mitad en el centro
    // y se nota un "pozo" de volumen.
    //
    // La ecuación y la comparación con el paneo lineal:
    //   https://www.cs.cmu.edu/~music/icm-online/readings/panlaws/
    float p = (pan + 1.0f) / 2.0f;
    float angle = p * juce::MathConstants<float>::halfPi;   // de 0 a pi/2
    float leftGain  = std::cos (angle);
    float rightGain = std::sin (angle);

    for (int i = 0; i < bufferToFill.numSamples; ++i)
    {
        int index = bufferToFill.startSample + i;

        float l = buffer->getSample (0, index);   // canal 0 = izquierdo
        float r = buffer->getSample (1, index);   // canal 1 = derecho

        buffer->setSample (0, index, l * leftGain);
        buffer->setSample (1, index, r * rightGain);
    }
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

    // ==== Sliders ====
    area.removeFromTop (30);
    auto gainRow = area.removeFromTop (30);
    gainLabel.setBounds (gainRow.removeFromLeft (110));
    gainSlider.setBounds (gainRow);

    area.removeFromTop (10);
    auto panRow = area.removeFromTop (30);
    panLabel.setBounds (panRow.removeFromLeft (110));
    panSlider.setBounds (panRow);
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
