unsigned char s = 0;
void setup() {
  DDRC &= 0b11111100; // 37      in
  DDRA =  0b11111111; // 22 - 29 out
  PORTA = 0b01110111; // 22 - 29 low
}

void loop() {
  if (PINC & 0b00000010) {
    PORTA = 0b01110111; // 0
    s = 0;
  }
  switch(s) {
  case 0:
    if (PINC & 0b00000001) {
      s = 1;
      delay(3);
    }
  break;
  case 1:
    PORTA = 0b00010100; // 1
    if (!(PINC & 0b00000001)) {
      s = 2;
      delay(3);
    }
  break;
  case 2:
    if (PINC & 0b00000001) {
      s = 3;
      delay(3);
    }
  break;
  case 3:
    PORTA = 0b10110011; // 2
    if (!(PINC & 0b00000001)) {
      s = 4;
      delay(3);
    }
  break;
  case 4:
    if (PINC & 0b00000001) {
      s = 5;
      delay(3);
    }
  break;
  case 5:
    PORTA = 0b10110110; // 3
    if (!(PINC & 0b00000001)) {
      s = 6;
      delay(3);
    }
  break;
  case 6:
    if (PINC & 0b00000001) {
      s = 7;
      delay(3);
    }
  break;
  case 7:
    PORTA = 0b11010100; // 4
    if (!(PINC & 0b00000001)) {
      s = 8;
      delay(3);
    }
  break;
  case 8:
    if (PINC & 0b00000001) {
      s = 9;
      delay(3);
    }
  break;
  case 9:
    PORTA = 0b11100110; // 5
    if (!(PINC & 0b00000001)) {
      s = 10;
      delay(3);
    }
  break;
  case 10:
    if (PINC & 0b00000001) {
      s = 11;
      delay(3);
    }
  break;
  case 11:
    PORTA = 0b11100111; // 6
    if (!(PINC & 0b00000001)) {
      s = 12;
      delay(3);
    }
  break;
  case 12:
    if (PINC & 0b00000001) {
      s = 13;
      delay(3);
    }
  break;
  case 13:
    PORTA = 0b00110100; // 7
  }
}
