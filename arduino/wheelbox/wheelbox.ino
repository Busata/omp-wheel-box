#include <Joystick.h>

Joystick_ Joystick;

const byte ROWS = 4;
const byte COLS = 5;

const byte encoderAPinA = 2;
const byte encoderAPinB = 3;

volatile int prevEncoderAState = 0;
volatile int currEncoderAState = 0;
volatile int counterA = 0;


const byte encoderBPinA = 1;
const byte encoderBPinB = 4;


volatile int prevEncoderBState = 0;
volatile int currEncoderBState = 0;
volatile int counterB = 0;

int prevCounterA = 0;
int prevCounterB = 0;

//Maps the buttons to a custom order.
int buttonMapping[] = {9,10,11,24,12,17,13,14,16,15,8,4,5,6,7,0,1,2,3,20};

byte rowPins[ROWS] = { 21, 20, 19, 18};
byte colPins[COLS] = { 15, 14, 16, 10, 9};

byte matrixState[ROWS][COLS] = {
  { 0, 0, 0, 0, 0},
  { 0, 0, 0, 0, 0},
  { 0, 0, 0, 0, 0},
  { 0, 0, 0, 0, 0},
};

void encoderISR() {
  cli();
  prevEncoderAState = currEncoderAState;
  currEncoderAState = digitalRead(encoderAPinA);
  if (prevEncoderAState == LOW && currEncoderAState == HIGH) {
    if (digitalRead(encoderAPinB) == LOW) {
      counterA--;  // clockwise rotation
    } else {
      counterA++;  // counterclockwise rotation
    }
  }
  sei();
}


void encoderISRB() {
  cli();
  prevEncoderBState = currEncoderBState;
  currEncoderBState = digitalRead(encoderBPinA);
  if (prevEncoderBState == LOW && currEncoderBState == HIGH) {
    if (digitalRead(encoderBPinB) == LOW) {
      counterB++;  // clockwise rotation
    } else {
      counterB--;  // counterclockwise rotation
    }
  }
  sei();
}


void setup() {

  Joystick.begin();

  pinMode(encoderAPinA, INPUT_PULLUP);
  pinMode(encoderAPinB, INPUT_PULLUP);


  pinMode(encoderBPinA, INPUT_PULLUP);
  pinMode(encoderBPinB, INPUT_PULLUP);

  // Attach interrupt to rotary encoder A pin
  attachInterrupt(digitalPinToInterrupt(encoderAPinA), encoderISR, CHANGE);

  attachInterrupt(digitalPinToInterrupt(encoderBPinA), encoderISRB, CHANGE);


  for (byte i = 0; i < COLS; i++) {
    pinMode(colPins[i], INPUT_PULLUP);
  }

  for (byte i = 0; i < ROWS; i++) {
    pinMode(rowPins[i], OUTPUT);
    digitalWrite(rowPins[i], HIGH);
  }
  
}

void loop() {
  for(byte row=0; row < ROWS; row++) {
    digitalWrite(rowPins[row], LOW);

    for(byte col=0; col < COLS; col++) {
      int buttonNumber = row * COLS + col;

      if(digitalRead(colPins[col]) == LOW) {
       matrixState[row][col] = 1;
      } else {
        matrixState[row][col] = 0;
      }

       Joystick.setButton(buttonMapping[buttonNumber], matrixState[row][col]);

    }
    digitalWrite(rowPins[row], HIGH);
  }

  if(counterA != prevCounterA) {
    if(counterA > prevCounterA) {
      Joystick.setButton(21, 1);
    } else {
      Joystick.setButton(20, 1);
    }
  } else {
    Joystick.setButton(20, 0);
    Joystick.setButton(21, 0);
  }

  if(counterB != prevCounterB) {
    if(counterB > prevCounterB) {
      Joystick.setButton(23, 1);
    } else {
      Joystick.setButton(22, 1);
    }
  } else {
    Joystick.setButton(23, 0);
    Joystick.setButton(22, 0);
  }

  prevCounterA = counterA;
  prevCounterB = counterB;

  delay(50);
}
