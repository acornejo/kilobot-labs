# Kilobot Labs

These labs are part of a workshop funded by NSF to make
programming the kilobots accessible to students and scientists
from different disciplines. They are a great way to get started
using the kilobots; the labs are meant to be done by teams of two people
(pair programming) using one laptop, a controller, a small whiteboard,
and two kilobots. The labs cover the main functionalities of the
kilobot, and end with some fun demos that can be run on with large
robot populations.

* [Getting Started](#getting-started)
* [Lab0: Blinky](#lab0-blinky)
* [Lab1: Movement](#lab1-movement)
* [Lab2: Communication](#lab2-communication)
* [Lab3: Putting It Together](#lab3-putting-it-together)
* [Lab4: Orbit](#lab4-orbit)
* [Lab5: Sync](#lab5-sync)
* [Lab6: Gradient](#lab6-gradient)
* [Lab7: Move to Light](#lab7-move-to-light)

## Getting Started

The following labs assume that you have at least one overhead controller
and two kilobots with the latest firmware, and a computer with the
KiloGUI tool.

To setup the latest firmware on the kilobots and controller and to use
the KiloGUI tool, follow the documentation provided
[here](https://www.kilobotics.com/documentation). To follow this
tutorial we recommend you use the kilobotics editor
[here](https://www.kilobotics.com/editor). You will need a Dropbox
account to store your files, which will be stored in the `Apps/KiloEdit`
folder.

## LAB0: BLINKY

* **Program:** Blink LEDS Red then Blue for 500ms each
* **Objective:** Introduce basic code structure (i.e. `setup` and
        `loop`) and basic functions such as `set_color` and `delay`
* **Code:** [blinkleds.c](https://github.com/acornejo/kilobot-labs/blob/master/blinkleds.c)

In the Kilobotics Editor, create a new file and name it `blinkleds.c`.
You'll see several things in the file, like `#include <kilobot.h>` which
contains the definitions of the [Kilobot library
API](https://www.kilobotics.com/docs/index.html).

Behind the scenes, the [Kilobotics
Editor](https://www.kilobotics.com/editor) will compile your code using
[avr-gcc](https://gcc.gnu.org/wiki/avr-gcc) and will link it against
[avr-libc](http://www.nongnu.org/avr-libc/) and
[kilolib](http://github.com/acornejo/kilolib). Some of the low level
functions, including math functions, are part of the
[avr-libc](http://www.nongnu.org/avr-libc/) library, whose documentation
is available
[here](http://www.nongnu.org/avr-libc/user-manual/modules.html). If you
choose to do so, you can install avr-gcc, avr-libc and kilolib on your
computer and develop locally instead of relying on the Kilobotics
Editor. However, for the purposes of this workshop, we will simply
assume you are using the Kilobotics Editor.

The main place to add code is in `loop`. These function will be run
repeatedly until you either reset or pause the robot. We will modify
`loop` to blink the robot LED. Take a look at the 
[Kilobot library API](https://www.kilobotics.com/docs/index.html) page
to see how to use the functions `set_color` and `delay`. Write the code
for the following:

```
set LED color to RED
delay 500ms
set LED color to BLUE
delay 500ms
```

Compile your code. This will produce a file called
`blinkleds.hex`. Now use the KiloGUI to upload the hex
file to your to your kilobot and see what happens (for detailed
info on how to use the KiloGUI, see [here](https://www.kilobotics.com/documentation)). If you run into
problems, take a look at the solution code above.

* *Questions: (1) What is the maximum delay rounded up to seconds, that
you can cause with a single call to delay? (2) How would you set the LED
to the color cyan?*

## LAB1: MOVEMENT

### 1.1 Calibrate your robots

* **Objective:** Manually calibrate the kilobots using the KiloGUI

The Kilobots use vibration motors to move, this is known as stick-slip
locomotion. Due to manufacturing differences the power required to
achieve good forward and turning motion varies from robot to robot, and
generally varies from surface to surface. In this lab you will learn how
to manually calibrate the values required for turning left, turning
right, and going straight. In the process you can also assign a unique
identifier to your kilobot, if you so desire. Follow the steps below

1.  Open up the KiloGUI program and click on the Calibration button, you
    will be presented with the following screen. ![](https://raw.github.com/acornejo/kilogui/docs/calib.png)
2.  Select a value for turning left, click test to tell the robot to
    move using this value. Values between 60 and 75 work best for
    turning, but this will depend on your robot on the surface being
    used. Choose different values until your robot can perform a full
    turn consistently on the surface being used.
3.  Follow the same procedure for turn right.
4.  To calibrate to go straight, you can use the values you already
    found for turn left and turn right as a good initial guess. Usually
    go straight values should be between 2 and 10 units smaller than the
    turning left and turning right values to achieve a good motion.
5.  Once you have calibrated all the values, make sure to click save to
    write these changes to the EEPROM memory of your robot. Before
    saving you can choose to set a unique ID for your robot by typing an
    integer in the unique ID box, and clicking test before you save the
    calibration data.

### 1.2 Simple-movement.c

* **Program:** Move forward 2 sec, clockwise 1 sec, anticlockwise 1 sec (repeat)
* **Objective:** Introduce `set_motors` and calibration constants `kilo_turn_left, kilo_turn_right, kilo_straight_left, kilo_straight_right`
* **Code:** [simple-movement.c](https://github.com/acornejo/kilobot-labs/blob/master/simple-movement.c)

In this lab we will make the Kilobot go through its motions --forward,
turn left, turn right-- in a loop. To do this we will use the
function `set_motors` that takes values for each of the two motors,
and we will use the calibrated constants that we set in Lab 1.1. Read
the [API page](https://www.kilobotics.com/docs/index.html) to see how to use `set_motors`. There's one
important caveat. When the motors are first turned on, we must set
the motors to the maximum speed for 15 ms or so, in order for the
kilobot to overcome static friction. We call this spinning up the
motors. Therefore, for the robot to move it must first spin up the
motors and then set its desired motion. Motors need to be spinup
every time the robot changes its direction of motion. This can be done
through the `spinup_motors` function, also described in the [API page](https://www.kilobotics.com/docs/index.html).

Create a file called `simple-movement.c`. Here's the
pseudocode you need to write. Then compile, upload and run your code!

```
spinup motors, set motors to forward, set LED to green
delay 2000ms
spinup motors, set motors to turn left, set LED to red 
delay 2000ms
spinup motors, set motors to turn right, set LED to blue 
delay 2000ms
```

###  1.3 Nonblocked-movement.c 

*  **Program:** Create a state machine with states `forward, turnleft`
and `turnright`, switching states every second.
* **Objective:** Introduce the `kilo_ticks` clock.
* **Code:** [nonblocked-movement.c](https://github.com/acornejo/kilobot-labs/blob/master/nonblocked-movement.c)

In the last two code examples, we used delay to control how long to
wait. But that prevents the robot from doing anything else during that
time - we call that "blocking" code. We will now rewrite the same code
in a "non-blocking" manner by using timers to control when to switch
from moving forward to turning. 

We will use the robot's own clock to check time, by reading the variable
`kilo_ticks`. One tick is equivalent to roughly 30 ms, or
equivalently there are approximatly 32 clock ticks every second. Clock
values must be stored in a non-negative 32-bit integer to avoid having
overflows. We'll start by writing some simple code that just blinks the
LED every 64 clock ticks (roughly every 2 seconds). Create a file called
`nonblocking-movement.c` with the following in proper code.

```
// before program loop declare the variables 
uint32_t last_changed;

// In program loop
if kilo_ticks > (last_changed + 64) then
    last_changed = kilo_ticks // remember the current time
    blink the LED (turn led yellow, delay 100ms, turn off)
```

Now that we have that working and tested, we will create a state machine
with three states,  Each time the timer expires, instead of blinking a
led, we will update the state and turn on a flag to signal that the
state has changed and the motion must be updated.

```
// before program loop declare another two more variables
int state = 0;
int state_changed = 1;

// in the program loop add code to implement the following
if state_changed then
    state_changed = 0
if state is 0 then
    set LED to green, spinup motors, set motors to move forward
if state is 1 then
    set LED to red, spinup motors, set motors to turn left
if state is 2 then
    set LED to blue, spinup motors, set motors to turn right
```

Once you have that working you have the basic block structure
for more complex code. The timer causes events to happen, but
between events other code can run - for example later on we will
have the robot listening for messages from other robots. Or we can
even have messages from other robots (or other sensed things)
trigger the change from one state to another, rather than the
timer.

Before moving on to the next lab, make sure to take a look at
the sample code provided to see how we implemented this
timer-driven state machine.

* *Questions: (1) When a kilobot is turned on its clock `kilo_ticks` is
set to zero, and this clock is incremented 32 times per second. How
long would a kilobot have to remain on for its clock to overflow?*

## LAB2: COMMUNICATION

In this Lab we will start using the ability of two kilobots to
communicate with each other. We will dedicate one kilobot to be
the **speaker** and the other kilobot to be
the **listener**. Eventually we will program robots
that do both, but the speaker/listener code here is also useful
for debugging for more complex programs.

### 2.1 test-speaker.c

* **Program:** Broadcast a message continuously.
* **Objective:** Introduce the `message_t` data structure, and the
`kilo_message_tx` callback.
* **Code:** [test-speaker.c](https://github.com/acornejo/kilobot-labs/blob/master/test-speaker.c)

A kilobot uses infrared (IR) to broadcast a message within an
approximately circular radius of three body lengths. Multiple robots
packed closely together will have overlapping transmission regions,
causing the IR signals to interfere.

The kilobots use a variant of the CSMA/CD media access control method
(carrier-sensing multiple access with collision detection) to ameliorate
the problems with interference.
Suffice it to say, if you instruct a robot to send a message, it may
take some time before the robot can actually broadcast the message.
Moreover, with even after the message is broadcast, it is possible that
this message cannot be decoded by the receiver robot due to noise.

In this lab we will instruct each robot to send a message, and the robot
will broadcast it as often as possible (depending on the congestion of
the channel).
Without any other robots within shouting distance, a kilobot achieves
roughly 3 messages/sec. To do this, we will first declare a variable
called `transmit_msg` of the structure type `message_t`. You should do
this near the top of the file. Next, we add the function `message_tx()`
that returns the address of the message we declared (`return &transmit_msg`).

```
message_t transmit_msg;

message_t *message_tx() {
    return &transmit_msg;
} 
```

We use the `setup()` function to the set the initial contents of the
message and compute the message CRC value (used for error detection)
through the function `message_crc()`.
The code below shows how to initialize an empty message.

```
void setup() {
    transmit_msg.type = NORMAL;
    transmit_msg.crc = message_crc(&transmit_msg);
}
```

Finally, you must register your message transition function with the
kilobot library as follows:

```
int main() {
    kilo_init();
    kilo_message_tx = message_tx;
    kilo_start(setup, loop);

    return 0;
}
```

Once you have added this code in the file you are pretty much
done. You may want to add something simple to the program loop -
like blink the LED magenta just so you know the kilobot is running
the speaker code. You can compile and upload, but the real test
will be once we have created the listener robot.

* *Note: Lecturer may need to explain the difference in the C language
between `&msg`, `*msg` and  `msg`. Also explain why the
message value and crc should not be set within the
callback but within the main program loop or setup phase.*

### 2.2. test-listener.c

* **Program:** If a received message is even, blink red, if odd, blink
blue. If no message is being detected, then led should be off.
* **Objective:** Introduce the `kilo_message_rx` callback and store
incoming messages.
* **Code:** [test-listener.c](https://github.com/acornejo/kilobot-labs/blob/master/test-listener.c)

Now we will create the listener robot. We first declare a
variable `rcvd_message` of type `message_t`
to store any new incoming messages and a boolean
variable (often called a "flag")
called `new_message` to indicate that a new message has been received.

Create a new file called `test-listener.c` and add the following code
after the include section.

```
int new_message = 0;
message_t rcvd_message;

void message_rx(message_t *msg, distance_measurement_t *dist) {
    rcvd_message = *msg;  //store the incoming message
    new_message = 1;      // set the flag to 1 to indicate that a new message arrived
}
```

Now in the program loop, we will check the flag to see if a new
message has arrived. If a new message has arrived, we will blink
the LED yellow.

```
void loop() {
    // Blink led yellow when you get a message
    if (new_message) {
        new_message = 0;
        set_color(RGB(1,1,0));
        delay(10);
        set_color(RGB(0,0,0));
    }
}
```

Finally, as before, you must modify your main section to register the message
reception function with the kilobot library as follows:

```
int main() {
    kilo_init();
    kilo_message_rx = message_rx;
    kilo_start(setup, loop);

    return 0;
}
```

The kilobot API guarantees that each time a message is successfully
decoded the function `message_rx()` will get called with the message and
the distance measurements as a parameter (for now, ignore the distance
measurements, which will be explained in a later lab).

### 2.3 test-speaker-mod.c

* **Program:** Change the transmitted message
every 1 second between "5" to "6".
* **Objective:** Explain how to change
transmitted message in the program code.
* **Code:** [test-speaker-mod.c](https://github.com/acornejo/kilobot-labs/blob/master/test-speaker-mod.c)

Once you've tested the speaker and listener together, we will modify the
speaker code to send two different messages. Specifically we will use a
timer to change the message being sent once every two seconds.
The code for switching should look similar to the way you made a state
machine in 1.3. 

First, we start by declaring two messages (instead of a
single message), and we use a flag to decide which message to send.

```
uint8_t odd = 0;
message_t message_odd;
message_t message_even;
```

Next use the `setup()` function to the set the initial contents of the
messages in the array.

```
void setup() {
    message_odd.type = NORMAL;
    message_odd.data[0] = 1;
    message_odd.crc = message_crc(&message_odd);

    message_even.type = NORMAL;
    message_even.data[0] = 0;
    message_even.crc = message_crc(&message_even);
}
```

Finally, we modify the main loop to include code that toggles the `odd`
flag every two seconds using `kilo_ticks`. In the message transition
callback you must return a pointer to `message_odd` or `message_even`
depending on the contents of the `odd` flag.

### 2.3 Modify test-listener.c
* **Program:** If a received message is even and nearby, blink red. If
it is even and further, blink magenta. If a recieved message is odd,
blink blue if nearby and cyan if far.
* **Objective:** Introduce how to check message content and distance.
* **Code:** [test-listener-mod.c](https://github.com/acornejo/kilobot-labs/blob/master/test-listener-mod.c)


Now modify the listener code from before to read the value of the
message by reading the value of the first byte of the recieved
message. If the value is odd (or one), then blink the LED blue. If the
value is even, then blink the LED red. Here's an example of how to
read the value of a message.

```
void message_rx(message_t *m, distance_measurement_t *d) {
    data = m->data[0];
    dist_measure = *d;
    new_message = 1;
}
```

Once you have that working, you can do the next step, which is
changing the behavior when the listener is close or far away from the
speaker. If the robot is "far" (more than 50mm away) then instead of
blue/red use cyan/magenta. Note that the diameter of a kilobot is 33
mm. Look up `estimate_distance` in the API documentation. Here's some
code that shows how to to use it.

```
uint8_t dist = 0;

// Then in your program loop when you get a new message you can convert
// the distance signal to a value like this

dist = estimate_distance(&dist_measure);
```

Now is a good time to test your code. Compile, upload, and run your
listener robot. Then do the same for your speaker. When the robots are
close together the listener should be blinking blue or red as messages
arrive. Moving the robots further away should cause the listener robot
to blink magenta or cyan. Moving the robots even further away should
cause the listener to stop blinking once it is outside the
communication range.

### 2.4 Explanation

We have only used a small part of the message structure. Read
the API to further understand the message structure. Things to
look at include: how to send larger messages, and how to send a
single message from one robot to another instead of the constant
broadcast we used here.

## LAB3: PUTTING IT TOGETHER

* **Program:** Create a single robot that both
sends and receives messages. When the robot receives a new
message from a neighbor, it should pick a random direction to
move in (50% straight, 25% left, 25% right).
* **Objective:** Put communication and motion
together, introduce `rand_soft()`, and create subroutine
for cleaner and more efficient motion code.
* **Code:** [test-listener-mod.c](https://github.com/acornejo/kilobot-labs/blob/master/test-listener-mod.c)

We will now create a single robot with the following behavior:
The robot will always broadcast a message and listen
for messages from other robots. If the robot does not hear any
neighboring robots, then it will turn off its motors and stand
still. However if it hears another robot nearby, it will move randomly.

Create a new file called `transmit-receive-randmotion.c`. Using your
previous code from lab 2 as examples, modify the message callbacks so
that the robot is transmitting a single message (set in `setup` to be
the robot ID) and when it receives a message, it turns the LED
yellow for 10ms. Feel free to test this code quickly with your two
robots. Both should blink when they see each other.

As the next step, modify the code above so that the robot
chooses randomly between three different colors to flash when it
gets a message. The code below show how to use the
function `rand_soft()` to pick red with 50% probability, green with 25%
probability, and blue with 25% probability. To do this, we first select
2 bits of the 8-bit random number, and then make the decision based on
the four possible values these 2-bits can take (each of these values is
taken with equal probability, that is 25% probability).

```
const uint8_t twobit_mask = 0b00000011;
uint8_t twobit_rand = rand_soft()&twobit_mask;

if twobit_rand == 0 || twobit_rand == 1 then
    set LED to red
else if twobit_rand == 2 then
    set LED to green
else if twobit_rand == 3 then
    set LED to green
```

Finally, lets add the motion. But first we will create a new
subroutine function called `set_motion`. This
function will check the current direction of the robot, and only
spinup and set motors if the robot is changing its direction. Note
that you must define this function before
the `loop()` code begins. One way is to use an integer to encode
the motion, where `0=stopped`, `1=forward`, `2=left` and `3=right`, here
is some pseudo-code.

```
int cur_motion = 0;

void set_motion(int new_motion) {
    if cur_motion is not equal to the new_motion then
        if new_motion is 0 then
            stop
        else if new_motion is 1 then
            move forward
        else if new_motion is 2 then
            turn clockwise
        else 
            turn counterclockwise
        cur_motion = new_motion;
}
```

Now in the program loop, modify the code so that in addition to
turning on the LEDs, the robot also sets a direction based on the
random number (red straight, green left, blue right).

## LAB4: ORBIT

Now that we've learned all the
basics, we will code up some fun and classic "demos" in the next few
labs. Orbiting is a commonly used primitive - basically it allows one
robot (the planet) to use the distance from another robot (the star)
as a way to move. It can be extended to edge-following if there is a
group of stars, then the planets can orbit that group of stars. We use
edge-following in many algorithms.

* **Objective**: Use distance sensing to have one robot orbit another
stationary robot while keeping a fixed distance.
* **Video**: [link](https://www.youtube.com/watch?v=EOEh9xnLB_0)

# 4.1 orbit-star.c

* **Code:** [orbit-star.c](https://github.com/acornejo/kilobot-labs/blob/master/orbit-star.c)

The orbit-star program is identical to the `test-speaker.c` program you
created in LAB2. Its purpose is to serve as a stationary reference that
constantly emits beacon messages to the oribiting robot.

# 4.2 orbit-planet.c

* **Code:** [orbit-planet.c](https://github.com/acornejo/kilobot-labs/blob/master/orbit-planet.c)

Create a new file `orbit-planet.c` and use a starting point the contents
of the `transmit-receive-randmotion.c` file.

For the first time, in this lab we will use the data available in the
`distance_measurement_t` structure to estimate the distance between two
robots via the `estimate_distance()` function of the kilolib API.
Namely, whenever a new message is received, we will turn on a flag
`new_message` and store the distance measurement. This distance
measurement is translated to a distance measurement in the main program
loop. The pseudo-code for this is as follows:

```
distance_measurement_t dist;
uint8_t cur_distance = 0;
uint8_t new_message = 0;

void message_rx(message_t *m, distance_measurement_t *d) {
    new_message = 1;
    dist = *d;
}

// ... somewhere inside the main program loop
    if (new_message) {
        new_message = 0;
        cur_distance = estimate_distance(&dist);
    }
```

The variable `cur_distance` contains the distance estimate to the robot
that originated the last message, measured in millimiters.

Our goal is for the planet robot to orbit at a fixed distance from the
star robot; specifically we will use an orbiting distance of 6cm, since
this is a good compromise between the communication range (10cm) and the
minimum robot distance (3.3cm).

If the current distance estimate to the robot is 2cm smaller (or more)
than the desired oribitng distance, then the robot moves forward
until the distance estimate becomes greater than the desired
orbiting distance (6cm). Otherwise, the robot will simply alternate
between turning left when the current distance is less than the desired
oribiting distance, and turning right when the current distance is
greater than the desrired oribiting distance.

We reuse the function `set_motion` of the
`transmit-receive-randmotion.c` file in the following pseudocode.

```
// ... somewhere inside the main program loop

if cur_distance <= 40 then
    too_close = 1

if too_close then
    if cur_distance > 60 then
        too_close = 0
    else
        set_motion(FORWARD)
else
    if cur_distance <= 60 then
        set_motion(LEFT)
    else
        set_motion(RIGHT)
```

## LAB5: Sync

Spontaneous synchronization is a classic collective behavior in nature,
from heart cells to fireflies, and there are many ways in which
individuals can synchronize to their neighbor. Here we will use a method
that relies on averaging. Each robot acts as an oscillator, flashing its
LED in a fixed period. But when it hears other robots flash, it collects
that information and uses the average to make an adjustment to its own
next flashing time. Sometimes this can be hard to get right, so there's
quite a few hints on how to do it below. After you've tested your code
on two robots, its fun to test with a whole collective!

* **Objective**: Create a logical synchronus clock between different
robots to allow two or more robots to blink an LED in unison roughly
every 4 seconds.

* **Video**: [link](https://www.youtube.com/watch?v=NK-n786jw4Y)

* **Code:** [sync.c](https://github.com/acornejo/kilobot-labs/blob/master/sync.c)

We will use the internal clock `kilo_ticks` available at each kilobot to
create a logical clock `modulo_clock`, which will be updated based on
the logical clock of neighboring robots. Specifically our `modulo_clock`
will be a 5-bit clock and only take values from 0 to 31 (i.e. 2^5=32
different values). Each robot will blink their LED and update their
clock when `modulo_clock` is equal to zero. Since `kilo_ticks` is incremented
roughly 32 times per second, and we want robots to blink roughly once
every four seconds, then we will let `modulo_clock=(kilo_ticks/4)%32`.

Along with every message, each robot will send the current value of its
`modulo_clock`, since the clock can only take 32 possible values, then
there are only 32 possible messages, these messages can be precomputed
in the `setup` phase as follows (where the first byte of the message
contains the value of `modulo_clock`)

```
message_t msgs[32];

void setup() {
    for (int i = 0; i < 32; i++) {
        msgs[i].data[0] = i;
        msgs[i].type = NORMAL;
        msgs[i].crc = message_crc(&msgs[i]);
        offsets[i] = 0;
    }
}
```

The message transmission callback simply returns the message indexed by
the current value of `modulo_clock` as follows:

```
message_t *message_tx() {
    return &msgs[modulo_clock];
}
```

The message reception callback will store the received clock values to
create a histogram of the differences between the value of the clock at
the sender and the receiver. Moreover, a robot will only store the
offset of a neighbor if the difference between the sender clock and
receiver clock is less than half the period (16). This technique ensures
that given two robots A and B, either A will adjust towards B, or
B will adjust towards A, but not both. This helps reduce the
oscillations of the logical clocks.

The pseudo-code to create the histogram of clock offsets follows (clock
offsets must be initialized to zero in the setup phase):

```
uint8_t offsets[32];

void message_rx(message_t *msg, distance_measurement_t *d) {
    if (modulo_clock > msg->data[0]) {
        if (modulo_clock - msg->data[0] < 16)
            offsets[modulo_clock-msg->data[0]]++;
    } else {
        if (msg->data[0] - modulo_clock > 16)
            offsets[modulo_clock + (32-msg->data[0])]++;
    }
}
```

Finally, in the main program loop a robot will blink their LED when
`modulo_clock` is equal to zero, and at the same time update their clock
using the average offset.


```
// ... in the main program loop
    modulo_clock = ((kilo_ticks-tick_offset)/4)%32
    if !modulo_clock is 0 then
        blink LED

        // compute offset average
        total = 0
        sum = 0
        for i in [0...32] do
            total += offsets[i]
            sum += i*offsets[i]
            offsets[i] = 0

        // adjust clock by average offset
        if total > 0 then
            tick_offset += sum/total
```

## LAB6: GRADIENT

* **Objective**: For each robot to compute its distance (measured in
number of hops) towards a distinguished root robot. We refer to this
distance as the gradient of a robot.

* **Code:** [gradient.c](https://github.com/acornejo/kilobot-labs/blob/master/gradient.c)
* **Video**: [link](https://www.youtube.com/watch?v=Z2nLhx3hMwI)

The algorithm is very simple. The root robot starts with a gradient
value of zero, and every other robots starts with the maximum gradient
value (for simplicity, think of this value as being infinity).
In the message transmission callback, a robot sends NO message (simply
do `return '\0'` in the transmission calback) if it has the
maximum gradient value, and otherwise it sends a message containing its
current gradient value.
In the message reception callback, if a robot receives a message with
gradient value `V` then it updates its own gradient value to 
`gradient_value = min(gradient_value,V+1)`.
Finally, in the main program loop robots choose the color of their LED
based on their gradient value.

Observe that the maximum distance between two robots is equal to the
diameter of the communication graph, which can be no greater than the
total number of robots. Therefore, by using a 16-bit value to hold the
gradient value allows the previously described algorithm to work in
swarms of more than 65,000 robots. Thus the only challenge in
translating the previously described algorithm to real code, is to pack
and unpack a 16-bit value into the 8-bit data bytes. Here is the
pseudo-code for doing just that:

```
uint16_t gradient_value =  UINT16_MAX;

//  pack gradient into message
msg.type = NORMAL;
msg.data[0] = gradient_value&0xFF;
msg.data[1] = (gradient_value>>8)&0xFF;
msg.crc = message_crc(&msg);

// unpack message into gradient
recvd_gradient = msg.data[0]  | (msg.data[1]<<8);
```

Remember to update the message (and compute the CRC) when a robot
updates its own gradient value (and outside the message callbacks).

Also you will need to have exactly one "root" robot. For that purpose
you should use the calibration section of KiloGUI to set a special
unique identifier for a robot (for example `1000`), and modify your code
to check `kilo_uid` and set the gradient value to zero if `kilo_uid ==
1000`.

## LAB7: MOVE TO LIGHT

* **Objective**: For each robot to move towards the direction of the
brightest light source.

* **Code:** [movetolight.c](https://github.com/acornejo/kilobot-labs/blob/master/movetolight.c)
* **Video**: [link](https://www.youtube.com/watch?v=pd1CseHrvA8)

This lab is the first one to measure environmental conditions using the
sensors available at the kilobots. Specifically, we will use the ambient
light sensor through the function `get_ambientlight()` to read the
current value for the sensor. When a sensor cannot return a
good reading, it returns -1.

Since sensors are inherently noisy, each time we sample the current
light we will average 300 sensor readings, discarding any bad sensor
readings. The pseudo-code follows:

```
    int numsamples = 0
    long average = 0

    while numsamples < 300 do
        int sample = get_ambientlight()
        if sample != -1 then
            average += sample
            numsamples++

    cur_light = average / 300
```

In this lab, we will require a dark room (no windows) and a single
light source (i.e. a desk lamp with an incandescent bulb).

In such a setting, if an idealized kilobot were to perform a series of uninterrupted
turns, then the readings
returned by the ambient light sensor will look like a sine
curve, where the peaks represent the moments where the sensor is facing
directly at the light source, and the valleys occur at the moments where
the sensor is facing away from the light source. 

In reality, due to sensor noise and the placement of the sensor, this
actual readings will differ from idealized setting, but its still
possible to classify the curve as being in a peak or a valley.

To get the robot to steer towards the light, we will aim to detect the
moment when the sensor transitions from being on a peak, and being on a
valley. Concretely, if the sensor reading is greater than 600 we will
say the curve is in the peak region, and if its lower than 300 we will
say the curve is in the valley region.

Whenever a valley condition is detected, the robot will set its motors
to turn right, and whenever a peak condition is detected, the robot will
set its motors to turn left. The net effect of this, is that the robot
will move forward at the point where the sensor readings transition from
a valley to a peak. We present some pseudo-code below.

```
void setup() {
    cur_direction = LEFT
    set motors to turn left
}

void loop() {
    sample_light()
    if cur_direction is LEFT then
        if cur_light is valley
            cur_direction = RIGHT
            set motors to turn right
    else if cur_direction is RIGHT then
        if cur_light is peak
            cur_direction = LEFT
            set motors to turn left
}
```

To test your code make sure you are in a room without windows (or close
all the shades), and turn on a single directed light source (a
desk lamp works well) and pointed towards the robot. The
robot should start turning until it faces the light, and then
switch back and forth between turning left and right as it moves
towards the light source.
