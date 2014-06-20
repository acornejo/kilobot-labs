#include "kilolib.h"

// Pure Transmitter
// Transmit: Always transmitting the message at a fixed but continual rate
message_t transmit_msg;
message_t *message_tx() {
    return &transmit_msg;
} 

void setup() {
    // put your setup code here, to be run only once
    transmit_msg.data[0] = 1;  // sending the same message always
    transmit_msg.crc = message_crc(&transmit_msg);
}

void loop() {
    // put your main code here, to be run repeatedly

    // blink led magenta twice per second
    set_color(RGB(1,0,1));
    delay(500);
    set_color(RGB(0,0,0));
    delay(500);
}

int main() {
    // initialize hardware
    kilo_init();
    // register message_tx function
    kilo_message_tx = message_tx;
    // register your program
    kilo_start(setup, loop);

    return 0;
}
