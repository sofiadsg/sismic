/**
 * Program to toggle green and red leds at different frequencies (green led @ 1kHz and red led @2kHz)
 */

#include <msp430.h>
int cont = 0;
#define TA0_CCR0_INT 53

#pragma vector=TA0_CCR0_INT
__interrupt void TA0_CCR0_ISR()
{
    if(cont == 0)
    {
        P4OUT ^= BIT7;
        cont = 1;
    }
    else
    {
        P4OUT ^= BIT7;
        P1OUT ^= BIT0;
        cont = 0;
    }
}
/**
 * main.c
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;
    P4DIR |= BIT7;
    P4OUT |= BIT7;
    P1DIR |= BIT0;
    P1OUT |= BIT0;
    TA0CCR0 = 16380;
    TA0CCTL0 = CCIE;
    TA0CTL = TASSEL__ACLK|MC__UP|TACLR;
    __enable_interrupt();
    while(1){}
    return 0;
}
