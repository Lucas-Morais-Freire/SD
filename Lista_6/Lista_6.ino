int main(void) {
  // inout
  DDRB  |= 0b10000000; // pwm 13 out

  // timer 0
  TCCR0A = 0b10000011; // non-inv with reg A
  TCCR0B = 0b00000001; // no prescaling
                       // fast pwm, TOP 0xFF
  OCR0A = 0; // initial DC = 0.39%

  // timer 2
  TCCR2A = 0b00000010; // no pwm
  TCCR2B = 0b00000111; // 1024 prescaling
                       // CTC mode
  sei();               // global interrupt enable
  TIMSK2 = 0b00000010; // interrupt comp A
  OCR2A  = 121; // LED cycle is 256*(121 + 1)*1024/16M =~ 1.998848s

  while(true) {}
}

ISR(TIMER2_COMPA_vect) {
  OCR0A++;
}
