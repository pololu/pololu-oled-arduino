#include <Arduino.h>

// This file uses a #define hack to include a font file stored in a special
// C-compatible format called X BitMap or "xbm".  To edit the font, you will
// need an editor that can work with this format, such as GIMP:
//
//   https://www.gimp.org/
//
// For compatibility with the PololuOLED library, characters run
// vertically in an 8-by-1120 pixel image, using five rows per character
// and no space in between.  Also, the characters are mirrored, to match the
// bit order used in the library.
//
// It is also allowed to use a smaller (shorter) image file, if you are not
// using all the characters.  In this example, we use a font file that defines
// only the 96 characters from 0x20 to 0x8f; non-ASCII characters above 0x8f
// are excluded.
//
// You can open original_font.xbm as a starting point.
//
// Consider rotating and mirroring the original font file so that you can see
// the letters in their proper orientation, transforming them back before
// exporting the font to "font.xbm" and renaming to "font.h".
//
// Open font.h in a text editor to make sure that it declares a variable
// exactly like this:
//
//   static unsigned char font_bits[]
//
// Our "hack" is to redefine the name font_bits as a macro below to turn that
// declaration into a replacement of the variable pololuOledFont in the
// library.  If it doesn't match exactly, you will either get an error or
// still have the original font.

#define font_bits unused_placeholder; extern const uint8_t PROGMEM pololuOledFont
#include "font.h"
