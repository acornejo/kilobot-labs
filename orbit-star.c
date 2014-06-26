#include <kilolib.h>

// declare variables
message_t msg;
uint8_t sent_message = 0;

void setup() {
    // initialize empty beacon message
    msg.type = NORMAL;
    msg.crc = message_crc(&msg);
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
    return &msg;
} 

void message_tx_success() {
    sent_message = 1;
}

int main() {
    kilo_init();
    kilo_message_tx = message_tx;
    kilo_message_tx_success = message_tx_success;
    kilo_start(setup, loop);

    return 0;
}
