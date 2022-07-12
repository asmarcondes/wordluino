#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>

#include "Display.h"
#include "FontsData.h"

#include "SerialDebug.h"

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 30
#define NUM_ZONES 6

#define CLK_PIN   8
#define DATA_PIN  7
#define CS_PIN    5

// Hardware SPI connection
// MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

// Arbitrary output pins
MD_Parola P = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

#define SPEED_TIME  0
#define PAUSE_TIME  0

struct sZone {
  char text[6];
  bool updateAvailable;
  char newText[6];
};

sZone zone[NUM_ZONES] = {
  {"aaaaa", false},
  {"eeeee", false},
  {"iiiii", false},
  {"ooooo", false},
  {"ooooo", false},
  {"vvvvv", false}
};

char newText[6];
bool newTextAvailable;

void beginDisplay() {
  P.begin(NUM_ZONES);

  // set the zone boundaries
  P.setZone(0, 0, 4);
  P.setZone(1, 5, 9);
  P.setZone(2, 10, 14);
  P.setZone(3, 15, 19);
  P.setZone(4, 20, 24);
  P.setZone(5, 25, 29);

  for (uint8_t i = 0; i < NUM_ZONES; i++) {
    P.displayZoneText(i, zone[i].text, PA_LEFT, SPEED_TIME, PAUSE_TIME, PA_NO_EFFECT, PA_NO_EFFECT);
    P.setCharSpacing(i, 0);
  }

  P.setFont(customFont);

  resetDisplay();

  // show the zones that have been set
  for (uint8_t i = 0; i < NUM_ZONES; i++)  {
    uint8_t a, b;
    P.getZone(i, a, b);

    PRINT("Zone ", i);
    PRINT(" [", a);
    PRINT(",", b);
    PRINTS("]\n");
  }
}

void cleanDisplay() {
  resetDisplay();
}

void changeDisplayData(char data[], uint8_t zoneIndex) {
  strcpy(zone[zoneIndex].newText, data);
  zone[zoneIndex].updateAvailable = true;
}

void resetDisplay() {
  for (uint8_t i = 0; i < NUM_ZONES; i++) {
    memset(zone[i].newText, 0, sizeof(zone[i].newText));

    if (i == 0) {
      zone[i].newText[0] = '^';
      zone[i].newText[1] = '\0';
    }

    zone[i].updateAvailable = true;
  }
}

void updateDisplay() {
  if (P.displayAnimate()) // animates and returns true when an animation is completed
  {
    for (uint8_t i = 0; i < NUM_ZONES; i++)
    {
      if (zone[i].updateAvailable) {
        if (P.getZoneStatus(i))
        {
          strcpy(zone[i].text, zone[i].newText);
          zone[i].updateAvailable = false;

          // tell Parola we have a new animation
          P.displayReset(i);
        }
      }
    }
  }
}
