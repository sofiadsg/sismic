/**
 * Program to toggle green and red leds at different frequencies (green led @ 2kHz and
 *  red led @5kHz) using two timer instances.
 */


#include <msp430.h>

#define TA0_CCR0_INT 53

#pragma vector=TA0_CCR0_INT
__interrupt void TA0_CCR0_ISR()
{
    P4OUT ^= BIT7;
}

#pragma vector=TIMER0_B0_VECTOR
__interrupt void TIMER0_B0_VECTOR_ISR (void)
{
    P1OUT ^= BIT0; // Timer overflow
    TB0R = 0;

}

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;
    P4DIR |= BIT7;
    P4OUT |= BIT7;
    P1DIR |= BIT0;
    P1OUT |= BIT0;

    TA0CCR0 = 8192 - 1;
    TA0CCTL0 = CCIE;
    TA0CTL = TASSEL__ACLK|MC__UP|TACLR;

    TB0CCTL0 = CCIE;
    TB0CTL = TBSSEL_1|MC_2|TBCLR|TBIE;
    TB0CCR0 = 3268 - 1;


    __enable_interrupt();
    while(1){}
    return 0;
}
