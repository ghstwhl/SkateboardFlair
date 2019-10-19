/* This creates a rainbow animation for a strand using the APA102 chipset
*  Copyright 2019 - Chris O'Halloran - merlin@ghostwheel.com
*  Released under the CC BY-NC-SA license: https://creativecommons.org/licenses/by-nc-sa/4.0/
*  
*  Guaranteed to give you more flair than Brian: https://www.youtube.com/watch?v=_ChQK8j6so8
*/

#include <avr/power.h>
#include <APA102.h>

// Data and Clock pins:
//attiny85
#define DATAPIN 0
#define CLOCKPIN 1
/*
// Arduino
#define DATAPIN 11
#define CLOCKPIN 13
*/

// Initialize the APA102 object
APA102<DATAPIN, CLOCKPIN> ledStrip;

// Set the number of LEDs to control.
#define LEDS  35
rgb_color ledbuffer[LEDS];

// Let's calcualte the drive across the strand:
const float colormovement = (float)360 / (float)LEDS;
uint16_t startingcolor = 0;

// brightness (0..31).
const uint8_t brightness = 8;


// This function shows up in sp many rainbow samples that 
// I don't know who to attribute it to.
rgb_color hsvToRgb(uint16_t h, uint8_t s, uint8_t v)
{
    uint8_t f = (h % 60) * 255 / 60;
    uint8_t p = (255 - s) * (uint16_t)v / 255;
    uint8_t q = (255 - f * (uint16_t)s / 255) * (uint16_t)v / 255;
    uint8_t t = (255 - (255 - f) * (uint16_t)s / 255) * (uint16_t)v / 255;
    uint8_t r = 0, g = 0, b = 0;
    switch((h / 60) % 6){
        case 0: r = v; g = t; b = p; break;
        case 1: r = q; g = v; b = p; break;
        case 2: r = p; g = v; b = t; break;
        case 3: r = p; g = q; b = v; break;
        case 4: r = t; g = p; b = v; break;
        case 5: r = v; g = p; b = q; break;
    }
    return (rgb_color){r, g, b};
}

void setup()
{
   if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
}

void loop()
{
  int movingcolor = startingcolor;
  for(uint16_t i = 0; i < LEDS; i++) {
    ledbuffer[i] = hsvToRgb((int)(movingcolor * colormovement), 255, 255);
    movingcolor++;
    if ( movingcolor == LEDS) {
      movingcolor = 0;
    }
  }

  // Increment the starting position, to give movement to the next cycle,
  // and wrap when you get to the end of the spectrum.
  startingcolor++;
  if (startingcolor == LEDS) {
    startingcolor = 0;
  }
  
  ledStrip.write(ledbuffer, LEDS, brightness);
}
