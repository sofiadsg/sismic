/**
 * RED led PWM with duty cycle that changes with the pressing of a switch.
 */

#include <msp430.h>
float mulconst = 0.5;


int main(void)
{

   WDTCTL = WDTPW | WDTHOLD;
    P1DIR |= BIT0;
    P1OUT |= BIT0;

    P1REN |= BIT1;                            // Enable P1.1 internal resistance
    P1OUT |= BIT1;                            // Set P1.1 as pull-Up resistance
    P1IES &= ~BIT1;                           // P1.1 Lo/Hi edge
    P1IFG &= ~BIT1;                           // P1.1 IFG cleared
    P1IE |= BIT1;

    P2REN |= BIT1;
    P2OUT |= BIT1;
    P2IES &= ~BIT1;
    P2IFG &= ~BIT1;
    P2IE |= BIT1;

    TB0CTL = TBSSEL__SMCLK|MC_2|TBCLR;
    TB0CCR0 = 20000 - 1;

    TA0CCTL1 = CCIE;
    TA0CTL = TASSEL__SMCLK|MC__UP|TACLR|TAIE;

    TA0CCR0 = 1000 - 1;
    TA0CCR1 = 500;

    __enable_interrupt();
    while(1)
    {
    }
}

#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
    TB0R = 0;
    while(TB0R < 15000);

    switch( __even_in_range( P2IV, P2IV_P2IFG7 )) {
    case P2IV_P2IFG1:
        if (mulconst == 0.1)
        {
            mulconst = 0.2;
        }
        mulconst -= 0.1;
        TA0CCR1 = 1000*mulconst - 2;
        break;
    default:   _never_executed();

    }
}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    TB0R = 0;
    while(TB0R < 15000);

    switch( __even_in_range( P1IV, P1IV_P1IFG7 )) {
    case P1IV_P1IFG1:
        if (mulconst == 0.9)
        {
            mulconst = 0.8;
        }
        mulconst += 0.1;
        TA0CCR1 = 1000*mulconst - 2;
        break;
    default:   _never_executed();

    }
}

#pragma vector=52;
__interrupt void P1ISR(void)
{
    switch(TA0IV)
    {
        case 0x02:
            P1OUT &= ~BIT0;
            break;
        case 0x0e:
            P1OUT |= BIT0;
            break;
        default: break;
    }
}
