#include "headers/ntendo.h"

char get_random_piece();

int main() {
    // portas das saidas das matrizes
    DDRA = 0xFF;
    DDRC = 0xFF;
    DDRL = 0xFF;
    // setup da API
    ntendo.set_ports(&PORTA, &PORTC, &PORTL, &DDRF, &DDRK);
    ntendo.begin(30);

    // variaveis utilizadas para logica
    bool     frame[24][16];
    char*    inputs;
    uint8_t  len;
    uint64_t curr_frame;

    // setup do jogo
    memset(frame, 0, sizeof(frame));
    char curr_piece = get_random_piece();
    char next_piece = get_random_piece();

    while (true) {
        inputs     = ntendo.get_inputs();
        len        = ntendo.get_input_len();
        curr_frame = ntendo.get_frame_count();

        
    
        ntendo.frame_ready(frame);
    }

    return 0;
}