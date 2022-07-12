enum color {
  green,
  yellow,
  black
};

void beginLight();
void lightOn(uint8_t position, uint8_t color);
void lightOff(uint8_t position);
// void updateLight();