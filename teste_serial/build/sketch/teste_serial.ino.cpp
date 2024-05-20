#include <Arduino.h>
#line 1 "C:\\Projetos\\github\\SD\\teste_serial\\teste_serial.ino"
#line 1 "C:\\Projetos\\github\\SD\\teste_serial\\teste_serial.ino"
int main();
#line 1 "C:\\Projetos\\github\\SD\\teste_serial\\teste_serial.ino"
int main() {
    UCSR0B  = 0b10010000;
    UCSR0C  = 0b00000110;
    UBRR0   = 103;

    DDRA  |= 0b00000001;
    PORTA &= 0b11111110;
    sei();

    while(true) {

    }
    return 0;
}

ISR(USART0_RX_vect) {
    if (UDR0 == '1') {
        PORTA |= 0b00000001;
    } else if (UDR0 = '0') {
        PORTA &= 0b11111110;
    }
}
