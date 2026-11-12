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

    // ==== Controles del filtro ====
    cutoffSlider.setRange (20.0, 20000.0, 1.0);
    cutoffSlider.setSkewFactorFromMidPoint (1000.0); // más recorrido para los graves, como un oído
    cutoffSlider.setValue (1000.0);
    cutoffSlider.setTextBoxStyle (juce::Slider::TextBoxRight, false, 70, 20);
    cutoffSlider.onValueChange = [this] { cutoffHz = (float) cutoffSlider.getValue(); };
    addAndMakeVisible (cutoffSlider);
    addAndMakeVisible (cutoffLabel);

    typeBox.addItem ("Pasabajos", 1);   // cada opción tiene un número (id), que empieza en 1
    typeBox.addItem ("Pasaaltos", 2);
    typeBox.setSelectedId (1);
    typeBox.onChange = [this] { highPass = (typeBox.getSelectedId() == 2); };
    addAndMakeVisible (typeBox);
    addAndMakeVisible (typeLabel);

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

    currentSampleRate = sampleRate; // guardarlo para usarlo en getNextAudioBlock
    lastOutput[0] = 0.0f;           // empezar con el filtro "vacío"
    lastOutput[1] = 0.0f;
}

// Se llama una y otra vez desde el hilo de audio: cada vez hay que llenar un bloque de muestras.
void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    // El reproductor llena el buffer con las muestras del archivo (o con silencio si está en stop).
    transport.getNextAudioBlock (bufferToFill);

    // ==== Filtro de primer orden, muestra por muestra ====
    //
    // En el ejemplo 02 suavizamos el medidor con:
    //     nuevoValor = ultimoValor * n + nuevoValor * (1 - n)
    // El filtro pasabajos de primer orden es EXACTAMENTE la misma fórmula, aplicada a cada muestra:
    //     y[i] = y[i-1] * n + x[i] * (1 - n)
    // x[i] es la muestra que entra, y[i] la que sale, y[i-1] la salida anterior (ultimoValor).
    //
    // La diferencia es de dónde sale n. En el 02 lo elegíamos con un slider; acá lo calculamos a
    // partir de la frecuencia de corte (fc) y del sample rate (sr):
    //
    //     n = exp (-2 * pi * fc / sr)
    //
    // Cómo leerla:
    //   - fc / sr dice qué tan alta es la frecuencia de corte comparada con el sample rate.
    //   - Si fc es muy baja, -2*pi*fc/sr es casi 0 y exp(casi 0) es casi 1: n cerca de 1,
    //     la salida casi no cambia de una muestra a la otra -> mucho suavizado, se van los agudos.
    //   - Si fc sube, n baja hacia 0: la salida sigue de cerca a la entrada -> pasa casi todo.
    //   - Usar sr hace que el filtro corte en la misma frecuencia (en Hz) con 44.1 kHz o con 48 kHz.
    // Sale de convertir un filtro analógico RC a muestras discretas. La deducción completa:
    //   https://en.wikipedia.org/wiki/Low-pass_filter#Difference_equation_through_discrete_time_sampling
    float n = std::exp (-2.0f * juce::MathConstants<float>::pi * cutoffHz / (float) currentSampleRate);

    // Leemos y escribimos cada muestra con getSample / setSample (como en los ejemplos 02 y 03).
    auto* buffer = bufferToFill.buffer;
    int numChannels = juce::jmin (buffer->getNumChannels(), 2);   // lastOutput tiene lugar para 2 canales

    for (int channel = 0; channel < numChannels; ++channel)
    {
        for (int i = 0; i < bufferToFill.numSamples; ++i)
        {
            int index = bufferToFill.startSample + i;
            float x = buffer->getSample (channel, index);   // la muestra que entra

            // Pasabajos: la fórmula del suavizado
            float lowPass = lastOutput[channel] * n + x * (1.0f - n);
            lastOutput[channel] = lowPass;  // guardar para la muestra siguiente (cada canal tiene la suya)

            // Pasaaltos: lo que queda al restarle a la señal su parte grave
            if (highPass)
                buffer->setSample (channel, index, x - lowPass);
            else
                buffer->setSample (channel, index, lowPass);
        }
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

    // ==== Controles del filtro ====
    area.removeFromTop (30);
    auto typeRow = area.removeFromTop (30);
    typeLabel.setBounds (typeRow.removeFromLeft (100));
    typeBox.setBounds (typeRow.removeFromLeft (150));

    area.removeFromTop (10);
    auto cutoffRow = area.removeFromTop (30);
    cutoffLabel.setBounds (cutoffRow.removeFromLeft (100));
    cutoffSlider.setBounds (cutoffRow);
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
