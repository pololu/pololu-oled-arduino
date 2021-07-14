// Copyright (C) Pololu Corporation.  See www.pololu.com for details.

#pragma once

#include <Arduino.h>

/// The array is defined using the "weak" attribute, which means
/// you can supply a definition in your own program if you want to override
/// the default font.
extern const uint8_t pololuOledFont[][5] PROGMEM;

namespace PololuOLEDHelpers {

static const uint8_t repeatBitsTable[16] PROGMEM = {
  0b00000000,
  0b00000011,
  0b00001100,
  0b00001111,
  0b00110000,
  0b00110011,
  0b00111100,
  0b00111111,
  0b11000000,
  0b11000011,
  0b11001100,
  0b11001111,
  0b11110000,
  0b11110011,
  0b11111100,
  0b11111111
};

static inline uint8_t repeatBits(uint8_t d)
{
  return pgm_read_byte(repeatBitsTable + d);
}

}
