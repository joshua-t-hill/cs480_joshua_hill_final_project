// GND to ground, Vin to 3.3-5VDC. Audio waveform out of DC; will register 0.67V when no sound detected (w/ some drift).
#include "Particle.h"

// Globals
const int MIC_PIN = A2; //A2 or A5

//
void micLoop()
{   
    double voltage = analogRead(MIC_PIN)/4095.0 * 3.3; //doesn't seem like value is right
    Log.info("Mic voltage: %f", voltage);
}

//
void micSetup()
{
    pinMode(MIC_PIN, AN_INPUT);
}