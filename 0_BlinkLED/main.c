#include “msp430.h”

int main(void)  {
    WDT_A_hold(WDT_A_BASE);                             // Stop WDT
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN1);       // P1.1 set output pin
    GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN1);   // LED power on
}
