#include <msp430.h> 

int state = 0; // initialze state for button
void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;
    //Initialize LEDs for pwm
        P2DIR |= 0x01;  //Pin set as output with wire connecting to led header instead of P1.0
        P2SEL |= 0x01;  //pin set to TA0.1
        P1DIR &= 0xFD;  //set P1.1 as input
        P1REN |= 0x02;  //enables the pull up resistor at P1.1
        P1OUT |= 0x02;  //set pull up resistor at P1.1
        P1IE |= 0x02;   //enable interrupt
        P1IES |= 0x02;  //toggle edge
        P1IFG &= 0xFD;  //clear flag

        //Set up CCRs for clock count and pwm control
        TB0CCR0  = 0xFF;      //After timerA counts to 255 reset
        TB0CCTL1 = OUTMOD_7;    //Put Capture Control 1 in set and reset mode
        TB0CCR1 = 0x7F; //Initialize Capture Control Register 1 to be at 50% duty cycle
        TB0CTL = TBSSEL_2 + MC_1 + ID_3;   //TimerA uses SMCLK, with divider of 8, in count up mode
        __bis_SR_register(GIE);       // Enter LPM0, interrupts enabled
        __no_operation(); // For debugger
}

#pragma vector= PORT1_VECTOR
__interrupt void PORT_1(void)
{
    switch (state){
    case 0:
       TB0CCR1 = 0xBF;      //increase duty cycle
       state = state + 1;
       break;
    case 1:
       TB0CCR1 = 0xFF;      //increase duty cycle
       state = state + 1;
       break;
    case 2:
       TB0CCR1 = 0x00;      //turn off LED
       state = state + 1;
       break;
    case 3:
       TB0CCR1 = 0x3F;      //increase duty cycle
       state = state + 1;
       break;
    case 4:
       TB0CCR1 = 0x7F;      //increase duty cycle
       state = 0;
       break;

    }
}
