/* This creates a dripping water type animation for a strand using the APA102 chipset
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

// Setup the strand
#define LEDS  35
rgb_color ledbuffer[LEDS];

#define PHASECOUNT 10
int whitemap[PHASECOUNT] = {192,255,128,64,32,0,0, 0,0,0};
uint8_t startingcolor = 0;

// brightness (0..31).
const uint8_t brightness = 31;

void setup()
{
   if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
}

void loop()
{
  int movingcolor = startingcolor;
  for(uint8_t i = 0; i < LEDS; i++) {
    ledbuffer[i] = (rgb_color){whitemap[movingcolor], whitemap[movingcolor], whitemap[movingcolor]};
    movingcolor++;
    if ( movingcolor == PHASECOUNT) {
      movingcolor = 0;
    }
  }

  // Increment the starting position, to give movement to the next cycle,
  // and wrap when you get to the end of the whitemap.
  startingcolor++;
  if ( startingcolor == PHASECOUNT) {
    startingcolor = 0;
  }
  
  ledStrip.write(ledbuffer, LEDS, brightness);
  delay(50);
}
