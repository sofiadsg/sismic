#include <msp430.h>

void main (void)
{
    WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer

    P1DIR |= BIT0;                            // Set P1.0 to output direction
    P1OUT &= ~BIT0;

    P1REN |= BIT1;                            // Enable P1.1 internal resistance
    P1OUT |= BIT1;                            // Set P1.1 as pull-Up resistance
    P1IES &= ~BIT1;                           // P1.1 Lo/Hi edge
    P1IFG &= ~BIT1;                           // P1.1 IFG cleared
    P1IE |= BIT1;                             // P1.1 interrupt enabled

    __bis_SR_register( GIE );                 // Enable interrupts globally

    while(1) {
        __no_operation();                     // Placeholder for while loop (not required)
    }
}

// Port 1 interrupt service routine
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    switch( __even_in_range( P1IV, P1IV_P1IFG7 )) {
    case P1IV_P1IFG1:                                       // Pin 1 (button 2)
        P1OUT^=BIT0;
        break;
    default:   _never_executed();

    }
}
