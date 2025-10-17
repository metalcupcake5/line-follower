/*
This code is to Remove ambience noise from sensor data.
IR LED  connected to Digital pin: 6
IR diode connected to analog input:A3

by-Abhilash  Patel
*/
int a,b,c;
int irPin = A0;
int digitalPin = 12; // 12 right 13 left
void setup() {
    Serial.begin(9600);
    pinMode(digitalPin,OUTPUT);
}

void  loop() {
    Serial.println(readPin(A0, 12));         // denoised signal
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
