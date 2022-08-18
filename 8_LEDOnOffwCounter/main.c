#include “driverlib.h”

int main(void)  {
    WDT_A_hold(WDT_A_BASE);                                                     // Stop WDT
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN1);                               // P1.1 set output pin
    GPIO_setAsInputPin(GPIO_PORT_P2, GPIO_PIN7);                                // P2.7 set input pin
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P2, GPIO_PIN7);
    // Enable P2.7 internal resistance as pull-up resistance for push button

    while(1)                                                                    // Endless loop
    {
        // The value of 503608 is calculated from stop watch.
        // It is almost equal ~6.1 seconds.
        uint32_t counter = 503608;

        while (counter > 0)
        {
            counter--;
            if (counter == 0)
            {
                GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN1);                // Turn the LED off.
            }
        }

        while (counter < 503608)
        {
            counter++;
            if (counter == 503608)
            {
                GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN1);               // Turn the LED on.
            }
        }
    }
}
