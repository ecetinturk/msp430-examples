/**************************
 * Include Headers
 **************************/
#include "driverApi/adc12.h"

void ConfigADC12(void)
{
    P6SEL = 0x06;                                       // Enable A/D channel inputs

    ADC12CTL0 = ADC12ON + ADC12MSC + ADC12SHT0_2;       // Turn on ADC12, set sampling time
    ADC12CTL1 = ADC12SHP + ADC12CONSEQ_1;               // Use sampling timer, single sequence
    ADC12MCTL1 = ADC12INCH_1;                           // ref+=AVcc, channel = A1
    ADC12MCTL2 = ADC12INCH_2 + ADC12EOS;                // ref+=AVcc, channel = A2
    ADC12IE = ADC12IFG2;                                // Enable ADC12IFG.2
    ADC12CTL0 |= ADC12ENC;                              // Enable conversions
}
