/* 
 * Project CS480 Final Project: "Internet of Bees (IoBs)"
 * Author: Joshua T. Hill
 * Date: 5/7/2024
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */

// Include Particle Device OS APIs
#include "Particle.h"
#include "Ubidots.h"
#include "mic/mic.h"
#include "oled/oled.h"
#include "gps/gps.h"
#include "bme688/bme688.h"

// Let Device OS manage the connection to the Particle Cloud
SYSTEM_MODE(AUTOMATIC);

// Run the application and system concurrently in separate threads
SYSTEM_THREAD(ENABLED);

// Show system, cloud connectivity, and application logs over USB
// View logs with CLI using 'particle serial monitor --follow'
SerialLogHandler logHandler(LOG_LEVEL_INFO);

// Globals
#ifndef TOKEN
    #define TOKEN "BBUS-YEhdrs7LSWjGACAyH7tAnjBJ6ihrG0"
#endif
const String LOG_TIME_PASSED = "millis() - startMillis = %ld";
const int TIMER = 1000; // Control frequency of sending data to Ubidots

extern bool gpsIncluded;
Ubidots ubidots(TOKEN, UBI_HTTP);
unsigned long startMillis;

/// @brief Driver Setup.
void setup() 
{
    Serial.begin(115200); // GPS serial rate

    oledSetup(); // Always first; gpsSetup() uses the OLED
    gpsSetup();
    bmeSetup();

    startMillis = millis(); // Begin TIMER
}

/// @brief Driver Loop.
void loop() 
{
    gpsLoop(); // Run outside of TIMER check because GPS.read() needs to run on every loop. Button press will attempt to send GPS data to Ubidots if available.
    micLoop(); // Constantly collects sound data and only calculates peak-to-peak when micDataToUbidots() is called.

    unsigned long timePassed = millis() - startMillis;
    if(millis() - startMillis > TIMER) // Only send data every ~1s
    {
        bool bufferSent = false;
        Log.info(LOG_TIME_PASSED, timePassed);
        ubidots.add("read_duration", timePassed);

        bmeLoop();
        micDataToUbidots(); 

        bufferSent = ubidots.send(); // Send all collected data up to Ubidots.
        if(gpsIncluded){ updateOledForGps(bufferSent); } // Only update OLED if the GPS data was included.

        startMillis = millis(); // Reset TIMER
    }
}
