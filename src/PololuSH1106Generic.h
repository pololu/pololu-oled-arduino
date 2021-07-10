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
  // Pin assignments
  static const uint8_t mosiPin = IO_D5, clkPin = 1, dcPin = 17, rstPin = 0;

public:
  void initPins()
  {
    FastGPIO::Pin<clkPin>::setOutputLow();
  }

  void reset()
  {
    FastGPIO::Pin<rstPin>::setOutputLow();
    delayMicroseconds(10);
    FastGPIO::Pin<rstPin>::setOutputHigh();
    delayMicroseconds(10);
  }

  void sh1106TransferStart()
  {
  }

  void sh1106TransferEnd()
  {
  }

  void sh1106CommandMode()
  {
    FastGPIO::Pin<dcPin>::setOutputLow();
  }

  void sh1106DataMode()
  {
    FastGPIO::Pin<dcPin>::setOutputHigh();
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
};

class PololuSH1106Generic : public PololuSH1106Base<PololuSH1106GenericCore>
{
};
