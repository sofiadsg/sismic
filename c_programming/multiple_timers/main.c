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

#pragma vector=TIMERB0_VECTOR
__interrupt void TIMERB0_ISR (void)
{
    P1OUT ^= BIT0; // Timer overflow

}

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;
    P4DIR |= BIT7;
    P4OUT |= BIT7;
    P1DIR |= BIT0;
    P1OUT |= BIT0;

    TA0CCR0 = 16379;
    TA0CCTL0 = CCIE;
    TA0CTL = TASSEL__ACLK|MC__UP|TACLR;

    TBCCTL0 = CCIE;
    TBCTL = TBSSEL__ACLK|MC__UP|TBCLR|TBIE;
    TBCCR0 = 6552 - 1;


    __enable_interrupt();
    while(1){}
    return 0;
}
