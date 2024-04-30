#include <avr/io.h>
#include <avr/interrupt.h>
unsigned char s = 0;
bool do_delay = false;
void setup() {
  DDRC &= 0b11111110; // 37      in
  DDRA =  0b11111111; // 22 - 29 out
  PORTA = 0b01110111; // 22 - 29 low
}

void loop() {
  analogWrite(A0, 127);
  switch(s) {
  case 0:
    if (PINC & 0b00000001) {
      s = 1;
      do_delay = true;
    }
  break;
  case 1:
    PORTA = 0b00010100; // 1
    if (do_delay) {
      delay(1000);
      do_delay = false;
    }
    if (!(PINC & 0b00000001)) {
      s = 2;
      delay(3);
    }
  break;
  case 2:
    if (PINC & 0b00000001) {
      s = 3;
      do_delay = true;
    }
  break;
  case 3:
    PORTA = 0b10110011; // 2
    if (do_delay) {
      delay(1000);
      do_delay = false;
    }
    if (!(PINC & 0b00000001)) {
      s = 4;
      delay(3);
    }
  break;
  case 4:
    if (PINC & 0b00000001) {
      s = 5;
      do_delay = true;
    }
  break;
  case 5:
    PORTA = 0b10110110; // 3
    if (do_delay) {
      delay(1000);
      do_delay = false;
    }
    if (!(PINC & 0b00000001)) {
      s = 6;
      delay(3);
    }
  break;
  case 6:
    if (PINC & 0b00000001) {
      s = 7;
      do_delay = true;
    }
  break;
  case 7:
    PORTA = 0b11010100; // 4
    if (do_delay) {
      delay(1000);
      do_delay = false;
    }
    if (!(PINC & 0b00000001)) {
      s = 8;
      delay(3);
    }
  break;
  case 8:
    if (PINC & 0b00000001) {
      s = 9;
      do_delay = true;
    }
  break;
  case 9:
    PORTA = 0b11100110; // 5
    if (do_delay) {
      delay(1000);
      do_delay = false;
    }
    if (!(PINC & 0b00000001)) {
      s = 10;
      delay(3);
    }
  break;
  case 10:
    if (PINC & 0b00000001) {
      s = 11;
      do_delay = true;
    }
  break;
  case 11:
    PORTA = 0b11100111; // 6
    if (do_delay) {
      delay(1000);
      do_delay = false;
    }
    if (!(PINC & 0b00000001)) {
      s = 12;
      delay(3);
    }
  break;
  case 12:
    if (PINC & 0b00000001) {
      s = 13;
      do_delay = true;
    }
  break;
  case 13:
    PORTA = 0b00110100; // 7
    if (do_delay) {
      delay(1000);
      do_delay = false;
    }
    if (!(PINC & 0b00000001)) {
      s = 14;
      delay(3);
    }
  break;
  case 14:
    if (PINC & 0b00000001) {
      s = 15;
      do_delay = true;
    }
  break;
  case 15:
    PORTA = 0b01110111; // 0
    if (do_delay) {
      delay(1000);
      do_delay = false;
    }
    if (!(PINC & 0b00000001)) {
      s = 0;
      delay(3);
    }
  break;
  }
}
