#include <kilolib.h>

// Declare state variable type to control motion.
typedef enum {
    STOP,
    FORWARD,
    LEFT,
    RIGHT
} motion_t;

motion_t cur_motion = STOP;

// Function to set new motion
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

uint8_t new_message = 0;
distance_measurement_t dist;

void message_rx(message_t *m, distance_measurement_t *d) {
    new_message = 1;
    dist = *d;
}

// declare constants
static const uint8_t TOOCLOSE_DISTANCE = 40; // 40 mm
static const uint8_t DESIRED_DISTANCE = 60; // 60 mm

// declare state variables
typedef enum {
    ORBIT_TOOCLOSE,
    ORBIT_NORMAL,
} orbit_state_t;

orbit_state_t orbit_state = ORBIT_NORMAL;
uint8_t cur_distance = 0;

// no setup code required
void setup() { }

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

int main() {
    // initialize hardware
    kilo_init();
    // register message reception callback
    kilo_message_rx = message_rx;
    // register your program
    kilo_start(setup, loop);

    return 0;
}
