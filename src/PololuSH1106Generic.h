#pragma once

class PololuSH1106GenericCore
{
public:
  void setRstPin(uint8_t pin) { rstPin = pin; }
  void setClkPin(uint8_t pin) { clkPin = pin; }
  void setMosiPin(uint8_t pin) { mosiPin = pin; }
  void setDcPin(uint8_t pin) { dcPin = pin; }
  void setCsPin(uint8_t pin) { csPin = pin; }

  void initPins()
  {
    pinMode(rstPin, OUTPUT);
    pinMode(clkPin, OUTPUT);
    digitalWrite(clkPin, LOW);
    pinMode(mosiPin, OUTPUT);
    if (dcPin != 255) { pinMode(dcPin, OUTPUT); }
    if (csPin != 255) { pinMode(csPin, OUTPUT); }
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
    dataMode = 0;
    if (dcPin != 255) { digitalWrite(dcPin, LOW); }
  }

  void sh1106DataMode()
  {
    dataMode = 1;
    if (dcPin != 255) { digitalWrite(dcPin, HIGH); }
  }

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
