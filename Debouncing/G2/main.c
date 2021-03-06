#include <msp430.h>


int count = 0; //variable to keep track of button interrupts

void main(void)
{
WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer


P1DIR |= 0x01; // Set LEDS as outputs
P1OUT &= 0xFE; // Initialize the LEDs off
P1REN |= 0x08; //Enables a resistor for the button.
P1OUT |= 0x08; //Setting the resistor as a pull-up.
P1IES |= 0x08; //Sets Edge so that the button is activated when pressed down.

P1IE |= 0x08; //Sets mask so that interrupt can't be ignored

TA0CCTL0 = CCIE;                           // CCR0 interrupt enabled
TACTL = TASSEL_2 + MC_2;                 // SMCLK set to continuous mode

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
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
count ^= 0x01;  // flips LED control
P1IE &= 0xF7; //disable button interrupts
P1IFG &= 0xF7; // Clear flag
P1OUT &= 0xFE; // Turn off LED

}

#pragma vector=TIMER0_A0_VECTOR               //Interrupt routine

__interrupt void Timer_A (void)
{

  P1IE |= 0x08;                       //enable button interrupts

}
