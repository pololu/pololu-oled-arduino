#include <PololuOLED.h>

PololuSH1106Generic display;

void setup()
{
  // TODO: display.setDcPin() ...
  delay(1000);
}

void loop()
{
  display.gotoXY(0, 0);
  display.print(millis() >> 6);
  display.print("       ");
}
