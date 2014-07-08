# Workshop

These labs are part of a workshop funded by NSF to make
programming the kilobots accessible to students and scientists
from different disciplines. They are a great way to get started
using the Kilobotics system or to teach the system to others. The
labs are meant to be done by teams of two people (pair
programming) using one laptop, a controller, a small whiteboard,
and two kilobots. The labs cover the main functionalities of the
Kilobot, and end with some fun demos that can be run on larger
robot groups.

* [Getting Started](#getting-started)
* [Lab0: Blinky](#lab0-blinky)
* [Lab1: Movement](#lab1-movement)
* [Lab2: Communication](#lab2-communication)
* [Lab3: Putting It Together](#lab3-putting-it-together)
* [Lab4 and beyond](#lab4-tbd)

## Getting Started

Add reminder to follow the documentation for setting up the GUI
and editor. Maybe also add helpful hints, like where in dropbox
the files are stored and a reminder on how to use KiloGUI to
upload a file?

## LAB0: BLINKY

* **Program:** Blink LEDS Red then Blue for 500ms each
* **Objective:** Introduce basic code structure (i.e. `setup` and `loop`) and basic functions such as `set_color` and `delay`

In the Kilobotics Editor, create a new file and name it `blinkleds.c`.
You'll see several things in the file, like `#include kilobot.h` which
contains the definitions of the kilobot library (API). The documentation
of the kilobot library is available [here](#). 

Behind the scenes, the Kilobotics Editor will compile your code using [avr-gcc](https://gcc.gnu.org/wiki/avr-gcc) and will link it against [avr-libc](http://www.nongnu.org/avr-libc/). Some of the low level functions, including math functions, are part of the [avr-libc](http://www.nongnu.org/avr-libc/) library, whose documentation is available [here](http://www.nongnu.org/avr-libc/user-manual/modules.html). If you choose to do so, you can install avr-gcc and avr-libc on your computer and develop locally instead of relying on the Kilobotics Editor. However, for the purposes of this workshop, we will simply assume you are using the Kilobotics Editor.

The main place to add code is
in `loop`. These function will be run repeatedly until you either reset or pause the robot. 
We will modify `loop` to blink the robot
LED. Take a look at the API page to see how to use the functions
`set_color` and `delay`. Write the code
for the following:

```
set LED color to RED
delay 500ms
set LED color to BLUE
delay 500ms
```

<p>Compile your code. This will produce a file called
`blinkleds.hex`. Now use the KiloGUI to upload the hex
file to your to your kilobot and see what happens (for detailed
info on how to use the KiloGUI, see here (link)). If you run into
problems, take a look at the solution code above.

_ Questions: (1) What is the maximum delay rounded up to seconds, that you can cause with a single call to delay? (2) How would you set the LED to the color cyan? _

## LAB1: MOVEMENT

### 1.1 Calibrate your robots

* **Objective:** Manually calibrate the kilobots using the KiloGUI

The Kilobots use vibration motors to move, this is known as stick-slip locomotion. Due to manufacturing differences the power required to achieve good forward and turning motion varies from robot to robot, and generally varies from surface to surface. In this lab you will learn how to manually calibrate the values required for turning left, turning right, and going straight. In the process you can also assign a unique identifier to your kilobot, if you so desire. Follow the steps below

1.  Open up the KiloGUI program and click on the Calibration button, you will be presented with the following screen.
![](img/calibwindow.png)
2.  Select a value for turning left, click test to tell the robot to move using this value. Values between 60 and 75 work best for turning, but this will depend on your robot on the surface being used. Choose different values until your robot can perform a full turn consistently on the surface being used.
3.  Follow the same procedure for turn right.
4.  To calibrate to go straight, you can use the values you already found for turn left and turn right as a good initial guess. Usually go straight values should be between 2 and 10 units smaller than the turning left and turning right values to achieve a good motion.
5.  Once you have calibrated all the values, make sure to click save to write these changes to the EEPROM memory of your robot. Before saving you can choose to set a unique ID for your robot by typing an integer in the unique ID box, and clicking test before you save the calibration data.

### 1.2 Simple-movement.c

* **Program:** Move forward 2 sec, clockwise 1 sec, anticlockwise 1 sec (repeat)
* **Objective:** Introduce `set_motors` and calibration constants `kilo_turn_left, kilo_turn_right, kilo_straight_left, kilo_straight_right`

In this lab we will make the Kilobot go through its motions -
forward, turn left, turn right - in a loop. To do this we will use
the function `set_motors` that takes values for each of
the two motors, and we will use the calibrated constants that we
set in Lab 1.1. Read the API page (link) to see how to use
`set_motors`. There's one important caveat. When the motors are first
turned on, we must set the motors to the maximum speed for 15 ms or so,
in order for the kilobot to overcome static friction. We call
this spinning up the motors. Therefore, for the robot to move it
must first spin up the motors and then set its desired motion.
Motors need to be spinup every time the robot changes its
direction of motion.

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

*  **Program:** Create a state machine with states `forward, turnleft` and `turnright`, switching states every second.

In the last two code examples, we used delay to control how
long to wait. But that prevents the robot from doing anything else
during that time - we call that "blocking" code. We will now
rewrite the same code in a "non-blocking" manner by using timers
to control when to switch from moving forward to turning. 

We will use the robot's own clock to check time, by reading the
variable `kilo_ticks`. One kilotick is equivalent to
roughly 30 ms, or equivalently there are approximatly 32 clock ticks every second.
Clock values must be stored in a non-negative 32-bit integer to avoid having overflows.
We'll start by writing some simple code that just blinks
the LED every 64 clock ticks (roughly every 2 seconds). Create a file
called `nonblocking-movement.c` with the following in proper code.

```
// before program loop declare the variables 
uint32_t last_changed;

// In program loop
if kilo_ticks > (last_changed + 64) then
last_changed = kilo_ticks // remember the current time
blink the LED (turn led yellow, delay 100ms, turn off)
```

Now that we have that working and tested, we will create a
state machine with three states,  Each time the timer expires,
instead of blinking a led, we will update the state and turn on a flag to signal that the state has changed and the motion must be updated.

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

* Questions: (1) When a kilobot is turned on its clock `kilo_ticks` is set
to zero. For how many years could you a kilobot have to remain on before
its clock overflows?*

## LAB2: COMMUNICATION

In this Lab we will start using the ability of two kilobots to
communicate with each other. We will dedicate one kilobot to be
the **speaker** and the other kilobot to be
the **listener**. Eventually we will program robots
that do both, but the speaker/listener code here is also useful
for debugging for more complex programs.

### 2.1 Test-speaker.c

* **Program:** Broadcast the number 5 continually; blink LED magenta

A kilobot uses infrared (IR) to broadcast a message within an
approximately circular radius of three body lengths. However
multiple robots will have overlapping regions, causing the IR
signals to interfere. The kilobots use a common scheme called CDMA
(collision detection multiple access) to avoid having their
messages too often. Suffice it to say, if you tell a robot to send
a message, it may take some time before the robot can speak it,
and the message may not be properly heard ewith some probability.

Here will will set a single message, and ask that the robot
broadcast it as often as possible. Without any other robots within
shouting distance, a kilobot achieves roughly X messages/sec. To do
this, we will first declare a variable
called `transmit_msg` of the structure type
`message_t`. You should do this near the top of the
file, before the function `message_tx()`, as shown in
the code below. Next, modify the function `message_tx()`
to return `&transmit_msg` (the address of message you
created), again as shown in the code below.

```
// Called when the messaging subsystem decides to broadcast a message
message_t transmit_msg;
message_t *message_tx() {
return &transmit_msg;
} 
```

Next, you should read about the message structure on the API
page (link). We will set the message being transmitted to be a
fixed value, so we only need to do this once. To do that we will
insert some code into the `program-init()` function; in
general you can put anything here that you want to run once before
the program loop starts. A message is X bytes and stored as an
array. The code below shows how to set the first byte of the array
to the number 5. Next the code calls the function message_crc()
which computes a error-correction code that helps a recieving
kilobot check if any of the message got corrupted (to an extent).

```
// Your program init (things that only need to happen once)
void program_init() {
transmit_msg.data[0] = 5;   // sending the same message always
transmit_msg.crc = message_crc(&transmit_msg);
}
```

Once you have added this code in the file you are pretty much
done. You may want to add something simple to the program loop -
like blink the LED magenta just so you know the kilobot is running
the speaker code. You can compile and upload, but the real test
will be once we have created the listener robot.

*Note for lecturer: May need to explain the difference
between `*msg`, `&amp;msg` and  `msg` to non-C users. Also explain why the
transmit message value and crc should not be set within the
callback but within the program init or loop.*

### 2.2. Test-listener.c

Now we will create the listener robot. To do this, we will
modify the callback `message_rx()`. We will declare a
variable `rcvd_message` of type `message_t`
to store any new incoming messages. We will also use a boolean
variable (often called a "flag")
called `new_rcvd_message` to indicate when a new
message as arrived and when it has been read. Create a new file
called `test-listener.c` Modify the code as shown
below.

```
int new_rcvd_message = 0;
message_t rcvd_message;
void message_rx(message_t *msg, distance_measurement_t *dist) {
rcvd_message = *msg;  //store the incoming message
new_rcvd_message = 1; // set the flag to 1 to indicate that a new message arrived
}
```

Behind the scenes, the kilobot is continually checking for
messages and when a new mmessage is successfully received, the
callback function `message-rx()` gets called and the
flag gets set. But we don't need to do anything else for that to
happen.

Now in the program loop, we will check the flag to see if a new
message has arrived. If a new message has arrived, we will blink
the LED yellow.

```
// Your program loop
void loop() {
    // Blink led yellow when you get a message
    if (new_rcvd_message) {
        new_rcvd_message = 0;
        set_color(RGB(1,1,0));
        delay(10);
        set_color(RGB(0,0,0));
    }
}
```

### 2.3 Modify test-speaker.c

Once you've tested the speaker and listener together, we will
modify the speaker code to switch its message. To do so, use a
timer in the program loop to change the message from the number
"5" to the number "6" every second (64 ticks) so that to that the
robot switches the message being sent. The code for switching
should look similar to the way you made a state machine in
1.3. Also remember that you need to use 

### 2.3 Modify test-listener.c

Now modify the listener code from before to read the value of
the message by reading the first byte of the recieved message
(`rcvd_message[0]`). If the value is odd, then blink
the LED blue. If the value is even, then blink the LED red.

Further modify the code to check the distance from its
neighbor. ADD DETAILS. 

Now is a good time to test your code. Compile, upload, and run
your listener robot. Then do the same for your speaker. Your
speaker robot should be blinking magenta pretty rapidly, and when
your listener is within a short distance, it should be blinking
blue or red as messages arrive. MOving further away should cause
the robot to blink magenta or cyan. Moving further away still
should cause the listener to stop blinking when it no longer
here's the messaages. 

### 2.4 Explanation

*   **Objective: ** Discuss the whole message
structure and callbacks, even though we haven't used the whole
power of the system yet

<p>We have only used a small part of the message structure. Read
the API to further understand how messages can be sent. Things to
look at include: how to send larger messages, and how to send a
single message from one robot to another instead of the constant
broadcast we used here.

<!-- ********* LAB3 ********* -->

## LAB3: PUTTING IT TOGETHER

### 3.1 Transmit-receive-randmotion.c

We will now create a single robot with the following behavior:
The robot will always broadcast its own ID and always be listening
for messages from other robots. If the robot does not hear any
neighboring robots, then it will turn off its motors and stand
still. However if it hears another robot nearby, then it should
move, but it should choose how to move with some randomness.

Create a new file
called `transmit-receive-randmotion.c`. Using your
previous code from lab 2 as examples, modify the message callbacks
so that the robot is transmitting a single message (set
in `program_init` to be the robot ID) and when it
recives a message, it turns the LED yellow for 10ms. Feel free to
test this code quickly with your two robots. Both should blink
when they see each other.

As the next step, modify the code above so that the robot
chooses randomly between three different colors to flash when it
gets a message. The code below show how to use the
function `rand()` to pick a number between 0 and
9. Note that we are favoring the color red by picking it with
higher probability than the other two colors. Make sure that the
code still works as before; the LED should be off if the robot
does not hear any neighbors.

```
int pickrand = 0;

//In Your Program loop

pickrand = rand() % 10;    // pick random number in the range 0 to 9

if (pickrand < 5) {  
    set LED to red
} else if (pickrand < 7) {
    set LED to green
} else {
    set LED to green
}
```

Finally, lets add the motion. But first we will create a new
subroutine function called `set-motion-direction`. This
function will check the current direction of the robot, and only
spinup and set motors if the robot is changing its direction. Note
that you must define this function before
the `program_loop()` code begins.

```
// Motion: Putting the movement in a function to make the code cleaner and more efficient
// Now, we only set the motors if the robot is changing what motion it was doing
// 0 = stopped 1 = forward 2 = clockwise 3 = counterclockwise

int current_direction = 0;

void set_motion_direction(int newdirection) {
if (current_direction is not equal to the newdirection) { 

if (newdirection == 0) { 
set motors to stop
} else if (newdirection == 1) {
set motors to move forward
} else if (newdirection == 2) {
set motors to turn clockwise
} else {
set motors to turn counterclockwise
}
current_direction = newdirection;   // remember the new direction
}
}
```

Now in the program loop, modify the code so that in addition to
turning on the LEDs, the robot also sets a direction based on the
random number (red straight, green left, blue right).

## LAB 4 and Beyond

Lab 4: Orbit (two programs orbit-star.c and orbit-planet.c) more complex movement

Lab 5: Sync (two programs test-oscillator.c and sync.c), more complex communication and timing

Lab 6: Move-to-light (introduce light sensing), light plus interesting movement

Lab 7: Gradient formation, using the message value

For Labs 6, 7, 8 we can actually bring the whole class
together. Especially for Lab 8 visualizing the who;e gradient by
havign a single seed robot and all the rest just flashing their hop
count color (mod something).
