// This example shows how to display text and graphics on the
// OLED at the same time using an external graphics buffer that
// is created and managed by the Adafruit_SSD1306 library.
//
// This allows you to use all the drawing functions defined in
// the Adafruit_GFX class, which are listed here:
// https://github.com/adafruit/Adafruit-GFX-Library/blob/master/Adafruit_GFX.h
//
// You will need to install the Adafruit_SSD1306 library using
// the Library Manager to compile this sketch.

#include <PololuOLED.h>
#include <Adafruit_SSD1306.h>

// You will need to modify the pin numbers on the line below
// to indicate what Arduino pins to use to control the display.
// The pins are specified in this order: CLK, MOS, RES, DC, CS.
// Pass 255 for RES, DC, or CS if you are not using them.
PololuSH1106 display(1, 30, 0, 17, 13);

Adafruit_SSD1306 gfx(128, 64, 13, 13, 13, 13, 13);

void setup()
{
  if (!gfx.begin())
  {
    while (1)
    {
      Serial.println(F("Allocation failed."));
      delay(1000);
    }
  }

  display.setLayout11x4WithGraphics(gfx.getBuffer());

  delay(1000);
}

void loop()
{
  // Shapes
  gfx.clearDisplay();
  display.noAutoDisplay();
  display.clear();
  display.print(F("shapes"));
  gfx.drawRect(0, 20, 60, 44, 1);
  gfx.drawRoundRect(4, 24, 52, 36, 8, 1);
  gfx.drawCircle(98, 40, 20, 1);
  gfx.drawTriangle(68, 63, 98, 16, 127, 63, 1);
  display.display();
  delay(2000);

  // Sine wave
  gfx.clearDisplay();
  display.noAutoDisplay();
  display.clear();
  display.print(F("sine wave"));
  for (uint8_t x = 0; x < 128; x++)
  {
    gfx.drawPixel(x, 40 + 20 * sin(x * 0.09817), 1);
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
  gfx.clearDisplay();
  display.noAutoDisplay();
  display.clear();
  display.gotoXY(6, 0);
  display.print(F("barV"));
  for (uint8_t i = 0; i < 6; i++)
  {
    gfx.writeFillRect(i * 8, 64 - barGraphData[i], 6, barGraphData[i], 1);
  }
  display.display();
  delay(2000);

  // Horizontal bar graph
  gfx.clearDisplay();
  display.noAutoDisplay();
  display.clear();
  display.print(F("bargraph H"));
  for (uint8_t i = 0; i < 6; i++)
  {
    gfx.writeFillRect(0, 17 + i * 8, barGraphData[i], 6, 1);
  }
  display.display();
  delay(2000);

}
