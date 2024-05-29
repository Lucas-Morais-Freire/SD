int main() {
    // inout  
    DDRF  = 0xFF; // all columns off
    PORTF = 0x00; // PORTF at 0V

    PORTA = 0x00; // all lines off
    DDRA  = 0xFF; // PORTA out

    // timer 1
    // TCCR1A default
    TCCR1B = (1<<WGM12)|(1<<CS10); // modo CTC, prescaler 1
    OCR1A  = 11110;                // interrupcao a cada (11110 + 1)*8/(16M) ~= 1/(60*24) segundos
    TIMSK1 = (1<<OCIE1A);          // habilitar interrupcao TIMER1_COMPA
    sei();                         // global interrupt enable

    while (true) {

    }
    return 0;
}

uint8_t data[8] = {
    0b00000000,
    0b01100110,
    0b01100110,
    0b00000000,
    0b00011000,
    0b10000001,
    0b01111110,
    0b00000000,
};

ISR(TIMER1_COMPA_vect) {
    static uint8_t i = 0;

    if (i < 8) {
        DDRF = data[i];
    } else {
        DDRF = 0x00;
    }

    PORTA = 1 << i;
    i = i == 23 ? 0 : i + 1;
}