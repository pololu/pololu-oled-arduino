// This sketch is a basic test of the PololuSH1106 class.
//
// It should display two increasing numbers.

#include <PololuOLED.h>

// You will need to modify the pin numbers on the line below
// to indicate what Arduino pins to use to control the display.
// The pins are specified in this order: CLK, MOS, RES, DC, CS.
// Pass 255 for RES, DC, or CS if you are not using them.
PololuSH1106 display(1, 30, 0, 17, 13);

uint32_t count = 0;

void setup()
{
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
