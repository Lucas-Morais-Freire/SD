// estado atual
unsigned char s = 0;
// valores binarios dos numeros para o display 7seg
uint8_t numbers[8] = {
  0b01110111, // 0
  0b00010100, // 1
  0b10110011, // 2
  0b10110110, // 3
  0b11010100, // 4
  0b11100110, // 5
  0b11100111, // 6
  0b00110100  // 7
};
// contador de numero
int i = 0;

int main() {
  DDRA   = 0b11111111; // 22 - 29 out
  PORTA  = 0b01110111; // 22 - 29 low
  
  // interrupt
  sei();               // global interrupt enable
  EICRA  = 0b00000101; // INT[0,1] any edge
  EIMSK |= 0b00000011; // INT[0,1] enable

  DDRD  &= 0b11111100; // INT[0,1] in

  while(true) {}

  return 0;
}

// botao clr
ISR (INT0_vect) {
  i = 0;
  PORTA = numbers[0];
  s = 0;
}

// botao inc
ISR (INT1_vect) {
  switch(s) {
  case 0:
    if (PIND & 0b00000010) {
      if (i != 7) {
        i++;
      }
      PORTA = numbers[i];
      s = 1;
      _delay_ms(3);
    }
  break;
  case 1:
    if (!(PIND & 0b00000010)) {
      s = 0;
      _delay_ms(3);
    }
  break;
  }
}
