#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *myMotor = AFMS.getMotor(2);
Adafruit_DCMotor *myMotor2 = AFMS.getMotor(3);

int test = 17;

void setup() {
  // put your setup code here, to run once:
  AFMS.begin();
  Serial.begin(9600);
}



void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(test);
  myMotor->setSpeed(0);
  myMotor2->setSpeed(0);
  myMotor->run(RELEASE);
  myMotor2->run(RELEASE);
  delay(500);
  Serial.println("running");
  myMotor->setSpeed(test);
  myMotor->run(FORWARD);
  myMotor2->setSpeed(test);
  myMotor2->run(FORWARD);
  delay(2000);
  test = test + 1;
}
