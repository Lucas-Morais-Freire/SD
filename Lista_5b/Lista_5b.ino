unsigned char i = 0;
bool is_on = false;

int main() {
  // inout
  DDRA  |= 0b00000001; // 22 out
  PORTA &= 0b11111110; // 22 low

  // timer
  TCCR0A = 0b00000010; // no pwm
  TCCR0B = 0b00000101; // 1024 prescaler
                       // CTC mode
  OCR0A  = 124;        // count to 124

  // interrupt
  sei();
  TIMSK0 = 0b00000010;

  while(1) {}
}

ISR(TIMER0_COMPA_vect) {
  if (++i == 125) { // count to 124 125 times
    if (is_on) {
      PORTA &= 0b11111110;
    } else {
      PORTA |= 0b00000001;
    }
    is_on = !is_on;
    i = 0;
  }
}
