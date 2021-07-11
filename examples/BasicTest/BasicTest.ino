#include <PololuOLED.h>

PololuSH1106Generic display;

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
