#include <PololuOLED.h>

PololuSH1106Generic display;

void setup()
{
  // TODO: display.setDcPin() ...
  display.core.setRstPin(0);
  display.core.setSckPin(1);
  display.core.setMosiPin(30);
  display.core.setDcPin(17);
  delay(1000);
}

void loop()
{
  display.gotoXY(0, 0);
  display.print(millis() >> 6);
  display.print("       ");
}
