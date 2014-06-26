#include <kilolib.h>

// declare variables
uint8_t new_message = 0;
uint8_t odd = 0;
uint8_t dist = 0;
distance_measurement_t dist_measure;

void setup() { }

void loop() {
    // Blink led yellow when on message received
    if (new_message) {
        new_message = 0;
        dist = estimate_distance(&dist_measure);
        if (dist <= 50) { // close
            if (odd)          // odd
                set_color(RGB(0,0,1)); // blink blue
            else              // even
                set_color(RGB(1,0,0)); // blink red
        } else {           // far-away
            if (odd)          // odd
                set_color(RGB(0,1,1)); // blink cyan
            else              // even
                set_color(RGB(1,0,1)); // blink magenta
        }
        delay(100);
        set_color(RGB(0,0,0));
    }
}

// update even an odd with message reception
void message_rx(message_t *m, distance_measurement_t *d) {
    odd = m->data[0];
    dist_measure = *d;
    new_message = 1;
}

int main() {
    kilo_init();
    kilo_message_rx = message_rx;
    kilo_start(setup, loop);

    return 0;
}
