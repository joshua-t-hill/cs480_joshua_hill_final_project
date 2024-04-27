#include "Particle.h"
#include <Adafruit_GPS.h>

// Globals
#define GPSSerial Serial1
Adafruit_GPS GPS(&GPSSerial);

// Unadjusted code from Lab9
void gpsLoop()
{
    GPS.read();

    // if(digitalRead(BUTTON_PIN)==LOW && millis() - timeLastPressed > DEBOUNCE_DELTA)
    // {
        //timeLastPressed = millis();

        // if a sentence is received, we can check the checksum, parse it...
        if (GPS.newNMEAreceived()) {
            if (!GPS.parse(GPS.lastNMEA())) // this also sets the newNMEAreceived() flag to false
            {
                Log.info("GPS Parse Failed");
                return; // we can fail to parse a sentence in which case we should just wait for another
            }

            float latitude = GPS.latitude;
            float longitude = GPS.longitude;

            Log.info("Latitude: %f | Longitude: %f", latitude, longitude);
        //}
    }
}

// Unadjusted code from Lab9
void gpsSetup()
{
    GPS.begin(9600);
    GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
    GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // 1 Hz update rate
}