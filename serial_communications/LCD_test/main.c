// testa todos os caracteres da tabela ASCII
#include <msp430.h>
#include <stdio.h>

#define PCF     0x3f //Endereço PCF8574T
#define PCF_ADR 0x3f //Endereço PCF8574T

#define BR100K  10
#define BR10K   21

#define SDA_SEL P3SEL
#define SDA_BIT BIT0
#define SDA_REN P3REN
#define SDA_OUT P3OUT
#define SCL_SEL P3SEL
#define SCL_BIT BIT1
#define SCL_REN P3REN
#define SCL_OUT P3OUT

#define BIT_RS BIT0
#define BIT_RW BIT1
#define BIT_E BIT2
#define BIT_BL BIT3
#define BIT_D4 BIT4
#define BIT_D5 BIT5
#define BIT_D6 BIT6
#define BIT_D7 BIT7

int PCF_read(void);
void PCF_write(char dado);
void delay(long limite);


void config_I2C(void){
    SDA_SEL |=  SDA_BIT;    // Use dedicated module
    SDA_REN |=  SDA_BIT;    // Resistor enable
    SDA_OUT |=  SDA_BIT;    // Pull-up

    SCL_SEL |=  SCL_BIT;    // Use dedicated module
    SCL_REN |=  SCL_BIT;    // Resistor enable
    SCL_OUT |=  SCL_BIT;    // Pull-up

    UCB0CTL1 |= UCSWRST;    // UCSI B0 em ressete

    UCB0CTL0 = UCSYNC |     //Síncrono
               UCMODE_3 |   //Modo I2C
               UCMST;       //Mestre

    //UCB0BRW = BR100K;       //100 kbps
    //UCB0BRW = BR20K;      // 20 kbps
    UCB0BRW = BR10K;      // 10 kbps
    UCB0CTL1 = UCSSEL_2;   //SMCLK e remove ressete
}

void LCD_aux(char dado){
    while ( (UCB0IFG & UCTXIFG) == 0);              //Esperar TXIFG=1
    UCB0TXBUF = ((dado<<4)&0XF0) | BIT_BL;          //PCF7:4 = dado;
    delay(50);
    while ( (UCB0IFG & UCTXIFG) == 0);              //Esperar TXIFG=1
    UCB0TXBUF = ((dado<<4)&0XF0) | BIT_BL | BIT_E;  //E=1
    delay(50);
    while ( (UCB0IFG & UCTXIFG) == 0);              //Esperar TXIFG=1
    UCB0TXBUF = ((dado<<4)&0XF0) | BIT_BL;          //E=0;
}

void PCF_STT_STP(void){
    int x=0;
    UCB0I2CSA = PCF_ADR;        //Endereço Escravo

    while (x<5){
        UCB0CTL1 |= UCTR    |   //Mestre TX
                    UCTXSTT;    //Gerar START
        while ( (UCB0IFG & UCTXIFG) == 0);  //Esperar TXIFG=1
        UCB0CTL1 |= UCTXSTP;                //Gerar STOP
        delay(200);
        if ( (UCB0CTL1 & UCTXSTP) == 0)   break;   //Esperar STOP
        x++;
    }
    while ( (UCB0CTL1 & UCTXSTP) == UCTXSTP);   //I2C Travado (Desligar / Ligar)
}

void LCD_inic(void){

    PCF_STT_STP();      //Colocar PCF em estado conhecido

    // Preparar I2C para operar
    UCB0I2CSA = PCF_ADR;    //Endereço Escravo
    UCB0CTL1 |= UCTR    |   //Mestre TX
                UCTXSTT;    //Gerar START
    while ( (UCB0IFG & UCTXIFG) == 0);          //Esperar TXIFG=1
    UCB0TXBUF = 0;                              //Saída PCF = 0;
    while ( (UCB0CTL1 & UCTXSTT) == UCTXSTT);   //Esperar STT=0
    if ( (UCB0IFG & UCNACKIFG) == UCNACKIFG)    //NACK?
                while(1);

    // Começar inicialização
    LCD_aux(0);     //RS=RW=0, BL=1
    delay(20000);
    LCD_aux(3);     //3
    delay(10000);
    LCD_aux(3);     //3
    delay(10000);
    LCD_aux(3);     //3
    delay(10000);
    LCD_aux(2);     //2

    // Entrou em modo 4 bits
    LCD_aux(2);     LCD_aux(8);     //0x28
    LCD_aux(0);     LCD_aux(8);     //0x08
    LCD_aux(0);     LCD_aux(1);     //0x01
    LCD_aux(0);     LCD_aux(6);     //0x06
    LCD_aux(0);     LCD_aux(0xF);   //0x0F

    while ( (UCB0IFG & UCTXIFG) == 0)   ;          //Esperar TXIFG=1
    UCB0CTL1 |= UCTXSTP;                           //Gerar STOP
    while ( (UCB0CTL1 & UCTXSTP) == UCTXSTP)   ;   //Esperar STOP
    delay(50);
}

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;
    TB0CTL = TBSSEL__ACLK|MC__STOP|ID_2|TBCLR|TBIE;
    config_I2C();
    LCD_inic();
    int i = 45;
    while (i>0)
    {
        PCF_write(0b01001001);
        PCF_write(0b01001101);
        PCF_write(0b01001001);
        PCF_write(0b00011001);
        PCF_write(0b00011101);
        PCF_write(0b00011001);
        i--;
    }

    return 0;
}

void delay(long limite){
    volatile long cont=0;
    while (cont++ < limite) ;
}

//void delay(int time) //delays time up to 4s, time is in ms
//{
//    TB0CTL |= MC__UP;
//    TB0CCR0 = time*16.384 - 1;
//    while(!(TB0CCTL0 & CCIFG));
//    TB0CCTL0 &= ~CCIFG;
//    TB0CTL |= MC__STOP|TBCLR;
//}


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


