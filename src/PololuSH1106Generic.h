#pragma once

// TODO: remove FastGPIO, make this actually be a generic class that can work
// on any Arduino board

#include <FastGPIO.h>

#define _P3PP_OLED_SEND_BIT(b) \
  FastGPIO::Pin<clkPin>::setOutputValueLow(); \
  asm volatile( \
    "sbrc %2, %3\n" "sbi %0, %1\n" \
    "sbrs %2, %3\n" "cbi %0, %1\n" \
    : : \
    "I" (FastGPIO::pinStructs[mosiPin].portAddr - __SFR_OFFSET), \
    "I" (FastGPIO::pinStructs[mosiPin].bit), \
    "r" (d), \
    "I" (b)); \
  FastGPIO::Pin<clkPin>::setOutputValueHigh();

class PololuSH1106GenericCore
{
public:
  void setRstPin(uint8_t pin) { rstPin = pin; }
  void setSckPin(uint8_t pin) { /*TODO*/ }
  void setMosiPin(uint8_t pin) { /*TODO*/ }
  void setDcPin(uint8_t pin) { dcPin = pin; }
  void setCsPin(uint8_t pin) { csPin = pin; }

  void initPins()
  {
    pinMode(rstPin, OUTPUT);
    pinMode(dcPin, OUTPUT);
    pinMode(csPin, OUTPUT);
    FastGPIO::Pin<clkPin>::setOutputLow();
  }

  void reset()
  {
    digitalWrite(rstPin, LOW);
    delayMicroseconds(10);
    digitalWrite(rstPin, HIGH);
    delayMicroseconds(10);
  }

  void sh1106TransferStart()
  {
    if (csPin != 255) { digitalWrite(csPin, LOW); }
  }

  void sh1106TransferEnd()
  {
    if (csPin != 255) { digitalWrite(csPin, HIGH); }
  }

  void sh1106CommandMode()
  {
    digitalWrite(dcPin, LOW);
  }

  void sh1106DataMode()
  {
    digitalWrite(dcPin, HIGH);
  }

  void sh1106Write(uint8_t d)
  {
    _P3PP_OLED_SEND_BIT(7);
    _P3PP_OLED_SEND_BIT(6);
    _P3PP_OLED_SEND_BIT(5);
    _P3PP_OLED_SEND_BIT(4);
    _P3PP_OLED_SEND_BIT(3);
    _P3PP_OLED_SEND_BIT(2);
    _P3PP_OLED_SEND_BIT(1);
    _P3PP_OLED_SEND_BIT(0);
  }

private:

  uint8_t rstPin = 13, dcPin = 13, csPin = 255;

  static const uint8_t mosiPin = IO_D5, clkPin = 1;
};

class PololuSH1106Generic : public PololuSH1106Base<PololuSH1106GenericCore>
{
};
