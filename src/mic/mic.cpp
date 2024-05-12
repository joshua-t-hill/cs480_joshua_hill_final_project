/// @brief GND to ground, Vin to 3.3-5VDC. Audio waveform out of DC (to ADC pin); will register 0.67V when no sound detected (w/ some drift).
#include "Particle.h"
#include "Ubidots.h"

// Globals
const int MIC_PIN = A5; //A0-A5 are all ADC; A3+A4 used for SDA+SCL

extern Ubidots ubidots;
unsigned int peakToPeak = 0; // Peak-to-peak level
unsigned int signalMax = 0;
unsigned int signalMin = 1024;
unsigned int sample = 0;

/// @brief Send the mic data up to Ubidots when Driver Loop TIMER expires.
void micDataToUbidots()
{
    peakToPeak = signalMax - signalMin;         // Max - min = peak-peak amplitude
    double volts = (peakToPeak * 5.0) / 1024;   // Convert to volts
    Log.info("Mic peak-to-peak (volts): %f", volts);

    ubidots.add("mic_peak_to_peak", volts > 10.0 ? 0.0 : volts); // Send in Driver Loop.

    // Reset variables
    peakToPeak = 0;
    signalMax = 0;
    signalMin = 1024;
}

/// @brief Continuously collect sound readings for the peak-to-peak calculation that will happen when Driver Loop TIMER expires.
void micLoop()
{   
    sample = analogRead(MIC_PIN);
    if (sample < 1024)  // Toss out spurious readings
    {
        if (sample > signalMax)
        {
            signalMax = sample;  // Save just the max levels
        }
        else if (sample < signalMin)
        {
            signalMin = sample;  // Save just the min levels
        }
   }
}