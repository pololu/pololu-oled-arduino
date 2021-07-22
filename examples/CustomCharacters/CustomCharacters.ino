// This example shows how to use the custom character feature
// of the PololuOLED library.  This is mostly available for
// compatibility with the HD44780 LCD, but it's an easy way to
// add a few special characters if you need them, without
// customizing the entire font.
//
// You are allowed to customize the first eight characters
// (numbered 0 to 7), which are blank by default.  In this
// example we have replaced them with three emoji faces, an
// integral sign, and some accented letters.

#include <PololuOLED.h>

// You will need to modify the pin numbers on the line below
// to indicate what Arduino pins to use to control the display.
// The pins are specified in this order: CLK, MOS, RES, DC, CS.
// Pass 255 for RES, DC, or CS if you are not using them.
PololuSH1106 display(1, 30, 0, 17, 13);

void loadCustomCharacters()
{
    static const char smiley[] PROGMEM = {
        0b01010,
        0b01010,
        0b01010,
        0b01010,
        0b00000,
        0b10001,
        0b01110,
        0b00000,
    };
    display.loadCustomCharacter(smiley, 0);
    static const char frown[] PROGMEM = {
        0b01010,
        0b01010,
        0b01010,
        0b01010,
        0b00000,
        0b01110,
        0b10001,
        0b00000,
    };
    display.loadCustomCharacter(frown, 1);
    static const char neutral[] PROGMEM = {
        0b01010,
        0b01010,
        0b01010,
        0b01010,
        0b00000,
        0b11111,
        0b00000,
        0b00000,
    };
    display.loadCustomCharacter(neutral, 2);
    static const char integral[] PROGMEM = {
        0b00010,
        0b00101,
        0b00100,
        0b00100,
        0b00100,
        0b00100,
        0b10100,
        0b01000,
    };
    display.loadCustomCharacter(integral, 3);
    static const char a_acute[] PROGMEM = {
        0b00010,
        0b00100,
        0b00000,
        0b01110,
        0b10001,
        0b10011,
        0b01101,
        0b00000,
    };
    display.loadCustomCharacter(a_acute, 4);
    static const char a_grave[] PROGMEM = {
        0b00100,
        0b00010,
        0b00000,
        0b01110,
        0b10001,
        0b10011,
        0b01101,
        0b00000,
    };
    display.loadCustomCharacter(a_grave, 5);
    static const char a_hat[] PROGMEM = {
        0b00100,
        0b01010,
        0b00000,
        0b01110,
        0b10001,
        0b10011,
        0b01101,
        0b00000,
    };
    display.loadCustomCharacter(a_hat, 6);
    static const char a_circle[] PROGMEM = {
        0b00100,
        0b01010,
        0b00100,
        0b01110,
        0b10001,
        0b10011,
        0b01101,
        0b00000,
    };
    display.loadCustomCharacter(a_circle, 7);
}

void setup()
{
  loadCustomCharacters();
  delay(1000);
  display.noAutoDisplay();
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
