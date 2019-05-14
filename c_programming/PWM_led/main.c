/**
 * Simple PWM with 50% duty cycle
 */
#include <msp430.h>

#pragma vector=52;
__interrupt void P1ISR(void)
{
    switch(TA0IV)
    {
        case 0x02:
            P1OUT &= ~BIT0;
            break;
        case 0x0e:
            P1OUT |= BIT0;
            break;
        default: break;
    }
}


int main(void)
{

    WDTCTL = WDTPW | WDTHOLD;

    P1DIR |=  BIT0;
    P1OUT &= ~BIT0;


    TA0CCTL1 = CCIE;
    TA0CTL = TASSEL__SMCLK|MC__UP|TACLR|TAIE;

    TA0CCR0 = 1000 - 1;
    TA0CCR1 = 10;

    __enable_interrupt();
    while(1)
    {
        while(TA0CCR1 < 1000)
        {
            TA0CCR1 +=1;
        }
        TA0CCR1 = 10;
    }
}
