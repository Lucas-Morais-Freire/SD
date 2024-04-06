void setup() {
  DDRA |= 0b00000001;
  DDRA &= 0b11111101;
}

void loop() {
  if (PINA & 0b00000010) {
    PORTA |= 0b00000001;
  } else {
    PORTA &= 0b11111110;
  }
}
