#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

Adafruit_MotorShield AFMS = Adafruit_MotorShield();

Adafruit_DCMotor *motorLeft = AFMS.getMotor(3); // 19 minimum
Adafruit_DCMotor *motorRight = AFMS.getMotor(4); // 27 minimum

// setup pins
int irPin = A0; // right
int irPin2 = A1; // left
int digitalIrRight = 12;
int digitalIrLeft = 13;

// set global variables
int colorValueRight = 0;
int colorValueLeft = 0;
int baseSpeed = 22;
int threshold = 600;
int diff = 8; // difference between motor minimums
int direction = 1; // 1 = right, 2 = left
int a,b,c; // temporary variables for denoising
int adjustment = 20; // adjustment to easily configure turning speed

// variables for serial input and control
char receivedChar;
boolean newData = false;
boolean on = false;

void setup() { 
  AFMS.begin();
  Serial.begin(9600);
  pinMode(digitalIrRight, OUTPUT);
  pinMode(digitalIrLeft, OUTPUT);
}

void loop() {
  // constantly check for serial input
  recvOneChar();
  showNewData();
  if (on) {
    // read sensor values and print
    colorValueRight = readPin(irPin, digitalIrRight);
    colorValueLeft = readPin(irPin2, digitalIrLeft);
    Serial.print("sensor right: ");
    Serial.print(colorValueRight);
    Serial.print(", sensor left: ");
    Serial.println(colorValueLeft);

    // check for line underneath sensor
    int rightUnder = colorValueRight > threshold;
    int leftUnder = colorValueLeft > threshold;
    
    if (leftUnder) { // turn left and set state to left
      Serial.println("turn left");
      motorLeft->setSpeed(baseSpeed - adjustment);
      motorRight->setSpeed(baseSpeed + diff + adjustment);
      direction = 2;
      // motorLeft->run(FORWARD);
      // delay(200);
    } else if (rightUnder) { // turn right and set state to right
      Serial.println("turn right");
      motorLeft->setSpeed(baseSpeed + adjustment + 20);
      motorRight->setSpeed(baseSpeed - adjustment);
      direction = 1;
      // motorLeft->run(FORWARD);
      // delay(400);
    } else { // if neither detected, continue last action
      if (direction == 1) {
        Serial.println("turn right");
        motorLeft->setSpeed(baseSpeed + adjustment);
        motorRight->setSpeed(baseSpeed - adjustment);
      }
      if (direction == 2) {
        Serial.println("turn left");
        motorLeft->setSpeed(baseSpeed - adjustment);
        motorRight->setSpeed(baseSpeed + diff + adjustment);
      }
    }

    motorLeft->run(FORWARD);
    motorRight->run(FORWARD);
  } else {
    // stop if running is disabled
    motorLeft->run(RELEASE);
    motorRight->run(RELEASE);
  }
}

// recieve a single character
void recvOneChar() {
    if (Serial.available() > 0) {
        receivedChar = Serial.read();
        newData = true;
    }
}

// check for new data and turn the processing on or off
void showNewData() {
    if (newData == true) {
      if (receivedChar == 's' || receivedChar == 'S') {
        Serial.println("start");
        on = true;
      }
      if (receivedChar == 'x' || receivedChar == 'X') {
        Serial.println("stop");
        on = false;
      }
      newData = false;
    }
}

// remove noise from the IR readings
int readPin(int irPin, int digitalPin) {
    digitalWrite(digitalPin,HIGH);
    delayMicroseconds(500);
    a=analogRead(irPin);
    digitalWrite(digitalPin,LOW);
    delayMicroseconds(500);
    b=analogRead(irPin);
    c=b-a;
    return c;
}

