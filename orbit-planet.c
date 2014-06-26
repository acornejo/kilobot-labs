#include <kilolib.h>

// declare constants
static const uint8_t TOOCLOSE_DISTANCE = 40; // 40 mm
static const uint8_t DESIRED_DISTANCE = 60; // 60 mm

// declare motion variable type
typedef enum {
    STOP,
    FORWARD,
    LEFT,
    RIGHT
} motion_t;

// declare state variable type
typedef enum {
    ORBIT_TOOCLOSE,
    ORBIT_NORMAL,
} orbit_state_t;

// declare variables
motion_t cur_motion = STOP;
orbit_state_t orbit_state = ORBIT_NORMAL;
uint8_t cur_distance = 0;
uint8_t new_message = 0;
distance_measurement_t dist;

// function to set new motion
void set_motion(motion_t new_motion) {
    if (cur_motion != new_motion) {
        cur_motion = new_motion;
        switch(cur_motion) {
            case STOP:
                set_motors(0,0);
                break;
            case FORWARD:
                spinup_motors();
                set_motors(kilo_straight_left, kilo_straight_right);
                break;
            case LEFT:
                spinup_motors();
                set_motors(kilo_turn_left, 0); 
                break;
            case RIGHT:
                spinup_motors();
                set_motors(0, kilo_turn_right); 
                break;
        }
    }
}

void orbit_normal() {
    if (cur_distance < TOOCLOSE_DISTANCE) {
        orbit_state = ORBIT_TOOCLOSE;
    } else {
        if (cur_distance < DESIRED_DISTANCE)
            set_motion(LEFT);
        else
            set_motion(RIGHT);
    }
}

void orbit_tooclose() {
    if (cur_distance >= DESIRED_DISTANCE)
        orbit_state = ORBIT_NORMAL;
    else
        set_motion(FORWARD);
}

// no setup code required
void setup() { }

void loop() {
    // Update distance estimate with every message
    if (new_message) {
        new_message = 0;
        cur_distance = estimate_distance(&dist);
    } else if (cur_distance == 0) // skip state machine if no distance measurement available
        return;

    // Orbit state machine
    switch(orbit_state) {
        case ORBIT_NORMAL:
            orbit_normal();
            break;
        case ORBIT_TOOCLOSE:
            orbit_tooclose();
            break;
    }
}

void message_rx(message_t *m, distance_measurement_t *d) {
    new_message = 1;
    dist = *d;
}

int main() {
    kilo_init();
    kilo_message_rx = message_rx;
    kilo_start(setup, loop);

    return 0;
}
