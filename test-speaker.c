#include <kilolib.h>

uint8_t message_sent = 0;
message_t transmit_msg;

// Transmit: Always transmit a message
message_t *message_tx() {
    return &transmit_msg;
}

void message_tx_succes() {
    message_sent = 1;
}

void setup() {
    // initialize message
    transmit_msg.type = NORMAL;
    transmit_msg.crc = message_crc(&transmit_msg);
}

void loop() {
    // blink led magenta on message sent
    if (message_sent) {
        message_sent = 0;
        set_color(RGB(1,0,1));
        delay(100);
        set_color(RGB(0,0,0));
    }
}

int main() {
    // initialize hardware
    kilo_init();
    // register message_tx function
    kilo_message_tx = message_tx;
    kilo_message_tx_success = message_tx_succes;
    // register your program
    kilo_start(setup, loop);

    return 0;
}
