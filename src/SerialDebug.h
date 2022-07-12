// Turn on debug statements to the serial output
#define  DEBUG  0

#if  DEBUG
#define PRINT(s, x) do { Serial.print(F(s)); Serial.print(x); } while (false)
#define PRINTS(s) do { Serial.print(F(s)); } while (false)
#define PRINTX(x) do { Serial.println(x, HEX); } while (false)
#else
#define PRINT(s, x)
#define PRINTS(x)
#define PRINTX(x)
#endif