#include "Particle.h"
#include <Adafruit_GPS.h>
#include "oled/oled.h"

// Globals
#define GPSSerial Serial1
Adafruit_GPS GPS(&GPSSerial);

const int BUTTON_PIN = A2;
const unsigned long int DEBOUNCE_DELTA = 1000;
unsigned long int timeLastPressed = 0;

// 
void gpsLoop()
{
    GPS.read();

    if(digitalRead(BUTTON_PIN)==LOW && millis() - timeLastPressed > DEBOUNCE_DELTA)
    {
        timeLastPressed = millis();

        // if a sentence is received, we can check the checksum, parse it...
        if (GPS.newNMEAreceived()) 
        {
            if (!GPS.parse(GPS.lastNMEA())) // this also sets the newNMEAreceived() flag to false
            {
                Log.info("GPS Parse Failed");
                return; // we can fail to parse a sentence in which case we should just wait for another
            }       

            float latitude = GPS.latitude;
            float longitude = GPS.longitude;

            String gpsText = String::format(" Latitude: %.5f\nLongitude: %.5f", latitude, longitude);
            printToOled(gpsText);
            Log.info(gpsText);
        }
    }
}

// 
void gpsSetup()
{
    GPS.begin(9600);
    GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
    GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // 1 Hz update rate

    pinMode(BUTTON_PIN, INPUT_PULLUP);

    Log.info("GPS setup() complete.");
}