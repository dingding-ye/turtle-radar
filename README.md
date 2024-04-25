# turtle-radar
Turtle crossing speed calculating radar sign

intro/catch: 
- turtle project pitch explanation

## Components & Subsystems

### Hypersonic Sensor (HC-SR04)
The first subsystem involves taking an external signal as input and performing speed and distance calculations from that signal. I chose a hypersonic sensor that uses the Doppler shift to calculate these data values. Much like the Doppler-shift Compensation behavior that bats exhibit for echolocation, the hypersonic sensor emits a sonic burst of 8 pulses of a few microseconds and waits for an "echo" to determine distance traveled, and from there the speed of the object.

I chose the [HC-SR04 module](https://cdn.sparkfun.com/datasheets/Sensors/Proximity/HCSR04.pdf) for its ease of use and its relatively low price point. 

hypersonic sensor
- explain doppler effect (link)
- chose part for its price point, datasheet included in coresponding folder

> [!WARNING]
> Different voltage levels used!

From the HC-SR04 datasheet, we can see that it operates at a 5V supply voltage, but feeds back to the microcontroller, which is operating at 3.3V, as the ECHO output signal. Thus, it is necessary to use a voltage divider to translate between these two voltage levels. 
As I only had 330Ω resistors, I created a three resistor voltage divider to translate down to 3.3V

$$3.3V = \frac{R_1 + R_2}{R_1+R_2+R_3} * 5V$$ for $'R_1 = R_2 = R_3 = 330\Omega'$
**insert equation**
![voltage-divider-image](https://github.com/dingding-ye/turtle-radar/Images/94885006/69ea2e6c-c193-4cf3-bb11-3c4b6123b7e8)


, but any equivalent voltage divider will work as well.

### 7-Segment LED Display
The 

### Serial Comms (UART)
An aspect that I found very useful useful for debugging and for displaying values in general is printing values to the terminal.

Thus, I selected the alternate functions of P1.1 and P1.2 to 
and configured TX and RX pins in the necessary configurations to print values to terminal.

On paper, this seemed relatively simple, but I ended up running into some issues with the *ltoa* function and displaying decimal values. I then added extra code, including 
to resolve these issues.
Finally, I was able to display the distance to the turtle and the speed of the turtle on the terminal to confirm consistent display values with the 7-segment display as well as provide capability for some very necessary debugging.
**include terminal picture**

## Execution & Overall System


