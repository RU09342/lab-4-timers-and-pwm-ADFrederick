#include <msp430.h>


int count = 0; //variable to keep track of button interrupts

void main(void)
{
WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer
PM5CTL0 &= ~LOCKLPM5;

P1DIR |= 0x01; // Set LEDS as outputs
P1OUT &= 0xFE; // Initialize the LEDs off
P5REN |= 0x40; //Enables a resistor for the button.
P5OUT |= 0x40; //Setting the resistor as a pull-up.
P5IES |= 0x40; //Sets Edge so that the button is activated when pressed down.

P5IE |= 0x40; //Sets mask so that interrupt can't be ignored

TB0CCTL0 = CCIE;                           // CCR0 interrupt enabled
TBCTL = TBSSEL_2 + MC_2;                 // SMCLK set to continuous mode

__enable_interrupt(); // enables interrupt for board

while (1) //infinite loop
{

if(count > 0)
{
P1OUT ^= 0x01; // Flip LED

__delay_cycles(100000); // Slow Clock
}
}

}

// Interrupt routine for port 1
#pragma vector=PORT5_VECTOR
__interrupt void Port_5(void)
{
count ^= 0x01;  // Flip LED control
P5IE &= 0xAF; //disable button interrupts
P5IFG &= 0xAF; // Clear flag
P5OUT &= 0xAF; // Turn off LED

}

#pragma vector=TIMER0_B0_VECTOR               //Interrupt routine

__interrupt void Timer_B (void)
{

  P5IE |= 0x40;                       //enable button interrupts

}
