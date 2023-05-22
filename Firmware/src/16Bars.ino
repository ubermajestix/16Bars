#include <Debounce.h>

// only pins 2 and 3 can interrupt
const byte clockSignalPin = 2;    // Connect your clock square wave signal here
const byte ledPin = LED_BUILTIN;  // Built-in LED
const byte resetButtonPin = 3;    // Reset button, push to reset counters to zero
const byte clockBarPin = 4;       // Outputs square wave every musical bar
const byte resetSignalPin = 5;    // Reset signal to reset seven segment LEDs
const byte triggerWidth = 50;     // milliseconds

// Debounce resetButton(resetButtonPin, 50, false); // Provides 50ms debounced button with methods #count and #resetCount
volatile byte beatCounter = 0;  // Counter for the clock pulses
volatile byte barCounter = 0;   // Counter for the musical bars
volatile byte barsPerPhrase = 16;
volatile byte beatsPerMeasure = 4;
volatile bool clockBarState = LOW;  // Led State, initially set to LOW
volatile bool resetOutputState = LOW;
volatile bool resetButtonState = LOW;     
volatile bool ledState = LOW;      
volatile bool clockState = LOW;
volatile bool lastClockState = LOW;
volatile bool lastResetButtonState = LOW;




void setup() {
  pinMode(clockSignalPin, INPUT);
  pinMode(resetButtonPin, INPUT);
  pinMode(clockBarPin, OUTPUT);
  pinMode(resetSignalPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  Serial.print("setup ");
}

void loop() {
  // TODO can we get the counters to count from 1 to 16 instead of 0 to 15? If barCounter == 0 send fast trigger to get leds to display 01 before next clock pulse?

    // TODO loop is reading the clock high see if clock changed? We were getting two loops within a short clock trigger need to detect rising edge
    // Alternatively set the loop speed to be longer than a clock, this doesn't handle gates, so we need to detect edges.
    // -- Loop 0
    // clock = LOW
    // lastClock set to LOW
    // NO OP clock not high
    // -- Loop 1
    // clock = HIGH
    // lastClock = LOW
    // Increment!
    // lastClock set to HIGH
    // --- Loop 2
    // clock = HIGH
    // lastClock = HIGH
    // NO OP, lastClock not low wait for clock to go low
    // -- Loop 3
    // clock = LOW
    // lastClock set to LOW
    // NO OP, clock not high
  lastClockState = clockState;
  lastResetButtonState = resetButtonState;
  clockState = digitalRead(clockSignalPin);
  if(clockState == LOW){
    lastClockState = LOW;
  }
  // bool resetButtonState = resetButton.read();
  if (resetButtonState && !lastResetButtonState) {
    Serial.println("----resetbutton---");
    barCounter = 0;
    beatCounter = 0;
    // digitalWrite(resetSignalPin, HIGH);
    resetOutputState = HIGH;
  }
  if (clockState == HIGH && lastClockState == LOW) {
    lastClockState = clockState;
    incrementCounters();
  }
}
// This function will be called whenever a rising edge is detected from the clock
void incrementCounters() {
  Serial.println("-------clock------");

  digitalWrite(clockBarPin, clockBarState);
  digitalWrite(resetSignalPin, resetOutputState);
  clockBarState = LOW;
  resetOutputState = LOW;

  beatCounter++;
  Serial.print("beat ");
  Serial.println(beatCounter);
  if (beatCounter >= beatsPerMeasure) {
    barCounter++;
    clockBarState = HIGH;
    // digitalWrite(clockBarPin, clockBarState);

    beatCounter = 0;
  }
  if (barCounter >= barsPerPhrase) {
    resetOutputState = HIGH;
    barCounter = 0;
    beatCounter = 0;
    // digitalWrite(resetSignalPin, resetOutputState);

    // TODO about to reset check state of switches for beatsPerMeasure and barsPerPhrase
  }

  Serial.print("bar ");
  Serial.println(barCounter);
  Serial.print("clockBar ");
  Serial.println(clockBarState);
  Serial.print("reset ");
  Serial.println(resetOutputState);
}


// TODO figure out different beat counters (4 or 8) based on switch
// TODO figure out time signatures 4/4, 8/4, 6/8, 7/8 -> how to pick numerator and denominator?
// IDEA: select numerator only with rotary encoder, 4,6,7,8,9,11,12,16 (8 position)
//       denominator depends on clock signal sent:
//       1ppqn (quarter notes): 4/4 select 4, 8/4 select 8, 11/4 select 11
//       2ppqn (8ths): 4/4 select 8, 7/8 select 7, 11/8 select 11
//       4ppqn (16ths); 4/4 select 16
//
