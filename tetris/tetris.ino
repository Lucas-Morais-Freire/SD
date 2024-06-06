#include "src/ntendo.h"

void update_pos(int &x, int &y);

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

    int x = 0, y = 0, a = 23, b = 7;
    memset(frame, 1, sizeof(frame));

    ntendo.set_ports(&PORTA, &PORTC, &PORTL, &DDRF, &DDRK);
    ntendo.begin(30);
    while (true) {
        inputs = ntendo.get_inputs();
        len    = ntendo.get_input_len();
        curr_frame = ntendo.get_frame_count();

        frame[x][y] = false;
        frame[a][b] = false;
        update_pos(x, y);
        update_pos(a, b);
        frame[x][y] = true;
        frame[a][b] = true;
        frame[23][8] = !frame[23][8];
        
        ntendo.frame_ready(frame);
    }

    return 0;
}

void update_pos(int &x, int &y) {
    if (x == 0 && y <= 6) {
        y++;
    } else if (y == 7 && x <= 22) {
        x++;
    } else if (x == 23 && y >= 1) {
        y--;
    } else if (y == 0 && x >= 1) {
        x--;
    }
}