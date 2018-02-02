/*
  Digital Light Wand + SD + LCD + Arduino MEGA - V MRR-3.0 (WS2812 RGB LED Light Strip)
  by Michael Ross 2014
  Based on original code by myself in 2010 then enhanced by Is0-Mick in 2012
  Changes on UI handling for more convenience by donjoergo in 2018

  The Digital Light Wand is for use in specialized Light Painting Photography
  Applications.

  This code is totally rewritten using code that IsO-Mick created made to specifically
  support the WS2812 RGB LED strips running with an SD Card, an LCD Display, and the
  Arduino Mega 2560 Microprocessor board.

  The functionality that is included in this code is as follows:

  Menu System
  1 - File select
  2 - Brightness
  3 - Initial Delay
  4 - Frame Delay
  5 - Repeat Times (The number of times to repeat the current file playback, zero is for infinite)
  6 - Repeat Delay (if you want a delay between repeated files)

  This code supports direct reading of a 24bit Windows BMP from the SD card.
  BMP images must be rotated 90 degrees clockwise and the width of the image should match the
  number of pixels you have on your LED strip.  The bottom of the tool will be the INPUT
  end of the strips where the Arduino is connected and will be the left side of the input
  BMP image.

  Mick also added a Gamma Table from adafruit code which gives better conversion of 24 bit to
  21 bit coloring.
*/


// Library initialization
#include <Adafruit_NeoPixel.h>           // Library for the WS2812 Neopixel Strip
#include <SD.h>                          // Library for the SD Card
#include <LiquidCrystal.h>               // Library for the LCD Display
#include <SPI.h>                         // Library for the SPI Interface
#include "defines.h"

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);  // Init the LCD
Adafruit_NeoPixel strip = Adafruit_NeoPixel(STRIP_LENGTH, NPPIN, NEO_GRB + NEO_KHZ800); // Init the strip

// Setup loop to get everything ready.  This is only run once at power on or reset
void setup() {
  pinMode(AuxButton, INPUT);
  digitalWrite(AuxButton, HIGH);
  pinMode(AuxButtonGND, OUTPUT);
  digitalWrite(AuxButtonGND, LOW);

  setupLEDs();
  setupLCDdisplay();
  setupSDcard();
  BackLightOn();
}


// The Main Loop for the program starts here...
// This will loop endlessly looking for a key press to perform a function
void loop() {
  if (menu_updated) {
    menu_updated = false;
    updateMenu();
  }

  int keypress = ReadKeypad();
  if (keypress >= keys::RIGHT && keypress <= keys::LEFT) {
    menu_updated = true;
    BackLightOn();
  }
  
  handleKeys(keypress);
  
  if (BackLightTimer == true) BackLightTime();
  delay(100);
}
