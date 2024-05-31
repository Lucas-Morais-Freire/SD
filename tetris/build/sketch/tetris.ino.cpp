#include <Arduino.h>
#line 1 "C:\\Projetos\\github\\SD\\tetris\\tetris.ino"
#include "src/ntendo.h"

#line 3 "C:\\Projetos\\github\\SD\\tetris\\tetris.ino"
int main();
#line 3 "C:\\Projetos\\github\\SD\\tetris\\tetris.ino"
int main() {
    ntd::ntendo_ ntendo;

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
