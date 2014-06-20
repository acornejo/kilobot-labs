#include <kilolib.h>

// declare message to transmit
message_t transmit_msg;
uint8_t sent_message = 0;

void setup() {
    // initialize empty beacon message
    transmit_msg.type = NORMAL;
    transmit_msg.crc = message_crc(&transmit_msg);
}

void loop() {
    if (sent_message) {
        sent_message = 0;
        set_color(RGB(1,0,0));
        delay(20);
        set_color(RGB(0,0,0));
    }
}

message_t *message_tx() {
    // transmit beacon message
    return &transmit_msg;
} 

void message_tx_success() {
    sent_message = 1;
}

int main() {
    // initialize hardware
    kilo_init();
    // register message_tx function
    kilo_message_tx = message_tx;
    kilo_message_tx_success = message_tx_success;
    // register your program
    kilo_start(setup, loop);

    return 0;
}
