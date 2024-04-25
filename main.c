#include <msp430.h> 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

// Connections
// P2.1 ECHO    purple
// P2.6 TRIGGER blue
// 1MHz clock frequency, so diff is in microseconds


#define DURATION 1    // in seconds, echolocation ping period

// 7-segment Display Connections (P1.0 to P1.6)
#define a BIT0
#define b BIT1
#define c BIT2
#define d BIT3
#define e BIT4
#define f BIT5
#define g BIT6

// Numbers to display for 7-segment
int numbers[10] = {a+b+c+d+e+f, b+c, a+b+g+e+d, a+b+g+c+d, f+g+b+c, a+f+g+c+d, a+f+e+d+c+g, a+b+c, a+b+c+d+e+f+g, a+b+c+d+f+g};

// Distance variable declarations
volatile int temp[2];           // Store rising and falling edge of TAC1CCR1 (ECHO) register
volatile float diff;            // Store differences in echolocation for calculating distance
volatile unsigned int i=0;

int dst_int;                    // Integer part of distance
int dst_flt;                    // Decimal part of distance
float tmp_flt;

char dst_char[5];             // Convert distance integer part into char for printing to terminal
char dst_flt_char[5];         // Convert distance decimal part into char for printing to terminal
char speed_char[5];           // Convert speed into char for printing to terminal

// Different phrases for printing to terminal
char printdist[] = "Distance to turtle is: ";
char printspeed[] = "Turtle speed: ";
char centimeter[] = " cm";
char mph[] = " cm/s";
char dot[] = ".";
char zerro[] = "0";
char newline[] = " \r\n";

void ser_output(char *str);


int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer
    BCSCTL1 = CALBC1_1MHZ;      // Set clock frequency to 1MHz
    DCOCTL = CALDCO_1MHZ;

    // Turn on LEDs of 7-segment display
    P1DIR = a + b + c + d + e + f + g; //

    // Set up pins for sensor
    P2DIR = BIT6;       // P2.6 TRIGGER
    P2SEL = BIT1|BIT6;  // P2.1 ECHO

//    // Set up pins for digital comms
//    P1SEL |= BIT1|BIT2;          // Set P1.1 and P1.2 as digital comms
//    P1SEL2 |= BIT1|BIT2;

//    // Serial comms setup
//    UCA0CTL1 |= UCSWRST + UCSSEL_2;
//    UCA0BR0 = 52;
//    UCA0BR1 = 0;
//    UCA0MCTL = UCBRS_0;
//    UCA0CTL1 &= ~UCSWRST;

    // Timer setup for sensor update
    TA0CTL = TASSEL_2 + MC_1 ;  // SMCLK, count up to CCR0
    TA0CCR0 = 0xFFFF;           // Count to 65535
    TA0CCR1 = 0x000A;           // Create 10usec pulse for TRIGGER input signal (see datasheet)
    TA0CCTL1 = OUTMOD_7;        // PWM output mode 7: reset/set

    TA1CTL = TASSEL_2 + MC_2 ;  // SMCLK, continuous up mode
    TA1CCTL1 = CAP + CCIE + CCIS_0 + CM_3 + SCS ;   // Capture mode setting (trigger on rising and falling edges), synchronize signal

    _enable_interrupts();

    float last_dist, curr_dist, speed, ones;    // Storing previous distance, and different current distance variable

    while(1) {
        curr_dist = diff / 58;                              // Convert distance to centimeters (58 from datasheet)
        speed = ((abs(last_dist - curr_dist)) / DURATION);  // Distance / time for speed calculation
        last_dist = curr_dist;                              // Store current distance as previous after calculation

//        ltoa(speed, speed_char, 10);    // Convert speed to char and print to terminal
//        ser_output(printspeed); ser_output(speed_char); ser_output(mph); ser_output(newline);
//
//        dst_int = floor(curr_dist);     // Separate integer part of distance
//        tmp_flt = curr_dist - dst_int;  // Get decimanl part of distance
//        ltoa(dst_int, dst_char, 10);    // Convert integer part of distance to char
//
//        // Print out decimal part of distance differently depending on value
//        if (tmp_flt < 0.01) {
//            dst_flt = floor(tmp_flt * 1000);
//            ltoa(dst_flt,dst_flt_char, 10);
//            ser_output(printdist); ser_output(dst_char); ser_output(dot); ser_output(zerro); ser_output(zerro); ser_output(dst_flt_char); ser_output(centimeter);
//        }
//        else if (tmp_flt < 0.1) {
//            dst_flt = floor(tmp_flt * 100);
//            ltoa(dst_flt,dst_flt_char, 10);
//            ser_output(printdist); ser_output(dst_char); ser_output(dot); ser_output(zerro); ser_output(dst_flt_char); ser_output(centimeter);
//        }
//        else {
//            dst_flt = floor(tmp_flt * 100);
//            ltoa(dst_flt,dst_flt_char, 10);
//            ser_output(printdist); ser_output(dst_char); ser_output(dot); ser_output(dst_flt_char); ser_output(centimeter);
//        }
//
//        ser_output(newline);

        ones = speed;
//        ones = fmod(speed, 10);                   // Can use to show ones place when turtle is moving faster than 10 cm/s (rare)

        // Display corresponding number on 7-segment for ones digit of the speed
        if (ones < 0.5) {P1OUT = numbers[0];}
        else if (ones < 1.5) {P1OUT = numbers[1];}
        else if (ones < 2.5) {P1OUT = numbers[2];}
        else if (ones < 3.5) {P1OUT = numbers[3];}
        else if (ones < 4.5) {P1OUT = numbers[4];}
        else if (ones < 5.5) {P1OUT = numbers[5];}
        else if (ones < 6.5) {P1OUT = numbers[6];}
        else if (ones < 7.5) {P1OUT = numbers[7];}
        else if (ones < 8.5) {P1OUT = numbers[8];}
        else if (ones < 9.5) {P1OUT = numbers[9];}
        else {P1OUT = a+g+d;}   // Default three horizontal lines if outside these conditions (too fast, too close to sensor, too far, etc.)

        __delay_cycles(DURATION * 1000000);  // 1 second delay for sensing (debouncing)

    }
}

#pragma vector = TIMER1_A1_VECTOR
__interrupt void Timer_A(void){
        temp[i] = TA1CCR1;              // Store ECHO value
        i += 1;                         // Rising edge stored at index 0, falling edge at index 1
        TA1CCTL1 &= ~CCIFG ;            // Turn off interrupt flag, wait for next interrupt
        if (i==2) {                     // After each ECHO pulse
            diff=temp[i-1]-temp[i-2];   // Calculate difference in microseconds
            i=0;                        // Reset temp ECHO value
        }
}

//void ser_output(char *str){               // Function to print to serial output
//    while(*str != 0) {
//        while (!(IFG2&UCA0TXIFG));      // If string exists to be printed
//        UCA0TXBUF = *str++;             // Print new string to terminal
//        }
//}
