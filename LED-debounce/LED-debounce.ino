unsigned char s = 0;

void setup() {
  DDRA  &= 0b11111110;
  DDRA  |= 0b00000010;
  PORTA &= 0b11111101;
}

void loop() {
  switch(s) {
  case 0:
    if (PINA & 0b00000001) {
      delay(3);
      s = 1;
    }
  break;
  case 1:
    PORTA |= 0b00000010;
    if (!(PINA & 0b00000001)) {
      delay(3);
      s = 2;
    }
  break;
  case 2:
    if (PINA & 0b00000001) {
      delay(3);
      s = 3;
    }
  break;
  case 3:
    PORTA &= 0b11111101;
    if (!(PINA & 0b00000001)) {
      delay(3);
      s = 0;
    }
  break;
  }
}
