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
    pinMode(dcPin, OUTPUT);
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
    digitalWrite(dcPin, LOW);
  }

  void sh1106DataMode()
  {
    digitalWrite(dcPin, HIGH);
  }

  void sh1106Write(uint8_t d)
  {
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
  uint8_t rstPin = 13, mosiPin = 13, clkPin = 13, dcPin = 13, csPin = 255;
};

class PololuSH1106Generic : public PololuSH1106Base<PololuSH1106GenericCore>
{
public:
  void setRstPin(uint8_t pin) { core.setRstPin(pin); }
  void setClkPin(uint8_t pin) { core.setClkPin(pin); }
  void setMosiPin(uint8_t pin) { core.setMosiPin(pin); }
  void setDcPin(uint8_t pin) { core.setDcPin(pin); }

  // If there is no CS pin, you can just avoid calling this, or supply
  // an argument of 255.
  void setCsPin(uint8_t pin) { core.setCsPin(pin); }
};
