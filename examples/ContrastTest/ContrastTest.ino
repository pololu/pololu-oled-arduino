// This example demonstrates the "contrast" setting on the
// SH1106, which controls the brightness of all the OLED
// pixels using a value from 0 to 255.

#include <PololuOLED.h>

// You will need to modify the pin numbers on the line below
// to indicate what Arduino pins to use to control the display.
// The pins are specified in this order: CLK, MOS, RES, DC, CS.
// Pass 255 for RES, DC, or CS if you are not using them.
PololuSH1106 display(1, 30, 0, 17, 13);

void setup()
{
  delay(1000);
  display.setLayout21x8();
  for(int y = 0; y < 8; y++)
  {
    display.gotoXY(0,y);
    for(int x = 0; x < 21; x++)
    {
      display.print('#');
    }
  }


  display.gotoXY(5, 2);
  display.print("           ");
  display.gotoXY(5, 3);
  display.print(" Contrast: ");
  display.gotoXY(5, 4);
  display.print("           ");
  display.gotoXY(5, 5);
  display.print("           ");
}

void updateContrast(uint8_t c)
{
  display.setContrast(c);
  display.gotoXY(9, 4);
  if(c < 16) display.print(' ');
  display.print(c, HEX);
  delay(50);
}

void loop()
{
  for(int16_t c = 0; c < 256; c ++)
  {
    updateContrast(c);
  }
  delay(500);
  for(int16_t c = 255; c >= 0; c --)
  {
    updateContrast(c);
  }
  delay(500);
}
