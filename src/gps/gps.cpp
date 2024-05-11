#include "Particle.h"
#include "Ubidots.h"
#include <Adafruit_GPS.h>
#include "oled/oled.h"

// Globals
const float gpsUbidotsValue = 1; 
const int BUTTON_PIN = A2;
const unsigned long int DEBOUNCE_DELTA = 1000;
const String oledInstruction = "Press button when red\nFIX light on GPS\nmodule blinks once\nevery 15 seconds.\n\nWill blink faster if not ready.";
const String oledGpsSuccess = "GPS upload \nsuccessful!\n\n Latitude: %.5f\nLongitude: %.5f";
const String oledGpsFailure = "GPS upload failed!\n\n Latitude: %.5f\nLongitude: %.5f";

#define GPSSerial Serial1
Adafruit_GPS GPS(&GPSSerial);
extern Ubidots ubidots;
unsigned long int timeLastPressed = 0;


/// @brief Loops infinitely on GPS.read() which gathers complete GPS data over time. Button press grabs GPS location if available.
void gpsLoop()
{
    GPS.read();

    if(digitalRead(BUTTON_PIN)==LOW && millis() - timeLastPressed > DEBOUNCE_DELTA)
    {
        timeLastPressed = millis();

        if (GPS.newNMEAreceived()) // if a sentence is received, we can check the checksum, parse it...
        {
            if (!GPS.parse(GPS.lastNMEA())) // this also sets the newNMEAreceived() flag to false
            {
                Log.info("GPS Parse Failed");
                return; // we can fail to parse a sentence in which case we should just wait for another
            } 
            bool bufferSent = false;
            char str_lat[30]; // Reserves memory to store context key values, add as much as you need
            char str_lng[30];
            char context[50]; // Reserves memory to store context array

            float latitude = GPS.latitude;
            float longitude = GPS.longitude;

            sprintf(str_lat, "%f", latitude); // Saves the coordinates as char
            sprintf(str_lng, "%f", longitude);
            ubidots.addContext("lat", str_lat); // Adds context key-value pairs
            ubidots.addContext("lng", str_lng);
            ubidots.getContext(context); // Builds the context with the coordinates to send to Ubidots

            ubidots.add("position", gpsUbidotsValue, context); // Sends the position
            bufferSent = ubidots.send();

            printToOled(String::format(bufferSent?oledGpsSuccess:oledGpsFailure, latitude, longitude)); // If buffersent==true, return success oled print; otherwise failure oled print
            Log.info("GPS readings\n\tSent to Ubidots:%d\n\tlat:%f\n\tlong:%f", bufferSent, latitude, longitude);
        }
    }
}

/// @brief Specify the GPS's settings in preparation to looping on GPS.read().
void gpsSetup()
{
    GPS.begin(9600);
    GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
    GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // 1 Hz update rate

    pinMode(BUTTON_PIN, INPUT_PULLUP);

    printToOled(oledInstruction);
    Log.info("GPS setup() complete.");
}