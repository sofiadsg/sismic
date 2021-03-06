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
int count = 0, rightcount=0;
float adcX, adcY, avX = 0, avY = 0, voltX, voltY;
void lcdChar(char c)
{
    int mostSignificantNibble = 0;
    int leastSignificantNibble = 0;
    mostSignificantNibble = c&0b11110000;
    leastSignificantNibble = (c&0b00001111) << 4;

    PCF_write(mostSignificantNibble|0b00001001);
    PCF_write(mostSignificantNibble|0b00001101);
    PCF_write(mostSignificantNibble|0b00001001);
    PCF_write(leastSignificantNibble|0b00001001);
    PCF_write(leastSignificantNibble|0b00001101);
    PCF_write(leastSignificantNibble|0b00001001);
}

void lcdHex8(char c)
{


    int mostSignificantNibble = 0;
    int leastSignificantNibble = 0;
    int firstNumber = 0;
    int secondNumber = 0;

    mostSignificantNibble = c >> 4;
    leastSignificantNibble = c&0b00001111;

    if (mostSignificantNibble > 0x09)
    {
        firstNumber = mostSignificantNibble + 0x37;
    }
    else
    {
        firstNumber = mostSignificantNibble  + 0x30;
    }

    if (leastSignificantNibble > 0x09)
    {
        secondNumber = leastSignificantNibble + 0x37;
    }
    else
    {
        secondNumber = leastSignificantNibble  + 0x30;
    }

    lcdChar(firstNumber);
    lcdChar(secondNumber);

}

void lcdHex16(unsigned int x)
{
    int firstNumber = 0;
    int secondNumber = 0;
    int thirdNumber = 0;
    int fourthNumber = 0;

    firstNumber = x >> 12;
    if (firstNumber > 0x09)
    {
        firstNumber = firstNumber + 0x37;
    }
    else
    {
        firstNumber = firstNumber + 0x30;
    }
    lcdChar(firstNumber);
    secondNumber = x&0b0000111100000000;
    secondNumber = secondNumber >> 8;
    if (secondNumber > 0x09)
    {
        secondNumber = secondNumber + 0x37;
    }
    else
    {
        secondNumber = secondNumber + 0x30;
    }
    lcdChar(secondNumber);
    thirdNumber = x&0b0000000011110000;
    thirdNumber = thirdNumber >> 4;
    if (thirdNumber > 0x09)
    {
        thirdNumber = thirdNumber + 0x37;
    }
    else
    {
        thirdNumber = thirdNumber + 0x30;
    }
    lcdChar(thirdNumber);
    fourthNumber = x&0b0000000000001111;
    if (fourthNumber > 0x09)
    {
        fourthNumber = fourthNumber + 0x37;
    }
    else
    {
        fourthNumber = fourthNumber + 0x30;
    }
    lcdChar(fourthNumber);


}

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

void clearAndReturnHome(void)
{
    PCF_write(0b00001000);
    PCF_write(0b00001100);
    PCF_write(0b00001000);
    PCF_write(0b00011000);
    PCF_write(0b00011100);
    PCF_write(0b00011000);
    delay(10000);
}

void lcdDec8(char c)
{
    int firstNumber,secondNumber,thirdNumber;
    firstNumber = (c/100);
    secondNumber = ((c - (c/100)*100)/10);
    thirdNumber = (c - (c/100)*100 - ((c - (c/100)*100)/10)*10);

    firstNumber = firstNumber + 0x30;
    secondNumber = secondNumber + 0x30;
    thirdNumber = thirdNumber + 0x30;

    lcdChar(firstNumber);
    lcdChar(secondNumber);
    lcdChar(thirdNumber);

}

void lcdDec16(unsigned int x)
{
    int firstNumber, secondNumber, thirdNumber, fourthNumber,fifthNumber;
    firstNumber = x/10000;
    x = x - 10000*firstNumber;
    secondNumber = x/1000;
    x = x - 1000*secondNumber;
    thirdNumber = x/100;
    x = x- thirdNumber*100;
    fourthNumber = x/10;
    x = x - fourthNumber*10;
    fifthNumber = x;

    firstNumber = firstNumber + 0x30;
    secondNumber = secondNumber + 0x30;
    thirdNumber = thirdNumber + 0x30;
    fourthNumber = fourthNumber + 0x30;
    fifthNumber = fifthNumber + 0x30;
    lcdChar(firstNumber);
    lcdChar(secondNumber);
    lcdChar(thirdNumber);
    lcdChar(fourthNumber);
    lcdChar(fifthNumber);
}

void lcdFloat1(float x)
{
    int firstNumber, secondNumber, thirdNumber, fourthNumber;
    firstNumber = x;
    x = x - firstNumber;
    secondNumber = 10*x;
    x = x - secondNumber*.1;
    thirdNumber = 100*x;
    x = x - thirdNumber*.01;
    fourthNumber = 1000*x;
    x = x - fourthNumber*.001;

    firstNumber = firstNumber + 0x30;
    secondNumber = secondNumber + 0x30;
    thirdNumber = thirdNumber + 0x30;
    fourthNumber = fourthNumber + 0x30;

    lcdChar(firstNumber);
    lcdChar('.');
    lcdChar(secondNumber);
    lcdChar(thirdNumber);
    lcdChar(fourthNumber);
}

void lcdCursor(char lin, char col)
{
    int pos;
    pos = lin*0x40 + col;

    int mostSignificantNibble = 0;
    int leastSignificantNibble = 0;
    mostSignificantNibble = pos&0b11110000;
    mostSignificantNibble = mostSignificantNibble + 0b10000000;
    leastSignificantNibble = (pos&0b00001111) << 4;

    PCF_write(mostSignificantNibble|0b00001000);
    PCF_write(mostSignificantNibble|0b00001100);
    PCF_write(mostSignificantNibble|0b00001000);
    PCF_write(leastSignificantNibble|0b00001000);
    PCF_write(leastSignificantNibble|0b00001100);
    PCF_write(leastSignificantNibble|0b00001000);
}
void config_adc(void)
{
    ADC12CTL0  &= ~ADC12ENC;

    ADC12CTL0 = ADC12SHT0_3 |
                ADC12MSC    |       //Uma conversão imediatamente após a outra
                ADC12ON;            //Ligar ADC

    ADC12CTL1 = ADC12CSTARTADD_0 |  //Resultado em ADC12MEM0
            ADC12SHS_1       |  // Selecionar TA0.1
            ADC12SHP         |  // S/H usar timer
            ADC12DIV_0       |  //Clock ADC Divisor = 1
            ADC12SSEL_3      |  //Clock ADC = SMCLK
            ADC12CONSEQ_3;      //Modo Sequência de canais repetido

    ADC12CTL2 = ADC12TCOFF |        // Desligar sensor temperatura
            ADC12RES_2;         // Resolução 12-bit

    ADC12MCTL0 = ADC12SREF_0 |      //VR+ = AVCC e VR- = AVSS
             ADC12INCH_0;       //(P6.0) Canal Y
    ADC12MCTL1 = ADC12EOS    |      //Fim de sequência
             ADC12SREF_0 |      //VR+ = AVCC e VR- = AVSS
             ADC12INCH_1;       //(P6.1) Canal Y

    ADC12IE |= ADC12IE0;
    ADC12IE |= ADC12IE1;


    ADC12CTL0 |= ADC12ENC;          //Habilitar ADC12

    P6SEL |= BIT0;          // Direção X
    P6SEL |= BIT1;          // Direção Y
}

void update_lcd()
{
    lcdCursor(0,4);
    voltX = avX*3.3/4096;
    voltY = avY*3.3/4096;
    lcdFloat1(voltX);
    delay(100);
    lcdCursor(1,4);
    lcdFloat1(voltY);
    delay(100);
    lcdCursor(0,12);
    lcdHex16(avX);
    delay(100);
    lcdCursor(1,12);
    lcdHex16(avY);
    delay(10000);
    P1OUT ^= BIT0;
}

int main(void) {


    WDTCTL = WDTPW | WDTHOLD;
    TB0CTL = TBSSEL_1|MC_2|TBCLR;
    P1DIR |= BIT0;
    P1OUT |= BIT0;
    TB0CCR0 = 16300;
    config_I2C();
    LCD_inic();
    config_adc();
    TA0CTL = TASSEL__ACLK | MC_1;
    TA0CCTL1 = OUTMOD_6;
    TA0CCR0 = 32767;
    TA0CCR1 = TA0CCR0/2;
    ADC12CTL0 |= ADC12ENC;
    lcdChar('A');
    lcdChar('0');
    lcdChar(':');
    lcdCursor(1,0);
    lcdChar('A');
    lcdChar('1');
    lcdChar(':');
    //__enable_interrupt();
    while(1)
    {
        while ((ADC12IFG & ADC12IFG0) == 0);     //Esperar conversão
        while ((ADC12IFG & ADC12IFG1) == 0);
        if(count%512 == 0)
        {
            adcX = ADC12MEM0;
            adcY = ADC12MEM1;
            avX += adcX/8;
            avY += adcY/8;
            ADC12IFG &= ~ADC12IFG0;
            ADC12IFG &= ~ADC12IFG1;

        }
        count++;
        rightcount = count/512;
        if(rightcount == 8)
        {
            update_lcd();
            count = 0;
            avX = 0;
            avY = 0;
        }
    }

    return 0;
}

void delay(long limite){
    volatile long cont=0;
    while (cont++ < limite) ;
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

