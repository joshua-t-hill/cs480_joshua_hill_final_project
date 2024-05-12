#include "Particle.h"
#include "Ubidots.h"
#include <Adafruit_GPS.h>
#include "oled/oled.h"

// Globals
const float GPS_UBIDOTS_VALUE = 1; 
const int BUTTON_PIN = A2;
const unsigned long int DEBOUNCE_DELTA = 1000;
const String LOG_SETUP_COMPLETE = "GPS setup() complete.";
const String LOG_GPS_PARSE_FAILED = "GPS Parse Failed";
const String LOG_GPS_READING = "GPS readings\n\tSent to Ubidots:%d\n\tlat:%f\n\tlong:%f";
const String OLED_INSTRUCTIONS = "Press button when red\nFIX light on GPS\nmodule blinks once\nevery 15 seconds.\n\nWill blink faster if not ready.";
const String OLED_GPS_SUCCESS = "GPS upload \nsuccessful!\n\n Latitude: %.5f\nLongitude: %.5f";
const String OLED_GPS_FAILURE = "GPS upload failed!\n\n Latitude: %.5f\nLongitude: %.5f";
const String FORMAT_F = "%f";

#define GPSSerial Serial1
Adafruit_GPS GPS(&GPSSerial);
extern Ubidots ubidots;
unsigned long int timeLastPressed = 0;
bool gpsIncluded = false;
float latitude;
float longitude;

/// @brief Update the OLED based on 'bufferSent' which indicates whether the Ubidots info was sent successfully or not.
void updateOledForGps(bool bufferSent)
{
    printToOled(String::format(bufferSent?OLED_GPS_SUCCESS:OLED_GPS_FAILURE, latitude, longitude)); // If buffersent==true, return success oled print; otherwise failure oled print
    Log.info(LOG_GPS_READING, bufferSent, latitude, longitude);

    gpsIncluded = false; // Reset to prevent updating OLED unless button pressed to change location.
}

/// @brief Loops infinitely on GPS.read() which gathers complete GPS data over time. Button press grabs GPS location if available.
void gpsLoop()
{
    GPS.read();

    if(digitalRead(BUTTON_PIN)==LOW && millis() - timeLastPressed > DEBOUNCE_DELTA)
    {
        timeLastPressed = millis();

        if (GPS.newNMEAreceived()) // If a sentence is received check the checksum and attempt to parse it.
        {
            if (!GPS.parse(GPS.lastNMEA())) // This also sets the newNMEAreceived() flag to false.
            {
                Log.info(LOG_GPS_PARSE_FAILED);
                return; // Can fail to parse a sentence; in which case just wait for another.
            } 
            char str_lat[30]; // Reserves memory to store context key values
            char str_lng[30]; // ^^^
            char context[50]; // Reserves memory to store context array

            latitude = GPS.latitude;
            longitude = GPS.longitude;

            sprintf(str_lat, FORMAT_F, latitude);               // Saves the coordinates as char
            sprintf(str_lng, FORMAT_F, longitude);              // ^^^
            ubidots.addContext("lat", str_lat);                 // Adds context key-value pairs
            ubidots.addContext("lng", str_lng);                 // ^^^
            ubidots.getContext(context);                        // Builds the context with the coordinates to send to Ubidots

            ubidots.add("position", GPS_UBIDOTS_VALUE, context);// Sends the position
            gpsIncluded = true; // Tell Driver Loop that GPS coordinates are included and to update the OLED.
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

    printToOled(OLED_INSTRUCTIONS);
    Log.info(LOG_SETUP_COMPLETE);
}