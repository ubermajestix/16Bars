#include "src/debug.h"
#include "src/UberPin/UberPin.h"

// TIP: for knockoff Arduino Nanos you may need a CH340 driver from Jan 2019 or
// earlier to upload this sketch or communicate with the board via USB.
// The CH340 driver can be found at Sparkfun: 
// https://learn.sparkfun.com/tutorials/how-to-install-ch340-drivers/all

const byte clockPinNum = 2;
const byte resetButtonPinNum = 3;
const byte clockBarPinNum = 4;
const byte resetOutPinNum = 5;
const byte barsPerPhrasePinNum = A4;
const byte beatsPerBarPins[5] = {7, 8, 9, 10, 11};
const byte firstBarOutPin = 12;
const byte middleBarOutPin = 13;
const byte firstBarLEDPin = A1;
// FIXME PCB layout put LEDs in the wrong spots, swapping pins to fix it.
const byte middleBarLEDPin = A2;
// const byte middleBarLEDPin = A3;
const byte lastBarOutPin = A0;
// const byte lastBarLEDPin = A2;
const byte lastBarLEDPin = A3;
const byte ledPins[3] = {firstBarLEDPin, middleBarLEDPin, lastBarLEDPin};

const byte beatsPerBarValues[5] = {4, 7, 8, 12, 16};
const byte barsPerPhraseValues[2] = {8,16}; 

// TESTING Internal clock settings
volatile unsigned long enterInternalClockModeTime = 0; // Enter this mode if reset button pressed for 3 seconds within 10 seconds of startup
volatile byte internalClock = 0; // Testing mode active when 1
const unsigned long internalBPM = 345; // 174 bpm 1/4 note is 345ms
volatile unsigned long lastClockTime = 0; // For auto-reset and internal clock feature. // TODO use "Last" output clock to self patch for testing?


UberPin clockPin(clockPinNum);
UberPin resetButtonPin(resetButtonPinNum, 5);
UberPin clockBarPin(clockBarPinNum);
UberPin resetOutPin(resetOutPinNum);

volatile byte beatCounter = 0; // Counter for the clock pulses
volatile byte barCounter = 0;  // Counter for bars
// Set default values for barPerPhrase and beatsPerBar but in setup() we'll select values off the slider inputs.
volatile byte barsPerPhrase = barsPerPhraseValues[1]; // Default is 16 bars because, uh, that's the name of the project.
volatile byte beatsPerBar= beatsPerBarValues[0]; // Default is 4. Can be set to 4, 7, 8, 12, 16 with switch 
volatile byte resetButtonPressed = 0;
volatile bool clockBarState = LOW; // Led State, initially set to LOW
volatile bool resetPhrase = LOW;
// TODO add separate extneral outputs for clockbar (firstbar == reset)
// TODO add another out 1 measure before middle bar?
volatile bool firstBar = LOW;
volatile bool middleBar = LOW;
volatile bool lastBar = LOW;

void setup(){
  pinMode(clockPinNum, INPUT);
  pinMode(barsPerPhrasePinNum, INPUT_PULLUP);
  pinMode(resetButtonPinNum, INPUT_PULLUP);
  // Set up the beatsPerBar 5 position switch pins as inputs
  for (int i = 0; i < 5; i++) {
    pinMode(beatsPerBarPins[i], INPUT);
  }
  pinMode(clockBarPinNum, OUTPUT);
  pinMode(resetOutPinNum, OUTPUT);
  pinMode(firstBarOutPin, OUTPUT);
  pinMode(firstBarLEDPin, OUTPUT);
  pinMode(middleBarOutPin, OUTPUT);
  pinMode(middleBarLEDPin, OUTPUT);
  pinMode(lastBarOutPin, OUTPUT);
  pinMode(lastBarLEDPin, OUTPUT);
  Serial.begin(9600);
  flashLedStartup();
  changeBeatsAndBars();

}

void flashLedStartup(){
  resetOutPin.write(HIGH);
  delay(10);
  resetOutPin.write(LOW);

  // Count from 0 to 16 then pause on 16 for 1 second
  for (int i = 0; i < 16; i++)
  {
    clockBarPin.write(HIGH);
    delay(50);
    if(i == 15){
      delay(650);
    }
    clockBarPin.write(LOW);
    delay(50);
  }

  // Set Counter to "01" instead of "00" on startup before LEDs flash
  resetOutPin.write(HIGH);
  resetOutPin.write(LOW);
  clockBarPin.write(HIGH); 
  clockBarPin.write(LOW);

  // Flash the leds three times
  for (int i = 0; i < 3; i++)
  {
    flashLEDs();
  }
}

void flashLEDs(){
 for (int i = 0; i < 3; i++)
  {
    digitalWrite(ledPins[i], HIGH);
    delay(50);
    digitalWrite(ledPins[i], LOW);
  }
 for (int i = 2; i >= 0; i--)
  {
    digitalWrite(ledPins[i], HIGH);
    delay(50);
    digitalWrite(ledPins[i], LOW);
  }
}
void flashLEDSync(){
  digitalWrite(ledPins[0], LOW);
  digitalWrite(ledPins[1], LOW);
  digitalWrite(ledPins[2], LOW); 
  digitalWrite(ledPins[0], HIGH);
  digitalWrite(ledPins[1], HIGH);
  digitalWrite(ledPins[2], HIGH);
  delay(150);
  digitalWrite(ledPins[0], LOW);
  digitalWrite(ledPins[1], LOW);
  digitalWrite(ledPins[2], LOW); 
}

void loop(){
  // Enter internal clock mode if reset button pressed and held for 3 seconds within 10 seconds of startup
  if(millis() <= 10000 && internalClock != 1 && enterInternalClockModeTime != 0 && millis() - enterInternalClockModeTime >= 3000){
    debugln("----Internal Clock ---");
    for (int i = 0; i < 3; i++){flashLEDSync(); delay(150);}
    internalClock = 1;
  }
  // Auto reset the counters if we haven't seen a clock signal for 4 seconds. 
  if(lastClockTime != 0 && millis() - lastClockTime >= 4000){
    debugln("----AUTO RESET ------");
    resetCounters();
  }
  
  if (resetButtonPin.changed()){
    if (resetButtonPin.direction == 1){
      debugln("---Reset Release---");
      resetOutPin.write(LOW);
      resetButtonPressed = 0;
    }
    else {
      debugln("----Reset Press---");
      // If reset button is long pressed for 3 seconds within 10 seconds post-setup, then enter test mode.
      enterInternalClockModeTime = millis();
      // When reset button is pressed we reset the counters, 
      resetCounters();
      resetButtonPressed = 1;
    }
  }
  // TESTING MODE: if internal clock is active don't read incoming clock 
  // and increment the counter based on internalBPM
  if (internalClock == 1){
    unsigned long internalClockTime = millis();
    if(internalClockTime - lastClockTime > internalBPM){
      if (!resetButtonPressed){
        incrementCounters();
      }
    }
  }
  // External clock input mode
  else{
    if (clockPin.changed() && clockPin.direction == 1){ 
      // if there's a clock and the resetbutton (pullup) is not pressed increment counter
      if (!resetButtonPressed){
        incrementCounters();
      }
    }
  }

}

void resetCounters(){
  debugln("------RESET------");
  // check if we need to change beats or bars and output a 
  // high reset signal to the CD4033 counters to reset the seven segment leds
  barCounter = 0;
  beatCounter = 0;
  firstBar = LOW;
  middleBar = LOW;
  lastBar = LOW;
  digitalWrite(firstBarOutPin, firstBar);
  digitalWrite(firstBarLEDPin, firstBar);
  digitalWrite(middleBarOutPin, middleBar);
  digitalWrite(middleBarLEDPin, middleBar);
  digitalWrite(lastBarOutPin, lastBar);
  digitalWrite(lastBarLEDPin, lastBar);
  changeBeatsAndBars();
  // Reset 4033's to "00"
  resetOutPin.write(HIGH);
  resetOutPin.write(LOW);
  // Increment 4033's to "01"
  clockBarPin.write(LOW); 
  clockBarPin.write(HIGH);
  clockBarPin.write(LOW);
}
// This function will be called whenever a rising edge is detected from the clock
void incrementCounters(){
  lastClockTime = millis();
  if(barCounter == 0){
    firstBar = HIGH;
  }
  clockBarPin.write(clockBarState);
  resetOutPin.write(resetPhrase); // sends reset to 4033
  // Incement 4033 counters to show "01" to count from 1 to barsPerPhrase instead of 0 to (barsPerPhrase - 1)
  if(resetPhrase == HIGH){
    resetOutPin.write(LOW);
    clockBarPin.write(LOW); 
    clockBarPin.write(HIGH);
    clockBarPin.write(LOW);
  }
  digitalWrite(firstBarOutPin, firstBar);
  digitalWrite(firstBarLEDPin, firstBar);
  digitalWrite(middleBarOutPin, middleBar);
  digitalWrite(middleBarLEDPin, middleBar);
  digitalWrite(lastBarOutPin, lastBar);
  digitalWrite(lastBarLEDPin, lastBar);

  
  // Reset output pins after writing any changes.
  // All these pins will output synchronously with the clock at the same 
  // length of the input clock pulse (~5 milliseconds).
  clockBarState = LOW;
  resetPhrase = LOW;
  firstBar = LOW; // TODO first bar and reseet are the same thing. Can we reset the 4033's, light up an led, and input to a schmitt trigger with one pin?
  middleBar = LOW;
  lastBar = LOW;
  
  beatCounter++;
  debug("beat ");
  debug(beatCounter);
  // End of measure, output HIGH clockbar on next clock signal
  if (beatCounter >= beatsPerBar){
    clockBarState = HIGH;
    barCounter++;
    beatCounter = 0;
  }
  // Check for first, middle, and last bar. 
  // TODO validate if this should happen AFTER incrementing barCounter.
  // firstBar is the same as reset, handled below.
 
  if(barCounter == barsPerPhrase/2){
    middleBar = HIGH;
  }
  if(barCounter == barsPerPhrase - 1){
    lastBar = HIGH;
  }
  // End of phrase: reset counters, check for beats/bar changes, and output HIGH reset on next clock
  if (barCounter >= barsPerPhrase){
    resetPhrase = HIGH;
    barCounter = 0;
    beatCounter = 0;
    changeBeatsAndBars();
  }
  debug(" reset: "); debug(resetPhrase);
  debug(" clockBar: "); debug(clockBarState); debug("/"); debug(barsPerPhrase);
  debug(" bar: "); debug(barCounter);
  debug(" firstbar: "); debug(firstBar); 
  debug(" middlebar: "); debug(middleBar); 
  debug(" lastbar: "); debugln(lastBar); 
}

// HOW TO: Select time signatures. 
// Using the 5 position switch we can pick the numerator of the time signature 4, 7, 8, 12, or 16. 
// By passing different divisions of the clock to the clock input we can select the denominator of the time signature. 
// Here are some example clock division inputs and some possible time signatures.
// | ---- Clock Division ---- | ------------- Time Signatures --------------|
// | 1ppqn (quarter notes)    | 4/4 select 4, 8/4 select 8, 7/4 select 7    |
// | 2ppqn (8ths)             | 4/4 select 8, 7/8 select 7, 12/8 select 12  |
// | 4ppqn (16ths)            | 4/4 select 16, 7/16 select 7, 3/4 select 12 |
//
// Changes to the barsPerPhrase and beatsPerBar switches will be picked up on the next phrase reset or when the reset button is pushed. 
void changeBeatsAndBars(){
  // Read phrase length, if HIGH set to 16 bars, if LOW set to 8 bars
  barsPerPhrase = digitalRead(barsPerPhrasePinNum) ? barsPerPhraseValues[1] : barsPerPhraseValues[0];
  debug("bars "); debug(barsPerPhrasePinNum); debug(" "); debug(digitalRead(barsPerPhrasePinNum)); debug(" "); debugln(barsPerPhrase);
  // Read 5 position switch to set beatsPerBar, 
  // each switch position is connected to it's own input pullup
  for (int i = 0; i < 5; i++) {
    int switchState = digitalRead(beatsPerBarPins[i]);
    debug("beatsPin "); debug(beatsPerBarPins[i]); debug(" "); debugln(switchState);
    if (switchState == HIGH) {
      beatsPerBar = beatsPerBarValues[i];
      break;
    }
    // If we looped through all the pins but none are reading high, set beatsPerBar to 4
    if (i == 4 && switchState == LOW){
      beatsPerBar = 4;
    }
  }
  debug("changeBeatsAndBars "); debug("beats "); debug(beatsPerBar); debug(" bars "); debugln(barsPerPhrase);
}
