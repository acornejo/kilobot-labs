#include <kilolib.h>

// declare state variables;
int state;
int state_changed;
uint32_t last_changed;

void setup() {
    // put your setup code here, to be run only once

    // starte in state 0
    state = 0;
    // trigger state change code
    state_changed = 1;
    last_changed = kilo_ticks;
}

void loop() {
    // put your main code here, to be run repeatedly
  
    // Change state every 64 clock ticks (roughly 2 seconds)
    if (kilo_ticks > last_changed + 64) {
        last_changed = kilo_ticks;
        state_changed = 1;
        state = (state+1) % 3;
    }

    // Run code when state is changed
    if (state_changed) {
        state_changed = 0;

        switch(state) {
            // State  0 => moving forward with green led on
            case 0:
                set_color(RGB(0,1,0));
                set_motors(255, 255);
                delay(15);
                set_motors(kilo_straight_left, kilo_straight_right);
                break;
            // State  1 => turn left with red led on
            case 1:
                set_color(RGB(1,0,0)); 
                set_motors(255, 0);
                delay(15);
                set_motors(kilo_turn_left,0);
                break;
            // State  2 => turn right with blue led on
            case 2:
                set_color(RGB(0,0,1));
                set_motors(0, 255);
                delay(15);
                set_motors(0, kilo_turn_right);
                break;
        }
    }
}

int main() {
    // initialize hardware
    kilo_init();
    // register your program
    kilo_start(setup, loop);

    return 0;
}
