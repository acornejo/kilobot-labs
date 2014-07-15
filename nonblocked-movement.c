#include <kilolib.h>

// declare constants
#define FORWARD_GREEN 0
#define LEFT_RED 1
#define RIGHT_BLUE  2

// declare variables
uint8_t state = FORWARD_GREEN;
uint8_t state_needs_update = 1;
uint32_t last_update;

void setup() {
    last_update = kilo_ticks;
}

void loop() {
    // Change state every 64 clock ticks (roughly 2 seconds)
    if (kilo_ticks > last_update + 64) {
        last_update = kilo_ticks;
        state = (state + 1) % 3;
        state_needs_update = 1;
    }

    if (state_needs_update) {

        switch(state) {
            case FORWARD_GREEN:
                set_color(RGB(0,1,0));
                spinup_motors();
                set_motors(kilo_straight_left, kilo_straight_right);
                break;
            case LEFT_RED:
                set_color(RGB(1,0,0)); 
                spinup_motors();
                set_motors(kilo_turn_left,0);
                break;
            case RIGHT_BLUE:
                set_color(RGB(0,0,1));
                spinup_motors();
                set_motors(0, kilo_turn_right);
                break;
        }

        state_needs_update = 0;
    }
}

int main() {
    kilo_init();
    kilo_start(setup, loop);

    return 0;
}
