// This is a simple screensaver-like example combining text
// and graphics.
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

struct MovingPoint
{
  int x, y, vx, vy;
};

MovingPoint p1 = {0, 0, 1, 5};
MovingPoint p2 = {30, 20, 3, 4};
MovingPoint op1 = p1;
MovingPoint op2 = p2;
int count = 10;

const int XMAX=127, YMAX=63;

void setup()
{
  delay(1000);

  if (!gfx.begin())
  {
    while (1)
    {
      Serial.println(F("Allocation failed."));
      delay(1000);
    }
  }

  display.setLayout8x2WithGraphics(gfx.getBuffer());

  gfx.clearDisplay();
  display.noAutoDisplay();
  display.print(F("Text and"));
  display.gotoXY(0,1);
  display.print(F("graphics"));
}

void move(MovingPoint &p)
{
  p.x += p.vx;
  p.y += p.vy;
  if(p.x > XMAX || p.x < 0) { p.vx = -p.vx; }
  if(p.y > YMAX || p.y < 0) { p.vy = -p.vy; }
}

void loop()
{
  // draw the leading line
  gfx.drawLine(p1.x, p1.y, p2.x, p2.y, SSD1306_INVERSE);
  move(p1);
  move(p2);
  if(count == 0)
  {
    // COUNT steps behind, draw a line to erase the old lines.
    // Note that both drawing functions use the INVERSE option,
    // which handles overlapping in a simple way.
    gfx.drawLine(op1.x, op1.y, op2.x, op2.y, SSD1306_INVERSE);
    move(op1);
    move(op2);
  }
  else
  {
    count -= 1;
  }

  display.display();
}
