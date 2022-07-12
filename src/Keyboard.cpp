#include "ShiftIn.h"
#include "Keyboard.h"

#define S_DATA_PIN 2        // Q7
#define LATCH_PIN 4         // PL - to latch the inputs into the registers
#define S_CLK_PIN 3         // CP
#define CLK_ENABLED_PIN 9   // CE - won't use, probably

const int8_t numberKeys = 28;

ShiftIn<4> shift;

void beginKeyboard() {
  shift.begin(LATCH_PIN, CLK_ENABLED_PIN, S_DATA_PIN, S_CLK_PIN);
}

bool keyboardStatusChanged() {
  // Read in all values and returns
  // true if any button has changed
  return shift.update();
}

byte countKeys() {
  return numberKeys;
}

byte keyStatus(uint8_t index) {
  return shift.state(index);
}