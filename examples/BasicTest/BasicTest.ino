#include <PololuOLED.h>

PololuSH1106Generic display;

void setup()
{
  display.setRstPin(0);
  display.setClkPin(1);
  display.setMosiPin(30);
  display.setDcPin(17);
  display.setCsPin(13);
  delay(1000);
}

uint16_t count = 0;

void loop()
{
  display.gotoXY(0, 0);
  display.print(count);
  display.print("       ");
  count++;
}
