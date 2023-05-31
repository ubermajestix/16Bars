#include "../debug.h"
#include "UberPin.h"

UberPin::UberPin(byte pin, byte debounce_ms = 50){
    _pin = pin;
    _debounce_ms = debounce_ms;
    lastState = digitalRead(_pin);
};

byte UberPin::pin(){
  _pin;
}
byte UberPin::read(){
  digitalRead(_pin);
}
void UberPin::write(bool val){
  digitalWrite(_pin, val);
}

// returns the UberPin button state: LOW or HIGH.
// TODO remove direction
// update lastState after checking if it changed
// store direction the change was in
// make direction public
bool UberPin::changed(bool direction){
  static unsigned long lastInterruptTime = 0;
  unsigned long interruptTime = millis();
  // If we ask for a change in less than 5ms, assume it's a bounce and ignore
  if (interruptTime - lastInterruptTime > _debounce_ms) {
    byte currentState = digitalRead(_pin);
    if (currentState != lastState) {
        lastState = currentState;
        if (currentState == direction) {
            debug("changed ");
            debug(_pin);
            debug(" ");
            debugln(direction);
            return true;
        }
    }
    return false;
  }
  lastInterruptTime = interruptTime;   
};