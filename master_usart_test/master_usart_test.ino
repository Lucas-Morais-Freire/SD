// 0: led pin D7;
// 1: led pin D6;
// 2: led pin D5;
int main() {
  	// inout
	DDRD  |= 0b00000010; // D1 out
  
  	// USART config
 	// UCSR0A default
  	UCSR0B |= (1 << TXEN0); // trns en
    UCSR0C |= (1 << UCSZ01)|(1 << UCSZ00); // character size 8
  	UCSR0C &= ~(1 << UCPOL0); // rising edge
      
    UBRR0 = 103; // baud rate 9600

	// setup
	uint8_t val = '0'; // initial

  	while(true) {
      	while (!(UCSR0A & (1 << UDRE0))); // wait while not empty
		UDR0 = val;
		val = val == '2' ? '0' : val + 1;
		_delay_ms(500);
  	}
	return 0;
}