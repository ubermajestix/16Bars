#ifndef UberPin_h
#define UberPin_h

#include "Arduino.h"

class UberPin{
public:
  UberPin(byte pin, byte debounce_ms = 50);
  byte pin();
  byte lastState;
  bool changed(bool direction);
  byte read();
  void write(bool val); 

private:
  byte _pin;
  byte _debounce_ms;
};
#endif