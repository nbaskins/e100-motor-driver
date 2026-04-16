#define PIN_OUTPUT 3

void setup() {
  pinMode(PIN_OUTPUT, OUTPUT);
}

void loop() {
  writeRC(1000);
  delay(2000);

  writeRC(1500);
  delay(2000);

  writeRC(2000);
  delay(2000);

  writeRC(1500);
  delay(2000);
}

void writeRC(uint16_t microseconds) {
  for (int i = 0; i < 100; i++) {
    digitalWrite(PIN_OUTPUT, HIGH);
    delayMicroseconds(microseconds);
    digitalWrite(PIN_OUTPUT, LOW);
    delayMicroseconds(20000 - microseconds);
  }
}