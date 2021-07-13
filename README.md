# Pololu OLED library for Arduino

## Summary

This is a library for the Arduino IDE that allows you to display text and
graphics on a 128x64 SH1106 OLED display with an SPI interface.
It works with the following Pololu products:

- [Graphical OLED Display: 128x64, 1.3", White, SPI][3760]

This library can display monospaced text without requiring you to store
the state of every pixel in RAM.

It can also display an external 1024-byte graphics buffer,
mixing it with the text.

It provides a separation between the low-level hardware access functions and
the high-level OLED update routines, so that the low-level functions can be
implemented with efficient application-specific code.

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
for other purposes while you are not writing to the OLED.


[3760]: https://www.pololu.com/product/3760
