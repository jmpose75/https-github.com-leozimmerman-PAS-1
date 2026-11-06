#pragma once

#include <JuceHeader.h>

//==============================================================================
// ToneApp: un seno que se controla con el mouse.
//   - Mover el mouse a la izquierda o a la derecha cambia la frecuencia.
//   - Moverlo hacia arriba o hacia abajo cambia el volumen.
//   - El botón "Mute" silencia el sonido.
//
// No hace falta entender cómo funciona el audio para hacer el TP:
// los valores que se pueden cambiar están agrupados en "Config", más abajo.
//==============================================================================
class MainComponent  : public juce::AudioAppComponent
{
public:
    //==============================================================================
    // Config: valores para modificar en el TP
    //==============================================================================
    struct Config
    {
        // Rango de frecuencias (en Hz) entre el borde izquierdo y el derecho de la ventana
        static constexpr float minFrequency = 100.0f;
        static constexpr float maxFrequency = 1000.0f;

        // Volumen máximo (1.0 = máximo posible; ¡cuidado con los oídos!)
        static constexpr float maxVolume = 0.25f;

        // Colores
        static inline const juce::Colour backgroundColour = juce::Colour (0xff1e1e2e);
        static inline const juce::Colour waveColour       = juce::Colours::orange;
        static inline const juce::Colour textColour       = juce::Colours::white;

        // Grosor de la línea de la onda (en píxeles)
        static constexpr float waveThickness = 3.0f;

        // Tamaño inicial de la ventana
        static constexpr int windowWidth  = 800;
        static constexpr int windowHeight = 500;
    };

    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    // Audio
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    // Interfaz
    void paint (juce::Graphics& g) override;
    void resized() override;
    void mouseMove (const juce::MouseEvent& e) override;
    void mouseDrag (const juce::MouseEvent& e) override;

private:
    //==============================================================================
    void updateFromMouse (juce::Point<float> position);

    // Interfaz
    juce::TextButton muteButton { "Mute" };

    // Valores que la interfaz escribe y el audio lee (por eso son "atomic")
    std::atomic<float> frequency { 440.0f };
    std::atomic<float> volume    { 0.0f };
    std::atomic<bool>  muted     { false };

    // Estado del audio
    double currentSampleRate = 44100.0;
    double phase = 0.0;                          // posición dentro del ciclo del seno (0 a 2π)
    juce::SmoothedValue<float> smoothedVolume;   // evita "clicks" cuando cambia el volumen

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
