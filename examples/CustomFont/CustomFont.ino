// This example shows how to customize the font used by the
// OLED using an X Bitmap file.  See font.cpp for details on
// making a custom font file. 

#include <PololuOLED.h>

// You will need to modify the pin numbers on the line below
// to indicate what Arduino pins to use to control the display.
// The pins are specified in this order: CLK, MOS, RES, DC, CS.
// Pass 255 for RES, DC, or CS if you are not using them.
PololuSH1106 display(1, 30, 0, 17, 13);

void setup()
{
  display.noAutoDisplay();

  display.setLayout11x4();
  display.gotoXY(0,1);
  display.print("Customized");
  display.gotoXY(0,2);
  display.print("OLED font");
  display.display();
  delay(2000);
  
  display.setLayout21x8();
}

void loop()
{
  for(int y=0; y<8; y++)
  {
    display.gotoXY(0,y);
    display.print(y, HEX);
    display.print(": ");
    for(int x=0; x<16; x++)
    {
      display.print((char)(y*16+x));
    }
    display.print(" :");
  }
  display.display();
  delay(1000);
}
