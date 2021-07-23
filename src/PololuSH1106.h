// Copyright (C) Pololu Corporation.  See www.pololu.com for details.

/// @file PololuSH1106.h

#pragma once

#include "PololuSH1106Main.h"

/// @brief SH1106 core class implemented using Arduino I/O functions.
///
/// This is a core class for use as the template parameter for PololuSH1106Main.
/// It shows how to implement the low-level SH1106 communication routines needed
/// by PololuSH1106Main using standard Arduino I/O functions.
///
/// This class should work on any Arduino-compatible board, but it does not have
/// ideal performance.  In 8x2 text+graphics mode on an ATmega32U4 running
/// at 16 MHz, updating the full screen takes about 110 ms.  You can speed this
/// up by about 15 times by using an optimized class that directly writes to
/// specific I/O registers instead of using this class.
class PololuSH1106Core
{
public:
  /// @brief Configures what pins this class will use.
  ///
  /// This function should be called at the beginning of your program
  /// before any functions that use the pins.
  ///
  /// For documentation of each parameter, see PololuSH1106::PololuSH1106().
  void setPins(uint8_t clk, uint8_t mos, uint8_t res = 255, uint8_t dc = 255,
    uint8_t cs = 255)
  {
    clkPin = clk;
    mosPin = mos;
    resPin = res;
    dcPin = dc;
    csPin = cs;
  }

  /// @brief This function is called by PololuSH1106Main to perform any
  /// initializations that might be needed for the other functions to work
  /// properly.
  void initPins()
  {
    if (resPin != 255) { pinMode(resPin, OUTPUT); }
    pinMode(clkPin, OUTPUT);
    digitalWrite(clkPin, LOW);
    pinMode(mosPin, OUTPUT);
    if (dcPin != 255) { pinMode(dcPin, OUTPUT); }
    if (csPin != 255) { pinMode(csPin, OUTPUT); }
  }

  /// @brief This function is called by PololuSH1106Main to reset the SH1106.
  void reset()
  {
    if (resPin == 255) { return; }
    digitalWrite(resPin, LOW);
    delayMicroseconds(10);
    digitalWrite(resPin, HIGH);
    delayMicroseconds(10);
  }

  /// @brief This function is called by PololuSH1106Main to start a data
  /// transfer to the SH1106.
  ///
  /// After calling this function, PololuSH1106Main will call sh1106CommandMode(),
  /// sh1106DataMode(), and sh1106Write() to send data to the SH1106, and then
  /// it will call sh1106TransferEnd() when it is done.
  void sh1106TransferStart()
  {
    pinMode(clkPin, OUTPUT);
    digitalWrite(clkPin, LOW);
    pinMode(mosPin, OUTPUT);
    if (csPin != 255) { digitalWrite(csPin, LOW); }
  }

  /// @brief This function is called by PololuSH1106Main to end a data
  /// transfer to the SH1106.
  void sh1106TransferEnd()
  {
    if (csPin != 255) { digitalWrite(csPin, HIGH); }
  }

  /// @brief This function is called by PololuSH1106Main to indicate that the
  /// bytes it is about to pass to sh1106Write are command bytes.
  void sh1106CommandMode()
  {
    dataMode = 0;
    if (dcPin != 255) { digitalWrite(dcPin, LOW); }
  }

  /// @brief This function is called by PololuSH1106Main to indicate that the
  /// bytes it is about to pass to sh1106Write are data bytes.
  void sh1106DataMode()
  {
    dataMode = 1;
    if (dcPin != 255) { digitalWrite(dcPin, HIGH); }
  }

  /// @brief This function is called by PololuSH1106Main to write commands or
  /// data to the SH1106.
  void sh1106Write(uint8_t d)
  {
    if (dcPin == 255)
    {
      digitalWrite(clkPin, LOW);
      digitalWrite(mosPin, dataMode);
      digitalWrite(clkPin, HIGH);
    }

    digitalWrite(clkPin, LOW);
    digitalWrite(mosPin, d >> 7 & 1);
    digitalWrite(clkPin, HIGH);

    digitalWrite(clkPin, LOW);
    digitalWrite(mosPin, d >> 6 & 1);
    digitalWrite(clkPin, HIGH);

    digitalWrite(clkPin, LOW);
    digitalWrite(mosPin, d >> 5 & 1);
    digitalWrite(clkPin, HIGH);

    digitalWrite(clkPin, LOW);
    digitalWrite(mosPin, d >> 4 & 1);
    digitalWrite(clkPin, HIGH);

    digitalWrite(clkPin, LOW);
    digitalWrite(mosPin, d >> 3 & 1);
    digitalWrite(clkPin, HIGH);

    digitalWrite(clkPin, LOW);
    digitalWrite(mosPin, d >> 2 & 1);
    digitalWrite(clkPin, HIGH);

    digitalWrite(clkPin, LOW);
    digitalWrite(mosPin, d >> 1 & 1);
    digitalWrite(clkPin, HIGH);

    digitalWrite(clkPin, LOW);
    digitalWrite(mosPin, d >> 0 & 1);
    digitalWrite(clkPin, HIGH);
  }

private:
  uint8_t clkPin = 13, mosPin = 13, resPin = 255, dcPin = 255, csPin = 255;
  bool dataMode;
};

/// @brief Generic SH1106 class implemented using Arduino I/O functions.
class PololuSH1106 : public PololuSH1106Main<PololuSH1106Core>
{
public:
  /// @brief Creates a new instance of PololuSH1106.
  ///
  /// @param clk The pin to use to control the
  ///   SH1106 CLK/SCL/D0 (clock input) pin.
  /// @param mos The pin to use to control the
  ///   SH1106 MOS/MOSI/SI/D1 (data input) pin.
  /// @param res The pin to use to control the
  ///   SH1106 RES (reset) pin.
  ///   If you are not using the RES pin, you can pass 255.
  /// @param dc The pin to use to control the
  ///   SH1106 DC/A0 (data mode / not command mode) pin.
  ///   If your display module uses 3-wire SPI mode so there is no DC pin to
  ///   control, you should pass 255.
  /// @param cs  Sets the pin to use to control the
  ///   SH1106 CS (chip select) pin.
  ///   If you are not using the CS pin, you can pass 255.
  ///   In that case, you should ensure the SH1106's CS pin is driven low when
  ///   you are communicating with it.
  PololuSH1106(uint8_t clk, uint8_t mos, uint8_t res = 255, uint8_t dc = 255,
    uint8_t cs = 255)
  {
    core.setPins(clk, mos, res, dc, cs);
  }
};
