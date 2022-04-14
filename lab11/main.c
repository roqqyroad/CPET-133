#include <msp430.h> 
#include <stdint.h>
#include <stdbool.h>

//******************************************************************//
//         CPET133 Lab11 - Traffic Light Controller
//
//
// this program functions as the controller for a traffic light with the
// following operation
// •    The main lights stay green and the side light is red until a car is
//      detected on the side road
// •    Once a car is detected, the main light goes yellow for 5 seconds and
//      then red for 5 seconds while the side light stays red
// •    After they are both red for 5 seconds, the side light goes green and
//      stays green for 20 seconds
// •    When 20 seconds has passed, the side light goes yellow for 5 seconds
//      and then red for 5 seconds while the main light stays red
// •    After they are both red for 5 seconds, the main lights go green. The
//      main lights will stay green for 20 seconds even if there is a car on
//      the side road
//
//This program uses the ECTET breakout board with the TI MSP430 LaunchPad
//
//The LEDs used are the three multi-colored ones (2 main and 1 side)
//The pushbutton used is the one labelled 'CAR'
//*****************************************************************//
void main()
{
    //declare the states and create the state and prevState variables
     enum state_type {MGSR, MYSR, MRSR1, MRSG, MRSY, MRSR2, MHOLD}
     state, prevState;
    volatile uint32_t i;        // iteration counter for delay loop
    volatile bool isCar;  //declare boolean variables to know if there is a car
    volatile bool isNewState; // boolean variables to know if it is a new state
    //declare state timer - can use 8 bit int since it never goes above 20
    uint8_t statetimer = 0;

    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;       //always include these two lines

    //Configure the port pins as inputs and outputs
    //P1 Ports
    P1DIR = 0b11111100;
    P1OUT = 0b11111100;
    //P2 Ports
    P2DIR = 0b11111101;
    //set the LEDs for Main Green, Side Red
    //side_red = 1.5
    //main green = 1.3
    P1OUT = 0b00101000;
    //initialize state and prevState
    state = MGSR;
    prevState = MHOLD;

    //start endless loop
    while(1) {
        isCar = ((P2IN & 0b00000010) == 0); //check to see if car is present
        isNewState = (state != prevState); //checks if is a new state
        prevState = state; //save the current state as prevState
        


       switch(state) {   //switch on the current state
           case MGSR:
              P1OUT = P1OUT = 0b00101000;      //turn on main green and side red
              if (isCar == 1) {              //if there is a car, switch states
                  state = MYSR;
              }
           break;


           case MYSR:
             if (isNewState) {      //if its a new state, reset the statetimer
                statetimer = 0;
             }
             P1OUT = 0b00100100;    //turn on main yellow, side red
             statetimer++;           //increment the state timer
             if(statetimer >= 5){    //if statetimer == 5, switch states
                 state = MRSR1;
             }
           break;


           case MRSR1:
             if (isNewState) {     //if its a new state, reset the statetimer
                statetimer = 0;
             }
             P1OUT = 0b10100000;    //turn on main red, side red
             statetimer++;            //increment the state timer
             if(statetimer >= 5){      //if statetimer == 5, switch states
                 state = MRSG;
             }
           break;


           case MRSG:
             if (isNewState) {      //if its a new state, reset the statetimer
                 statetimer = 0;
             }
             P1OUT = 0b10010000;   //turn on main red, side green
             statetimer++;          //increment the state timer
             if(statetimer >= 20){   //if statetimer == 20, switch states
                 state = MRSY;
             }
           break;


           case MRSY:
             if (isNewState) {    //if its a new state, reset the statetimer
                 statetimer = 0;
             }
             P1OUT = 0b11000000;    //turn on main red, side yellow
             statetimer++;           //increment the state timer
             if(statetimer >= 5){     //if statetimer == 5, switch states
                 state = MRSR2;
             }
           break;


           case MRSR2:
             if (isNewState) {   //if its a new state, reset the statetimer
                 statetimer = 0;
             }
             P1OUT = 0b10100000;    //turn on main red, side red
             statetimer++;           //increment the state timer
             if(statetimer >= 5){     //if statetimer == 5, switch states
                 state = MHOLD;
             }
           break;


           case MHOLD:
             if (isNewState) {   //if its a new state, reset the statetimer
                 statetimer = 0;
             }
             P1OUT = 0b00101000;      //turn on main green, side red
             statetimer++;             //increment the state timer
             if(statetimer >= 20){      //if statetimer == 20, switch states
                 state = MGSR ;
             }
           break;



       }  //end switch
       for(i=47500;i>0;i--);  //this kind of creates 1 second delay
                              //not very accurate
     } //end while
 } //end main
