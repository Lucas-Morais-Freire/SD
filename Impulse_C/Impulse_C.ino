void setup() {
  pinMode(25, OUTPUT);
  pinMode(24, INPUT);
}

void loop() {
  if (!digitalRead(24)) {
    digitalWrite(25, HIGH);
  } else {
    digitalWrite(25, LOW);
  }
}
