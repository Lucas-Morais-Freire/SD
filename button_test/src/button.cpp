#include "button.h"
#include <avr/interrupt.h>

namespace ntd {

volatile uint64_t ovfs = 0;

button::button(void (*func_press)(), void (*func_hold)(), volatile uint8_t* PINx, uint8_t Pxn, uint64_t delay) {
	static bool setup_done = false;
	if (!setup_done) {
		// timer 1
		// TCCR1A default
		TCCR1B = (1<<CS11);  // prescaler 8
		TIMSK1 = (1<<TOIE1); // overflow interrupt enable
		sei();
		setup_done = true;
	}
	state = !(*PINx & (1<<Pxn));    // o estado inicial eh o inverso do estado do pino (0V = apertado, 5V = desapertado)
	this->func_press = func_press;
	this->func_hold = func_hold;
	this->delay = delay << 1;
	this->time = 0;
	this->cycle = 0;
	this->PINx = PINx;
	this->Pxn = Pxn;
}

void button::check_button() {
	// se o tempo do delay tiver passado
	if (delay <= (TCNT1 + 0x10000*(ovfs - cycle)) - time) {
		// e o estado for "desapertado"
		if (!state) {
			// e o pino estiver em 0V
			if (!(*PINx & (1<<Pxn))) {
				func_press(); // fazer algo
				state = true; // trocar estado
				time = TCNT1; // registrar tempo
				cycle = ovfs; // e ciclo
			}
		} else { // se o estado for "apertado"
			// e o pino esta em 5V
			if (*PINx & (1<<Pxn)) {
				state = false;
				time = TCNT1;
				cycle = ovfs;
			} else { // se ainda estiver em 0V
				func_hold();
			}
		}
	}
}

}

ISR(TIMER1_OVF_vect) {
	ntd::ovfs++;
}