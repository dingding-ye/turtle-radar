# turtle-radar
Turtle crossing speed calculating radar sign

intro/catch: 
- turtle project pitch explanation

Materials:
- MSP4302553 Microcontroller (1)
- HC-SR04 Ultra sonic sensor (1)
- 7-segment display (1, but can use more for more digits displayed)
- Resistors (2+)
- Wires and breadboard (alternatively, can use printed board and soldering iron for more consolidated feel)

## Components & Subsystems

### Hypersonic Sensor (HC-SR04)
The first subsystem involves taking an external signal as input and performing speed and distance calculations from that signal. I chose a hypersonic sensor that uses the Doppler shift to calculate these data values. Much like the Doppler-shift Compensation behavior that bats exhibit for echolocation, the hypersonic sensor emits a sonic burst of 8 pulses of ultrasound at 40KHz and waits for an "echo" to determine distance traveled, and from there the speed of the object.

I chose the [HC-SR04 module](https://cdn.sparkfun.com/datasheets/Sensors/Proximity/HCSR04.pdf) for its ease of use and its relatively low price point. It has four pins, of which I connected VCC to 5V, GND to GND, TRIGGER to P2.6, and ECHO to P2.1 through a voltage divider (described in detail below) to the MSP430. 

> [!WARNING]
> Different voltage levels used!

From the HC-SR04 datasheet, we can see that it operates at a 5V supply voltage, but feeds back to the microcontroller, which is operating at 3.3V, as the ECHO output signal. Thus, it is necessary to use a voltage divider to translate between these two voltage levels. 
As I only had 330Ω resistors, I created a three resistor voltage divider to translate down to 3.3V.

$$3.3V = \frac{R_1 + R_2}{R_1+R_2+R_3} * 5V \text{, for } R_1 = R_2 = R_3 = 330\Omega$$
![image](https://github.com/dingding-ye/turtle-radar/assets/94885006/d4da7f6e-c575-4cea-a43b-d0ad34b41ae1)

If you only have certain resistor values on hand, any equivalent voltage divider will work as well.

### Timers and Interrupts
The TRIGGER input signal is described by the HC-SR04 datasheet as needing to supply an input pulse with a width of 10 microseconds to determine when to start the ranging. The datasheet also recommends a period of the signal between pulses of at least 60 milliseconds. 

In order to generate this signal to the TRIGGER pin, I used Timer A0. Setting the timer interrupt to Up Count mode (where the interrupt trigger) and to use the SMCLK at a frequency of 1MHz, I enabled Output mode 7 (reset/set) to be able to use PWM with the CCR0 and CCR1 registers. Then, the CCR1 register can be set to the 10 microsecond pulse while still staying within the minimum period limit defined by the datasheet. 


### Distance and Speed Calculations
From the diagram from the HC-SR04 below, we can see that ECHO 
![image](https://github.com/dingding-ye/turtle-radar/assets/94885006/e54606bb-1edf-4355-be12-1866123c17b7)


### 7-Segment LED Display
For displaying the speed as an output, I chose to use a 7-segment display for clear and clean conveying of data. The parts are from UXCell and are a set of common cathode 7-segment displays. I wired up one of the displays to pins P1.0 to P1.6 of the MSP430 (one pin for each segment of the display for separate control of each).

Originally, the speed was shown in miles per hour like many radar signs for Houston traffic, but as I could not get the turtles to go much faster than 1 mph (if even that), I changed my units to cm/s (the metric dark side) for a wider range of representation of speeds.


### Serial Comms (UART)
An aspect that I found very useful useful for debugging and for displaying values in general is printing values to the terminal.

Thus, I selected the alternate functions of P1.1 and P1.2 to be able to do so and configured TX and RX pins in the necessary configurations to print values to terminal.
On paper, this seemed relatively simple, but I ended up running into some issues with the *ltoa* function and displaying decimal values. I then added extra code, including conditions of printing depending on the value of the decimal part of the distance, to resolve these issues.
Finally, I was able to display the distance to the turtle and the speed of the turtle on the terminal to confirm consistent display values with the 7-segment display as well as provide capability for some very necessary debugging.
**include terminal picture**

## Execution & Overall System


