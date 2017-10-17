#include <msp430.h>
int state = 0;
int PWM = 500;

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;                   //enable GPIO pins

    P1OUT &= 0xFE;
    P1DIR |= 0x01;//set P1.0
    P1DIR &= 0xFD;//set P1.1 as input
    P1REN |= 0x02;//enables the pull up resistor at P1.1
    P1OUT |= 0x02;//set pull up resistor at P1.1
    P1IE |= 0x02;//enable interrupt
    P1IES |= 0x02;//falling edge
    P1IFG &= 0xFD;//clear flag

    TB0CCR0 = 1000;
    TB0CTL = TBSSEL_2 + MC_1 + TBCLR;
    TB1CCTL0 = CCIE;
    TB1CCR0 = 50000;


    __enable_interrupt();
    __bis_SR_register(GIE); // LPM0 with interrupts enabled

    for(;;)
    {
        if(TB0R <= PWM)
        {
            P1OUT ^= 0x01;
        }
        else if(TB0R > PWM)
        {
             P1OUT &= 0xFE;
        }
    }
}

#pragma vector= PORT1_VECTOR        // Button interupt
__interrupt void PORT_1(void)
{
    TB1CTL = TBSSEL_2 + MC_1 + TBCLR;
    if(state == 0)
    {
        P2OUT |= 0x01;
        if(PWM < 1000)
        {
            PWM = PWM + 100;
        }
        else
        {
            PWM = 0;
        }
    }

    P1IFG &= 0xFD;
    P1IE &= 0xFD;
}

// Timer B0 interrupt
#pragma vector = TIMER1_B0_VECTOR
__interrupt void Timer_B (void)
{
    P1IE |= 0x02;
    if(state == 0)
    {
        P1IES &= 0xFD;
        state = 1;
    }
    else if(state == 1)
    {
        P1IFG &= 0xFD;
        P1IES |= 0x02;
        state = 0;
    }


    TB1CTL &= ~TBSSEL_2;
    TB1CTL |= TBCLR;

}
