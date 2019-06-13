#include <msp430.h> 
int adc;
void config_adc(void)
{   ADC12CTL0 &= ~ADC12ENC; //Desabilitar para configurar
    ADC12CTL0 = ADC12SHT0_3 | //ADC tempo amostragem ADCMEM[0-7]
                ADC12ON;

    ADC12CTL1 = ADC12CSTARTADD_0 |
            ADC12SHS_0|
            ADC12SHP|
            ADC12DIV_0|
            ADC12SSEL_3|
            ADC12CONSEQ_0;
    ADC12CTL2 = ADC12TCOFF | // Desligar sensor temperatura
            ADC12RES_2; // Resolução 12-bit
    ADC12MCTL0 = ADC12EOS| //Fim de sequência
            ADC12SREF_0| //VR+ = AVCC e VR- = AVSS
            ADC12INCH_0;
    P6SEL |= BIT0;
    ADC12CTL0 |= ADC12ENC;
}
/**
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	config_adc();
	while(1)
	{
	    ADC12CTL0 &= ~ADC12SC;
	    ADC12CTL0 |= ADC12SC;
	    ADC12CTL0 &= ~ADC12SC;
	    while ( (ADC12IFG&ADC12IFG0) == 0);
	    adc = ADC12MEM0;
	}
	return 0;
}
