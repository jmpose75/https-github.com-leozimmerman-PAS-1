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

    // ==== NUEVO: controles del delay ====
    timeSlider.setRange (1.0, 2000.0, 1.0);
    timeSlider.setValue (delayTimeMs);
    timeSlider.onValueChange = [this] { delayTimeMs = (float) timeSlider.getValue(); };

    feedbackSlider.setRange (0.0, 0.95, 0.01); // menos de 1: si no, el eco nunca se apaga
    feedbackSlider.setValue (feedback);
    feedbackSlider.onValueChange = [this] { feedback = (float) feedbackSlider.getValue(); };

    mixSlider.setRange (0.0, 1.0, 0.01);
    mixSlider.setValue (mix);
    mixSlider.onValueChange = [this] { mix = (float) mixSlider.getValue(); };

    timeSlider.setTextBoxStyle (juce::Slider::TextBoxRight, false, 60, 20);
    feedbackSlider.setTextBoxStyle (juce::Slider::TextBoxRight, false, 60, 20);
    mixSlider.setTextBoxStyle (juce::Slider::TextBoxRight, false, 60, 20);

    addAndMakeVisible (timeSlider);
    addAndMakeVisible (feedbackSlider);
    addAndMakeVisible (mixSlider);
    addAndMakeVisible (timeLabel);
    addAndMakeVisible (feedbackLabel);
    addAndMakeVisible (mixLabel);
    // ==== Fin de lo nuevo ====

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

    // ==== NUEVO: reservar el buffer circular ====
    // Se reserva acá y no en getNextAudioBlock: en el hilo de audio no se pide memoria.
    currentSampleRate = sampleRate;
    int maxDelaySamples = (int) (2.0 * sampleRate);  // 2 segundos
    delayBuffer.setSize (2, maxDelaySamples);        // 2 canales
    delayBuffer.clear();                             // todo en silencio
    writePosition = 0;
}

// Se llama una y otra vez desde el hilo de audio: cada vez hay que llenar un bloque de muestras.
void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    // El reproductor llena el buffer con las muestras del archivo (o con silencio si está en stop).
    transport.getNextAudioBlock (bufferToFill);

    // ==== NUEVO: delay con buffer circular ====
    // Cada muestra que entra se guarda en el buffer circular. Para el eco, se lee la muestra que
    // se guardó delaySamples muestras atrás. Al llegar al final del buffer se vuelve al principio.
    //
    // El buffer circular (delayBuffer), visto como una fila de casilleros:
    //
    //     0   1   2   3   4   5   6   7   8   9          <- posiciones (en el ejemplo, tamaño 10)
    //   +---+---+---+---+---+---+---+---+---+---+
    //   |   |   |   | R |   |   |   | W |   |   |
    //   +---+---+---+---+---+---+---+---+---+---+
    //                 ^               ^
    //            readPosition    writePosition       readPosition = writePosition - delaySamples
    //                                                (acá delaySamples = 4)
    //
    // En cada muestra, R y W avanzan un casillero hacia la derecha. Cuando W llega al final (10)
    // vuelve a 0: el buffer "da la vuelta", como si el final estuviera pegado al principio.
    // Si writePosition - delaySamples da negativo (por ejemplo W = 1 -> R = -3), le sumamos el
    // tamaño del buffer: R = -3 + 10 = 7, que es la muestra guardada 4 lugares antes "dando la vuelta".
    // Más sobre buffers circulares: https://en.wikipedia.org/wiki/Circular_buffer
    //
    // El recorrido de la señal (diagrama de bloques):
    //
    //   entrada ──┬──────────────────────────────────────── x (1 - wet) ──┐
    //             │                                                       (+)──> salida
    //             └──>(+)──> [ buffer circular: retardo ] ──┬─ x wet ─────┘
    //                  ^                                    │
    //                  └──────────── x feedback <───────────┘
    //
    // Lo que sale del retardo vuelve a entrar multiplicado por feedback: por eso el eco se repite,
    // cada vez más bajo (0.5, 0.25, 0.125... con feedback = 0.5). Con feedback >= 1 nunca se apagaría.
    // Es un "filtro peine" con realimentación: https://en.wikipedia.org/wiki/Comb_filter#Feedback_form
    auto* buffer = bufferToFill.buffer;
    int numChannels = juce::jmin (buffer->getNumChannels(), 2);
    int delayBufferSize = delayBuffer.getNumSamples();

    // De milisegundos a muestras: a 44100 Hz, 1000 ms son 44100 muestras
    int delaySamples = (int) (delayTimeMs / 1000.0f * (float) currentSampleRate);
    delaySamples = juce::jlimit (1, delayBufferSize - 1, delaySamples);

    float fb = feedback;
    float wet = mix;

    // Ojo con el orden de los loops: en los ejemplos 02 y 04 el loop de afuera recorría los canales
    // y el de adentro las muestras. Acá es al revés (afuera las muestras, adentro los canales)
    // porque writePosition es UNA sola para los dos canales: primero procesamos la muestra i de
    // ambos canales y recién después avanzamos writePosition un lugar.
    for (int i = 0; i < bufferToFill.numSamples; ++i)
    {
        // Posición de lectura: delaySamples muestras antes de la de escritura
        int readPosition = writePosition - delaySamples;
        if (readPosition < 0)
            readPosition += delayBufferSize;  // dar la vuelta

        for (int channel = 0; channel < numChannels; ++channel)
        {
            float input   = buffer->getSample (channel, bufferToFill.startSample + i);
            float delayed = delayBuffer.getSample (channel, readPosition);

            // Guardar la entrada más una parte del eco (feedback): así se repite varias veces
            delayBuffer.setSample (channel, writePosition, input + delayed * fb);

            // Salida: mezcla de la señal original (dry) y el eco (wet)
            float output = input * (1.0f - wet) + delayed * wet;
            buffer->setSample (channel, bufferToFill.startSample + i, output);
        }

        // Avanzar la posición de escritura, y volver a 0 al llegar al final
        writePosition = writePosition + 1;
        if (writePosition == delayBufferSize)
            writePosition = 0;
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

    // ==== NUEVO: sliders del delay ====
    area.removeFromTop (30);
    auto timeRow = area.removeFromTop (30);
    timeLabel.setBounds (timeRow.removeFromLeft (100));
    timeSlider.setBounds (timeRow);

    area.removeFromTop (10);
    auto feedbackRow = area.removeFromTop (30);
    feedbackLabel.setBounds (feedbackRow.removeFromLeft (100));
    feedbackSlider.setBounds (feedbackRow);

    area.removeFromTop (10);
    auto mixRow = area.removeFromTop (30);
    mixLabel.setBounds (mixRow.removeFromLeft (100));
    mixSlider.setBounds (mixRow);
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
