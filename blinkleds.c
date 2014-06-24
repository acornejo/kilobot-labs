#include <kilolib.h>

void setup() {
    // put your setup code here, to be run only once
}

void loop() {
    // put your main code here, to be run repeatedly
    
    // Turn led blue
    set_color(RGB(0,0,1));
    // Wait half a second (500ms)
    delay(500);
    // Turn led red
    set_color(RGB(1,0,0));
    // Wait half a second (500ms)
    delay(500);
}

int main() {
    // initialize hardware
    kilo_init();
    // register your program
    kilo_start(setup, loop);

    return 0;
}
