#include "../debug.h"
#include "UberPin.h"

UberPin::UberPin(byte pin, byte debounce_ms = 5){
    pin = pin;
    debounce_ms = debounce_ms;
    lastState = digitalRead(pin);
};

byte UberPin::read(){
  digitalRead(pin);
}
void UberPin::write(bool val){
  digitalWrite(pin, val);
}

// returns the UberPin button state: LOW or HIGH.
bool UberPin::changed(bool direction){
  static unsigned long lastInterruptTime = 0;
  unsigned long interruptTime = millis();
  debug('changed ');
  debugln(interruptTime - lastInterruptTime);
  // If we ask for a change in less than 5ms, assume it's a bounce and ignore
  if (interruptTime - lastInterruptTime > debounce_ms) {
    byte currentState = digitalRead(pin);
    debug('last state ');
    debug(lastState);
    debug(' currentState ');
    debugln(currentState);
    if (currentState != lastState) {
        lastState = currentState;
        if (currentState == direction) {
            return true;
        }
    }
    return false;
  }
  lastInterruptTime = interruptTime;   
};