#include <msp430.h>


int count = 0; //variable to keep track of button interrupts

void main(void)
{
WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer
PM5CTL0 &= ~LOCKLPM5;

P1DIR |= 0x01; // Set LEDS as outputs
P1OUT &= 0xFE; // Initialize the LEDs off
P1REN |= 0x02; //Enables a resistor for the button.
P1OUT |= 0x02; //Setting the resistor as a pull-up.
P1IES |= 0x02; //Sets Edge so that the button is activated when pressed down.

P1IE |= 0x02; //Sets mask so that interrupt can't be ignored

TB0CCTL0 = CCIE;                           // CCR0 interrupt enabled
TBCTL = TBSSEL_2 + MC_2;                 // SMCLK set to continuous mode

__enable_interrupt(); // enables interrupt for board

while (1) //infinite loooooooooooop
{

if(count > 0)
{
P1OUT ^= 0x01; // flipping everytime it loops

__delay_cycles(100000); // slow down clock so you can actually see the blinking
}
}

}

// Interrupt routine for port 1
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
count ^= 0x01;  // flips LED so that the if statement to blink the LED is activated.
P1IE &= 0xFD; //disable button interrupts
P1IFG &= 0xFD; // Clear flag when done
P1OUT &= 0xFE; // Turn's LEDs off after flag is cleared.

}

#pragma vector=TIMER0_B0_VECTOR               //Interrupt routine

__interrupt void Timer_B (void)
{

  P1IE |= 0x02;                       //enable button interrupts

}
