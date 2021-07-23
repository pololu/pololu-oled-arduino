// This example reads serial data and displays it on the screen,
// scrolling the text up when it reaches the bottom.
// Backspace/delete and newline/carriage return are supported,
// and Ctrl+L will clear the screen.

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
  display.clear();
  Serial.begin(9600);
}

void newLine()
{
  if(display.getY() < 7)
  {
    display.gotoXY(0, display.getY()+1);
  }
  else
  {
    display.gotoXY(0, 7);
    display.scrollDisplayUp();
  }
}

void backspace()
{
  if(display.getX() == 0)
  {
    return;
  }
  display.gotoXY(display.getX() - 1, display.getY());
  display.print(' ');
  display.gotoXY(display.getX() - 1, display.getY());
}

void loop()
{  if(Serial.available())
  {
    char c = Serial.read();
    switch(c)
    {
    case '\n':
    case '\r':
      newLine();
      break;
    case '\x0c':
      display.clear();
      break;
    case '\x7f':
    case '\x08':
      backspace();
      break;
    default:
      display.print(c);
      break;
    }
    
    if(display.getX() >= 21)
    {
      newLine();
    }
  }
}
