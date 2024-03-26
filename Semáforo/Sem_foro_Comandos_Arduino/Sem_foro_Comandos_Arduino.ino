void setup() {
  pinMode(22, OUTPUT);
  pinMode(23, OUTPUT);
  pinMode(24, OUTPUT);
}

void loop() {
  digitalWrite(22, HIGH);
  digitalWrite(23, LOW);
  digitalWrite(24, LOW);
  delay(5000);
  
  digitalWrite(22, LOW);
  digitalWrite(23, HIGH);
  digitalWrite(24, LOW);
  delay(2000);

  digitalWrite(22, LOW);
  digitalWrite(23, LOW);
  digitalWrite(24, HIGH);
  delay(5000);
}
