#include <kilolib.h>

void setup() { }

void loop() {
    // Turn led green
    set_color(RGB(0,1,0));
    // spinup motors
    spinup_motors();
    // move straight for 2 seconds (2000ms)
    set_motors(kilo_straight_left, kilo_straight_right);
    delay(2000);
    
    // Turn led red
    set_color(RGB(1,0,0));
    // spinup for 15ms to overcome frction
    spinup_motors();
    // turn left for 2 seconds (2000ms)
    set_motors(kilo_turn_left, 0);
    delay(2000);

    // Turn led blue
    set_color(RGB(0,0,1));
    // spinup for 15ms to overcome frction
    spinup_motors();
    // turn right for 2 seconds (200 ms)
    set_motors(0, kilo_turn_right);
    delay(2000);
    
    // Turn off led and stop for half sec (500ms)
    set_color(RGB(0,0,0));
    set_motors(0,0);
    delay(500);
}

int main() {
    kilo_init();
    kilo_start(setup, loop);

    return 0;
}
