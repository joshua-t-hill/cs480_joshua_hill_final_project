/* 
 * Project myProject
 * Author: Your Name
 * Date: 
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */

// Include Particle Device OS APIs
#include "Particle.h"
#include "mic/mic.h"

// Let Device OS manage the connection to the Particle Cloud
SYSTEM_MODE(AUTOMATIC);

// Run the application and system concurrently in separate threads
SYSTEM_THREAD(ENABLED);

// Show system, cloud connectivity, and application logs over USB
// View logs with CLI using 'particle serial monitor --follow'
SerialLogHandler logHandler(LOG_LEVEL_INFO);

// Globals
const unsigned long int DEBOUNCE_DELTA = 1000;
unsigned long int timeLastPressed = 0;

// setup()
void setup() 
{
    Serial.begin(115200); //gps serial rate

    micSetup();
}

// loop()
void loop() 
{
  micLoop();

  delay(50);
}
