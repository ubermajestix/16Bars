#ifndef UberPin_h
#define UberPin_h

#include "Arduino.h"

class UberPin{
public:
  UberPin(byte pin, byte debounce_ms = 5);
  byte pin;
  bool changed(bool direction);
  byte read();
  void write(bool val); 

private:
  byte debounce_ms;
  byte lastState;
};
#endif