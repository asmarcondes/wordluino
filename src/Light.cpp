#include <FastLED.h>

#include "SerialDebug.h"

#define NUM_LEDS 240
#define DATA_PIN 6

#define NUM_SPOTS 30
#define DENSITY 8

#define DELAY 30

const CRGB colors[3] = {
  CRGB(58, 163, 148),
  CRGB(211, 173, 105),
  CRGB(97, 84, 88)
};

CRGB leds[NUM_LEDS];
long int time = 0;

// void blink() {
//   if (lightOn) {
//     for (uint8_t i = 0; i < NUM_SQUARE_LEDS; i++) {
//       leds[i] = CRGB::Black;
//     }

//     FastLED.show();
//     lightOn = false;
//   } else {
//     for (uint8_t i = 0; i < NUM_SQUARE_LEDS; i++) {
//       leds[i] = CRGB::Red;
//     }

//     FastLED.show();
//     lightOn = true;
//   }
// }

// void lightSquare(uint8_t index, CRGB color) {
//   for (uint8_t i = 0; i < NUM_SQUARE_LEDS; i++) {
//     leds[index * NUM_SQUARE_LEDS + i] = color;
//   }
// }

// void down() {  
//   if (dot < NUM_SPOTS) {
//     lightSquare(dot, CRGB(58, 163, 148));    
//     FastLED.show();

//     // clear this led for the next time around the loop
//     lightSquare(dot, CRGB(211, 173, 105));
    
//     dot++;
//     // delay(30);
//   }
// }

void lightOn(uint8_t position, uint8_t color) {
  PRINT("light: ", position);
  PRINT(" / ", color);
  PRINTS("\n");

  for (uint8_t i = 0; i < DENSITY; i++) {
    leds[position * DENSITY + i] = colors[color];
  }

  FastLED.show();
}

void lightOff(uint8_t position) {
  for (uint8_t i = 0; i < DENSITY; i++) {
    leds[position * DENSITY + i] = CRGB::Black;
  }

  FastLED.show();
}

void beginLight() {
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);

  // for(int i = 0; i < NUM_LEDS; i++) {    
  //   leds[i] = CRGB::Black;  // set our current dot to black before we continue
  //   FastLED.show();
  // }
}

// void updateLight() {
//   if (millis() - time > DELAY) {
//     down();

//     time = millis();
//   }
// }