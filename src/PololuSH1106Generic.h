// Copyright (C) Pololu Corporation.  See www.pololu.com for details.

/// @file PololuSH1106Generic.h
///
/// This file is not meant to be included directly:
/// include PololuOLED.h instead.

#pragma once

/// @brief SH1106 core class implemented using Arduino I/O functions.
///
/// This is a core class for use as the template parameter for PololuSH1106Base.
/// It show how to implement the low-level SH1106 communication routines needed
/// by PololuSH1106Base using standard Arduino I/O functions.
///
/// This class should work on any Arduino-compatible board, but it does not have
/// ideal performance.  In 8x2 text+graphics mode on an ATmega32U4 running at
/// 16 MHz, updating the full screen takes about 110 ms.  You can speed this up
/// by about 15 times by using an optimized class that directly writes to
/// specific I/O registers instead of using this class.
///
/// This class has several functions for configuring which pins to use, and
/// those functions should be called at the beginning of your program before
/// any functions that use the pins.
class PololuSH1106GenericCore
{
public:
  // @brief Sets the pin to use to control the SH1106 RES (reset) pin.
  void setRstPin(uint8_t pin) { rstPin = pin; }

  // @brief Sets the pin to use to control the SH1106 CLK/SCL/D0 (clock input)
  // pin.
  void setClkPin(uint8_t pin) { clkPin = pin; }

  // @brief Sets the pin to use to control the SH1106 MOSI/MOS/SI/D1
  // (data input) pin.
  void setMosiPin(uint8_t pin) { mosiPin = pin; }

  // @brief Sets the pin to use to control the SH1106 DC/A0
  // (data mode / not command mode) pin.
  //
  // If you are using 3-wire SPI mode so there is no DC pin to control, you can
  // just avoid calling this, or supply an argument of 255.
  void setDcPin(uint8_t pin) { dcPin = pin; }

  // @brief Sets the pin to use to control the SH1106 CS (chip select) pin.
  //
  // If there is no CS pin, you can just avoid calling this, or supply
  // an argument of 255.  You should ensure the CS pin of the OLED is driven
  // low at all times, or at least when you are communicating with it.
  void setCsPin(uint8_t pin) { csPin = pin; }

  // @brief This function is called by PololuSH1106Base to perform any
  // initializations that might be needed for the other functions to work
  // properly.
  void initPins()
  {
    pinMode(rstPin, OUTPUT);
    pinMode(clkPin, OUTPUT);
    digitalWrite(clkPin, LOW);
    pinMode(mosiPin, OUTPUT);
    if (dcPin != 255) { pinMode(dcPin, OUTPUT); }
    if (csPin != 255) { pinMode(csPin, OUTPUT); }
  }

  // @brief This function is called by PololuSH1106Base to reset the SH1106.
  void reset()
  {
    digitalWrite(rstPin, LOW);
    delayMicroseconds(10);
    digitalWrite(rstPin, HIGH);
    delayMicroseconds(10);
  }

  // @brief This function is called by PololuSH1106Base to start a data
  // transfer to the SH1106.
  //
  // After calling this function, PololuSH1106Base will call sh1106CommandMode(),
  // sh1106DataMode(), and sh1106Write() to send data to the SH1106, and then
  // it will call sh1106TransferEnd when it is done.
  void sh1106TransferStart()
  {
    if (csPin != 255) { digitalWrite(csPin, LOW); }
  }

  // @brief This function is called by PololuSH1106Base to end a data
  // transfer to the SH1106.
  void sh1106TransferEnd()
  {
    if (csPin != 255) { digitalWrite(csPin, HIGH); }
  }

  // @brief This function is called by PololuSH1106Base to indicate that the
  // bytes it is about to pass to sh1106Write are command bytes.
  void sh1106CommandMode()
  {
    dataMode = 0;
    if (dcPin != 255) { digitalWrite(dcPin, LOW); }
  }

  // @brief This function is called by PololuSH1106Base to indicate that the
  // bytes it is about to pass to sh1106Write are data bytes.
  void sh1106DataMode()
  {
    dataMode = 1;
    if (dcPin != 255) { digitalWrite(dcPin, HIGH); }
  }

  // @brief This function is called by PololuSH1106Base to write commands or
  // data to the SH1106.
  void sh1106Write(uint8_t d)
  {
    if (dcPin == 255)
    {
      digitalWrite(clkPin, LOW);
      digitalWrite(mosiPin, dataMode);
      digitalWrite(clkPin, HIGH);
    }

    digitalWrite(clkPin, LOW);
    digitalWrite(mosiPin, d >> 7 & 1);
    digitalWrite(clkPin, HIGH);

    digitalWrite(clkPin, LOW);
    digitalWrite(mosiPin, d >> 6 & 1);
    digitalWrite(clkPin, HIGH);

    digitalWrite(clkPin, LOW);
    digitalWrite(mosiPin, d >> 5 & 1);
    digitalWrite(clkPin, HIGH);

    digitalWrite(clkPin, LOW);
    digitalWrite(mosiPin, d >> 4 & 1);
    digitalWrite(clkPin, HIGH);

    digitalWrite(clkPin, LOW);
    digitalWrite(mosiPin, d >> 3 & 1);
    digitalWrite(clkPin, HIGH);

    digitalWrite(clkPin, LOW);
    digitalWrite(mosiPin, d >> 2 & 1);
    digitalWrite(clkPin, HIGH);

    digitalWrite(clkPin, LOW);
    digitalWrite(mosiPin, d >> 1 & 1);
    digitalWrite(clkPin, HIGH);

    digitalWrite(clkPin, LOW);
    digitalWrite(mosiPin, d >> 0 & 1);
    digitalWrite(clkPin, HIGH);
  }

private:
  uint8_t rstPin = 13, mosiPin = 13, clkPin = 13, dcPin = 255, csPin = 255;
  bool dataMode;
};


/// @brief SH1106 class implemented using Arduino I/O functions.
///
/// This uses PololUSH1106Base and PololuSH1106GenericCore to provide a
/// complete class that can be used to draw text or graphics on an SH1106
/// connected via SPI.
///
/// The first thing you should do with objects of this class is to call
/// the following functions to configure which pins you are using to
/// control the SH1106:
/// - setRstPin() (required)
/// - setClkPin() (required)
/// - setMosiPin() (required)
/// - setDcPin() (optional)
/// - setCsPin() (optional)
///
/// After specifying your pins by callin those functions, you can call the
/// functions defined by PololuSH1106Base to write to the OLED.
class PololuSH1106Generic : public PololuSH1106Base<PololuSH1106GenericCore>
{
public:
  // @brief Sets the pin to use to control the SH1106 RES (reset) pin.
  void setRstPin(uint8_t pin) { core.setRstPin(pin); }

  // @brief Sets the pin to use to control the SH1106 CLK/SCL/D0 (clock input)
  // pin.
  void setClkPin(uint8_t pin) { core.setClkPin(pin); }

  // @brief Sets the pin to use to control the SH1106 MOSI/MOS/SI/D1
  // (data input) pin.
  void setMosiPin(uint8_t pin) { core.setMosiPin(pin); }

  // @brief Sets the pin to use to control the SH1106 DC/A0
  // (data mode / not command mode) pin.
  //
  // If you are using 3-wire SPI mode so there is no DC pin to control, you can
  // just avoid calling this, or supply an argument of 255.
  void setDcPin(uint8_t pin) { core.setDcPin(pin); }

  // @brief Sets the pin to use to control the SH1106 CS (chip select) pin.
  //
  // If there is no CS pin, you can just avoid calling this, or supply
  // an argument of 255.  You should ensure the CS pin of the OLED is driven
  // low at all times, or at least when you are communicating with it.
  void setCsPin(uint8_t pin) { core.setCsPin(pin); }
};
