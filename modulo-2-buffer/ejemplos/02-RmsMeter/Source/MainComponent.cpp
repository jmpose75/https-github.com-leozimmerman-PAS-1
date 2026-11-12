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

    // ==== Controles del medidor ====
    smoothingSlider.setRange (0.0, 0.99, 0.01); // n: 0 = sin suavizado, cerca de 1 = muy suavizado
    smoothingSlider.setValue (0.8);
    smoothingSlider.setTextBoxStyle (juce::Slider::TextBoxRight, false, 60, 20);
    addAndMakeVisible (smoothingSlider);
    addAndMakeVisible (smoothingLabel);

    valueLabel.setFont (juce::FontOptions (20.0f));
    addAndMakeVisible (valueLabel);

    startTimerHz (30); // timerCallback() se va a llamar 30 veces por segundo

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

    // ==== Calcular el RMS del bloque, muestra por muestra ====
    // RMS = raíz cuadrada del promedio de los cuadrados de las muestras.
    //
    //              ___________________________________
    //   RMS  =   \/ (x[0]² + x[1]² + ... + x[N-1]²) / N
    //
    // La fórmula y su explicación gráfica: https://en.wikipedia.org/wiki/Root_mean_square
    auto* buffer = bufferToFill.buffer;
    float sumOfSquares = 0.0f;

    for (int channel = 0; channel < buffer->getNumChannels(); ++channel)
    {
        for (int i = 0; i < bufferToFill.numSamples; ++i)
        {
            float sample = buffer->getSample (channel, bufferToFill.startSample + i);
            sumOfSquares += sample * sample;                  // 1. elevar al cuadrado y sumar
        }
    }

    int totalSamples = buffer->getNumChannels() * bufferToFill.numSamples;
    if (totalSamples == 0)
        return;                                               // bloque vacío: no dividir por cero

    float mean = sumOfSquares / (float) totalSamples;         // 2. promedio
    blockRms = std::sqrt (mean);                              // 3. raíz cuadrada

    // JUCE también lo calcula con buffer->getRMSLevel (canal, inicio, cantidad).
}

void MainComponent::releaseResources()
{
    transport.releaseResources();
}

//==============================================================================
// El Timer. Corre en el hilo de la interfaz, 30 veces por segundo.

void MainComponent::timerCallback()
{
    float newValue = blockRms;                       // el último RMS que calculó el hilo de audio
    float n = (float) smoothingSlider.getValue();

    // Suavizado: el valor nuevo se mezcla con el anterior.
    //     nuevoValor = ultimoValor * n + nuevoValor * (1 - n)
    // Con n = 0 el medidor salta a cada valor nuevo; con n cerca de 1 se mueve lento.
    smoothedRms = smoothedRms * n + newValue * (1.0f - n);

    valueLabel.setText ("RMS: " + juce::String (smoothedRms, 3), juce::dontSendNotification);

    repaint (meterArea); // pide que se vuelva a dibujar el medidor: llama a paint()
}

//==============================================================================
// Interfaz gráfica

void MainComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    // ==== el medidor, una barra horizontal ====
    g.setColour (juce::Colours::black);
    g.fillRect (meterArea);                                    // fondo

    float level = juce::jlimit (0.0f, 1.0f, smoothedRms);      // RMS entre 0 y 1
    auto bar = meterArea.withWidth ((int) (meterArea.getWidth() * level));
    g.setColour (juce::Colours::limegreen);
    g.fillRect (bar);                                          // barra proporcional al RMS

    g.setColour (juce::Colours::white);
    g.drawRect (meterArea);                                    // borde
}

void MainComponent::resized()
{
    // Cómo se ubican los componentes: removeFromTop / removeFromLeft
    //
    // Partimos de un rectángulo (area) con todo el espacio de la ventana y lo vamos "recortando":
    //   - area.removeFromTop (30)  corta una franja de 30 píxeles de alto de ARRIBA de area,
    //                              la devuelve, y area se achica (ahora empieza 30 px más abajo).
    //   - row.removeFromLeft (100) hace lo mismo pero de costado: corta 100 px de ancho
    //                              de la IZQUIERDA de row, y row queda con lo que sobra a la derecha.
    // Si no guardamos lo que devuelve, la franja simplemente se descarta: sirve para dejar espacio.
    //
    //   +--------------------- area ---------------------+
    //   | [Cargar] [Play] [Stop]       <- removeFromTop (30)
    //   |                              <- removeFromTop (30)  (espacio vacío)
    //   | [=========== medidor ===========] <- removeFromTop (40)
    //   | ...
    //   +------------------------------------------------+
    //
    // También existen removeFromBottom y removeFromRight.

    auto area = getLocalBounds().reduced (20); // todo el componente, con un margen de 20 px

    // Fila de botones del reproductor
    auto row = area.removeFromTop (30);
    loadButton.setBounds (row.removeFromLeft (100));
    row.removeFromLeft (10);
    playButton.setBounds (row.removeFromLeft (100));
    row.removeFromLeft (10);
    stopButton.setBounds (row.removeFromLeft (100));

    // ==== Medidor, valor y slider ====
    area.removeFromTop (30);
    meterArea = area.removeFromTop (40);

    area.removeFromTop (10);
    valueLabel.setBounds (area.removeFromTop (30));

    area.removeFromTop (20);
    auto sliderRow = area.removeFromTop (30);
    smoothingLabel.setBounds (sliderRow.removeFromLeft (100));
    smoothingSlider.setBounds (sliderRow);
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
