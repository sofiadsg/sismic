/**
 * Counter up to 2 bits. S1 increases count and S2 decreases it
 */
#include <msp430.h> 

enum state
{
    s0,
    s1,
    s2,
    s3

};

void debounce(void)
{
    int cont = 100000;
    while(cont>0)
    {
        cont -= 1;
    }
}

/**
 * main.c
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    P2DIR &= ~BIT1;
    P2REN |= BIT1;
    P2OUT |= BIT1;
    P1DIR &= ~BIT1;
    P1REN |= BIT1;
    P1OUT |= BIT1;

    P1DIR |= BIT0;
    P4DIR |= BIT7;

    enum state currentState = s0;
    int oldS1 = 0;
    int oldS2 = 0;

    while(1)
    {
        if (((P2IN&BIT1)!=oldS1) || ((P1IN&BIT1!=oldS2)))
        {
            switch(currentState)
            {
                case s0:
                    P1OUT &= ~BIT0;
                    P4OUT &= ~BIT7;
                    if((P2IN&BIT1) == 0x00)
                    {
                        currentState = s3;
                    }
                    else if((P1IN&BIT1) == 0x00)
                    {
                        currentState = s1;
                    }
                    else
                    {
                        currentState = s0;
                    }
                    break;

                case s1:
                    P1OUT |= BIT0;
                    P4OUT &= ~BIT7;
                    if((P2IN&BIT1) == 0x00)
                    {
                        currentState = s0;
                    }
                    else if((P1IN&BIT1) == 0x00)
                    {
                        currentState = s2;
                    }
                    else
                    {
                        currentState = s1;
                    }
                    break;
                case s2:
                    P1OUT &= ~BIT0;
                    P4OUT |= BIT7;
                    if((P2IN&BIT1) == 0x00)
                    {
                        currentState = s1;
                    }
                    else if((P1IN&BIT1) == 0x00)
                    {
                        currentState = s3;
                    }
                    else
                    {
                        currentState = s2;
                    }
                    break;
                case s3:
                    P1OUT |= BIT0;
                    P4OUT |= BIT7;
                    if((P2IN&BIT1) == 0x00)
                    {
                        currentState = s2;
                    }
                    else if((P1IN&BIT1) == 0x00)
                    {
                        currentState = s0;
                    }
                    else
                    {
                        currentState = s3;
                    }
                    break;
            }
        }
    oldS1 = P2IN&BIT1;
    oldS2 = P1IN&BIT2;
    }
    return 0;
}
