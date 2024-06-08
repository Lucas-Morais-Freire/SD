#include <Arduino.h>
#line 1 "C:\\Projetos\\github\\SD\\tetris\\tetris.ino"
#include "headers/ntendo.h"

int hash(char c);

bool input_log[4] = {false};

#line 7 "C:\\Projetos\\github\\SD\\tetris\\tetris.ino"
int main();
#line 7 "C:\\Projetos\\github\\SD\\tetris\\tetris.ino"
int main() {
    // portas das saidas das matrizes
    DDRA = 0xFF;
    DDRC = 0xFF;
    DDRL = 0xFF;
    // setup da API
    ntendo.set_ports(&PORTA, &PORTC, &PORTL, &DDRF, &DDRK);
    ntendo.begin(30);

    // variaveis utilizadas para logica
    bool frame[24][16];
    char* inputs;
    uint8_t len;
    uint64_t curr_frame;

    // setup do jogo
    memset(frame, 0, sizeof(frame));

    while (true) {
        inputs     = ntendo.get_inputs();
        len        = ntendo.get_input_len();
        curr_frame = ntendo.get_frame_count();

        
    
        ntendo.frame_ready(frame);
    }

    return 0;
}

int hash(char c) {
    switch (c) {
    case 'r':
        return 0;
    case 'R':
        return 0;
    case 'l':
        return 1;
    case 'L':
        return 1;
    case 'd':
        return 2;
    case 'D':
        return 2;
    case 'u':
        return 3;
    case 'U':
        return 3;
    }
}
