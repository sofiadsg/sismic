/**
 * Distance measurements
 */
#include <msp430.h>

int long t1 = 0;
int long t2 = 0;
int long diff = 0;
/**
 * main.c
 */


#pragma vector=52;
__interrupt void P1ISR(void)
{
    switch(TA0IV)
    {
        case 0x04:
            if(TA0CCTL2 & CCI)
            {
                t1 = TA0R;
            }
            else
            {
                t2 = TA0R;
                diff  = t2 - t1;
                if (diff < 1100)
                {
                    P1OUT |= BIT0;
                    P4OUT &= ~BIT7;

                }
                else if (diff < 2200)
                {
                    P1OUT &= ~BIT0;
                    P4OUT |= BIT7;
                }
                else
                {
                    P1OUT |= BIT0;
                    P4OUT |= BIT7;
                }


            }
            break;

        case 0x08:
            P1OUT &= ~BIT5;
            break;

        case 0x0e:
            P1OUT |= BIT5;
            break;
    }
}


int main(void)
{

    WDTCTL = WDTPW | WDTHOLD;

    P4DIR |=  BIT7;
    P4OUT &= ~BIT7;

    P1DIR |=  BIT0;
    P1OUT &= ~BIT0;

    P1DIR |=  BIT5;

    P1DIR &= ~BIT3;
    P1SEL |=  BIT3;

    TA0CCTL2 = CM_3|CAP|CCIE;
    TA0CCTL4 = CCIE;
    TA0CTL = TASSEL__SMCLK|MC__UP|TACLR|TAIE;

    TA0CCR0 = 20000 - 1;
    TA0CCR4 = 100 - 1;

    __enable_interrupt();
    while(1)
    {
    }
}
