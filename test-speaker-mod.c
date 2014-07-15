#include <kilolib.h>

// declare variables
uint32_t last_changed = 0;
uint8_t message_sent = 0;
uint8_t odd = 0;
message_t message_odd;
message_t message_even;

void setup() {
    // initialize even an odd messages
    message_odd.type = NORMAL;
    message_odd.data[0] = 1;
    message_odd.crc = message_crc(&message_odd);
    message_even.type = NORMAL;
    message_even.data[0] = 0;
    message_even.crc = message_crc(&message_even);
}

void loop() {
    // toggle even/odd  every 2 seconds (64 ticks)
    if (kilo_ticks > last_changed + 64) {
        last_changed = kilo_ticks;
        odd = !odd;
    }

    // blink led magenta on message sent
    if (message_sent) {
        message_sent = 0;
        set_color(RGB(1,0,1));
        delay(100);
        set_color(RGB(0,0,0));
    }
}

message_t *message_tx() {
    if (odd)
        return &message_odd;
    else
        return &message_even;
}

void message_tx_succes() {
    message_sent = 1;
}

int main() {
    kilo_init();
    kilo_message_tx = message_tx;
    kilo_message_tx_success = message_tx_succes;
    kilo_start(setup, loop);

    return 0;
}
