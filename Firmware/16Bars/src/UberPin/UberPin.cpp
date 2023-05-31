#include "../debug.h"
#include "UberPin.h"

UberPin::UberPin(byte pin, byte debounce_ms = 1)
{
  _pin = pin;
  _debounce_ms = debounce_ms;
  lastState = digitalRead(_pin);
  lastInterruptTime = 0;
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
bool UberPin::changed(bool direction){
  unsigned long interruptTime = millis();
  // If we ask for a change in less than 5ms, assume it's a bounce and ignore
  if (interruptTime - lastInterruptTime > _debounce_ms)
  {
    lastInterruptTime = interruptTime;
    byte currentState = digitalRead(_pin);
    bool changedState = false;
    if (currentState != lastState){
    // if(_pin == 2)
    // {
    debug("changed pin "); debug(_pin); 
    debug(" current "); debug(currentState); debug(" last "); debug(lastState); debug(" direction "); debug(direction);
    debug(" changed "); debug(currentState != lastState); 
  // }
      lastState = currentState;
      if(currentState == direction)
    {
    debug(" in correct direction "); debugln(currentState == direction);
      // lastState = currentState;
       changedState = true;
    }
    }
  return changedState;
  }
};