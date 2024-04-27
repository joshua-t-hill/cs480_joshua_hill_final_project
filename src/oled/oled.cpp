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

// Consolidating common code for displaying text on the OLED.
void printToOled(String text)
{
    display.clearDisplay();
    display.setCursor(0,0);
    display.printf(text);
    display.display();
}

// 
void oledLoop()
{

}

//
void oledSetup()
{
    display.setTextSize(1);     // Normal 1:1 pixel scale
    display.setTextColor(WHITE);// Draw white text
    printToOled("Initializing...");
}