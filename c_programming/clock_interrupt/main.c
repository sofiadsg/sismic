/**
* Program to measure distances using a ultrassonic distance sensor. Lights green and red led for 
* distances smaller than 20cm, red for smaller than 40cm and greater than 20cm and green for 
* greater than 40cm
*/
#include <msp430.h>

#define TA0_CCR0_INT 53

#pragma vector=TA0_CCR0_INT
__interrupt void TA0_CCR0_ISR()
{
    P4OUT ^= BIT7;
}

int main(void)
{
    __enable_interrupt();
    WDTCTL = WDTPW | WDTHOLD;
    P4DIR |= BIT7;
    P4OUT |= BIT7;
    TA0CCR0 = 32760;
    TA0CCTL0 = CCIE;
    TA0CTL = TASSEL__ACLK|MC__UP|TACLR;
    while(1){}
    return 0;
}

