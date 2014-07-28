#include <kilolib.h>

/*
 * The original model of the kilobots has a through hole component as a
 * light sensor that is directly behind the axis of motion of the
 * kilobots, and sits on top of the battery.
 *
 * For this version of the kilobots the following code implements a
 * better strategy for moving towards the light.
 */

// declare constants
static const uint8_t THRESH_HIGH = 5;
static const uint8_t THRESH_LOW  = 5;

// declare variables
int cur_direction = 0;
long cur_light = 0;
long  high_thresh = 0, low_thresh = 1024;

// To reduce noise we take the average of 300 samples, discarding
// samples where the ADC sensor failed (returned -1).
void sample_light() {
    int numsamples = 0;
    long average = 0;

    while (numsamples < 300) {
        int sample = get_ambientlight();
        if (sample != -1) {
            average += sample;
            numsamples++;
        }
    }

    cur_light = average / 300;
}

void switch_directions() {
    if (cur_direction) {
        spinup_motors();
        set_motors(0, kilo_turn_right);
        set_color(RGB(2,0,0));
    } else {
        spinup_motors();
        set_motors(kilo_turn_left, 0);
        set_color(RGB(0,2,0));
    }
    cur_direction = !cur_direction;
}

void update_thresh() {
    high_thresh = cur_light*(100+THRESH_HIGH)/100;
    low_thresh = cur_light*(100-THRESH_LOW)/100;
}

void compute_direction() {
    if (cur_light < low_thresh) {
        update_thresh();
    } else if (cur_light > high_thresh) {
        update_thresh();
        switch_directions();
        delay(300); // to avoid fast switches
    }
}

void setup() {
    switch_directions(); // start turning in any direction
}

void loop() {
    sample_light();
    compute_direction();
}

int main() {
    kilo_init();
    kilo_start(setup, loop);

    return 0;
}
