// #pragma GCC optimize ("O0")
// #include <stdio.h>

namespace myDHT {

enum SensorType {
    DHT11,
    DHT22
};

class Sensor {
        SensorType type;
       float curr_temp = 0.0f, avg_temp = 0.0f, hum = 0.0f;
    public:
        uint8_t bytes[5];
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
    uint8_t prev_SREG = SREG;
		uint16_t prev_TCCR3A = TCCR3A, prev_TCCR3B = TCCR3B, prev_OCR3A = OCR3A;
    // save values for relevant registers
    uint8_t current_byte = 0; // to store current byte
    int i, j;                 // iterators

    TCCR3A = 0b00000000; // outs disconnected
    TCCR3B = 0b00001001; // no prescaling
                         // CTC mode
    OCR3A  = 16159;      // count to (16159 + 1)/(16M) = 1.01ms

    cli();                        // disable interrupts (timing is important)
    DDRB  |= 0b01000000;          // pin 12 out
    PORTB &= 0b10111111;          // pull down to start communication

    TCNT3  = 0;                   // reset counter
    TIFR3 |= 0b00000010;          // clear compare match
    while(!(TIFR3 & 0b00000010)); // wait until compare match (1.01ms)

    PORTB |= 0b01000000;          // pull up to wait for response
    OCR3A = 767;                  // count to (767 + 1)/(16M) = 48us
    DDRB  &= 0b10111111;          // set as input
    while (PINB & 0b01000000);    // wait for low response
    while (!(PINB & 0b01000000)); // low response received, wait for high response for ready
    while (PINB & 0b01000000);    // get ready for bits

    for (j = 0; j < 5; j++) {
        for (i = 7; i > -1; i--) {
            while(!(PINB & 0b01000000)); // wait for high signal
            TCNT3 = 0;                   // reset counter
            TIFR3 |= 0b00000010;         // clear compare match
            while(PINB & 0b01000000);    // wait for low
            current_byte |= ((TIFR3 & 0b00000010) >> 1) << i;
            // if time spent on HIGH was greater than 48us, write 1, else write 0.
        }
        bytes[j] = current_byte;
        current_byte = 0;
    }
    while(!(PINB & 0b01000000)); // wait for pin to be released
    DDRB  |= 0b01000000; // set as out again
    PORTB |= 0b01000000; // pull up

  	// return registers to their previous states
    TCCR3A = prev_TCCR3A;
    TCCR3B = prev_TCCR3B;
 		OCR3A  = prev_OCR3A;
    SREG   = prev_SREG;

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
	avg_temp = 0.0f;
	for (int i = 0; i < 10; i++) {
		_delay_ms(25);
		receiveData();
		avg_temp += curr_temp;
	}
	avg_temp /= 10;
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
ISR (TIMER1_COMPA_vect) {
	if (high_freq) {
		TCCR0B = 0b00000100;
	} else {
		TCCR0B = 0b00000011;
	}
	high_freq = !high_freq;
}

volatile bool normal_mode = true;
volatile bool enable = false;
volatile bool alert_is_on = false;
void alert_on() {
	if (enable) {
		high_freq = true;
		TCCR0B = 0b00000011;  // start with high frequency
		TCNT1 = 0;        	  // reset counter
		TCCR0A  = 0b11000011; // turn on PWM output
		TIMSK1 |= 0b00000010; // enable timer 1 interrupt
		alert_is_on = true;
	}
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

	cli();
	PORTA = inst;
	PORTC &= 0b00111111; // Select IR, Write
	PORTC |= 0b00100000; // enable high
	PORTC &= 0b11011111; // enable low
	sei();

	PORTA  = 0b00000000; // clear pins
	DDRA   = 0b00000000; // 22 - 29 in
	PORTC |= 0b01000000; // Read BF
	PORTC |= 0b00100000; // enable high
	PORTC &= 0b11011111; // enable low
	while (PINA & 0b10000000) { // wait for BF to be low
		PORTC |= 0b00100000; // enable high
		PORTC &= 0b11011111; // enable low
	}
	DDRA   = 0b11111111; // 22 - 29 out

	_delay_us(1);
}

// Enviar dados ao LCD
void lcd_send(uint8_t data) {
	cli();
	PORTA = data;
	PORTC |= 0b10000000; // Select DR
	PORTC &= 0b10111111; // Write to DR
	PORTC |= 0b00100000; // enable high
	PORTC &= 0b11011111; // enable low
	sei();

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
	DDRA   = 0b11111111; // 22 - 29

	_delay_us(1);
}

// Escreve string em posicao
void lcd_string(const char* str, uint8_t addr) {
	lcd_exec(set_ddram_addr(addr));
	while (*str != '\0') {
		lcd_send(*(str++));
	}
}

void lcd_float(float val, uint8_t addr) {
	char str[6];
	dtostrf(val, 6, 2, str);
	lcd_string(str, addr);
}

bool buttons[5];

ISR(INT4_vect) {
  if (!buttons[4]) {
    if (!(PINE & 0b00010000)) {
      if (enable) {
				alert_off();
				enable = false;
				lcd_string("Off", 0x0D);
			} else {
				enable = true;
				lcd_string(" On", 0x0D);
			}
      buttons[4] = true;
    }
  } else {
    if (PINE & 0b00010000) {
      buttons[4] = false;
    }
  }

	_delay_ms(3);
}

float trigger = 27.0f;

uint8_t EEPROM_read(uint16_t addr) {
	uint8_t prev_SREG = SREG;
	while(EECR & (1<<EEPE));
	EEAR = addr;
	EECR |= (1<<EERE);
	SREG = prev_SREG;
	return EEDR;
}

void EEPROM_write(uint16_t addr, uint8_t data) {
	uint8_t prev_SREG = SREG;
	while(EECR & (1<<EEPE));
	EEAR = addr;
	EEDR = data;
	EECR |= (1<<EEMPE);
	EECR |= (1<<EEPE);
	SREG = prev_SREG;
}

void EEPROM_write_float(uint16_t addr, float data) {
	uint8_t prev_SREG = SREG;
	float* p_data = &data;
	uint8_t* bytes = (uint8_t*)p_data;

	for (uint8_t i = 0; i < 4; i++) {
		EEPROM_write(addr + i, bytes[i]);
	}
}

float EEPROM_read_float(uint16_t addr) {
	uint8_t prev_SREG = SREG;
	uint8_t bytes[4];

	for (uint16_t i = 0; i < 4; i++) {
		bytes[i] = EEPROM_read(addr + i);
	}

	return *((float*)bytes);
}
uint8_t celsius = 0xFF;
uint8_t interval = 0;

ISR(INT3_vect) {
	// Serial.println("int3");
  if (!buttons[3]) {
    if (!(PIND & 0b00001000)) {
		trigger -= 0.5f;
		if (trigger < 0.0f) {
			trigger = 0.0f;
		}
		eeprom_write_float(0x000, trigger);

		lcd_float(celsius ? trigger : trigger*1.8 + 32, 0x46);
      	buttons[3] = true;
    }
  } else {
    if (PIND & 0b00001000) {
      buttons[3] = false;
    }
  }
	_delay_ms(3);
}

ISR(INT2_vect) {
	// Serial.println("int2");
  if (!buttons[2]) {
    if (!(PIND & 0b00000100)) {
			trigger += 0.5f;
			if (trigger > 40.0f) {
				trigger = 40.0f;
			}
			eeprom_write_float(0x000, trigger);
			
			lcd_float(celsius ? trigger : trigger*1.8 + 32, 0x46);
      buttons[2] = true;
    }
  } else {
    if (PIND & 0b00000100) {
      buttons[2] = false;
    }
  }

	_delay_ms(3);
}

ISR(INT1_vect) {
	if (!buttons[1]) {
			if (!(PIND & 0b00000010)) {
				celsius = ~celsius;
				eeprom_write_byte((uint8_t*)0x100, celsius);
				if (celsius == 0xFF) {
					lcd_string("C", 0x0B);
					lcd_string("C", 0x4D);
					lcd_float(trigger, 0x46);
				} else {
					lcd_string("F", 0x0B);
					lcd_string("F", 0x4D);
					lcd_float(trigger*1.8 + 32, 0x46);
				}
			buttons[1] = true;
			}
	} else {
		if (PIND & 0b00000010) {
			buttons[1] = false;
		}
	}
	_delay_ms(3);
}

void mode0_setup() {
	lcd_exec(clear_display());

	lcd_string("T =", 0x00);
	lcd_string("Trig:", 0x40);
	lcd_string("Off", 0x0D);
	lcd_exec(set_ddram_addr(0x0A));
	lcd_send(0xDF);
	lcd_exec(set_ddram_addr(0x4C));
	lcd_send(0xDF);

	if (eeprom_read_byte((uint8_t*)0x100) != celsius) {
		celsius = 0x00;
		lcd_string("F", 0x0B);
		lcd_string("F", 0x4D);
	} else {
		lcd_string("C", 0x0B);
		lcd_string("C", 0x4D);
	}

	float aux = eeprom_read_float(0x000);
// 	if (*((uint32_t*)(&aux)) != 0xFFFFFFFF) {
// 		trigger = eeprom_read_float(0x000);
// 		Serial.println(*((uint32_t*)(&aux)), BIN);
// ;	}
	lcd_float(celsius ? trigger : trigger*1.8 + 32, 0x46);
}

int main() {
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

	// timer 3 is only used for delays, registers defined differently for each case.

	// inout
	DDRA   = 0b11111111; // 22 - 29 out     (LCD data)
	DDRC  |= 0b11100000; // 30 - 32 out     (LCD ctrl)
	DDRB  |= 0b11000000; // 12 - 13 out     (buzzer pwm and dht11)
	PORTB |= 0b01000000; // 12 high         (pull up dht ctrl pin)
	DDRD  &= 0b11110000; // 18 - 21 in      (push-buttons pins)
	PORTD |= 0b00001111; // 18 - 21 pull up (pull up)
	DDRE  &= 0b11101111; // 2 in            (enable push-button)
	PORTE |= 0b00010000; // 2 pull up       (pull up)

	// initialize button states
	buttons[4] = PINE & 0b00010000;
	buttons[3] = PIND & 0b00001000;
	buttons[2] = PIND & 0b00000100;
	buttons[1] = PIND & 0b00000010;
	buttons[0] = PIND & 0b00000001;

	// external interrupts
	EICRA  = 0b11111111; // extint 0 - 3 any edge
	EICRB |= 0b00000011; // extint 4 any edge
	EIMSK |= 0b00011111; // enable extint 0 - 4

	sei();
	// LCD init
	_delay_ms(5);

	lcd_exec(function_set(true, true, false)); // 8bits interface, two lines, font 5x8
	lcd_exec(display_control(true, false, false)); // display on, curs off, blink off
	lcd_exec(entry_mode_set(true, false)); // increment right, no shift

	mode0_setup();

	float temp;
 	while(true) {
   		// Get temperature reading
		dht11.receive10();
		temp = dht11.getAvgTemperature();
		lcd_float(celsius ? temp : temp*1.8 + 32, 0x04);
		// Serial.println(temp);
		// Serial.println(trigger);
		// Serial.println();
		temp = dht11.getAvgTemperature();

		if (temp > trigger) {
			alert_on();
		} else if (temp <= trigger) {
			alert_off();
		}
	}
}