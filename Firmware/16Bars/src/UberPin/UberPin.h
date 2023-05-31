#ifndef UberPin_h
#define UberPin_h

#include "Arduino.h"

class UberPin{
public:
  UberPin(byte pin, byte debounce_ms = 1);
  byte pin();
  byte lastState;
  bool changed();
  byte read();
  void write(bool val); 
  byte direction;

private:
  byte _pin;
  byte _debounce_ms;
  unsigned long lastInterruptTime;
};
#endif