#include <string.h>
uint16_t start, end;

int main() {
    // UART config
    UCSR0A |= (1<<U2X0);
    UCSR0B |= (1<<RXCIE0)|(1<<RXEN0)|(1<<TXEN0);  // en recv intr, en recv, en trans
    UCSR0C |= (1<<UPM01)|(1<<UCSZ00)|(1<<UCSZ01); // even parity, char size 8
    UCSR0C &= ~(1 << UCPOL0);                     // rising edge
    UBRR0   = 0;                                  // baud rate 1Mbps
    sei();

    // timer 1 config
    // TCCR1A default
    TCCR1B |= (1<<WGM12)|(1<<CS11); // CTC mode, prescaler 8
    TCNT1 = 0;
    while (!(UCSR0A & (1<<UDRIE0)));
    UDR0 = 'a';
    start = TCNT1;

    while (true) {
        
    }
    return 0;
}

String s;

ISR(USART0_RX_vect) {
    uint8_t val = UDR0;
    if (val == 'k') {
        end = TCNT1;
        while (!(UCSR0A & (1<<UDRIE0)));
    	UDR0 = '\n';
		s = String(end - start) + '\n';
		for (int i = 0; i < s.length(); i++) {
			while (!(UCSR0A & (1<<UDRIE0)));
			UDR0 = s[i];
		}
    }
}