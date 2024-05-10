// GND to ground, Vin to 3.3-5VDC. Audio waveform out of DC (to ADC pin); will register 0.67V when no sound detected (w/ some drift).
#include "Particle.h"

// Globals
const int MIC_PIN = A5; //A0-A5 are all ADC; A3+A4 used for SDA+SCL

//test
const int sampleWindow = 50; // Sample window width in mS (50 mS = 20Hz)
unsigned int sample;

/// @brief 
void micLoop()
{   
    //analogRead gives values between 0 and 4095, convert to volts by multiplying 0.0008 volts
    //mic has DC bias of 0.67 volts; this is the read when completely quiet.
    // double milliVolts = (analogRead(MIC_PIN) * 0.8) - 670;
    // Log.info("Mic milliVolts: %.1f", milliVolts);

    // delay(50);

   unsigned long startMillis= millis();  // Start of sample window
   unsigned int peakToPeak = 0;   // peak-to-peak level

   unsigned int signalMax = 0;
   unsigned int signalMin = 1024;

   // collect data for 50 mS
   while (millis() - startMillis < sampleWindow)
   {
      sample = analogRead(MIC_PIN);
      if (sample < 1024)  // toss out spurious readings
      {
         if (sample > signalMax)
         {
            signalMax = sample;  // save just the max levels
         }
         else if (sample < signalMin)
         {
            signalMin = sample;  // save just the min levels
         }
      }
   }
   peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude
   double volts = (peakToPeak * 5.0) / 1024;  // convert to volts

   Serial.println(volts);

}