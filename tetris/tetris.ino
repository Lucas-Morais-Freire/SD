// #pragma GCC optimize("O0")
#include "src/ntendo.h"

int main() {
    DDRA = 0xFF;
    DDRC = 0xFF;
    DDRL = 0xFF;

    ntd::ntendo_ ntendo;

    bool frame[24][16];
    char* inputs;
    uint64_t curr_frame;

    ntendo.set_ports(&PORTA, &PORTC, &PORTL, &DDRF, &DDRK);
    ntendo.begin(30);
    while (true) {
        inputs = ntendo.get_inputs();
        curr_frame = ntendo.get_frame_count();
        // tetris
        //
        ntendo.frame_ready(frame);
    }

    return 0;
}