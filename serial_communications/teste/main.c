// SHS=1 --> Disparar por TA0.1
// CONSEQ=3 --> Repetir Sequência de Canais (0, 1, 2 e 3)

#include <msp430.h> 

#define TRUE    1
#define FALSE   0

void config_timer(void);
void config_adc(void);
//void ADCx(void);
//void ADCy(void);

volatile float x = 0, y =0;
volatile float Vx = 0, Vy = 0;
volatile int cont = 0;
volatile float Xt = 0, Yt = 0;
volatile float Xf = 0, Yf = 0;

int main(void){
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    config_timer();
    config_adc();

    __enable_interrupt();

    ADC12CTL0 |= ADC12ENC;          //Habilitar ADC12
    while (TRUE){
        //while ((ADC12IFG & ADC12IFG0) == 0);     //Esperar conversão
        //while ((ADC12IFG & ADC12IFG1) == 0);     //Esperar conversão
        //x = ADC12MEM0;            //Ler resultado
        //y = ADC12MEM1;            //Ler resultado

        Vx = x/1239,4;
        Vy = y/1239,4;

        Xt += Vx;
        Yt += Vy;

        if ( cont == 8 ){
            Xf = Xt/8;
            Yf = Yt/8;

            Xt = 0;
            Yt = 0;
            cont = 0;
        }

        cont++;
    }
    //return 0;
}

#pragma vector = 54
__interrupt void ADC(void){
//    int n;
//    n = __even_in_range(ADC12IV, 0x10);
//    switch(n){
//        case 0x0 : break;                case 0x2: break;
//        case 0x4 : break;                case 0x6: ADCx();break;
//        case 0x8 : ADCy();break;         case 0xA: break;
//        case 0xC : break;                case 0xE: break;
//        case 0x10: break;
//    }

    x = ADC12MEM0;            //Ler resultado
    y = ADC12MEM1;            //Ler resultado

    ADC12IFG &= ~ADC12IFG0;
    ADC12IFG &= ~ADC12IFG1;
}

//void ADCx(void){

//    x = ADC12MEM0;            //Ler resultado

//}

//void ADCy(void){

//    y = ADC12MEM1;            //Ler resultado
//}

void config_adc(void){
    ADC12CTL0  &= ~ADC12ENC;        //Desabilitar para configurar

    ADC12CTL0 = ADC12SHT0_3 |       //ADC tempo amostragem ADCMEM[0-7]
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
                 ADC12INCH_0;       //(P6.0) A0 = Canal 0
    ADC12MCTL1 = ADC12EOS    |      //Fim de sequência
                 ADC12SREF_0 |      //VR+ = AVCC e VR- = AVSS
                 ADC12INCH_1;       //(P6.0) A0 = Canal 0

    ADC12IE |= ADC12IE0;
    ADC12IE |= ADC12IE1;


    ADC12CTL0 |= ADC12ENC;          //Habilitar ADC12

    P6SEL |= BIT0;          // Direção X
    P6SEL |= BIT1;          // Direção Y
}

void config_timer(void){
    TA0CTL = TASSEL__ACLK | MC_1;
    TA0CCTL1 = OUTMOD_6;
    TA0CCR0 = 32768;         // Período de 2s
    TA0CCR1 = TA0CCR0/32;    // 16Hz (32 batidas em 2s)
}
