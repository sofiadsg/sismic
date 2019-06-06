// Piscar Back Light
#include <msp430.h>

#define PCF 0x27 //Endereço PCF8574T

int PCF_read(void);
void PCF_write(char dado);
void delay(int tempo);

int porta=0; //Úlitmo valor escrito na porta

int main(void) {
    volatile int zz=0;
    WDTCTL = WDTPW | WDTHOLD;
    P3SEL |= BIT0|BIT1; //SDA: P3.0
    P3REN |= BIT0|BIT1; //SCL: P3.1
    P3OUT |= BIT0|BIT1;

    UCB0CTL1 |= UCSWRST;
    UCB0CTL0 = UCSYNC |UCMODE_3|UCMST;
    UCB0BRW = 100; //10kbps
    UCB0CTL1 = UCSSEL_2;

    TB0CTL = TBSSEL__ACLK|MC__STOP|TBCLR|TBIE;

    porta = 0;
    PCF_write(porta);

    while(1){
        PCF_write(0b00001000);
        delay(1000);
        PCF_write(0);
        delay(1000);
    }
}

void delay(int time) //delays time up to 2s, time is in ms
{
    TB0CTL |= MC__UP;
    TB0CCR0 = time*32.768 - 1;
    while(!(TB0CCTL0 & CCIFG));
    TB0CCTL0 &= ~CCIFG;
    TB0CTL |= MC__STOP|TBCLR;
}


void PCF_write(char dado)
{
    UCB0I2CSA = PCF;
    UCB0CTL1 |= UCTR |
            UCTXSTT;
    while ( (UCB0IFG & UCTXIFG) == 0);
    if ( (UCB0IFG & UCNACKIFG) == UCNACKIFG)
    {
        P1OUT |= BIT0;
        while(1);
    }
    UCB0TXBUF = dado;
    while ( (UCB0IFG & UCTXIFG) == 0);
    UCB0CTL1 |= UCTXSTP;
    while ( (UCB0CTL1 & UCTXSTP) == UCTXSTP);
    delay(50);
}


int PCF_read(void)
{
    int dado;
    UCB0I2CSA = PCF;
    UCB0CTL1 &= ~UCTR;
    UCB0CTL1 |= UCTXSTT;
    while ( (UCB0CTL1 & UCTXSTT) == UCTXSTT);
    UCB0CTL1 |= UCTXSTP;
    while ( (UCB0IFG & UCRXIFG) == 0);
    dado=UCB0RXBUF;
    delay(50);
    return dado;
}
