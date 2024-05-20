#include <Arduino.h>
#line 1 "C:\\Projetos\\github\\SD\\master_spi_test\\master_spi_test.ino"
// 0: led pin D7;
// 1: led pin D6;
// 2: led pin D5;

uint8_t spi_transaction(uint8_t data);

#line 7 "C:\\Projetos\\github\\SD\\master_spi_test\\master_spi_test.ino"
int main();
#line 7 "C:\\Projetos\\github\\SD\\master_spi_test\\master_spi_test.ino"
int main() {
  	// inout
  	DDRB  |= 0b00101000; // SCK, MOSI out
  	DDRB  &= 0b11101111; // MISO      in
  
  	DDRD  |= 0b00000001; // D0        out (SS0)
  	PORTD |= 0b00000001; // D0        high (SS0)

	// timer 0
	TCCR0B = 0b00000001;
  
  	// SPI config
  	SPCR |= (1<<SPE)|(1<<MSTR);
    //       en spi | master
    // 		 no int, msb first, phase 0, rate fosc/4, idle SCK low
	// SPSR default
      
	// setup
	uint8_t val = '0'; // initial
  	volatile uint8_t recv;

  	while(true) {
      	PORTD &= 0b11111110;         // SS0 low
		spi_transaction(val);
        PORTD |= 0b00000001;         // SS0 high
      	val = val == '2' ? '0' : val + 1; // update val
		_delay_ms(500);
  	}
	return 0;
}

uint8_t spi_transaction(uint8_t data) {
	SPDR = data;                 // transmit data via spi.
	while (!(SPSR & (1<<SPIF))); // wait until transfer is complete.
	return SPDR;
}
