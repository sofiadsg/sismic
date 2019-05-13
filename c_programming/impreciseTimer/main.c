/**
 * Blinking LED without a timer, using a while loop to spend time
 */
#include <msp430.h> 

/**
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
    P4DIR |= BIT7;
    P4OUT |= BIT7;
    while(1)
    {
        double i = 10000000000000;
        while (i>0)
        {
            i-=0.000001;
        }
        P4OUT ^= BIT7;

    }
	
	return 0;
}
