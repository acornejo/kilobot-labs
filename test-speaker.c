#include <kilolib.h>

message_t transmit_msg;

// Transmit: Always transmit a message
message_t *message_tx() {
    return &transmit_msg;
} 

void setup() {
    // initialize message
    transmit_msg.type = NORMAL;
    transmit_msg.crc = message_crc(&transmit_msg);
}

void loop() {
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
