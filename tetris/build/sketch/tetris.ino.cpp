#include <Arduino.h>
#line 1 "C:\\Projetos\\github\\SD\\tetris\\tetris.ino"
// #pragma GCC optimize("O0")
#include "src/ntendo.h"

#line 4 "C:\\Projetos\\github\\SD\\tetris\\tetris.ino"
int main();
#line 4 "C:\\Projetos\\github\\SD\\tetris\\tetris.ino"
int main() {
    DDRA = 0xFF;
    DDRC = 0xFF;
    DDRL = 0xFF;

    ntd::ntendo_ ntendo;

    bool frame[24][16];
    char* inputs;
    uint8_t len;
    uint64_t curr_frame;

    memset(frame, 0, 24*16*sizeof(bool));

    ntendo.set_ports(&PORTA, &PORTC, &PORTL, &DDRF, &DDRK);
    ntendo.begin(30);
    while (true) {
        inputs = ntendo.get_inputs();
        len    = ntendo.get_input_len();
        curr_frame = ntendo.get_frame_count();

        for (int i = 0; i < 24; i++) {
            for (int j = 0; j < 16; j++) {
                frame[i][j] = (i + j) % 2 == 0 ? true : false;
            }
        }
        for (int i = 0; i < 24; i++) {
            for (int j = 0; j < 16; j++) {
                frame[i][j] = (i + j) % 3 == 0 ? true : false;
            }
        }
        for (int i = 0; i < 24; i++) {
            for (int j = 0; j < 16; j++) {
                frame[i][j] = (i + j) % 4 == 0 ? true : false;
            }
        }
        for (int i = 0; i < 24; i++) {
            for (int j = 0; j < 16; j++) {
                frame[i][j] = (i + j) % 5 == 0 ? true : false;
            }
        }
        for (int i = 0; i < 24; i++) {
            for (int j = 0; j < 16; j++) {
                frame[i][j] = (i + j) % 6 == 0 ? true : false;
            }
        }
        for (int i = 0; i < 24; i++) {
            for (int j = 0; j < 16; j++) {
                frame[i][j] = (i + j) % 7 == 0 ? true : false;
            }
        }
        for (int i = 0; i < 24; i++) {
            for (int j = 0; j < 16; j++) {
                frame[i][j] = (i + j) % 8 == 0 ? true : false;
            }
        }
        ntendo.frame_ready(frame);
    }

    return 0;
}
