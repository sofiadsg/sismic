/**
 * Lights LED when S1 is pressed
 */
#include <msp430.h> 


/**
 * main.c
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    P2DIR &= ~BIT1;
    P2REN |= BIT1;
    P2OUT |= BIT1;

    P1DIR |= BIT0;

    while(1)
    {
        if ((P2IN&BIT1) == 0x00)
        {
            P1OUT |= BIT0;
        }
        else
        {
            P1OUT &= ~BIT0;
        }
    }
    return 0;
}
