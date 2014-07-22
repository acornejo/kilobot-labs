#include <kilolib.h>

// declare constants
#define PERIOD 32

// declare variables
uint8_t modulo_clock;
message_t msgs[PERIOD];
uint8_t offsets[PERIOD];
uint8_t total;
uint16_t average;

// send modulo_clock message
message_t *message_tx() {
    return &msgs[modulo_clock];
}

// receive modulo_clock message
void message_rx(message_t *msg, distance_measurement_t *d) {
    // store clock offset
    if (modulo_clock > msg->data[0]) {
        if (modulo_clock - msg->data[0] < PERIOD/2)
            offsets[modulo_clock-msg->data[0]]++;
    } else {
        if (msg->data[0] - modulo_clock > PERIOD/2)
            offsets[modulo_clock + (PERIOD-msg->data[0])]++;
    }
}

void setup() {
    for (int i = 0; i < PERIOD; i++) {
        msgs[i].data[0] = i;
        msgs[i].type = NORMAL;
        msgs[i].crc = message_crc(&msgs[i]);
        offsets[i] = 0;
    }
    kilo_ticks += (rand_hard())<<2;
}

void loop() {
    modulo_clock = (kilo_ticks>>2)%PERIOD;
    if (!modulo_clock) {
        // blink LED
        set_color(RGB(0,1,0));
        delay(100);
        set_color(RGB(0,0,0));
        // compute average offset
        total = 0;
        average = 0;
        for (int i = 0; i < PERIOD; i++) {
            average += i*offsets[i];
            total += offsets[i];
            offsets[i] = 0;
        }
        // adjust clock by average offset
        if (total > 0) {
            average /= total;
            kilo_ticks -= (average<<2);
        }
    }
}


int main() {
    kilo_init();
    kilo_message_rx = message_rx;
    kilo_message_tx = message_tx;
    kilo_start(setup, loop);

    return 0;
}
