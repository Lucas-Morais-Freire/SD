#include <Arduino.h>
#line 1 "C:\\Projetos\\github\\SD\\slave_usart_test\\slave_usart_test.ino"
void r_led();
void y_led();
void g_led();

// 0: led pin D7;
// 1: led pin D6;
// 2: led pin D5;
#line 8 "C:\\Projetos\\github\\SD\\slave_usart_test\\slave_usart_test.ino"
int main();
#line 8 "C:\\Projetos\\github\\SD\\slave_usart_test\\slave_usart_test.ino"
int main() {
  	// inout
	DDRD  |= 0b11100000; // D5 - D7 out
  	PORTD &= 0b00011111; // D5 - D7 low
  
  	// USART config
 // UCSR0A default
  	UCSR0B |= (1 << RXEN0)|(1 << RXCIE0); // rcv en, rcv intr en
    UCSR0C |= (1 << UCSZ01)|(1 << UCSZ00); // character size 8
  	UCSR0C &= ~(1 << UCPOL0); // rising edge
      
    UBRR0 = 103; // baud rate 9600
  
  	sei(); // global interrupt enable
  
  	while(true) {
      	int x = 0;
        x++;
  	}
	return 0;
}

ISR(USART_RX_vect) {
	uint8_t val = UDR0;
  	switch(val) {
    case '0':
      	r_led();
      	break;
    case '1':
      	y_led();
      	break;
    case '2':
      	g_led();
  	}
}

void r_led() {
	PORTD |= 0b10000000; // D7     high
  	PORTD &= 0b10011111; // D5, D6 low
}

void y_led() {
	PORTD |= 0b01000000; // D6     high
  	PORTD &= 0b01011111; // D5, D7 low
}

void g_led() {
	PORTD |= 0b00100000; // D5     high
  	PORTD &= 0b00111111; // D6, D7 low
}
