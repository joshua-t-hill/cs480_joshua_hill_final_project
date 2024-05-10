// GND to ground, Vin to 3.3-5VDC. Audio waveform out of DC (to ADC pin); will register 0.67V when no sound detected (w/ some drift).
#include "Particle.h"

// Globals
const int MIC_PIN = A5; //A0-A5 are all ADC; A3+A4 used for SDA+SCL

/// @brief 
void micLoop()
{   
    //analogRead gives values between 0 and 4095, convert to volts by multiplying 0.0008 volts
    //mic has DC bias of 0.67 volts; this is the read when completely quiet.
    double milliVolts = (analogRead(MIC_PIN) * 0.8) - 670;
    Log.info("Mic milliVolts: %.1f", milliVolts);

    delay(50);
}