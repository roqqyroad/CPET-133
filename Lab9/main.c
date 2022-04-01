//******************************************************************//
//         CPET133 Lab9 - Introduction to Launchpad and CCS
//
//
//  This program functions as a simple flashing LED controller
//  -port pin P1.0 is set as an output and driven to a 1 (LED on)
//  in a continuous loop, P1.0 in inverted and then the program is delayed.
//  The looping of inverting and delaying causes a blinking LED.
//*****************************************************************//

#include <msp430.h> 
#include <stdint.h>

int main(void)
{
  volatile uint16_t i;

  WDTCTL = WDTPW+WDTHOLD;                   // Stop WDT
  PM5CTL0 &= ~LOCKLPM5;                     // Always include these two lines
//initialize ports here
  P1DIR = 0b00000010;                       // P1.0 (port 1, bit 0) set as output
  P1OUT = 0b00000010;                       // P1.0 (port 1, bit 0) set to 1
// continuous loop
  while(1)
  {
    P1OUT = P1OUT ^ 0b00000010;             // XOR P1.0 with 1 to invert it
    for(i=5;i>0;i--){}                      // Delay (do nothing for 50000 clock cycles)
  }
}
