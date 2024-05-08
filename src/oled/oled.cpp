#include "Particle.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Globals
#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
String DISPLAY_TEXT = "Solar panel voltage: %.1f";
float tempVar = 0.0; //TEMP

// Consolidating common code for displaying text on the OLED.
void printToOled(String text)
{
    display.clearDisplay();
    display.setCursor(0,0);
    display.printf(text);
    display.display();
}

// OLED loop; will likely be test only and not part of main loop.
void oledLoop()
{
    String text = String::format("Solar panel voltage: %.1f", tempVar);
    printToOled(text);
    Log.info(DISPLAY_TEXT, tempVar);

    tempVar+=1.1; //TEMP
}

// Setup for OLED.
void oledSetup()
{
    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { Log.info("SSD1306 allocation failed"); }

    display.setTextSize(1);     // Normal 1:1 pixel scale
    display.setTextColor(WHITE);// Draw white text
    printToOled("Waiting...");
    
    Log.info("OLED setup() complete.");
}