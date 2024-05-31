#line 1 "C:\\Projetos\\github\\SD\\tetris\\src\\ntendo.cpp"
#include "ntendo_private.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>

namespace ntd {

void ntendo_::set_ports(volatile uint8_t *PORTt,
                        volatile uint8_t *PORTm,
                        volatile uint8_t *PORTb,
                        volatile uint8_t *PORTl,
                        volatile uint8_t *PORTr) {
    _PORTt = PORTt;
    _PORTm = PORTm;
    _PORTb = PORTb;
    _PORTl = PORTl;
    _PORTr = PORTr;
}

void ntendo_::begin(uint8_t frame_rate) {
    _scans_per_frame = 60/frame_rate;

    _temp_frame = new uint8_t*[24];
    _temp_frame[0] = new uint8_t[16*24];
    for (int i = 1; i < 24; i++) {
        _temp_frame[i] = _temp_frame[i - 1] + 16;
    }
    memset((void*)(_temp_frame[0]), 0, 48);

    _write_frame = new uint8_t*[24];
    _write_frame[0] = new uint8_t[16*24];
    for (int i = 1; i < 24; i++) {
        _write_frame[i] = _write_frame[i - 1] + 16;
    }
    memset((void*)(_write_frame[0]), 0, 48);

    _read_inputs = new char[256];
    _temp_inputs = new char[256];

    cli();
    // UART config
    // USCR0A default
    UCSR0B |= (1<<RXCIE0)|(1<<RXEN0)|(1<<TXEN0);  // en recv intr, en recv, en trans
    UCSR0C |= (1<<UPM01)|(1<<UCSZ00)|(1<<UCSZ01); // even parity, char size 8
    UCSR0C &= ~(1 << UCPOL0);                     // rising edge
    UBRR0   = 0;                                  // baud rate 1Mbps

    // timer 1 config
    // TCCR1A default
    TCCR1B |= (1<<WGM12)|(1<<CS10); // CTC mode, prescaler 1
    TIMSK1 |= (1<<OCIE1A);          // enable interrupt with reg A
    OCR1A = 11110;                  // interrupt every 11111/(16M) = 0.0006944375 seconds (approx 1/(60*24))

    sei();
    while(_frame_ready);
}

void ntendo_::frame_ready(bool (&frame)[24][16]) {
    memset(_temp_frame, 0, 48);
    for (int i = 0; i < 24; i++) {
        for (int j = 0; j < 16; j++) {
            if (j < 8) {
                _temp_frame[i][0] = frame[i][7 - j] << j;
            } else {
                _temp_frame[i][1] = frame[i][15 - j] << (j - 8);
            }
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

}

ISR(TIMER1_COMPA_vect) {
    static uint8_t line = 23;
    static uint64_t scan_count = ntd::_scans_per_frame;
    if (scan_count == ntd::_scans_per_frame) {
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
                while (!(UCSR0A & (1 << UDRE0)));
                UDR0 = 's';
                // comecar a mostrar graficos do jogo
                ntd::_state = ntd::GAME;
                // liberar logica do jogo
                ntd::_frame_ready = false;
                break;

            // erro de paridade
            case ntd::PARITY_ERROR:
                // byte de reenviar
                while (!(UCSR0A & (1 << UDRE0)));
                UDR0 = 'r';
                break;
            }
            ntd::_recv_state = ntd::NO_RESPONSE;
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
                    while (!(UCSR0A & (1 << UDRE0)));
                    UDR0 = 'a';
                    ntd::_state = ntd::ACK_CONTROLLER;
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
                    while (!(UCSR0A & (1 << UDRE0)));
                    UDR0 = 's';

                    // incrementar contagem de frames
                    ntd::_frame_count++;
                    // liberar para logica do jogo
                    ntd::_frame_ready = false;
                    break;

                // erro de paridade
                case ntd::PARITY_ERROR:
                    // pedir para reenviar
                    while (!(UCSR0A & (1 << UDRE0)));
                    UDR0 = 'r';

                    // incrementar contagem de frames
                    ntd::_frame_count++;
                    // liberar para logica do jogo
                    ntd::_frame_ready = false;
                    break;
                }
                ntd::_recv_state = ntd::NO_RESPONSE;
            } // se o frame nao ficou pronto, nao fazer nada
            
            break;
        }

        line = 0;
    }

    // placeholder (andre)
    *ntd::_PORTt = 1 << line;
    *ntd::_PORTl = 0x00;
    // end placeholder

    if (line == 23) {
        line = 0;
        scan_count = scan_count == ntd::_scans_per_frame ? 0 : scan_count++;
    } else {
        line++;
    }
}

ISR(USART0_RX_vect) {
    static int place;
    static bool error;
    sei();

    if (ntd::_recv_state == ntd::NO_RESPONSE) {
        error = false;
        place = -1;
        ntd::_recv_state == ntd::RECEIVING;
    }

    error = error || ((bool)(UCSR0A & (1<<UPE0)));
    uint8_t val = UDR0;

    switch (ntd::_state) {
    case ntd::ACK_CONTROLLER:
        if (error) {
            ntd::_recv_state = ntd::PARITY_ERROR;
            return;
        }
        if (val == 'k') {
            ntd::_recv_state = ntd::SUCCESS;
        } else {
            ntd::_recv_state = ntd::NO_RESPONSE;
        }
        break;
    case ntd::GAME:
        if (place == -1) {
            ntd::_temp_len = val;
        } else {
            ntd::_temp_inputs[place] = val;
        }

        place++;

        ntd::_recv_state = error ? ntd::PARITY_ERROR : ntd::SUCCESS;
    }
}