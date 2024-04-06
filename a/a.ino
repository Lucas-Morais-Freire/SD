unsigned char s = 0;
int main() {
  DDRA  |= 0b00000001; // 22 out
  PORTA &= 0b11111110; // 22 low

  TCCR0B = 0b00000101; // fclk/1024
  TIMSK0 = 0b00000001; // int ovf
  sei();

  while(1) {}
}

ISR (TIMER0_OVF_vect) {
  if (!s) {
    PORTA |= 0b00000001;
    s = 1;
  } else {
    PORTA &= 0b11111110;
    s = 0;
  }
}
