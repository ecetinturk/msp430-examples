#include “driverlib.h”
#include “msp430.h”
int main(void)  {
    WDTCTL = WDTPW + WDTHOLD;                   // Stop WDT

    P1DIR = BIT1;                               // P1.1 set output pin
    P1OUT &= ~BIT1;                             // P1.1 LED off

    P2IN = BIT7;                                // P2.7 set output pin
    P2REN |= BIT7;                              // P2.7 enable pull up/down
    P2OUT = BIT7;                               // P2.7 enable pull-up resistance for switch button

    P2IE |= BIT7;                               // P2.7 interrupt enabled
    P2IES |= BIT7;                              // P2.7 Hi/lo falling edge
    P2IFG &= ~BIT7;                             // P2.7 IFG cleared

    __bis_sr_register(LPM4_bits + GIE);         // Enter LPM4, Enable global interrupt

    while(1)                                    // Endless loop
    {

    }
}

#pragma vector = PORT2_VECTOR                   // PORT2 vector service routine
__interrupt void Port_2(void)
{
    P1OUT ^= BIT1;                              // P1.1 LED Toggle
    P2IFG &= ~BIT7;                             // P2.7 IFG cleared
}
