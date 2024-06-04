int main() {
    // UART config
    UCSR0A |= (1<<U2X0);
    UCSR0B |= (1<<RXCIE0)|(1<<RXEN0)|(1<<TXEN0);  // en recv intr, en recv, en trans
    UCSR0C |= (1<<UCSZ00)|(1<<UCSZ01); // even parity, char size 8
    UCSR0C &= ~(1 << UCPOL0);                     // rising edge
    UBRR0   = 0;                                  // baud rate 1Mbps
    sei();

    while (true) {

    }
    return 0;
}

ISR(USART_RX_vect) {
	uint8_t val = UDR0;
  	switch (val) {
    case 'a':
        while (!(UCSR0A & (1 << UDRE0))); // wait while not empty
		UDR0 = 'k';
        break;
    case 's':
        String s = "r";
        while (!(UCSR0A & (1 << UDRE0))); // wait while not empty
		UDR0 = s.length();
        for (int i = 0; i < s.length(); i++) {
            while (!(UCSR0A & (1 << UDRE0)));
            UDR0 = s[i];
        }
    }
}