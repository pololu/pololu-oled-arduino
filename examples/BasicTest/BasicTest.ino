// This sketch is a basic test of the PololuSH1106Generic class.
//
// You will need to modify the pin numbers in setup() to match
// how the SH1106 is connected to the Arduino.  If you are not
// using the RST pin, then comment out the call to setRstPin.
// If you are using 3-wire SPI (no DC pin), then comment out the
// call to setDcPin.  If you are not using the CS pin, comment
// out the call to setCsPin (but make sure you are pulling it
// low some other way).
//
// This sketch should display two increasing numbers on your
// OLED.

#include <PololuSH1106.h>

PololuSH1106 display;

uint32_t count = 0;

void setup()
{
  display.setRstPin(0);
  display.setClkPin(1);
  display.setMosiPin(30);
  display.setDcPin(17);
  display.setCsPin(13);
  delay(1000);
}

void loop()
{
  display.gotoXY(0, 0);
  display.print(count);
  display.print("       ");
  display.gotoXY(0, 1);
  display.print(millis() >> 8);
  display.print("       ");
  count++;
}
