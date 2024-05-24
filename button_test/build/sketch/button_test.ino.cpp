#include <Arduino.h>
#line 1 "C:\\Projetos\\github\\SD\\button_test\\button_test.ino"
#include "src/button.h"

void button0_press();
void button0_hold();
void button1_press();
void button1_hold();

#line 8 "C:\\Projetos\\github\\SD\\button_test\\button_test.ino"
int main();
#line 8 "C:\\Projetos\\github\\SD\\button_test\\button_test.ino"
int main() {
	// inout
	DDRB  &= 0b11111100; // 8, 9 input
	PORTB |= 0b00000011; // 8, 9 pullup

	// setup
	Button b1(button0_press, button0_hold, &PINB, PB0, 3000);
	Button b2(button1_press, button1_hold, &PINB, PB1, 3000);

	Serial.begin(9600); // para verificar o aperto dos botoes
	sei();

	while(true) {
		b1.check_button();
		// b2.check_button();
	}
	return 0;
}

void button0_press() {
	Serial.println("press 0");
}

void button1_press() {
	Serial.println("press 1");
}

void button0_hold() {
	// Serial.println("hold 0");
}

void button1_hold() {
	// Serial.println("hold 1");
}
