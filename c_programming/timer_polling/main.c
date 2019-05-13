/**
 * Program that uses timer polling to toggle led 
*/


#include <msp430.h> 

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;
    P4DIR |= BIT7;
    P4OUT |= BIT7;
    TA0CCR0 = 32760;
    TA0CTL = TASSEL__ACLK | MC__UP;
    while(1){
        while(!(TA0CCTL0 & CCIFG));
        TA0CCTL0 &= ~CCIFG;
        P4OUT ^= BIT7;
    }
	return 0;
}
