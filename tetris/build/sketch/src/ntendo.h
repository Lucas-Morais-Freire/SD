#line 1 "C:\\Projetos\\github\\SD\\tetris\\src\\ntendo.h"
// #pragma GCC optimize("O0")

#ifndef NTENDO_H
#define NTENDO_H
#include <avr/io.h>

namespace ntd {

struct ntendo_ {
    void set_ports(
        volatile uint8_t* PORTt,
        volatile uint8_t* PORTm,
        volatile uint8_t* PORTb,
        volatile uint8_t* DDRl,
        volatile uint8_t* DDRr
    );
    void begin(uint8_t frame_rate);

    uint64_t get_frame_count();
    char* get_inputs();
    uint8_t get_input_len();
    void frame_ready(bool (&frame)[24][16]);
};

}

// ntd::ntendo_ ntendo;

#endif // NTENDO_H