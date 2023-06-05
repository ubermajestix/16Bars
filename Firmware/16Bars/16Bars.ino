#include "src/debug.h"
#include "src/UberPin/UberPin.h"

const byte clockPinNum = 2;
const byte resetButtonPinNum = 3;
const byte clockBarPinNum = 4;
const byte resetOutPinNum = 5;
const byte barsPerPhrasePinNum = 6;
const byte beatsPerBarPins[5] = {7, 8, 9, 10, 11};
const byte beatsPerBarValues[5] = {4, 7, 8, 12, 16};
const byte barsPerPhraseValues[2] = {8,16}; 
const byte firstBarOutPin = 12;
const byte middleBarOutPin = 13;
const byte lastBarOutPin = 14;

UberPin clockPin(clockPinNum);
UberPin resetButtonPin(resetButtonPinNum, 5);
UberPin clockBarPin(clockBarPinNum);
UberPin resetOutPin(resetOutPinNum);

volatile byte beatCounter = 0; // Counter for the clock pulses
volatile byte barCounter = 0;  // Counter for bars
volatile byte barsPerPhrase = barsPerPhraseValues[1]; // Default is 16 bars because, uh, that's the name of the project.
volatile byte beatsPerBar= beatsPerBarValues[0]; // Default is 4. Can be set to 4, 7, 8, 12, 16 with switch 
volatile byte resetButtonPressed = 0;
volatile bool clockBarState = LOW; // Led State, initially set to LOW
volatile bool resetPhrase = LOW;
volatile bool firstBar = LOW;
volatile bool middleBar = LOW;
volatile bool lastBar = LOW;

void setup(){
  pinMode(clockPinNum, INPUT);
  pinMode(barsPerPhrasePinNum, INPUT_PULLUP);
  pinMode(resetButtonPinNum, INPUT_PULLUP);
  // Set up the beatsPerBar 5 position switch pins as inputs
  for (int i = 0; i < 5; i++) {
    pinMode(beatsPerBarPins[i], INPUT_PULLUP);
  }
  pinMode(clockBarPinNum, OUTPUT);
  pinMode(resetOutPinNum, OUTPUT);
  pinMode(firstBarOutPin, OUTPUT);
  pinMode(middleBarOutPin, OUTPUT);
  pinMode(lastBarOutPin, OUTPUT);
  Serial.begin(9600);
}

void loop(){
  // TODO can we get the counters to count from 1 to 16 instead of 0 to 15? If barCounter == 0 send fast trigger to get leds to display 01 before next clock pulse?
  if (resetButtonPin.changed()){
    if (resetButtonPin.direction == 1){
      debugln("---resetRelease---");
      resetOutPin.write(LOW);
      resetButtonPressed = 0;
    }
    else {
      debugln("----resetbutton---");
      // When reset button is pressed we reset the counters, 
      // check if we need to change beats or bars and output a 
      // high reset signal to the CD4033 counters to reset the seven segment leds
      barCounter = 0;
      beatCounter = 0;
      changeBeatsAndBars();
      resetOutPin.write(HIGH);
      resetButtonPressed = 1;
    }
  }
  if (clockPin.changed() && clockPin.direction == 1){ 
    // if there's a clock and the resetbutton (pullup) is not pressed increment counter
    if (!resetButtonPressed){
      incrementCounters();
    }
  }
}
// This function will be called whenever a rising edge is detected from the clock
void incrementCounters(){
  debugln("-------clock------");
  clockBarPin.write(clockBarState);
  resetOutPin.write(resetPhrase);
  digitalWrite(firstBarPin, firstBar);
  digitalWrite(middleBarPin, middleBar);
  digitalWrite(lastBarPin, lastBar);
  
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
  debugln(beatCounter);
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
    firstBar = HIGH;
    barCounter = 0;
    beatCounter = 0;
    changeBeatsAndBars();
  }
  debug("bar ");
  debugln(barCounter);
  debug("clockBar ");
  debugln(clockBarState);
  debug("reset ");
  debugln(resetPhrase);
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
  // REead 5 position switch to set beatsPerBar, 
  // each switch position is connected to it's own input pullup
  for (int i = 0; i < 5; i++) {
    int switchState = digitalRead(beatsPerBarPins[i]);
    if (switchState == LOW) {
      beatsPerBar = beatsPerBarValues[i];
      break;
    }
  }
}
