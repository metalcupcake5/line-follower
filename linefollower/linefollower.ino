#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

Adafruit_MotorShield AFMS = Adafruit_MotorShield();

// motor greater runs 4 points higher than 3
Adafruit_DCMotor *motorLeft = AFMS.getMotor(3); // 25 minimum
Adafruit_DCMotor *motorRight = AFMS.getMotor(2); // 21 minimum

int irPin = A0; // right
int irPin2 = A1; // left
int colorValueRight = 0;
int colorValueLeft = 0;
int baseSpeed = 22;
int threshold = 300;
int diff = 4;
int direction = 0;

char receivedChar;
boolean newData = false;
boolean on = false;

void setup() { 
  AFMS.begin();
  Serial.begin(9600);
}

void loop() {
  recvOneChar();
  showNewData();
  if (on) {
    colorValueRight = analogRead(irPin);
    colorValueLeft = analogRead(irPin2);
    Serial.print("sensor right: ");
    Serial.print(colorValueRight);
    Serial.print(", sensor left: ");
    Serial.println(colorValueLeft);
    int rightUnder = colorValueRight < threshold;
    int leftUnder = colorValueLeft < threshold;
    if (rightUnder) {
      direction = 0;
    } else if (leftUnder) {
      direction = 1;
    }
    // right = 0, left = 1
    //
    if (direction == 0) {
      Serial.println("turn right");
      motorLeft->setSpeed(baseSpeed + 5);
      motorRight->setSpeed(baseSpeed);
    } else if (direction == 1) {
      Serial.println("turn left");
      motorLeft->setSpeed(baseSpeed);
      motorRight->setSpeed(baseSpeed + diff + 10);
    }

    
    // if ((rightUnder && leftUnder) || (!rightUnder && !leftUnder)) {
    //   Serial.println("straight");
    //   motorLeft->setSpeed(baseSpeed);
    //   motorRight->setSpeed(baseSpeed + diff);
    // } else if (rightUnder) {
    //   Serial.println("turn right");
    //   motorLeft->setSpeed(baseSpeed + 15);
    //   motorRight->setSpeed(baseSpeed);
    //   // motorLeft->run(FORWARD);
    //   // delay(200);
    // } else if (leftUnder) {
    //   Serial.println("turn left");
    //   motorLeft->setSpeed(baseSpeed);
    //   motorRight->setSpeed(baseSpeed + diff + 15);
    //   // motorLeft->run(FORWARD);
    //   // delay(200);
    // } else {
    //   Serial.println("straight");
    //   motorLeft->setSpeed(baseSpeed);
    //   motorRight->setSpeed(baseSpeed + diff);
    // }
    
    // motorLeft->run(RELEASE);
    // motorRight->run(RELEASE);
    motorLeft->run(FORWARD);
    motorRight->run(FORWARD);
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
