void setup() {
  pinMode(22, OUTPUT);
  pinMode(23, INPUT);
}

void loop() {
  if (digitalRead(23)) {
    digitalWrite(22, HIGH);
  } else {
    digitalWrite(22, LOW);
  }
}
