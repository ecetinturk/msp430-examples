#include “driverlib.h”

int main(void)  {
    WDT_A_hold(WDT_A_BASE);                                                             // Stop WDT
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN1);                                       // P1.1 set output pin
    // GPIO_setAsInputPin(GPIO_PORT_P2, GPIO_PIN7);                                     // P2.7 set input pin
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P2, GPIO_PIN7);
    // Enable P2.7 internal resistance as pull-up resistance for push button
    GPIO_selectInterruptEdge(GPIO_PORT_P1, GPIO_PIN1, GPIO_LOW_TO_HIGH_TRANSITION);     // P1.1 Lo/Hi edge
    GPIO_clearInterrupt(GPIO_PORT_P1, GPIO_PIN1);                                       // P1.1 IFG cleared

    while(1)
    {
        if (GPIO_getInterruptStatus(GPIO_PORT_P2, GPIO_PIN7))
        {
            GPIO_clearInterrupt(GPIO_PORT_P2, GPIO_PIN7);                               // P2.7 IFG cleared
            GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN1);                            // Toggle LED On/Off
        }
    }
}
