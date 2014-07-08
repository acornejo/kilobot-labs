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
    // blink magenta when is message sent
    if (message_sent) {
        message_sent = 0;
        set_color(RGB(1,0,1));
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
    // initialize hardware
    kilo_init();
    // register message_tx function
    kilo_message_tx = message_tx;
    kilo_message_tx_success = message_tx_success;
    // register your program
    kilo_start(setup, loop);

    return 0;
}
