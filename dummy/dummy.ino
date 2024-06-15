int main() {
    // inout  
    DDRK  = 0x00; // all columns off
    PORTK = 0x00; // PORTK at 0V

    DDRF = 0x00;
    PORTF = 0x00;

    DDRA  = 0xFF; // PORTA out
    PORTA = 0x00; // all lines off

    DDRC = 0xFF;
    PORTC = 0x00;
    
    DDRL = 0xFF;
    PORTL = 0x00;

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

uint8_t data1[8] = {
    0b10000000,
    0b11000000,
    0b11100000,
    0b11110000,
    0b11111000,
    0b11111100,
    0b11111110,
    0b11111111,
};

uint8_t data2[8] = {
    0b11111111,
    0b01111111,
    0b00111111,
    0b00011111,
    0b00001111,
    0b00000111,
    0b00000011,
    0b00000001,
};

uint8_t data3[8] = {
    0b00011000,
    0b00111000,
    0b00111100,
    0b01111100,
    0b01111110,
    0b11111110,
    0b11111111,
    0b11000011,
};

uint8_t data4[8] = {
    0b10000001,
    0b01000010,
    0b00100100,
    0b00011000,
    0b10101010,
    0b11001100,
    0b11110000,
    0b00001111,
};

uint8_t data5[8] = {
    0b11000011,
    0b01100110,
    0b11100111,
    0b10011001,
    0b00111100,
    0b11100111,
    0b11111111,
    0b10101010,
};

uint8_t data6[8] = {
    0b10000000,
    0b11000000,
    0b01000000,
    0b01000011,
    0b11100111,
    0b10101010,
    0b01010101,
    0b11001100,
};

ISR(TIMER1_COMPA_vect) {
    static uint8_t i = 0;

    if (i < 8) {
        DDRK = data1[i];
        DDRF = data4[i];
        PORTA = 1 << i;
        PORTC = 0x00;
        PORTL = 0x00;
    } else if (i < 16) {
        DDRK = data2[i - 8];
        DDRF = data5[i - 8];
        PORTA = 0x00;
        PORTC = 1 << (i - 8);
        PORTL = 0x00;
    } else {
        DDRK = data3[i - 16];
        DDRF = data6[i - 16];
        PORTA = 0x00;
        PORTC = 0x00;
        PORTL = 1 << (i - 16);
    }
    i = i == 23 ? 0 : i + 1;
}