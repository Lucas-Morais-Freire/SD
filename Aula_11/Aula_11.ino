int curr_val;

int main() {
  // intout
  DDRF &=  0b11111110; // A0 in

  Serial.begin(9600);

  // ADC
  ADCSRA = 0b10000000; // div 2
  ADMUX  = 0b00000000; // AREF, read A0

  while(1) {
    ADCSRA |= 0b01000000;
    while (!(ADCSRA & 0b00010000)) {}
    curr_val = (int)(ADCL) + ((int)(ADCH) << 8);
    Serial.println(curr_val);
  }
}
