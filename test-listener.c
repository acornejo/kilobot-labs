#include <kilolib.h>

uint8_t new_message = 0;

// turn flag on message reception
void message_rx(message_t *msg, distance_measurement_t *dist) {
    new_message = 1;
}

void setup() {
    // put your setup code here, to be run only once
}

// Your program loop
void loop() {
    // Blink led yellow when you get a message
    if (new_message) {
        new_message = 0;
        set_color(RGB(1,1,0));
        delay(100);
        set_color(RGB(0,0,0));
    }
}

int main() {
    // initialize hardware
    kilo_init();
    // register message reception callback
    kilo_message_rx = message_rx;
    // register your program
    kilo_start(setup, loop);

    return 0;
}
