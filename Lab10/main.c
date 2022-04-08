//******************************************************************//
//         CPET133 Lab10 - MultiFunction Flashlight
//
//
// This program functions as a multi-function flashlight
//  -on the first button push the two �main� LEDs turn on yellow
//  -on the second button push the single �side� LED  turns on red
//  -on the third button push all the LEDs turn on green
//  -on the fourth button push all the LEDs flash green
//  -on the fifth button push all the LEDs turn off
//This program uses the ECET breakout board with the TI MSP430 LaunchPad
//
//The LEDs used are the three multi-colored ones and the pushbutton used
//is the one labeled 'WALK'
//*****************************************************************//

#include <msp430.h>
#include <stdint.h>

void main()
{
    uint8_t ButtonCount = 0;    // variable to track button pushes
    volatile uint16_t i;        // iteration counter for delay loop

    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;

    //Configure the port pins as inputs and outputs
    P1DIR = 0b11111100;      //P1.7-->P1.2 set as outputs
    P1OUT = 0b11111100;

    P2DIR = 0b11111110;
    //P2IN = 0b00000000;      //P2.0 set as input
    //turn all of the LEDs off
    P1OUT = 0b00000000;

    while(1) {
        if ((P2IN & 0b00000001) == 0 ) {  //determine if button is pushed - active low.
            //do the following if button is pressed
            ButtonCount++;        //increment count on each push
            if (ButtonCount == 5 ) {  //if count == 5, set it back to 0 and turn off all LEDs
                P1OUT = 0b00000000;
                ButtonCount = 0;
            }
            while ((P2IN & 0b00000001) == 0); //wait here for pin to be released.

        }
        //turn on different lights based on button count.
        if ( ButtonCount == 1 ) {  //turn on "main" yellow LEDs if count == 1
           P1OUT = P1OUT ^ 0b00000100;
        }
        else if (ButtonCount == 2 ) { //turn off yellow LEDs, turn on red "side" LED if count == 2
            P1OUT = 0b00000000;
            P1OUT = P1OUT ^ 0b00100000;
        }
        else if ( ButtonCount == 3 ){ //turn off red "side" LED and turn on all green LEDs if count ==3
            P1OUT = 0b00000000;
            P1OUT = P1OUT ^ 0b00011000;
        }
        else if ( ButtonCount == 4 ){ //flash green LEDs if count == 4
            P1OUT = P1OUT ^ 0b00011000;
           for(i=5000;i>0;i--){};       //Create a delay loop for flash
        }
    }
    return;
}
