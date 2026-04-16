#include <Servo.h>

#define PIN_OUTPUT 3

Servo myServo;

void setup() {
  myServo.attach(PIN_OUTPUT); 
}

void loop() {
  myServo.writeMicroseconds(1000);
  delay(2000);

  myServo.writeMicroseconds(1500);
  delay(2000);

  myServo.writeMicroseconds(2000);
  delay(2000);

  myServo.writeMicroseconds(1500);
  delay(2000);
}