#include <kilolib.h>

// Declare state variable type to control motion.
typedef enum {
    STOP = 0,
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
message_t msg;

void message_rx(message_t *m, distance_measurement_t *d) {
    new_message = 1;
}

message_t *message_tx() {
    return &msg;
}

void setup() {
    msg.type = NORMAL;
    msg.crc = message_crc(&msg);
}

void loop() {
    if (new_message) {
        uint8_t rand_direction = rand_soft()&0b00000011;
        if (rand_direction == 0 || rand_direction == 1)
            set_motion(FORWARD);
        else if (rand_direction == 2)
            set_motion(LEFT);
        else if (rand_direction == 3)
            set_motion(RIGHT);
        delay(1000);
        set_motion(STOP);
        new_message = 0;
    }
}

int main() {
    // initialize hardware
    kilo_init();
    // register message callbacks
    kilo_message_rx = message_rx;
    kilo_message_tx = message_tx;
    // register your program
    kilo_start(setup, loop);

    return 0;
}
