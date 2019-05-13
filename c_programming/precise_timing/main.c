/**
 * Blinking LED with a timer
 */
#include <msp430.h> 

/**
 * main.c
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    P4DIR |= BIT7;
    P4OUT |= BIT7;
    TACTL = MC_2 | ID_3 | TASSEL_2 | TACLR;
    while(1)
    {
        while ( TACTL_bit . TAIFG == 0) {}
        TACTL_bit . TAIFG = 0;
        P4OUT ^= BIT7;

    }

    return 0;
}
