#include "src/ntendo.h"

int main() {
    bool frame[24][16];

    ntendo.set_ports(&PORTA, &PORTC, &PORTL, &PORTF, &PORTK);
    ntendo.begin(30);
    while (true) {
        ntendo.get_inputs();
        ntendo.get_frame_count();
        // tetris
        ntendo.frame_ready(frame);
    }

    return 0;
}