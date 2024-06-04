#include "src/ntendo.h"

int main() {
    uint16_t start, end;
    DDRA = 0xFF;
    DDRC = 0xFF;
    DDRL = 0xFF;

    ntd::ntendo_ ntendo;

    bool frame[24][16];
    char* inputs;
    uint8_t len;
    uint64_t curr_frame;

    int x = 1, y = 0;

    ntendo.set_ports(&PORTA, &PORTC, &PORTL, &DDRF, &DDRK);
    ntendo.begin(30);
    while (true) {
        inputs = ntendo.get_inputs();
        len    = ntendo.get_input_len();
        curr_frame = ntendo.get_frame_count();

        memset(frame, 0, 24*16*sizeof(bool));
        if (x == 0 && y <= 6) {
            y++;
        } else if (y == 7 && x <= 6) {
            x++;
        } else if (x == 7 && y >= 1) {
            y--;
        } else if (y == 0 && x >= 1) {
            x--;
        }
        frame[x][y] = true;
        ntendo.frame_ready(frame);
    }

    return 0;
}