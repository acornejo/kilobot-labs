#include <kilolib.h>

// declare variables
uint8_t message_sent = 0;
message_t msg;

void setup() {
    // initialize message
    msg.type = NORMAL;
    msg.crc = message_crc(&msg);
}

void loop() {
    // blink red when message is sent
    if (message_sent) {
        message_sent = 0;
        set_color(RGB(1,0,0));
        delay(20);
        set_color(RGB(0,0,0));
    }
}

message_t *message_tx() {
    return &msg;
} 

void message_tx_success() {
    message_sent = 1;
}

int main() {
    kilo_init();
    kilo_message_tx = message_tx;
    kilo_message_tx_success = message_tx_success;
    kilo_start(setup, loop);

    return 0;
}
