#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

Adafruit_MotorShield AFMS = Adafruit_MotorShield();

// motor greater runs 4 points higher than 3
Adafruit_DCMotor *motorLeft = AFMS.getMotor(3); // 19 minimum
Adafruit_DCMotor *motorRight = AFMS.getMotor(4); // 27 minimum

int irPin = A0; // right
int irPin2 = A1; // left
int digitalIrRight = 12;
int digitalIrLeft = 13;
int colorValueRight = 0;
int colorValueLeft = 0;
int baseSpeed = 22;
int threshold = 600;
int diff = 8;
int direction = 1;
int a,b,c;
int adjustment = 20;

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
  recvOneChar();
  showNewData();
  if (on) {
    colorValueRight = readPin(irPin, digitalIrRight);
    colorValueLeft = readPin(irPin2, digitalIrLeft);
    Serial.print("sensor right: ");
    Serial.print(colorValueRight);
    Serial.print(", sensor left: ");
    Serial.println(colorValueLeft);
    int rightUnder = colorValueRight > threshold;
    int leftUnder = colorValueLeft > threshold;
    
    if (leftUnder) {
      Serial.println("turn left");
      motorLeft->setSpeed(baseSpeed - adjustment);
      motorRight->setSpeed(baseSpeed + diff + adjustment);
      direction = 2;
      // motorLeft->run(FORWARD);
      // delay(200);
    } else if (rightUnder) {
      Serial.println("turn right");
      motorLeft->setSpeed(baseSpeed + adjustment + 20);
      motorRight->setSpeed(baseSpeed - adjustment);
      direction = 1;
      // motorLeft->run(FORWARD);
      // delay(400);
    } else {
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
    // motorLeft->run(RELEASE);
    // motorRight->run(RELEASE);
  } else {
    motorLeft->run(RELEASE);
    motorRight->run(RELEASE);
  }
}

void recvOneChar() {
    if (Serial.available() > 0) {
        receivedChar = Serial.read();
        newData = true;
    }
}

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

