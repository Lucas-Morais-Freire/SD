void setup() {
  DDRA |= 0b00000111;
}

void loop() {
  PORTA |= 0b00000001;
  PORTA &= 0b11111001;
  delay(5000);
  
  PORTA |= 0b00000010;
  PORTA &= 0b11111010;
  delay(2000);

  PORTA |= 0b00000100;
  PORTA &= 0b11111100;
  delay(5000);
}
