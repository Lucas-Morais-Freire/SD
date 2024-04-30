// estado atual
unsigned char s = 0;
// valores binarios dos numeros para o display 7seg
uint8_t intensities[3] = {
  0,  // 0%
  127, // 50%
  255 // 100%
};
// contador de intensidades
int i = 0;

int main() {
  Serial.begin(9600);
  // inout
  DDRB  |= 0b10000000; // pwm 13 out
  DDRD  &= 0b11111110; // 21 in

  // temp
  TCCR0A = 0b10000011; // non-inverting pwm
  TCCR0B = 0b00000001; // no prescaling

  // interrupt
  sei();               // global interrupt enable
  EICRA  = 0b00000001; // INT[0] any edge
  EIMSK |= 0b00000001; // INT[0] enable

  while (1) {
    Serial.println((int)(intensities[i]));
  }

  return 0;
}

ISR (INT0_vect) {
  switch (s) {
    case 0:
      if (PIND & 0b00000001) {
        i = i != 2 ? i + 1 : 0;
        OCR0A = intensities[i];
        s = 1;
        _delay_ms(50);
      }
      break;
    case 1:
      if (!(PIND & 0b00000001)) {
        s = 0;
        _delay_ms(50);
      }
      break;
  }
}
