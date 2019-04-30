/**
 * Lights LED when S1 is pressed
 */
#include <msp430.h> 


/**
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	P2DIR &= ~BIT1;
	P2REN |= BIT1;
	P2OUT |= BIT1;

	P4DIR |= BIT7;
	
	while(1)
	{
	    if ((P2IN&BIT1) == 0x00)
	    {
	        P4OUT |= BIT7;
	    }
	    else
	    {
	        P4OUT &= ~BIT7;
	    }
	}
	return 0;
}
