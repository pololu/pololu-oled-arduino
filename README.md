# Pololu OLED library for Arduino

## Summary

This is a library for the Arduino IDE that allows you to display text and
graphics on a 128x64 SH1106 OLED display with an SPI interface.
It works with the following Pololu products:

- [Graphical OLED Display: 128x64, 1.3", White, SPI][3760]

This library can display text without requiring you to store the state of every
pixel in RAM.

It can also display an external graphics buffer, combining the graphics and text
together as it writes to the display.

It provides a separation between the low-level hardware access functions and
the high-level OLED update routines, so that the low-level functions can be
implemented with efficient application-specific code.

Applications written for the [PololuHD44780 LCD library]
can often be ported to this library with minimal effort since it provides
the `clear`, `gotoXY`, `write`, `print`, and `loadCustomCharacter` functions.


## Supported platforms

This library should work on any Arduino-compatible board.

## Getting started

### Hardware

An [SH1106 OLED display with an SPI interface][3760] can be purchased from
Pololu's website.

You will need to connect your Arduino's GND pin to the ground pin of the
display module, and you will also need to provide an appropriate power source
to the display.  Consult your display's documentation to learn how to power it.

**Consult your display module's documentation to see what
voltage levels its I/O pins require, and add level shifters if your
Arduino I/O voltages are outside of that range.**

Choose two I/O pins of your Arduino and connect them to your module's
CLK/SCL/D0 (clock input) pin and its MOSI/MOS/SI/D1 (data input) pin, using
level shifters if required.

If your module is configured for 4-wire SPI mode, choose another I/O pin
and connect it to your module's DC/A0 (data mode / not command mode) pin,
using a level shifter if required.

If possible, chose another I/O pin and connect it to your module's
RES (reset) pin, using a level shifter if required.  This pin is optional, but
connecting it will make this library more reliable because it will be able to
reset the OLED to known state.

You can also connect another I/O pin to your module's CS (chip select) pin.
This is optional, but it allows you to use the clock and data pins
for other purposes while you are not updated the OLED.


### Software

You can install this library using the Arduino library manager:

1. In the Arduino IDE, open the "Tools" menu and select "Manage Libraries...".
2. Search for "PololuOLED".
3. Click the PololuOLED entry in the list.
4. Click "Install".

If this does not work, you can manually install the library:

1. Download the
   [latest release archive from GitHub](https://github.com/pololu/pololu-oled-arduino/releases)
   and decompress it.
2. Rename the folder "pololu-oled-arduino-xxxx" to "PololuOLED".
3. Drag the "PololuOLED" folder into the "libraries" directory inside your
   Arduino sketchbook directory. You can view your sketchbook location by
   opening the "File" menu and selecting "Preferences" in the Arduino IDE. If
   there is not already a "libraries" folder in that location, you should make
   the folder yourself.
4. After installing the library, restart the Arduino IDE.

### Examples

Several example sketches are available that show how to use the library.
You can access them from the Arduino IDE by opening the "File" menu, selecting
"Examples", and then selecting "PololuOLED".  If you cannot find these
examples, the library was probably installed incorrectly and you should
retry the installation instructions above.

### Documentation

For complete documentation of this library, see the
https://pololu.github.io/pololu-oled-arduino.

In particular the documentation of the PololuSH1106Main class gives a good
overview of how the library works and lists all the functions you can call to
display graphics or text.

### Version history

* 1.0.0: Original release.

[3760]: https://www.pololu.com/product/3760
[PololuHD44780 LCD library]: https://github.com/pololu/pololu-hd44780-arduino
