#ifndef NTENDO_H
#define NTENDO_H

namespace ntd {

struct ntendo_ {
    void set_ports(
        volatile uint8_t* PORTt,
        volatile uint8_t* PORTm,
        volatile uint8_t* PORTb,
        volatile uint8_t* PORTl,
        volatile uint8_t* PORTr
    );
    void begin(uint8_t frame_rate);

    uint64_t get_frame_count();
    char* get_inputs();
    void frame_ready(bool (&frame)[24][16]);
};

}

ntd::ntendo_ ntendo;

#endif // NTENDO_H