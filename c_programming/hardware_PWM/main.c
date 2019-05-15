/**
 * Simple PWM with 50% duty cycle
 */
#include <msp430.h>

void main(void)

{

WDTCTL = WDTPW + WDTHOLD; // Stop WDT

P1DIR |= 0x40; // P1.6 to output

P1SEL |= 0x40; // select P1.6 for PWM output function, see more in PxSEL page

TA0CCR0 = 10000; // PWM Period

TA0CCTL1 = OUTMOD_7; // let OUTMOD _ 7

TA0CCR1 = 5000; // CCR1 PWM duty cycle – 50%

TA0CTL = TASSEL_2 + MC_1 + ID_3; // SMCLK, up mode, divide source frequency by 8

}
