// This example shows how to display text and graphics on the
// OLED at the same time using an external graphics buffer.

#include <PololuOLED.h>

// You will need to modify the pin numbers on the line below
// to indicate what Arduino pins to use to control the display.
// The pins are specified in this order: CLK, MOS, RES, DC, CS.
// Pass 255 for RES, DC, or CS if you are not using them.
PololuSH1106 display(1, 30, 0, 17, 13);

// Set up our 1024-byte graphics buffer.
// Each byte of the graphics buffer represents a 1x8 vertical
// column of pixels, with the least-significant bit holding the
// top-most pixel.  The first byte represents the pixels in the
// upper left corner of the screen, and the bytes in the buffer
// are ordered left-to-right, then top-to-bottom.  (So byte at
// offset 128 is displayed immediately below the byte at offset
// 0.)
uint8_t graphics[1024];

void clearGraphics()
{
  memset(graphics, 0, sizeof(graphics));
}

void setPixel(uint8_t x, uint8_t y, bool value)
{
  if (x >= 128 || y >= 64) { return; }
  if (value)
  {
    graphics[x + (y >> 3) * 128] |= 1 << (y & 7);
  }
  else
  {
    graphics[x + (y >> 3) * 128] &= ~(1 << (y & 7));
  }
}

void drawRectangle(uint8_t topLeftX, uint8_t topLeftY,
  uint8_t width, uint8_t height)
{
  for (uint8_t x = topLeftX; x < topLeftX + width; x++)
  {
    setPixel(x, topLeftY, 1);
    setPixel(x, topLeftY + height - 1, 1);
  }

  for (uint8_t y = topLeftY; y < topLeftY + height; y++)
  {
    setPixel(topLeftX, y, 1);
    setPixel(topLeftX + width - 1, y, 1);
  }
}

void drawSolidRectangle(uint8_t topLeftX, uint8_t topLeftY,
  uint8_t width, uint8_t height)
{
  if (topLeftX >= 128) { return; }
  if (width > (uint8_t)(128 - topLeftX)) { width = 128 - topLeftX; }

  uint8_t y = topLeftY;
  for (uint8_t page = 0; page < 8; page++)
  {
    if (height == 0) { return; }
    if (y >= 8) { y -= 8; continue; }

    uint8_t heightOnThisPage = min(height, (uint8_t)(8 - y));
    uint8_t mask = ((1 << heightOnThisPage) - 1) << y;
    for (uint8_t x = topLeftX; x < topLeftX + width; x++)
    {
      graphics[x + 128 * page] |= mask;
    }
    y = 0;
    height -= heightOnThisPage;
  }
}

void setup()
{
  display.setLayout11x4WithGraphics(graphics);

  delay(1000);
}

void loop()
{
  // Rectangle
  clearGraphics();
  display.noAutoDisplay();
  display.clear();
  display.print(F("rectangle"));
  drawRectangle(16, 20, 98, 40);
  display.display();
  delay(2000);

  // Sine wave
  clearGraphics();
  display.noAutoDisplay();
  display.clear();
  display.print(F("sine wave"));
  for (uint8_t x = 0; x < 128; x++)
  {
    setPixel(x, 40 + 20 * sin(x * 0.09817), 1);
  }
  display.display();
  delay(2000);

  uint8_t barGraphData[6];
  barGraphData[0] = 11;
  barGraphData[1] = 62;
  barGraphData[2] = 64;
  barGraphData[3] = 40;
  barGraphData[4] = 1 + (millis() & 63);
  barGraphData[5] = random(1, 64);

  // Vertical bar graph
  clearGraphics();
  display.noAutoDisplay();
  display.clear();
  display.gotoXY(6, 0);
  display.print(F("barV"));
  for (uint8_t i = 0; i < 6; i++)
  {
    drawSolidRectangle(i * 8, 64 - barGraphData[i], 6, barGraphData[i]);
  }
  display.display();
  delay(2000);

  // Horizontal bar graph
  clearGraphics();
  display.noAutoDisplay();
  display.clear();
  display.print(F("bargraph H"));
  for (uint8_t i = 0; i < 6; i++)
  {
    for (uint8_t x = 0; x < barGraphData[i]; x++)
    {
      graphics[x + (i + 2) * 128] = 0b00111111;
    }
  }
  display.display();
  delay(2000);

}
