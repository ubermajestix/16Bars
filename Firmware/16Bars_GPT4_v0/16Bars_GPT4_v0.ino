// only pins 2 and 3 can interrupt
const byte clockSignalPin = 2;    // Connect your clock square wave signal here
const byte ledPin = LED_BUILTIN;  // Built-in LED
const byte resetButtonPin = 3;    // Reset button, push to reset counters to zero
const byte clockBarPin = 4;       // Outputs square wave every musical bar
const byte resetSignalPin = 5;    // Reset signal to reset seven segment LEDs
const byte triggerWidth = 50;     // milliseconds

volatile byte beatCounter = 0;  // Counter for the clock pulses
volatile byte barCounter = 0;   // Counter for the musical bars
volatile byte barsPerPhrase = 16;
volatile byte beatsPerMeasure = 4;
volatile bool clockBarState = LOW;  // Led State, initially set to LOW
volatile bool resetState = LOW;     // Led State, initially set to LOW
volatile bool ledState = LOW;       // Led State, initially set to LOW

void setup() {
  pinMode(clockSignalPin, INPUT);
  pinMode(resetButtonPin, INPUT);
  pinMode(clockBarPin, OUTPUT);
  pinMode(resetSignalPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  Serial.print("setup ");

  // Attach an interrupt to the clockSignalPin
  // The 'blinkLed' function will be called whenever a rising edge is detected
  // attachInterrupt(digitalPinToInterrupt(clockSignalPin), incrementCounters, RISING);
  // TODO debounce button?
  // TODO not sure if this is the right way to do this, this seems to be causing reset to trigger early without a button push
  // do we need a pull down resistor?
  // attachInterrupt(digitalPinToInterrupt(resetButtonPin), resetCounters, RISING);
}

void loop() {
  // todo check timers to emit longer triggers

  bool clock = digitalRead(clockSignalPin);
  bool reset = digitalRead(resetButtonPin);
  if (reset == HIGH) {
    Serial.println("----resetbutton---");
    barCounter = 0;
    beatCounter = 0;
    digitalWrite(resetSignalPin, HIGH);
  }
  if (clock == HIGH) {
    incrementCounters();
  }
}

// This function will be called whenever a rising edge is detected from the clock
void incrementCounters() {
  Serial.println("-------clock------");
  clockBarState = LOW;
  resetState = LOW;
  digitalWrite(clockBarPin, clockBarState);
  digitalWrite(resetSignalPin, resetState);

  beatCounter++;
  if (beatCounter >= beatsPerMeasure) {
    barCounter++;
    clockBarState = HIGH;
    digitalWrite(clockBarPin, clockBarState);

    beatCounter = 0;
  }
  if (barCounter >= barsPerPhrase) {
    resetState = HIGH;
    barCounter = 0;
    beatCounter = 0;
    digitalWrite(resetSignalPin, resetState);

    // TODO about to reset check state of switches for beatsPerMeasure and barsPerPhrase
  }
  Serial.println("-------loop------");
  Serial.print("beat ");
  Serial.println(beatCounter);
  Serial.print("bar ");
  Serial.println(barCounter);
  Serial.print("clockBar ");
  Serial.println(clockBarState);
  Serial.print("reset ");
  Serial.println(resetState);
}


// TODO figure out different beat counters (4 or 8) based on switch
// TODO figure out time signatures 4/4, 8/4, 6/8, 7/8 -> how to pick numerator and denominator?
// IDEA: select numerator only with rotary encoder, 4,6,7,8,9,11,12,16 (8 position)
//       denominator depends on clock signal sent:
//       1ppqn (quarter notes): 4/4 select 4, 8/4 select 8, 11/4 select 11
//       2ppqn (8ths): 4/4 select 8, 7/8 select 7, 11/8 select 11
//       4ppqn (16ths); 4/4 select 16
//
