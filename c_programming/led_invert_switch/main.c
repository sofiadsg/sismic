/**
 * Inverts red LED state when S1 or S2 are pressed. Not fully working
 * TODO: fix this whole thing
 */
#include <msp430.h> 

void debounce(void)
{
    float cont = 10000000;
    while (cont > 0)
    {
        cont -= 1;
    }

}

/**
 * main.c
 */
int main(void)
{
    int oldSwitch = 0;
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    P2DIR &= ~BIT1;
    P2REN |= BIT1;
    P2OUT |= BIT1;

    P1DIR |= BIT0;

    while(1)
    {
/*       if ((P2IN&BIT1) != oldSwitch)
        {
            debounce();
        }
*/        if ((P2IN&BIT1) == 0x00 & oldSwitch != 0x00)
        {
            P1OUT ^= BIT0;
        }
        oldSwitch = P2IN&BIT1;
    }
    return 0;
}
