/* 
 * Project CS480 Final Project: "Internet of Bees (IoBs)"
 * Author: Joshua T. Hill
 * Date: 5/7/2024
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */

// Include Particle Device OS APIs
#include "Particle.h"
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

// setup()
void setup() 
{
    // For lack of time, probably use Ubidots to display data because it's really easy and powerful; includes ability to track device locations on a map already.
    Serial.begin(115200); //gps serial rate

    micSetup(); // Not complete
    //solarSetup(); //need to look at documentation for solar charger chip
    //bmeSetup(); // Complete

    //oledSetup(); //OLED works
    //gpsSetup(); //GPS works; decided to use button to control GPS polling as errors tend to happen otherwise
}

// loop()
void loop() 
{
    micLoop(); // Not complete
    //gpsLoop(); //Complete
    //bmeLoop(); //Complete
}
