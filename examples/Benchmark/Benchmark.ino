// Sketch to help test the speed and function of the display
// routines in the PololuOLED library.

#include <PololuOLED.h>

// By default we use the PololuSH1106Generic class.  Comment out
// USE_GENERIC and uncomment one of the other options if you want
// to benchmark a different class.
#define USE_GENERIC
//#define USE_POLOLU_3PI_PLUS_32U4_LIB

#ifdef USE_POLOLU_3PI_PLUS_32U4_LIB
#include <Pololu3piPlus32U4.h>
using namespace Pololu3piPlus32U4;
OLED display;
#endif

#ifdef USE_GENERIC
PololuSH1106 display(1, 30, 0, 17, 13);
#endif

uint32_t startTime;
uint32_t benchmarkTime;

uint8_t graphics[8 * 128];

const uint8_t counterClockwiseChar[] PROGMEM = {
  0b01110,
  0b10001,
  0b10101,
  0b10101,
  0b11101,
  0b00001,
  0b10001,
  0b01110,
};

const uint8_t clockwiseChar[] PROGMEM = {
  0b01110,
  0b10001,
  0b10101,
  0b10101,
  0b10111,
  0b10000,
  0b10001,
  0b01110,
};

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
  display.loadCustomCharacter(clockwiseChar, 6);
  display.loadCustomCharacter(counterClockwiseChar, 7);

  delay(1000);
}

void benchmarkStart()
{
  startTime = micros();
}

void benchmarkEnd()
{
  benchmarkTime = micros() - startTime;
}

void benchmarkReport(const __FlashStringHelper * name)
{
  uint8_t nameLength = Serial.print(name);
  Serial.print(F(": "));
  for (uint8_t i = nameLength; i < 50; i++) { Serial.print(' '); }
  Serial.println(benchmarkTime);

  delay(1000);
}

void loop()
{
  display.clear();
  display.setLayout8x2();
  display.display(); // first display takes extra time clearing RAM

  display.noAutoDisplay();
  display.gotoXY(0, 0);
  display.print(F("\6 P1   \7"));
  display.gotoXY(0, 1);
  display.print(F("\6 P2   \7"));
  display.gotoXY(0, 2);
  display.print(F("0123456789 0123456789"));
  display.gotoXY(0, 3);
  display.print(F("ABCDEFGHIJKLMNOPQRSTU"));
  display.gotoXY(0, 4);
  display.print(F("\6\6\6\6\6           \7\7\7\7\7"));
  display.gotoXY(0, 5);
  display.print(F(" \6\6\6\6\6         \7\7\7\7\7 "));
  display.gotoXY(0, 6);
  display.print(F("  \6\6\6\6\6       \7\7\7\7\7  "));
  display.gotoXY(0, 7);
  display.print(F("   \6\6\6\6\6     \7\7\7\7\7   "));

  benchmarkStart();
  display.display();
  benchmarkEnd();
  benchmarkReport(F("8x2, full update"));

  display.gotoXY(0, 0);
  benchmarkStart();
  display.print(F("\6 P3   \7"));
  benchmarkEnd();
  benchmarkReport(F("8x2, partial update, 1 char, 8 times"));

  display.gotoXY(0, 1);
  benchmarkStart();
  display.print("\6 P4   \7");
  benchmarkEnd();
  benchmarkReport(F("8x2, partial update, 8 chars"));

  display.setLayout8x2WithGraphics(graphics);
  display.display(); // first display takes extra time clearing RAM

  benchmarkStart();
  display.display();
  benchmarkEnd();
  benchmarkReport(F("8x2+graphics, full update"));

  display.gotoXY(0, 0);
  benchmarkStart();
  display.print(F("\6 P5   \7"));
  benchmarkEnd();
  benchmarkReport(F("8x2+graphics, partial update, 1 char, 8 times"));

  display.gotoXY(0, 1);
  benchmarkStart();
  display.print("\6 P6   \7");
  benchmarkEnd();
  benchmarkReport(F("8x2+graphics, partial update, 8 chars"));

  display.setLayout11x4();
  display.display(); // first display takes extra time clearing RAM

  benchmarkStart();
  display.display();
  benchmarkEnd();
  benchmarkReport(F("11x4, full update"));

  display.gotoXY(0, 0);
  benchmarkStart();
  display.print("\6 P7   \7");
  benchmarkEnd();
  benchmarkReport(F("11x4, partial update, 8 chars"));

  display.setLayout11x4WithGraphics(graphics);
  display.display(); // first display takes extra time clearing RAM

  benchmarkStart();
  display.display();
  benchmarkEnd();
  benchmarkReport(F("11x4+graphics, full update"));

  display.gotoXY(0, 1);
  benchmarkStart();
  display.print("\6 P8   \7");
  benchmarkEnd();
  benchmarkReport(F("11x4+graphics, partial update, 8 chars"));

  display.setLayout21x8();
  display.display(); // first display takes extra time clearing RAM

  benchmarkStart();
  display.display();
  benchmarkEnd();
  benchmarkReport(F("21x8, full update"));

  display.gotoXY(0, 0);
  benchmarkStart();
  display.print("\6 P9   \7");
  benchmarkEnd();
  benchmarkReport(F("21x8, partial update, 8 chars"));

  display.setLayout21x8WithGraphics(graphics);
  display.display(); // first display takes extra time clearing RAM

  benchmarkStart();
  display.display();
  benchmarkEnd();
  benchmarkReport(F("21x8+graphics, full update"));

  display.gotoXY(0, 1);
  benchmarkStart();
  display.print("\6 P10  \7");
  benchmarkEnd();
  benchmarkReport(F("21x8, partial update, 8 chars"));

  Serial.println();

  delay(1000);
}
