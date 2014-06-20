#include "kilolib.h"

// Pure Listener 
// to show when messages are being transmitted by neighboring robots

// Recieve: If new_message == 1, then a new message was recieved. 
// Must reset new_message after you've read it in the program
// Note that old messages get overwritten by new ones.
// Called when the messaging subsystem receives a message
int new_message = 0;
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
