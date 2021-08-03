// Copyright (C) Pololu Corporation.  See www.pololu.com for details.

/// @file PololuSH1106Main.h

#pragma once

#include "PololuOLEDHelpers.h"

#define SH1106_SET_COLUMN_ADDR_LOW 0x00
#define SH1106_SET_COLUMN_ADDR_HIGH 0x10
#define SH1106_SET_CONTRAST 0x81
#define SH1106_SET_SEGMENT_REMAP 0xA0
#define SH1106_SET_INVERT_DISPLAY 0xA6
#define SH1106_SET_DISPLAY_ON 0xAE
#define SH1106_SET_PAGE_ADDR 0xB0
#define SH1106_SET_COM_SCAN_DIR 0xC0

/// @brief This class makes it easy to display text and graphics on a
/// 128x64 SH1106 OLED.
///
/// @section core Core class
///
/// Instead of passing pin numbers to this class to specify what pins your
/// OLED is on, you pass a core class as a first template parameter.  The
/// core class must implement the following public functions which take care of
/// communication with the SH1106 at a low level:
///
/// - void initPins();
/// - void reset();
/// - void sh1106TransferStart();
/// - void sh1106TransferEnd();
/// - void sh1106CommandMode();
/// - void sh1106DataMode();
/// - void sh1106Write(uint8_t);
///
/// For an example implementation of a core class, see PololuSH1106Core.
///
/// @section text-buffer Text buffer
///
/// This class holds a text buffer to keep track of what text to show on the
/// screen.  The clear(), write(), and print() functions all write to the
/// text buffer.
///
/// This class inherits from the Arduino Print class
/// (via PololuSH1106Main), so you can call print() with a variety of arguments.
/// See the [Arduino print() documentation](http://arduino.cc/en/Serial/Print)
/// for more information.
///
/// The default contents of the text buffer, and the contents after you call
/// clear(), consists entirely of space characters.
///
/// You can also use the getLinePointer() function to get a pointer
/// to a specific line in the text buffer, and then manipulate the characters
/// using arbitrary code.
///
/// @section font Font
///
/// This library maps each character code from 0 to 255 to a corresponding
/// 8x5 image.
///
/// Characters from 0 to 7 are custom characters: they are blank by default, but
/// you can use loadCustomCharacter() or loadCustomCharacterFromRam() to
/// specify their appearance.
///
/// Characters 8 through 31 are hardcoded to be blank.
///
/// Characters 32 through 255 come from pololuOledFont.
///
/// It is possible to change the appearance (but not the size) of the font used
/// for characters 32 through 255 without modifying this library.
/// To do so, simply copy the file font.cpp into your sketch directory,
/// remove `__attribute__((weak))`, and then make your changes.
///
/// @section graphics-buffer Graphics buffer
///
/// This class can be configured to hold a pointer to an external 1024-byte
/// graphics buffer that will get mixed (XORed) with the text.
/// Each byte of the graphics buffer represents a 1x8 vertical column of pixels,
/// with the least-significant bit holding the top-most pixel.
/// The first byte represents the pixels in the upper left corner of the screen,
/// and the bytes in the buffer are ordered left-to-right, then top-to-bottom.
/// (So byte at offset 128 is displayed immediately below the byte at offset 0.)
///
/// You can define and manipulate this graphics buffer using your own code, or
/// you can use a third-party library such as Adafruit_SSD1306 to do it.
///
/// @section lcd-compat HD44780 LCD class compatibility
///
/// This class implements clear(), gotoXY(), write(),
/// print() (provided by the Arduino Print class),
/// loadCustomCharacter(), and loadCustomCharacterFromRam(),
/// so a class based on PololuSH1106Main can be used as a
/// a drop-in replacement for a class based on PololuHD44780Base from the
/// [PololuHD44780 library](https://github.com/pololu/pololu-hd44780-arduino)
/// in most applications.
///
/// However, one difference between the SH1106 OLED and the HD44780 LCD is that
/// if you have an application that frequently clears the screen and then
/// redraws it, you will probably notice a flickering effect due to the fast
/// response time of the OLED.
/// To solve this, you can call noAutoDisplay() before clear() to tell
/// the library to refrain from writing to the display automatically.
/// Then, when you are done using functions such as clear(), gotoXY(), write(),
/// and print() to update the desired text on the screen, you can call
/// display() to write the new contents to the entire screen without flicker.
///
/// The display() function turns auto-display mode back on, so you will need to
/// call noAutoDisplay() again whenever you want to do a flickerless update.
template<class C> class PololuSH1106Main : public Print
{
public:

  PololuSH1106Main()
  {
    memset(textBuffer, ' ', sizeof(textBuffer));
    setLayout8x2();
  }

  //// Low-level helpers
private:

  void clearDisplayRam()
  {
    core.sh1106TransferStart();
    core.sh1106CommandMode();
    core.sh1106Write(SH1106_SET_COLUMN_ADDR_LOW | 2);
    for (uint8_t page = 0; page < 8; page++)
    {
      core.sh1106CommandMode();
      core.sh1106Write(SH1106_SET_PAGE_ADDR | page);
      core.sh1106Write(SH1106_SET_COLUMN_ADDR_HIGH | 0);
      core.sh1106DataMode();
      for (uint8_t i = 0; i < 128; i++)
      {
        core.sh1106Write(0);
      }
    }
    core.sh1106TransferEnd();
    clearDisplayRamOnNextDisplay = false;
  }

  void configureDefault()
  {
    core.sh1106TransferStart();
    core.sh1106CommandMode();
    core.sh1106Write(SH1106_SET_SEGMENT_REMAP | 1);  // flip horizontally
    core.sh1106Write(SH1106_SET_COM_SCAN_DIR | 8);   // flip vertically
    core.sh1106Write(SH1106_SET_CONTRAST);
    core.sh1106Write(0xFF);                // maximum brightness
    core.sh1106Write(SH1106_SET_DISPLAY_ON | 1);
    core.sh1106TransferEnd();
  }

  //////// Initialization and settings

private:

  void init2()
  {
    core.initPins();
    core.reset();
    clearDisplayRam();
    configureDefault();
    initialized = true;
  }

public:

  /// @brief Initializes the OLED if it has not already been initialized.
  ///
  /// This resets the OLED, clears the contents of the OLED RAM, sets some
  /// default settings, and turns on the display.
  ///
  /// Most users do not have to call this, because it is automatically called
  /// by any public function in this class that writes to the OLED.
  void init() { if (!initialized) { init2(); } }

  /// @brief Reinitializes the OLED and its settings.
  ///
  /// This redoes the initialization that is automatically done by init() the
  /// first time any function is called that writes to the OLED.
  ///
  /// This can be useful if you want to plug the OLED into the robot after the
  /// library has already been initialized.
  ///
  /// The screen will be blank after calling this, so you might want to call
  /// display() to show the graphics/text that you were shown before.
  void reinitialize()
  {
    init2();
  }

  /// @brief Configures the OLED to invert all the pixels, resulting in
  /// black-on-white text.
  void invert()
  {
    init();
    core.sh1106TransferStart();
    core.sh1106CommandMode();
    core.sh1106Write(SH1106_SET_INVERT_DISPLAY | 1);
    core.sh1106TransferEnd();
  }

  /// @brief Configures the OLED to not invert its pixels (the default).
  void noInvert()
  {
    init();
    core.sh1106TransferStart();
    core.sh1106CommandMode();
    core.sh1106Write(SH1106_SET_INVERT_DISPLAY | 0);
    core.sh1106TransferEnd();
  }

  /// @brief Configures the OLED to rotate its display 180 degrees from normal.
  void rotate180()
  {
    init();
    core.sh1106TransferStart();
    core.sh1106CommandMode();
    core.sh1106Write(SH1106_SET_SEGMENT_REMAP);
    core.sh1106Write(SH1106_SET_COM_SCAN_DIR);
    core.sh1106TransferEnd();
  }

  /// @brief Configures the OLED to use the default orientation.
  ///
  /// This is the orientation that results in the text shown on the
  /// [Graphical OLED Display: 128x64, 1.3", White, SPI][3760]
  /// from Pololu being the same orientation as the pin labels.
  ///
  /// [3760]: https://www.pololu.com/product/3760
  void noRotate()
  {
    init();
    core.sh1106TransferStart();
    core.sh1106CommandMode();
    core.sh1106Write(SH1106_SET_SEGMENT_REMAP | 1);
    core.sh1106Write(SH1106_SET_COM_SCAN_DIR | 8);
    core.sh1106TransferEnd();
  }

  /// @brief Sets the contrast (i.e. brightness) of the OLED.
  ///
  /// @param contrast A number between 0 (darkest, but still visible) and
  /// 255 (brightest, default).
  void setContrast(uint8_t contrast)
  {
    init();
    core.sh1106TransferStart();
    core.sh1106CommandMode();
    core.sh1106Write(SH1106_SET_CONTRAST);
    core.sh1106Write(contrast);
    core.sh1106TransferEnd();
  }

  /// @brief Configures this library to use its default layout, which allows for
  /// 8 columns and 2 rows of text.
  void setLayout8x2()
  {
    graphicsBuffer = nullptr;
    displayFunction = &PololuSH1106Main::display8x2Text;
    displayPartialFunction = &PololuSH1106Main::display8x2TextPartial;
    clearDisplayRamOnNextDisplay = true;
  }

  /// @brief Configures this library to use a layout with 8 columns and
  /// 2 rows of text, XORed with a graphics buffer.
  ///
  /// @param graphics A pointer to a 1024-byte graphics buffer.
  void setLayout8x2WithGraphics(const uint8_t * graphics)
  {
    graphicsBuffer = graphics;
    displayFunction = &PololuSH1106Main::display8x2TextAndGraphics;
    displayPartialFunction = &PololuSH1106Main::display8x2TextAndGraphicsPartial;
    clearDisplayRamOnNextDisplay = true;
  }

  /// @brief Configures this library to use a layout with
  /// 11 columns and 4 rows of text.
  ///
  /// Note that the last column of text will have one column of pixels
  /// cut off.  You can choose not to use this column or only use it
  /// for narrow characters such as punctuation.
  void setLayout11x4()
  {
    graphicsBuffer = nullptr;
    displayFunction = &PololuSH1106Main::display11x4Text;
    displayPartialFunction = &PololuSH1106Main::display11x4TextPartial;
    clearDisplayRamOnNextDisplay = true;
  }

  /// @brief Configures this library to use a layout with 11 columns and
  /// 4 rows of text, XORed with a graphics buffer.
  ///
  /// @param graphics A pointer to a 1024-byte graphics buffer.
  ///
  /// Note that the last column of text will have one column of pixels
  /// cut off.  You can choose not to use this column or only use it
  /// for narrow characters such as punctuation.
  void setLayout11x4WithGraphics(const uint8_t * graphics)
  {
    graphicsBuffer = graphics;
    displayFunction = &PololuSH1106Main::display11x4TextAndGraphics;
    displayPartialFunction = &PololuSH1106Main::display11x4TextAndGraphicsPartial;
    clearDisplayRamOnNextDisplay = true;
  }

  /// @brief Configures this library to use a layout with
  /// 21 columns and 8 rows of text.
  void setLayout21x8()
  {
    graphicsBuffer = nullptr;
    displayFunction = &PololuSH1106Main::display21x8Text;
    displayPartialFunction = &PololuSH1106Main::display21x8TextPartial;
    clearDisplayRamOnNextDisplay = true;
  }

  /// @brief Configures this library to use a layout with 21 columns and
  /// 8 rows of text, XORed with a graphics buffer.
  ///
  /// @param graphics A pointer to a 1024-byte graphics buffer.
  void setLayout21x8WithGraphics(const uint8_t * graphics)
  {
    graphicsBuffer = graphics;
    displayFunction = &PololuSH1106Main::display21x8TextAndGraphics;
    displayPartialFunction = &PololuSH1106Main::display21x8TextAndGraphicsPartial;
    clearDisplayRamOnNextDisplay = true;
  }

  //////// Display routines

private:

  uint8_t getGlyphColumn(uint8_t glyph, uint8_t pixelX)
  {
    if (glyph >= 0x20)
    {
      return pgm_read_byte(&pololuOledFont[glyph - 0x20][pixelX]);
    }
    else if (glyph < 8)
    {
      return customChars[glyph][pixelX];
    }
    else
    {
      return 0;
    }
  }

  void writePageGraphics(uint8_t page)
  {
    core.sh1106CommandMode();
    core.sh1106Write(SH1106_SET_PAGE_ADDR | page);
    core.sh1106Write(SH1106_SET_COLUMN_ADDR_HIGH | 0);
    core.sh1106Write(SH1106_SET_COLUMN_ADDR_LOW | 2);
    core.sh1106DataMode();
    const uint8_t * g = graphicsBuffer + page * 128;
    for (uint8_t x = 0; x < 128; x++) { core.sh1106Write(*g++); }
  }

  void writeSegmentUpperText(uint8_t page, uint8_t columnAddr,
    const uint8_t * text, uint8_t textLength)
  {
    core.sh1106CommandMode();
    core.sh1106Write(SH1106_SET_PAGE_ADDR | page);
    core.sh1106Write(SH1106_SET_COLUMN_ADDR_HIGH | (columnAddr >> 4));
    core.sh1106Write(SH1106_SET_COLUMN_ADDR_LOW | (columnAddr & 0xF));
    core.sh1106DataMode();
    for (uint8_t i = 0; i < textLength; i++)
    {
      uint8_t glyph = *text++;
      for (uint8_t pixelX = 0; pixelX < 5; pixelX++)
      {
        uint8_t column = PololuOLEDHelpers::repeatBits(
          getGlyphColumn(glyph, pixelX) & 0xF);
        core.sh1106Write(column);
        core.sh1106Write(column);
      }
      core.sh1106Write(0);
      core.sh1106Write(0);
    }
  }

  void writeSegmentLowerText(uint8_t page, uint8_t columnAddr,
    const uint8_t * text, uint8_t textLength)
  {
    core.sh1106CommandMode();
    core.sh1106Write(SH1106_SET_PAGE_ADDR | page);
    core.sh1106Write(SH1106_SET_COLUMN_ADDR_HIGH | (columnAddr >> 4));
    core.sh1106Write(SH1106_SET_COLUMN_ADDR_LOW | (columnAddr & 0xF));
    core.sh1106DataMode();
    for (uint8_t i = 0; i < textLength; i++)
    {
      uint8_t glyph = *text++;
      for (uint8_t pixelX = 0; pixelX < 5; pixelX++)
      {
        uint8_t column = PololuOLEDHelpers::repeatBits(
          getGlyphColumn(glyph, pixelX) >> 4);
        core.sh1106Write(column);
        core.sh1106Write(column);
      }
      core.sh1106Write(0);
      core.sh1106Write(0);
    }
  }

  void writeSegmentUpperTextAndGraphics(uint8_t page, uint8_t columnAddr,
    const uint8_t * text, uint8_t textLength)
  {
    core.sh1106CommandMode();
    core.sh1106Write(SH1106_SET_PAGE_ADDR | page);
    core.sh1106Write(SH1106_SET_COLUMN_ADDR_HIGH | (columnAddr >> 4));
    core.sh1106Write(SH1106_SET_COLUMN_ADDR_LOW | (columnAddr & 0xF));
    core.sh1106DataMode();
    const uint8_t * g = graphicsBuffer + page * 128 + (columnAddr - 2);
    for (uint8_t i = 0; i < textLength; i++)
    {
      uint8_t glyph = *text++;
      for (uint8_t pixelX = 0; pixelX < 5; pixelX++)
      {
        uint8_t column = PololuOLEDHelpers::repeatBits(
          getGlyphColumn(glyph, pixelX) & 0xF);
        core.sh1106Write(column ^ *g++);
        core.sh1106Write(column ^ *g++);
      }
      core.sh1106Write(*g++);
      core.sh1106Write(*g++);
    }
  }

  void writeSegmentLowerTextAndGraphics(uint8_t page, uint8_t columnAddr,
    const uint8_t * text, uint8_t textLength)
  {
    core.sh1106CommandMode();
    core.sh1106Write(SH1106_SET_PAGE_ADDR | page);
    core.sh1106Write(SH1106_SET_COLUMN_ADDR_HIGH | (columnAddr >> 4));
    core.sh1106Write(SH1106_SET_COLUMN_ADDR_LOW | (columnAddr & 0xF));
    core.sh1106DataMode();
    const uint8_t * g = graphicsBuffer + page * 128 + (columnAddr - 2);
    for (uint8_t i = 0; i < textLength; i++)
    {
      uint8_t glyph = *text++;
      // Note the extra check for g < graphicsBuffer + 1023, which avoids
      // reading past the end of the graphics array in 11x4 mode.
      for (uint8_t pixelX = 0; pixelX < 5 && g < graphicsBuffer + 1023; pixelX++)
      {
        uint8_t column = PololuOLEDHelpers::repeatBits(
          getGlyphColumn(glyph, pixelX) >> 4);
        core.sh1106Write(column ^ *g++);
        core.sh1106Write(column ^ *g++);
      }
      if(g < graphicsBuffer + 1023)
      {
        core.sh1106Write(*g++);
        core.sh1106Write(*g++);
      }
    }
  }

  void writePageUpperTextAndGraphics(uint8_t page, const uint8_t * text,
    uint8_t leftMargin, uint8_t textLength)
  {
    core.sh1106CommandMode();
    core.sh1106Write(SH1106_SET_PAGE_ADDR | page);
    core.sh1106Write(SH1106_SET_COLUMN_ADDR_HIGH | 0);
    core.sh1106Write(SH1106_SET_COLUMN_ADDR_LOW | 2);
    core.sh1106DataMode();
    const uint8_t * g = graphicsBuffer + page * 128;
    for (uint8_t i = 0; i < leftMargin; i++) { core.sh1106Write(*g++); }
    for (uint8_t textX = 0; textX < textLength; textX++)
    {
      uint8_t glyph = *text++;
      for (uint8_t pixelX = 0; pixelX < 5; pixelX++)
      {
        uint8_t column = PololuOLEDHelpers::repeatBits(
          getGlyphColumn(glyph, pixelX) & 0xF);
        core.sh1106Write(column ^ *g++);
        core.sh1106Write(column ^ *g++);
       }
       core.sh1106Write(*g++);
       core.sh1106Write(*g++);
    }
    for (uint8_t x = leftMargin + textLength * 12; x < 128; x++)
    {
      core.sh1106Write(*g++);
    }
  }

  void writePageLowerTextAndGraphics(uint8_t page, const uint8_t * text,
    uint8_t leftMargin, uint8_t textLength)
  {
    core.sh1106CommandMode();
    core.sh1106Write(SH1106_SET_PAGE_ADDR | page);
    core.sh1106Write(SH1106_SET_COLUMN_ADDR_HIGH | 0);
    core.sh1106Write(SH1106_SET_COLUMN_ADDR_LOW | 2);
    core.sh1106DataMode();
    const uint8_t * g = graphicsBuffer + page * 128;
    for (uint8_t i = 0; i < leftMargin; i++) { core.sh1106Write(*g++); }
    for (uint8_t textX = 0; textX < textLength; textX++)
    {
      uint8_t glyph = *text++;
      // Note the extra check for g < graphicsBuffer + 1023, which avoids
      // reading past the end of the graphics array in 11x4 mode.
      for (uint8_t pixelX = 0; pixelX < 5 && g < graphicsBuffer + 1023; pixelX++)
      {
        uint8_t column = PololuOLEDHelpers::repeatBits(
          getGlyphColumn(glyph, pixelX) >> 4);
        core.sh1106Write(column ^ *g++);
        core.sh1106Write(column ^ *g++);
       }
       if(g < graphicsBuffer + 1023)
       {
         core.sh1106Write(*g++);
         core.sh1106Write(*g++);
       }
    }
    for (uint8_t x = leftMargin + textLength * 12; x < 128; x++)
    {
      core.sh1106Write(*g++);
    }
  }

  void writeSegmentText(uint8_t page, uint8_t columnAddr,
    const uint8_t * text, uint8_t textLength)
  {
    core.sh1106CommandMode();
    core.sh1106Write(SH1106_SET_PAGE_ADDR | page);
    core.sh1106Write(SH1106_SET_COLUMN_ADDR_HIGH | (columnAddr >> 4));
    core.sh1106Write(SH1106_SET_COLUMN_ADDR_LOW | (columnAddr & 0xF));
    core.sh1106DataMode();
    for (uint8_t i = 0; i < textLength; i++)
    {
      uint8_t glyph = *text++;
      for (uint8_t pixelX = 0; pixelX < 5; pixelX++)
      {
        core.sh1106Write(getGlyphColumn(glyph, pixelX));
      }
      core.sh1106Write(0);
    }
  }

  void writeSegmentTextAndGraphics(uint8_t page, uint8_t columnAddr,
    const uint8_t * text, uint8_t textLength)
  {
    core.sh1106CommandMode();
    core.sh1106Write(SH1106_SET_PAGE_ADDR | page);
    core.sh1106Write(SH1106_SET_COLUMN_ADDR_HIGH | (columnAddr >> 4));
    core.sh1106Write(SH1106_SET_COLUMN_ADDR_LOW | (columnAddr & 0xF));
    core.sh1106DataMode();
    const uint8_t * g = graphicsBuffer + page * 128 + (columnAddr - 2);
    for (uint8_t i = 0; i < textLength; i++)
    {
      uint8_t glyph = *text++;
      for (uint8_t pixelX = 0; pixelX < 5; pixelX++)
      {
        core.sh1106Write(getGlyphColumn(glyph, pixelX) ^ *g++);
      }
      core.sh1106Write(*g++);
    }
  }

  void writePageTextAndGraphics(uint8_t page, const uint8_t * text,
    uint8_t leftMargin, uint8_t textLength)
  {
    core.sh1106CommandMode();
    core.sh1106Write(SH1106_SET_PAGE_ADDR | page);
    core.sh1106Write(SH1106_SET_COLUMN_ADDR_HIGH | 0);
    core.sh1106Write(SH1106_SET_COLUMN_ADDR_LOW | 2);
    core.sh1106DataMode();
    const uint8_t * g = graphicsBuffer + page * 128;
    for (uint8_t i = 0; i < leftMargin; i++) { core.sh1106Write(*g++); }
    for (uint8_t textX = 0; textX < textLength; textX++)
    {
      uint8_t glyph = *text++;
      for (uint8_t pixelX = 0; pixelX < 5; pixelX++)
      {
        core.sh1106Write(getGlyphColumn(glyph, pixelX) ^ *g++);
       }
       core.sh1106Write(*g++);
    }
    for (uint8_t x = leftMargin + textLength * 6; x < 128; x++)
    {
      core.sh1106Write(*g++);
    }
  }


  //// 8x2 layout //////////////////////////////////////////////////////////////
  // Character size:                 10x16
  // Character horizontal margin:    2
  // Screen left and right margins:  (128 - 8*10 - 7*2)/2 = 17
  // Line 0:                         pages 2 and 3
  // Line 1:                         pages 5 and 6

  void display8x2TextPartial(uint8_t x, uint8_t y, uint8_t width)
  {
    if (x >= 8 || y >= 2) { return; }
    if (width > (uint8_t)(8 - x)) { width = 8 - x; }
    if (width == 0) { return; }

    const uint8_t page = 2 + y * 3;
    const uint8_t columnAddr = 2 + 17 + x * 12;
    const uint8_t * const text = getLinePointer(y) + x;

    core.sh1106TransferStart();
    writeSegmentUpperText(page, columnAddr, text, width);
    writeSegmentLowerText(page + 1, columnAddr, text, width);
    core.sh1106TransferEnd();
  }

  void display8x2Text()
  {
    core.sh1106TransferStart();
    writeSegmentUpperText(2, 2 + 17, getLinePointer(0), 8);
    writeSegmentLowerText(3, 2 + 17, getLinePointer(0), 8);
    writeSegmentUpperText(5, 2 + 17, getLinePointer(1), 8);
    writeSegmentLowerText(6, 2 + 17, getLinePointer(1), 8);
    core.sh1106TransferEnd();
  }

  void display8x2TextAndGraphicsPartial(uint8_t x, uint8_t y, uint8_t width)
  {
    if (x >= 8 || y >= 2) { return; }
    if (width > (uint8_t)(8 - x)) { width = 8 - x; }
    if (width == 0) { return; }

    const uint8_t page = 2 + y * 3;
    const uint8_t columnAddr = 2 + 17 + x * 12;
    uint8_t * const text = getLinePointer(y) + x;

    core.sh1106TransferStart();
    writeSegmentUpperTextAndGraphics(page, columnAddr, text, width);
    writeSegmentLowerTextAndGraphics(page + 1, columnAddr, text, width);
    core.sh1106TransferEnd();
  }

  void display8x2TextAndGraphics()
  {
    core.sh1106TransferStart();
    writePageGraphics(0);
    writePageGraphics(1);
    writePageUpperTextAndGraphics(2, getLinePointer(0), 17, 8);
    writePageLowerTextAndGraphics(3, getLinePointer(0), 17, 8);
    writePageGraphics(4);
    writePageUpperTextAndGraphics(5, getLinePointer(1), 17, 8);
    writePageLowerTextAndGraphics(6, getLinePointer(1), 17, 8);
    writePageGraphics(7);
    core.sh1106TransferEnd();
  }

  //// 11x4 layout /////////////////////////////////////////////////////////////
  //   Character size:               10x16
  //   Character horizontal margin:  2
  //   Screen left margin:           0
  //   Screen right margin:          -2  (or 10 if you don't use the last column; off center: more room for graphics)
  //   Line 0:                       pages 0 and 1
  //   Line 1:                       pages 2 and 3
  //   Line 3:                       pages 4 and 5
  //   Line 4:                       pages 6 and 7

  void display11x4TextPartial(uint8_t x, uint8_t y, uint8_t width)
  {
    if (x >= 11 || y >= 4) { return; }
    if (width > (uint8_t)(11 - x)) { width = 11 - x; }
    if (width == 0) { return; }

    const uint8_t page = y * 2;
    const uint8_t columnAddr = 2 + x * 12;
    const uint8_t * const textStart = getLinePointer(y) + x;

    core.sh1106TransferStart();
    writeSegmentUpperText(page, columnAddr, textStart, width);
    writeSegmentLowerText(page + 1, columnAddr, textStart, width);
    core.sh1106TransferEnd();
  }

  void display11x4Text()
  {
    core.sh1106TransferStart();
    writeSegmentUpperText(0, 2, getLinePointer(0), 11);
    writeSegmentLowerText(1, 2, getLinePointer(0), 11);
    writeSegmentUpperText(2, 2, getLinePointer(1), 11);
    writeSegmentLowerText(3, 2, getLinePointer(1), 11);
    writeSegmentUpperText(4, 2, getLinePointer(2), 11);
    writeSegmentLowerText(5, 2, getLinePointer(2), 11);
    writeSegmentUpperText(6, 2, getLinePointer(3), 11);
    writeSegmentLowerText(7, 2, getLinePointer(3), 11);
    core.sh1106TransferEnd();
  }

  void display11x4TextAndGraphicsPartial(uint8_t x, uint8_t y, uint8_t width)
  {
    if (x >= 11 || y >= 4) { return; }
    if (width > (uint8_t)(10 - x)) { width = 11 - x; }
    if (width == 0) { return; }

    const uint8_t page = y * 2;
    const uint8_t columnAddr = 2 + x * 12;
    uint8_t * const text = getLinePointer(y) + x;

    core.sh1106TransferStart();
    writeSegmentUpperTextAndGraphics(page, columnAddr, text, width);
    writeSegmentLowerTextAndGraphics(page + 1, columnAddr, text, width);
    core.sh1106TransferEnd();
  }

  void display11x4TextAndGraphics()
  {
    core.sh1106TransferStart();
    writePageUpperTextAndGraphics(0, getLinePointer(0), 0, 11);
    writePageLowerTextAndGraphics(1, getLinePointer(0), 0, 11);
    writePageUpperTextAndGraphics(2, getLinePointer(1), 0, 11);
    writePageLowerTextAndGraphics(3, getLinePointer(1), 0, 11);
    writePageUpperTextAndGraphics(4, getLinePointer(2), 0, 11);
    writePageLowerTextAndGraphics(5, getLinePointer(2), 0, 11);
    writePageUpperTextAndGraphics(6, getLinePointer(3), 0, 11);
    writePageLowerTextAndGraphics(7, getLinePointer(3), 0, 11);
    core.sh1106TransferEnd();
  }

  //// 21x8 layout /////////////////////////////////////////////////////////////
  //   Character size:               5x8
  //   Character horizontal margin:  1
  //   Screen left margin:           0
  //   Screen right margin:          3  (off center: more room for graphics)
  //   Line number = Page number

  void display21x8TextPartial(uint8_t x, uint8_t y, uint8_t width)
  {
    if (x >= 21 || y >= 8) { return; }
    if (width > (uint8_t)(21 - x)) { width = 21 - x; }
    if (width == 0) { return; }

    const uint8_t columnAddr = 2 + x * 6;
    const uint8_t * const textStart = getLinePointer(y) + x;

    core.sh1106TransferStart();
    writeSegmentText(y, columnAddr, textStart, width);
    core.sh1106TransferEnd();
  }

  void display21x8Text()
  {
    core.sh1106TransferStart();
    writeSegmentText(0, 2, getLinePointer(0), 21);
    writeSegmentText(1, 2, getLinePointer(1), 21);
    writeSegmentText(2, 2, getLinePointer(2), 21);
    writeSegmentText(3, 2, getLinePointer(3), 21);
    writeSegmentText(4, 2, getLinePointer(4), 21);
    writeSegmentText(5, 2, getLinePointer(5), 21);
    writeSegmentText(6, 2, getLinePointer(6), 21);
    writeSegmentText(7, 2, getLinePointer(7), 21);
    core.sh1106TransferEnd();
  }

  void display21x8TextAndGraphicsPartial(uint8_t x, uint8_t y, uint8_t width)
  {
    if (x >= 21 || y >= 8) { return; }
    if (width > (uint8_t)(21 - x)) { width = 21 - x; }
    if (width == 0) { return; }

    const uint8_t columnAddr = 2 + x * 6;
    uint8_t * const text = getLinePointer(y) + x;

    core.sh1106TransferStart();
    writeSegmentTextAndGraphics(y, columnAddr, text, width);
    core.sh1106TransferEnd();
  }

  void display21x8TextAndGraphics()
  {
    core.sh1106TransferStart();
    writePageTextAndGraphics(0, getLinePointer(0), 0, 21);
    writePageTextAndGraphics(1, getLinePointer(1), 0, 21);
    writePageTextAndGraphics(2, getLinePointer(2), 0, 21);
    writePageTextAndGraphics(3, getLinePointer(3), 0, 21);
    writePageTextAndGraphics(4, getLinePointer(4), 0, 21);
    writePageTextAndGraphics(5, getLinePointer(5), 0, 21);
    writePageTextAndGraphics(6, getLinePointer(6), 0, 21);
    writePageTextAndGraphics(7, getLinePointer(7), 0, 21);
    core.sh1106TransferEnd();
  }

public:

  /// @brief Writes all of the text/graphics to the OLED.
  ///
  /// This also turns on auto display mode, undoing the effect of
  /// noAutoDisplay().
  ///
  /// Note that this function does not always write to the entire display:
  /// for text-only modes, it will normally only write to the portion of the
  /// screen containing text.
  void display()
  {
    init();
    if (clearDisplayRamOnNextDisplay) { clearDisplayRam(); }
    ((*this).*(displayFunction))();
    disableAutoDisplay = false;
  }

  /// @brief Writes a certain region of text/graphics to the OLED.
  ///
  /// This function is like display(), but it only writes text/graphics to the
  /// OLED which are in a region corresponding to one or more consecutive
  /// characters in a line of text.
  /// This function cannot write arbitrary regions.
  ///
  /// Most users will not need to call this because the clear(), write(), and
  /// print() functions automatically write to the display as needed.
  /// You can also use the display() function to write all of the
  /// text/graphics.
  ///
  /// However, this function is useful to users who have disabled auto
  /// display mode, or are directly writing to the text buffer, and want an
  /// efficient way to update part of the screen.
  ///
  /// @param x The row number of the text to update (0 means top row).
  /// @param y The column number of the first character to update
  ///          (0 means left-most column).
  /// @param width The number of characters to update.
  void displayPartial(uint8_t x, uint8_t y, uint8_t width)
  {
    init();
    if (clearDisplayRamOnNextDisplay) { clearDisplayRam(); }
    ((*this).*(displayPartialFunction))(x, y, width);
  }


  //////// Text

  /// @brief Turns off auto display mode.
  ///
  /// This causes the clear(), write(), and print() functions to not perform
  /// any I/O or write any data to the OLED.
  ///
  /// Calling display() will write to the display and turn auto display mode
  /// on again.
  void noAutoDisplay()
  {
    disableAutoDisplay = true;
  }

  /// @brief Gets a pointer to a line of text in this library's text buffer.
  ///
  /// This is for advanced users who want to use their own code to directly
  /// manipulate the text buffer.
  ///
  /// The returned pointer will point to a region of memory at least 21 bytes
  /// long that holds the specified line of text.  You can perform arbitrary
  /// operations on these bytes.
  ///
  /// Note that you should not assume anything about where the lines are in
  /// relation to each other, and you should not assume it is safe to write
  /// beyond the 21st byte of a line.
  ///
  /// Note that functions like snprintf will add a null (0) character at the
  /// end of their output.  This is probably undesirable if you have configured
  /// character 0 to be a custom character using loadCustomCharacter().
  /// Also, it means that the maximum content they can safely write to the text
  /// buffer is limited to 19 characters.
  uint8_t * getLinePointer(uint8_t line)
  {
    return textBuffer + line * textBufferWidth;
  }

  /// @brief Changes the location of the text cursor.
  ///
  /// This function changes the text cursor, which is the location of the text
  /// that will be overwritten by the next call to write() or print().
  ///
  /// @param x The column number (0 means left-most column).
  /// @param y The row number (0 means top row).
  void gotoXY(uint8_t x, uint8_t y)
  {
    textCursorX = x;
    textCursorY = y;
  }

  /// @brief Gets the X coordinate of the text cursor.
  uint8_t getX() { return textCursorX; }

  /// @brief Gets the Y coordinate of the text cursor.
  uint8_t getY() { return textCursorY; }

  /// @brief Moves all the text up one row. (Does not change the cursor position.)
  void scrollDisplayUp()
  {
    memmove(textBuffer, textBuffer + textBufferWidth, textBufferWidth * (textBufferHeight - 1));
    memset(textBuffer + textBufferWidth * (textBufferHeight - 1), ' ', textBufferWidth);
    if (!disableAutoDisplay) { display(); }
  }

  /// @brief Clears the text and resets the text cursor to the upper left.
  ///
  /// After calling this function, the text buffer will consist entirely of
  /// space characters.
  ///
  /// By default, this function also calls display() to write these changes
  /// to the OLED, but noAutoDisplay() disables that behavior.
  void clear()
  {
    memset(textBuffer, ' ', sizeof(textBuffer));
    gotoXY(0, 0);
    if (!disableAutoDisplay) { display(); }
  }

  /// @brief Writes a string of text.
  ///
  /// This function writes the specified string of text to the text buffer
  /// at the location specified by the text cursor (see gotoXY()), and moves
  /// the text cursor to the position immediately to the right of the string.
  ///
  /// By default, this function also calls displayPartial() to write these
  /// changes to the OLED, but noAutoDisplay() disables that behavior.
  ///
  /// There is no limit to how much text you can pass to this function, but
  /// the text will be discarded when you reach the end of the current line.
  ///
  /// To advance to the next line, use gotoXY().  The newline and carriage
  /// return characters do *not* have any special effect on the text cursor
  /// position like they might have in a terminal emulator.
  ///
  /// This function is called by (certain overloads of) the print() function
  /// provided by the Arduino print class.
  size_t write(const uint8_t * buffer, size_t size) override
  {
    if (textCursorY >= textBufferHeight) { return 0; }
    if (textCursorX >= textBufferWidth) { return 0; }
    if (size > (uint8_t)(textBufferWidth - textCursorX))
    {
      size = textBufferWidth - textCursorX;
    }

    memcpy(getLinePointer(textCursorY) + textCursorX, buffer, size);

    if (!disableAutoDisplay)
    {
      displayPartial(textCursorX, textCursorY, size);
    }

    textCursorX += size;
    return size;
  }

  /// @brief Writes a single character of text.
  ///
  /// This is equivalent to writing a single character using
  /// write(const uint8_t *, size_t).
  size_t write(uint8_t d) override
  {
    if (textCursorY >= textBufferHeight) { return 0; }
    if (textCursorX >= textBufferWidth) { return 0; }

    *(getLinePointer(textCursorY) + textCursorX) = d;

    if (!disableAutoDisplay)
    {
      displayPartial(textCursorX, textCursorY, 1);
    }

    textCursorX++;
    return 1;
  }

  /// @brief Defines a custom character from RAM.
  /// @param picture A pointer to the character dot pattern, in RAM.
  /// @param number A character code between 0 and 7.
  void loadCustomCharacterFromRam(const uint8_t * picture, uint8_t number)
  {
    uint8_t * columns = customChars[number];
    for (uint8_t i = 0; i < 5; i++)
    {
      columns[i] = 0;
    }
    for (uint8_t i = 0; i < 8; i++)
    {
      uint8_t row = picture[i];
      uint8_t mask = 1 << i;
      if (row & (1 << 0)) { columns[4] |= mask; }
      if (row & (1 << 1)) { columns[3] |= mask; }
      if (row & (1 << 2)) { columns[2] |= mask; }
      if (row & (1 << 3)) { columns[1] |= mask; }
      if (row & (1 << 4)) { columns[0] |= mask; }
    }
  }

  /// @brief Defines a custom character.
  /// @param picture A pointer to the character dot pattern, in program space.
  /// @param number A character code between 0 and 7.
  void loadCustomCharacter(const uint8_t * picture, uint8_t number)
  {
    uint8_t ram_picture[8];
    for (uint8_t i = 0; i < 8; i++)
    {
      ram_picture[i] = pgm_read_byte(picture + i);
    }
    loadCustomCharacterFromRam(ram_picture, number);
  }

  /// @brief Defines a custom character.
  ///
  /// This overload is only provided for compatibility existing code that
  /// defines char arrays instead of uint8_t arrays.
  void loadCustomCharacter(const char * picture, uint8_t number) {
    loadCustomCharacter((const uint8_t *)picture, number);
  }

  //////// Member variables and constants

  /// This object handles all low-level communication with the SH1106.
  C core;

private:

  bool initialized;

  bool clearDisplayRamOnNextDisplay;

  bool disableAutoDisplay;

  // We use the display routines through these member function pointers and are
  // careful about where we refer to them so that the routines for unused
  // layouts do not take up program space.
  void (PololuSH1106Main::*displayFunction)();
  void (PololuSH1106Main::*displayPartialFunction)(uint8_t, uint8_t, uint8_t);

  static const uint8_t textBufferWidth = 21, textBufferHeight = 8;

  uint8_t textBuffer[textBufferHeight * textBufferWidth];
  uint8_t textCursorX;
  uint8_t textCursorY;
  uint8_t customChars[8][5];

  const uint8_t * graphicsBuffer;
};
