#include “driverlib.h”

int main(void)  {
    WDT_A_hold(WDT_A_BASE);                                             // Stop WDT
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN1);                       // P1.1 set output pin
    GPIO_setAsInputPin(GPIO_PORT_P2, GPIO_PIN7);                        // P2.7 set input pin
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P2, GPIO_PIN7);
    // Enable P2.7 internal resistance as pull-up resistance for push button

    while(1)    {
        GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN1);                // LED power off
        __delay_cycles(3400000);                                        // Delay cycle without clock settings
        GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN1);               // LED power on
        __delay_cycles(3400000);                                        // Delay cycle without clock settings
    }
}
