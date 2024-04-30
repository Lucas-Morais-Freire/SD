#include <Arduino.h>
#line 1 "C:\\Projetos\\github\\SD\\dht_test\\dht_test.ino"
namespace myDHT {

enum SensorType {
    DHT11,
    DHT22
};

class Sensor {
        SensorType type;
        float curr_temp = 0.0f, avg_temp = 0.0f, hum = 0.0f;
        uint8_t bytes[5];
    public:
        float last10[10];
        Sensor(SensorType type);
        void receiveData();
        void receive10();
        float getCurrTemperature();
        float getAvgTemperature();
        float getHumidity();
};

Sensor::Sensor(SensorType type) {
    this->type = type;
}

void Sensor::receiveData() {
    uint8_t prev_SREG = SREG, prev_TCCR2A = TCCR2A, prev_TCCR2B = TCCR2B, prev_OCR2A = OCR2A;
    // save values for relevant registers
    uint8_t current_byte = 0; // to store current byte
    int i, j;                 // iterators

    TCCR2A = 0b00000010; // outs disconnected
    TCCR2B = 0b00000101; // prescaler 128
                         // CTC mode
    OCR2A  = 126;        // count to (126 + 1)*128/(16M) = 1.016ms

    cli();                        // disable interrupts (timing is important)
    DDRB  |= 0b01000000;          // pin 12 out
    PORTB &= 0b10111111;          // pull down to start communication

    TIFR2 |= 0b00000010;          // clear compare match
    TCNT2  = 0;                   // reset counter
    while(!(TIFR2 & 0b00000010)); // wait until compare match (~1ms)

    PORTB |= 0b01000000;          // pull up to wait for response
    TCCR2B = 0b00000010;          // prescaler 8 for counting how long input spends on HIGH
    OCR2A = 95;                   // count to (95 + 1)*8/(16M) = 48us
    DDRB  &= 0b10111111;          // set as input
    while (PINB & 0b01000000);    // wait for low response
    while (!(PINB & 0b01000000)); // low response received, wait for high response for ready
    while (PINB & 0b01000000);    // get ready for bits

    for (j = 0; j < 5; j++) {
        for (i = 7; i > -1; i--) {
            while(!(PINB & 0b01000000)); // wait for high signal
            TIFR2 |= 0b00000010;         // clear compare match
            TCNT2 = 0;                   // reset counter
            while(PINB & 0b01000000);    // wait for low
            current_byte |= ((TIFR2 & 0b00000010) >> 1) << i;
            // if time spent on HIGH was greater than 48us, write 1, else write 0.
        }
        bytes[j] = current_byte;
        current_byte = 0;
    }
    while(!(PINB & 0b01000000)); // wait for pin to be released
    DDRB  |= 0b01000000; // set as out again
    PORTB |= 0b01000000; // pull up

  	// return registers to their previous states
    SREG   = prev_SREG;
    TCCR2A = prev_TCCR2A;
    TCCR2B = prev_TCCR2B;
 	OCR2A  = prev_OCR2A;

 	// calc temp and hum
    if (type == DHT22) {
        curr_temp = (0b10000000 & bytes[2]) ? // if value is negative
        -(((uint16_t)(bytes[2] & 0b01111111) << 8 | bytes[3])/10.0f) : // clear the negative bit and concat
        ((uint16_t)bytes[2] << 8 | bytes[3])/10.0f;                    // else, concat
    } else if (type == DHT11) {
   		curr_temp = bytes[2] + bytes[3]/10.0f;
    }

    hum = ((uint16_t)bytes[0] << 8 | bytes[1])/10.0f;
}

void Sensor::receive10() {
	uint8_t prev_TCCR3A = TCCR3A, prev_TCCR3B = TCCR3B, prev_OCR3A = OCR3A;
	// store values for relevant registers

	TCCR3A = 0b00000000; // outs disconnected
	TCCR3B = 0b00001101; // 1024 prescaler
	OCR3A  = 390;        // count to (390 + 1)*1024/(16M) = 25.024ms

	avg_temp = 0.0f;
	for (int i = 0; i < 10; i++) {
		TIFR3 |= 0b00000010;          // clear compare match
		TCNT3  = 0;                   // reset counter  
		while(!(TIFR3 & 0b00000010)); // wait 25.024ms to get next reading
		receiveData();
		avg_temp += curr_temp;
	}
	avg_temp /= 10;

 	// restore relevant registers
	TCCR2A = prev_TCCR3A;
	TCCR2B = prev_TCCR3B;
	OCR3A =  prev_OCR3A;
}

float Sensor::getHumidity() {
    return hum;
}

float Sensor::getCurrTemperature() {
    return curr_temp;
}

float Sensor::getAvgTemperature() {
    return avg_temp;
}
}

myDHT::Sensor dht11(myDHT::DHT11);

bool high_freq = false;
#line 137 "C:\\Projetos\\github\\SD\\dht_test\\dht_test.ino"
void alert_on();
#line 146 "C:\\Projetos\\github\\SD\\dht_test\\dht_test.ino"
void alert_off();
#line 153 "C:\\Projetos\\github\\SD\\dht_test\\dht_test.ino"
uint8_t clear_display();
#line 157 "C:\\Projetos\\github\\SD\\dht_test\\dht_test.ino"
uint8_t return_home();
#line 161 "C:\\Projetos\\github\\SD\\dht_test\\dht_test.ino"
uint8_t entry_mode_set(bool right, bool shift);
#line 167 "C:\\Projetos\\github\\SD\\dht_test\\dht_test.ino"
uint8_t display_control(bool display_on, bool curs_on, bool curs_blink);
#line 174 "C:\\Projetos\\github\\SD\\dht_test\\dht_test.ino"
uint8_t mov_cursor_or_shift(bool shift, bool right);
#line 180 "C:\\Projetos\\github\\SD\\dht_test\\dht_test.ino"
uint8_t function_set(bool interf_8bits, bool two_lines, bool font_5x8);
#line 187 "C:\\Projetos\\github\\SD\\dht_test\\dht_test.ino"
uint8_t set_cgram_addr(uint8_t addr);
#line 191 "C:\\Projetos\\github\\SD\\dht_test\\dht_test.ino"
uint8_t set_ddram_addr(uint8_t addr);
#line 196 "C:\\Projetos\\github\\SD\\dht_test\\dht_test.ino"
void lcd_exec(uint8_t inst);
#line 216 "C:\\Projetos\\github\\SD\\dht_test\\dht_test.ino"
void lcd_send(uint8_t data);
#line 238 "C:\\Projetos\\github\\SD\\dht_test\\dht_test.ino"
void lcd_string(const char* str, uint8_t addr);
#line 245 "C:\\Projetos\\github\\SD\\dht_test\\dht_test.ino"
int main();
#line 127 "C:\\Projetos\\github\\SD\\dht_test\\dht_test.ino"
ISR (TIMER1_COMPA_vect) {
	if (high_freq) {
		TCCR0B = 0b00000100;
	} else {
		TCCR0B = 0b00000011;
	}
	high_freq = !high_freq;
}

bool alert_is_on = false;
void alert_on() {
	high_freq = true;
	TCCR0B = 0b00000011;  // start with high frequency
	TCNT1 = 0;        // reset counter
	TCCR0A  = 0b11000011; // turn on PWM output
	TIMSK1 |= 0b00000010; // enable timer 1 interrupt
	alert_is_on = true;
}

void alert_off() {
	TCCR0A  = 0b00000011; // turn off PWM output
	TIMSK1 &= 0b11111101; // disable timer 1 interrupt
	alert_is_on = false;
}

// Implementacao dos comandos LCD
uint8_t clear_display() {
  	return 1;
}

uint8_t return_home() {
  	return 2;
}

uint8_t entry_mode_set(bool right, bool shift) {
  	return 0b00000100 |
	right << 1 |
	shift;
}

uint8_t display_control(bool display_on, bool curs_on, bool curs_blink) {
  	return 0b00001000 |
	display_on << 2 |
	curs_on    << 1 |
	curs_blink;
}

uint8_t mov_cursor_or_shift(bool shift, bool right) {
  	return 0b00010000 |
	shift << 3 |
	right << 2;
}

uint8_t function_set(bool interf_8bits, bool two_lines, bool font_5x8) {
	return 0b00100000 |
    interf_8bits << 4 |
    two_lines    << 3 |
    font_5x8     << 2;
}

uint8_t set_cgram_addr(uint8_t addr) {
	return 0b01000000 | addr;
}

uint8_t set_ddram_addr(uint8_t addr) {
	return 0b10000000 | addr;
}

// Executar instrucoes no LCD
void lcd_exec(uint8_t inst) {
	PORTA = inst;
	PORTC &= 0b00111111; // Select IR, Write
	PORTC |= 0b00100000; // enable high
	PORTC &= 0b11011111; // enable low

	PORTA  = 0b00000000; // clear pins
	DDRA   = 0b00000000; // 22 - 29 in
	PORTC |= 0b01000000; // Read BF
	PORTC |= 0b00100000; // enable high
	PORTC &= 0b11011111; // enable low
	while (PINA & 0b10000000) { // wait for BF to be low
		PORTC |= 0b00100000; // enable high
		PORTC &= 0b11011111; // enable low
	}
	_delay_us(1);
	DDRA   = 0b11111111; // 22 - 29 out
}

// Enviar dados ao LCD
void lcd_send(uint8_t data) {
	PORTA = data;
	PORTC |= 0b10000000; // Select DR
	PORTC &= 0b10111111; // Write to DR
	PORTC |= 0b00100000; // enable high
	PORTC &= 0b11011111; // enable low

	PORTA  = 0b00000000; // clear pins
	DDRA   = 0b00000000; // 22 - 29 in
	PORTC &= 0b01111111; // Select IR
	PORTC |= 0b01000000; // Read BF
	PORTC |= 0b00100000; // enable high
	PORTC &= 0b11011111; // enable low
	while (PINA & 0b10000000) { // wait for BF to be low
		PORTC |= 0b00100000; // enable high
		PORTC &= 0b11011111; // enable low
	}
	_delay_us(1);
	DDRA   = 0b11111111; // 22 - 29
}

// Escreve string em posicao
void lcd_string(const char* str, uint8_t addr) {
	lcd_exec(set_ddram_addr(addr));
	while (*str != '\0') {
		lcd_send(*(str++));
	}
}

int main() {
	// inout
	DDRB  |= 0b11000000; // pin 12, 13 out
	PORTB |= 0b01000000; // pin 12 high

	// timer 0 (buzzer PWM)
	TCCR0A = 0b00000011; // inverted mode with reg A
	TCCR0B = 0b00000011; // no prescaling
			// fast pwm, TOP 0xFF
	OCR0A = 127; // P = 50%
	// P =(256 - OCR0A - 1)/256

	// timer 1 (buzzer freq change)
	TCCR1A = 0b00000000;  // no out
	TCCR1B = 0b00001101;  // 1024 prescale
				// CTC mode
	OCR1A = 7812;         // interrupt every (7812 + 1)*1024/(16M) = 0.5s
	TIMSK1 &= 0b11111101; // disable timer 1 interrupt for now
	sei();
	Serial.begin(9600);

	DDRA =  0b11111111; // 22 - 29 out
	DDRC |= 0b11100000; // 30 - 32 out
	
	_delay_ms(50);
	lcd_exec(function_set(true, true, false)); // 8bits interface, two lines, font 5x8
	lcd_exec(display_control(true, false, false)); // display on, curs off, blink off
	lcd_exec(entry_mode_set(true, false)); // increment right, no shift
	lcd_exec(clear_display());

	lcd_string("Temp:", 0x00);
	lcd_string("123", 0x09);
	lcd_string("Var:", 0x40);
	lcd_string("40", 0x47);
	lcd_string("957", 0x4A);

 	while(true) {
   		// Get temperature reading
		dht11.receive10();
		// Serial.println(dht11.getAvgTemperature());

		if (dht11.getAvgTemperature() > 26 && !alert_is_on) {
			alert_on();
		} else if (dht11.getAvgTemperature() <= 26 && alert_is_on) {
			alert_off();
		}
	}
}
