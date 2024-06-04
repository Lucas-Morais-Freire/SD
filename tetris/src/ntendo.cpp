#include "ntendo_private.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include <WString.h>
#include <util/delay.h>

namespace ntd {

void ntendo_::set_ports(volatile uint8_t *PORTt,
                        volatile uint8_t *PORTm,
                        volatile uint8_t *PORTb,
                        volatile uint8_t *DDRl,
                        volatile uint8_t *DDRr) {
    _PORTt = PORTt;
    _PORTm = PORTm;
    _PORTb = PORTb;
    _DDRl = DDRl;
    _DDRr = DDRr;

    *_PORTb = 0b10000000;
}

void ntendo_::begin(uint8_t frame_rate) {
    _scans_per_frame = 60/frame_rate;

    _temp_frame = new uint8_t*[24];
    _temp_frame[0] = new uint8_t[2*24];
    for (int i = 1; i < 24; i++) {
        _temp_frame[i] = _temp_frame[i - 1] + 2;
    }

    _write_frame = new uint8_t*[24];
    _write_frame[0] = new uint8_t[2*24];
    for (int i = 1; i < 24; i++) {
        _write_frame[i] = _write_frame[i - 1] + 2;
    }
    memset((void*)(_write_frame[0]), 0, 48);

    _read_inputs = new char[256];
    _temp_inputs = new char[256];

    cli();
    // UART config
    UCSR0A |= (1<<U2X0);
    UCSR0B |= (1<<RXCIE0)|(1<<RXEN0)|(1<<TXEN0);  // en recv intr, en recv, en trans
    UCSR0C |= (1<<UCSZ00)|(1<<UCSZ01); // even parity, char size 8
    UCSR0C &= ~(1 << UCPOL0);                     // rising edge
    UBRR0   = 0;                                  // baud rate 1Mbps

    // timer 1 config
    // TCCR1A default
    TCCR1B |= (1<<WGM12)|(1<<CS10); // CTC mode, prescaler 1
    OCR1A = 11110;                  // interrupt every 11111/(16M) = 0.0006944375 seconds (approx 1/(60*24))
    TIMSK1 |= (1<<OCIE1A);          // enable interrupt with reg A

    sei();
    while(_frame_ready);
}

void ntendo_::frame_ready(bool (&frame)[24][16]) {
    volatile uint8_t byte;
    for (int i = 0; i < 24; i++) {
        for (int j = 0; j < 2; j++) {
            byte = 0;
            for (int k = 0; k < 8; k++) {
                if (frame[i][(j << 3) + k]) {
                    byte |= (1 << k);
                }
            }
            _temp_frame[i][j] = byte;
        }
    }
    _frame_ready = true;
    while(_frame_ready);
}

uint64_t ntendo_::get_frame_count() {
    return _frame_count;
}

char* ntendo_::get_inputs() {
    return (char*)_read_inputs;
}

uint8_t ntendo_::get_input_len() {
    return _read_len;
}

}

ISR(TIMER1_COMPA_vect) {
    static uint8_t line = 0;
    static uint8_t scan_count = ntd::_scans_per_frame;
    if (scan_count == ntd::_scans_per_frame) {
        scan_count = 0;
        switch (ntd::_state) {
        case ntd::ACK_CONTROLLER: // verificar se controle responde
            switch (ntd::_recv_state) {
            
            // sem resposta
            case ntd::NO_RESPONSE:
                /// modificar lcd para "sem controle"

                // enviar byte de ack ate responder
                while (!(UCSR0A & (1 << UDRE0)));
                UDR0 = 'a';
                break;

            // recebeu com sucesso
            case ntd::SUCCESS:
                // pedir para enviar entradas
                ntd::_state = ntd::GAME;
                ntd::_recv_state = ntd::NO_RESPONSE;
                while (!(UCSR0A & (1 << UDRE0)));
                UDR0 = 's';
                break;
            }
            break;

        case ntd::GAME: // processamento grafico do jogo
            if (ntd::_frame_ready) { // se o frame esta pronto
                // trocar os frames
                volatile uint8_t** swap_frames = ntd::_temp_frame;
                ntd::_temp_frame = ntd::_write_frame;
                ntd::_write_frame = swap_frames;

                // verificar recepcao de entradas do controle
                switch (ntd::_recv_state) {
            
                // sem resposta
                case ntd::NO_RESPONSE:
                    // modificar lcd para "sem controle" (TODO)

                    // enviar byte de ack
                    ntd::_state = ntd::ACK_CONTROLLER;
                    while (!(UCSR0A & (1 << UDRE0)));
                    UDR0 = 'a';
                    break;

                // recebeu com sucesso
                case ntd::SUCCESS:
                    // trocar a string do frame anterior
                    // pela string recentemente recebida  
                    volatile char* swap_inputs = ntd::_temp_inputs;
                    ntd::_temp_inputs = ntd::_read_inputs;
                    ntd::_read_inputs = swap_inputs;
                    
                    // carregar o novo comprimento da string
                    ntd::_read_len = ntd::_temp_len;

                    // pedir para enviar entradas
                    ntd::_recv_state = ntd::NO_RESPONSE;
                    while (!(UCSR0A & (1 << UDRE0)));
                    UDR0 = 's';

                    // incrementar contagem de frames
                    ntd::_frame_count++;
                    // liberar para logica do jogo
                    ntd::_frame_ready = false;
                    break;
                }
            } else {
                while (!(UCSR0A & (1 << UDRE0)));
                UDR0 = 'n';
            }
            break;
        }
    }

    uint8_t temp = *ntd::_PORTt >> 7;
    *ntd::_PORTt = (*ntd::_PORTt << 1) | (*ntd::_PORTb >> 7);
    *ntd::_PORTb = (*ntd::_PORTb << 1) | (*ntd::_PORTm >> 7);
    *ntd::_PORTm = (*ntd::_PORTm << 1) | temp;

    *ntd::_DDRl = ntd::_write_frame[line][0];
    *ntd::_DDRr = ntd::_write_frame[line][1];

    if (line == 23) {
        scan_count++;
        line = 0;
    } else {
        line++;
    }
}

ISR(USART0_RX_vect) {
    UCSR0B &= ~(1<<RXCIE0);
    uint8_t val = UDR0;

    switch (ntd::_state) {
    case ntd::ACK_CONTROLLER:
        if (val == 'k') {
            ntd::_recv_state = ntd::SUCCESS;
        } else {
            ntd::_recv_state = ntd::NO_RESPONSE;
        }
        break;
    case ntd::GAME:
        ntd::_temp_len = val;
        for (int i = 0; i < ntd::_temp_len; i++) {
            while (!(UCSR0A & (1<<RXC0)));
            ntd::_temp_inputs[i] = (char)UDR0;
        }
        ntd::_recv_state = ntd::SUCCESS;
        break;
    }
    UCSR0B |= (1<<RXCIE0);
}