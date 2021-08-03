// This sketch shows all the layouts provided by the library.

#include <PololuOLED.h>

// You will need to modify the pin numbers on the line below
// to indicate what Arduino pins to use to control the display.
// The pins are specified in this order: CLK, MOS, RES, DC, CS.
// Pass 255 for RES, DC, or CS if you are not using them.
PololuSH1106 display(1, 30, 0, 17, 13);

uint8_t graphics[8 * 128];

void drawGraphics()
{
  for (uint8_t i = 0; i < 8; i++)
  {
    // Solid diagonal line on the left side.
    graphics[i * 128 + i] = 0xFF;

    // Dotted diagonal line left of center.
    graphics[i * 128 + 32 + i] = 0xAA;

    // Dotted diagonal line right of center.
    graphics[i * 128 + (104 - i)] = 0x55;

    // Solid diagonal line on the right side.
    graphics[i * 128 + (127 - i)] = 0xFF;
  }

  // Solid bar in the center
  for (uint8_t i = 1; i < 7; i++)
  {
    for (uint8_t x = 56; x < 72; x++)
    {
      graphics[i * 128 + x] = 0xFF;
    }
  }

  // Lower corners
  graphics[7 * 128 + 0] |= 0x80;
  graphics[7 * 128 + 127] |= 0x80;
}

void setup()
{
  drawGraphics();

  for (uint8_t line = 0; line < 8; line++)
  {
    memset(display.getLinePointer(line), '0' + line, 21);
  }

  delay(1000);
}

void loop()
{
  display.setLayout8x2();  // the default
  display.display();
  delay(2000);

  display.setLayout8x2WithGraphics(graphics);
  display.display();
  delay(2000);

  display.setLayout11x4();
  display.display();
  delay(2000);

  display.setLayout11x4WithGraphics(graphics);
  display.display();
  delay(2000);

  display.setLayout21x8();
  display.display();
  delay(2000);

  display.setLayout21x8WithGraphics(graphics);
  display.display();
  delay(2000);
}
