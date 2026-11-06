#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    muteButton.setClickingTogglesState (true);
    muteButton.onClick = [this]
    {
        muted.store (muteButton.getToggleState());
        repaint();
    };
    addAndMakeVisible (muteButton);

    setSize (Config::windowWidth, Config::windowHeight);

    // 0 canales de entrada, 2 de salida (estéreo)
    setAudioChannels (0, 2);
}

MainComponent::~MainComponent()
{
    shutdownAudio();
}

//==============================================================================
// AUDIO
//==============================================================================
void MainComponent::prepareToPlay (int /*samplesPerBlockExpected*/, double sampleRate)
{
    currentSampleRate = sampleRate;
    phase = 0.0;

    smoothedVolume.reset (sampleRate, 0.05);   // los cambios de volumen tardan 50 ms
    smoothedVolume.setCurrentAndTargetValue (0.0f);
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    auto* buffer = bufferToFill.buffer;

    smoothedVolume.setTargetValue (muted.load() ? 0.0f : volume.load());

    // Cuánto avanza la fase en cada muestra para obtener la frecuencia pedida
    const double phaseIncrement = juce::MathConstants<double>::twoPi * frequency.load() / currentSampleRate;

    for (int i = 0; i < bufferToFill.numSamples; ++i)
    {
        const float sample = (float) std::sin (phase) * smoothedVolume.getNextValue();

        phase += phaseIncrement;
        if (phase >= juce::MathConstants<double>::twoPi)
            phase -= juce::MathConstants<double>::twoPi;

        // La misma muestra en todos los canales (izquierdo y derecho)
        for (int ch = 0; ch < buffer->getNumChannels(); ++ch)
            buffer->setSample (ch, bufferToFill.startSample + i, sample);
    }
}

void MainComponent::releaseResources()
{
}

//==============================================================================
// INTERFAZ
//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    g.fillAll (Config::backgroundColour);

    const auto bounds  = getLocalBounds().toFloat();
    const float centreY = bounds.getCentreY();
    const float freq    = frequency.load();
    const float vol     = muted.load() ? 0.0f : volume.load();

    // --- Dibujo de la onda ---------------------------------------------------
    // Cantidad de ciclos visibles: más frecuencia = más ciclos en pantalla
    const float cyclesOnScreen = juce::jmap (freq, Config::minFrequency, Config::maxFrequency, 2.0f, 12.0f);
    const float amplitudePx    = (bounds.getHeight() * 0.4f) * (vol / Config::maxVolume);

    juce::Path wave;
    for (float x = 0.0f; x <= bounds.getWidth(); x += 2.0f)
    {
        const float angle = juce::MathConstants<float>::twoPi * cyclesOnScreen * (x / bounds.getWidth());
        const float y = centreY - std::sin (angle) * amplitudePx;

        if (x == 0.0f)
            wave.startNewSubPath (x, y);
        else
            wave.lineTo (x, y);
    }

    g.setColour (Config::waveColour);
    g.strokePath (wave, juce::PathStrokeType (Config::waveThickness));

    // --- Texto ---------------------------------------------------------------
    g.setColour (Config::textColour);
    g.setFont (juce::FontOptions (18.0f));

    const juce::String info = juce::String (freq, 1) + " Hz   |   volumen: "
                            + juce::String (vol / Config::maxVolume * 100.0f, 0) + " %";
    g.drawText (info, getLocalBounds().removeFromBottom (40), juce::Justification::centred);

    if (muted.load())
        g.drawText ("MUTE", getLocalBounds(), juce::Justification::centred);
    else if (volume.load() == 0.0f)
        g.drawText ("Mové el mouse sobre la ventana", getLocalBounds(), juce::Justification::centred);
}

void MainComponent::resized()
{
    muteButton.setBounds (getLocalBounds().removeFromTop (40).removeFromRight (100).reduced (6));
}

void MainComponent::mouseMove (const juce::MouseEvent& e)
{
    updateFromMouse (e.position);
}

void MainComponent::mouseDrag (const juce::MouseEvent& e)
{
    updateFromMouse (e.position);
}

void MainComponent::updateFromMouse (juce::Point<float> position)
{
    const float width  = (float) juce::jmax (1, getWidth());
    const float height = (float) juce::jmax (1, getHeight());

    // X: de izquierda a derecha, de minFrequency a maxFrequency
    const float x = juce::jlimit (0.0f, 1.0f, position.x / width);
    frequency.store (juce::jmap (x, Config::minFrequency, Config::maxFrequency));

    // Y: abajo = silencio, arriba = volumen máximo
    const float y = juce::jlimit (0.0f, 1.0f, 1.0f - position.y / height);
    volume.store (y * Config::maxVolume);

    repaint();
}
