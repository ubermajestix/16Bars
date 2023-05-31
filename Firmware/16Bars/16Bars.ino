#include "src/debug.h"
#include "src/UberPin/UberPin.h"

const byte clockPinNum = 2;
const byte resetButtonPinNum = 3;
const byte clockBarPinNum = 4;
const byte resetOutPinNum = 5;

UberPin clockPin(clockPinNum);
UberPin resetButtonPin(resetButtonPinNum, 5);
UberPin clockBarPin(clockBarPinNum);
UberPin resetOutPin(resetOutPinNum);

volatile byte beatCounter = 0; // Counter for the clock pulses
volatile byte barCounter = 0;  // Counter for the musical bars
volatile byte barsPerPhrase = 4;
volatile byte beatsPerMeasure = 4;
volatile byte resetButtonPressed = 0;
volatile bool clockBarState = LOW; // Led State, initially set to LOW
volatile bool resetOutputState = LOW;
volatile bool resetPhrase = LOW;

void setup()
{
  pinMode(clockPinNum, INPUT);
  pinMode(resetButtonPinNum, INPUT_PULLUP);
  pinMode(clockBarPinNum, OUTPUT);
  pinMode(resetOutPinNum, OUTPUT);
  Serial.begin(9600);
  Serial.println("setup");
  debug("setup ");
  debug("clock ");
  debugln(clockPin.pin());
  debugln(clockPin.read());
  debug("reset ");
  debugln(resetButtonPin.pin());
  debugln(resetButtonPin.read());
}

void loop()
{
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

  if (resetButtonPin.changed())
  {
    if (resetButtonPin.direction == 1)
    {
      debugln("---resetRelease---");
      resetOutPin.write(LOW);
      resetButtonPressed = 0;
    }
    else
    {
      debugln("----resetbutton---");
      // TODO when we reset check state of switches for beatsPerMeasure and barsPerPhrase
      barCounter = 0;
      beatCounter = 0;
      resetOutPin.write(HIGH);
      resetButtonPressed = 1;
    }
  }
  // TODO added 5millisecond default debounce to PinMonitor::changed
  // so we only pickup clean signals, is this too long for trigger pulses?
  // if(clockPin.changed(HIGH) && resetButtonPin.read() == HIGH){ // if there's a clock and the resetbutton (pullup) is not pressed
  if (clockPin.changed() && clockPin.direction == 1)
  { // if there's a clock and the resetbutton (pullup) is not pressed
    debugln("clock incoming");
    debug("reset button pressed: ");
    debugln(!!resetButtonPressed);
    if (!resetButtonPressed)
    {
      incrementCounters();
    }
  }
}
// This function will be called whenever a rising edge is detected from the clock
void incrementCounters()
{
  debugln("-------clock------");

  clockBarPin.write(clockBarState);
  resetOutPin.write(resetOutputState);

  clockBarState = LOW;
  resetOutputState = LOW;
  beatCounter++;
  debug("beat ");
  debugln(beatCounter);
  if (beatCounter >= beatsPerMeasure)
  {
    clockBarState = HIGH;
    barCounter++;
    beatCounter = 0;
  }
  if (barCounter >= barsPerPhrase)
  {
    resetOutputState = HIGH;
    barCounter = 0;
    beatCounter = 0;
  }

  debug("bar ");
  debugln(barCounter);
  debug("clockBar ");
  debugln(clockBarState);
  debug("reset ");
  debugln(resetOutputState);
}

// TODO figure out different beat counters (4 or 8) based on switch
// TODO figure out time signatures 4/4, 8/4, 6/8, 7/8 -> how to pick numerator and denominator?
// IDEA: select numerator only with rotary encoder, 4,6,7,8,9,11,12,16 (8 position)
//       denominator depends on clock signal sent:
//       1ppqn (quarter notes): 4/4 select 4, 8/4 select 8, 11/4 select 11
//       2ppqn (8ths): 4/4 select 8, 7/8 select 7, 11/8 select 11
//       4ppqn (16ths); 4/4 select 16
//
