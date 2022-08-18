#include “driverlib.h”
int main(void)  {
    WDT_A_hold(WDT_A_BASE);                                                     // Stop WDT
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN1);                               // P1.1 set output pin
    GPIO_setAsInputPin(GPIO_PORT_P2, GPIO_PIN6);                                // P2.6 set input pin
    GPIO_setAsInputPin(GPIO_PORT_P2, GPIO_PIN7);                                // P2.7 set input pin
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P2, GPIO_PIN6);
    // Enable P2.6 internal resistance as pull-up resistance for push button
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P2, GPIO_PIN7);
    // Enable P2.7 internal resistance as pull-up resistance for push button

    GPIO_enableInterrupt(GPIO_PORT_P2, GPIO_PIN6);                              // P2.6 interrupt enabled
    GPIO_enableInterrupt(GPIO_PORT_P2, GPIO_PIN7);                              // P2.7 interrupt enabled
    GPIO_clearInterrupt(GPIO_PORT_P2, GPIO_PIN6);                               // P2.6 IFG cleared
    GPIO_clearInterrupt(GPIO_PORT_P2, GPIO_PIN7);                               // P2.7 IFG cleared
    GPIO_selectInterruptEdge(GPIO_PORT_P2, GPIO_PIN6, 1);
    GPIO_selectInterruptEdge(GPIO_PORT_P2, GPIO_PIN7, 1);

    __bis_sr_register(LPM4_bits + GIE);                                         // Enter LPM4, Enable global interrupt

    while(1)                                                                    // Endless loop
    {

    }
}

#pragma vector = PORT2_VECTOR                                                   // PORT2 vector service routine
__interrupt void Port_2(void)
{
    if (GPIO_getInputPinValue(GPIO_PORT_P2, GPIO_PIN6) == 0)
    {
        P1OUT |= BIT1;                                                          // P1.1 LED on
        P2IFG &= ~BIT6;                                                         // P2.6 IFG cleared
    }

    else if (GPIO_getInputPinValue(GPIO_PORT_P2, GPIO_PIN7) == 0)
    {
        P1OUT &= ~BIT1;                                                         // P1.1 LED off
        P2IFG &= ~BIT7;                                                         // P2.7 IFG cleared
    }
}
