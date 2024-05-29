#ifndef NTENDO_PRIVATE_H
#define NTENDO_PRIVATE_H

#include "ntendo.h"

namespace ntd {

enum status : uint8_t {
    ACK_CONTROLLER,
    GAME
};

enum recv_status : uint8_t {
    SUCCESS,
    PARITY_ERROR,
    NO_RESPONSE
};

uint8_t _frame_rate;
volatile bool _frame_ready = true;
uint8_t** _temp_frame;
volatile uint8_t* _PORTt,* _PORTm,* _PORTb,* _PORTl,* _PORTr;
volatile uint64_t _frame_count = 0;
volatile status _state = ACK_CONTROLLER;
volatile uint8_t _recv_state = NO_RESPONSE;
volatile char* _read_inputs;
volatile char* _temp_inputs;
volatile uint8_t _len;

bool uart_ack();
void uart_trans(char c);
char uart_recv();

uint8_t** _write_frame;
}


#endif