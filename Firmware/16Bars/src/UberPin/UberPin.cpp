#include "../debug.h"
#include "UberPin.h"

UberPin::UberPin(byte pin, byte debounce_ms = 1)
{
  _pin = pin;
  _debounce_ms = debounce_ms;
  lastState = digitalRead(_pin);
  lastInterruptTime = 0;
  direction = 0;
};

byte UberPin::pin()
{
  _pin;
}
byte UberPin::read()
{
  digitalRead(_pin);
}
void UberPin::write(bool val)
{
  digitalWrite(_pin, val);
}

// returns the UberPin button state: LOW or HIGH.
// TODO remove direction
// update lastState after checking if it changed
// store direction the change was in
// make direction public
bool UberPin::changed()
{
  unsigned long interruptTime = millis();
  // If we ask for a change in less than 5ms, assume it's a bounce and ignore
  bool changedState = false;
  if (interruptTime - lastInterruptTime > _debounce_ms)
  {
    lastInterruptTime = interruptTime;
    byte currentState = digitalRead(_pin);
    if (currentState != lastState)
    {
      changedState = true;
      direction = changedState > lastState;
      // if(_pin == 2)
      // {
      // debug("changed pin "); debug(_pin);
      // debug(" current "); debug(currentState); debug(" last "); debug(lastState); debug(" direction "); debugln(direction);
      // }
    }
    else{
      changedState = false;
    }
    lastState = currentState;
  }
  return changedState;
};