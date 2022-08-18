/*******************************
 * Include Headers
 *******************************/
#include "driverApi/clock_config.h"
#include "driverlib.h"

/*******************************
 * Global Variables
 *******************************/
uint16_t            status;
unsigned long       systemTimeTick = 0;
unsigned long       systemCurrentTimeTick = 0;
unsigned long       currentTimeTick = 300;
unsigned long       tempTimeTick = 0;
unsigned long       i = 0;
bool                buttonPressedFlag1 = false;
bool                buttonPressedFlag2 = false;
bool                FlagChecker = false;

void main(void)
{
    WDT_A_hold(WDT_A_BASE);                                                     // Stop WDT

    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN1);                               // P1.1 set output pin
    GPIO_setAsInputPin(GPIO_PORT_P2, GPIO_PIN6);                                // P2.6 set input pin
    GPIO_setAsInputPin(GPIO_PORT_P2, GPIO_PIN7);                                // P2.7 set input pin
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P2, GPIO_PIN6);
    // Enable P2.6 internal resistance as pull-up resistance for push button
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P2, GPIO_PIN7);
    // Enable P2.7 internal resistance as pull-up resistance for push button
    GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN1);                           // P1.1 set LED power on

    GPIO_enableInterrupt(GPIO_PORT_P2, GPIO_PIN6, GPIO_PIN7);                   // P2.6 & P2.7 interrupt enabled
    GPIO_clearInterrupt(GPIO_PORT_P2, GPIO_PIN6, GPIO_PIN7);                    // P2.6 & P2.7 IFG cleared

    ConfigClockSetting();                                                       // Target frequency for SMCLK = 12MHz
    ConfigTimer0_A0();                                                          // Timer configuration

    __bis_SR_register(GIE);                                                     // Enable global interrupt

    while(1)                                                                    // Endless loop
    {
        for (i=0; i<100; i++)                                                   // Wait ~100ms for debouncing
        {
            __delay_cycles(12000);                                              // Wait ~1ms because SMCLK = 12MHz
        }

        // When the LED is on, pressing the P2.7 for more than 5 seconds turns off the LED.

        systemCurrentTimeTick = systemTimeTick;

        while (FlagChecker == false && (GPIO_getInputPinValue(GPIO_PORT_P2, GPIO_PIN7) == GPIO_INPUT_PIN_LOW)) {}

        if (FlagChecker == false && (systemTimeTick - systemCurrentTimeTick > 500))
        {
            P1OUT &= ~BIT1;
            systemCurrentTimeTick = 0;
            FlagChecker = true;
            GPIO_disableInterrupt(GPIO_PORT_P2, GPIO_PIN6 + GPIO_PIN7);
        }

        // When the LED is off, pressing the P2.7 for more than 5 seconds turns on the LED.

        systemCurrentTimeTick = systemTimeTick;

        while (FlagChecker == true && (GPIO_getInputPinValue(GPIO_PORT_P2, GPIO_PIN7) == GPIO_INPUT_PIN_LOW)) {}

        if (FlagChecker == true && (systemTimeTick - systemCurrentTimeTick > 500))
        {
            systemCurrentTimeTick = 0;
            FlagChecker = true;
            GPIO_enableInterrupt(GPIO_PORT_P2, GPIO_PIN6 + GPIO_PIN7);
        }

        // Set the buttonPressedFlag1 to false when the P2.6 is not pressed.
        if (GPIO_getInputPinValue(GPIO_PORT_P2, GPIO_PIN6) == GPIO_INPUT_PIN_HIGH)
        {
            buttonPressedFlag1 = false;
        }

        // Set the buttonPressedFlag2 to false when the P2.7 is not pressed.
        if (GPIO_getInputPinValue(GPIO_PORT_P2, GPIO_PIN7) == GPIO_INPUT_PIN_HIGH)
        {
            buttonPressedFlag2 = false;
        }
    }
}

#pragma vector = UNMI_VECTOR                                                               // UNMI_VECTOR interrupt vector service routine
__interrupt void NMI_ISR(void)
{
    do
    {
        // If it still can't clear the oscillator fault flags after the timeout,
        // trap and wait here.
        status = UCS_clearAllOscFlagsWithTimeout(1000);
    }
    while (status != 0);
}

#pragma vector = TIMER0_A0_VECTOR                                                           // TIMER0_A0_VECTOR interrupt vector service routine
__interrupt void Timer0_A0(void)
{
    systemTimeTick++;
    if (systemTimeTick % currentTimeTick == 0)
    {
        if (FlagChecker == false)
        {
            GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN1);
        }
    }
}

#pragma vector = PORT2_VECTOR                                                               // PORT2_VECTOR interrupt vector service routine
__interrupt void Port_2(void)
{
    switch(__even_in_range(P2IV,16))
    {
    case  0: break;                                                                         // Vector 0: No interrupt
    case  2: break;                                                                         // Vector 2: P2.0
    case  4: break;                                                                         // Vector 4: P2.1
    case  6: break;                                                                         // Vector 6: P2.2
    case  8: break;                                                                         // Vector 8: P2.3
    case  10: break;                                                                        // Vector 10: P2.4
    case  12: break;                                                                        // Vector 12: P2.5
    case 14:                                                                                // Vector 14: P2.6
        if (currentTimeTick > 100 && buttonPressedFlag1 != true)                            // If currentTimeTick > 100 and P2.6 is pressed
        {
            currentTimeTick -= 100;                                                         // Do currentTimeTick - 100
        }
        buttonPressedFlag1 = true;                                                          // Make buttonPressedFlag1 true
        __delay_cycles(12000);
        GPIO_clearInterrupt(GPIO_PORT_P2, GPIO_PIN6);                                       // P2.6 IFG cleared
        break;
    case 16:                                                                                // Vector 16: P2.7
        if (currentTimeTick < 600 && buttonPressedFlag2 != true)                            // If currentTimeTick < 600 and P2.7 is pressed
        {
            currentTimeTick += 100;                                                         // Do currentTimeTick + 100
        }
        buttonPressedFlag2 = true;                                                          // Make buttonPressedFlag2 true
        __delay_cycles(12000);
        GPIO_clearInterrupt(GPIO_PORT_P2, GPIO_PIN7);                                       // P2.7 IFG cleared
        break;
    default: break;                                                                         // Default
    }
}
