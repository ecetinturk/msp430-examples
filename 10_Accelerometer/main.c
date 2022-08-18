/*******************************
 * Include Headers
 *******************************/
#include "driverlib.h"
#include "driverApi/adc12.h"
#include "driverApi/clock_config.h"
#include "driverApi/ADXL322.h"


/*******************************
 * Global Variables
 *******************************/
uint16_t            status;                                                     // Variable to store status of Oscillator fault flags
float               fBias[2] = {0x00, 0x00};                                    // Bias
float               voltOutput[2] = {0x00, 0x00};                               // Voltage output
float               accOutput[2] = {0x00, 0x00};                                // Accelerometer output
int32_t             systemTimeTick = 0;

int main(void)
{
    WDT_A_hold(WDT_A_BASE);                                                     // Stop WDT

    ConfigClockSetting();                                                       // Target frequency for SMCLK = 12MHz

    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN1);                               // P1.1 set output pin for LED2
    GPIO_setAsOutputPin(GPIO_PORT_P6, GPIO_PIN0);                               // P6.0 set output pin for accelerometer
    GPIO_setOutputHighOnPin(GPIO_PORT_P6, GPIO_PIN0);                           // Enable accelerometer

    ConfigTimer0_A0();                                                          // Timer configuration
    ConfigADC12();                                                              // Analog-to-Digital Converter configuration

    __bis_SR_register(LPM4_bits + GIE);                                         // Enter LPM4 + Global interrupt enable
    CalibrateADCXL322(fBias);                                                   // Calibrate sensor

    for(;;)
    {
        while(ADC12CTL1 & ADC12BUSY);
        ADC12CTL0 |= ADC12SC;                                                   // Start conversion - software trigger

        getVoltage(voltOutput);
        getAccelerometer(accOutput, voltOutput, fBias);
    }
}

#pragma vector = UNMI_VECTOR                                                    // UNMI_VECTOR interrupt vector service routine
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

#pragma vector = TIMER0_A0_VECTOR                                               // TIMER0_A0_VECTOR interrupt vector service routine
__interrupt void Timer0_A0(void)
{
    __bic_SR_register_on_exit(LPM4_bits);                                       // Exit active CPU, SET BREAKPOINT HERE
    ADC12CTL0 |= ADC12SC;                                                       // Start conversion - software trigger
    systemTimeTick++;
    if (systemTimeTick % 100 == 0)                                              // Every 1s
    {
        GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN1);                        // Toggle LED ON-OFF
    }
}

#pragma vector = ADC12_VECTOR                                                               // PORT2_VECTOR interrupt vector service routine
__interrupt void ADC12ISR(void)
{
    switch(__even_in_range(ADC12IV,34))
    {
    case  0: break;                                                                         // Vector 0: No interrupt
    case  2: break;                                                                         // Vector 2: ADC overflow
    case  4: break;                                                                         // Vector 4: ADC timing overflow
    case  6: break;                                                                         // Vector 6: ADC12IFG0
    case  8: break;                                                                         // Vector 8: ADC12IFG1
    case  10:                                                                               // Vector 10: ADC12IFG2
        ADCresults[0] = ADC12MEM1;                                              // Move results, IFG is cleared
        ADCresults[1] = ADC12MEM2;                                              // Move results, IFG is cleared
        dataReadyFlag = true;
        break;
    case  12: break;                                                                        // Vector 12: ADC12IFG3
    case  14: break;                                                                        // Vector 12: ADC12IFG4
    case  16: break;                                                                        // Vector 12: ADC12IFG5
    case  18: break;                                                                        // Vector 12: ADC12IFG6
    case  20: break;                                                                        // Vector 12: ADC12IFG7
    case  22: break;                                                                        // Vector 12: ADC12IFG8
    case  24: break;                                                                        // Vector 12: ADC12IFG9
    case  26: break;                                                                        // Vector 12: ADC12IFG10
    case  28: break;                                                                        // Vector 12: ADC12IFG11
    case  30: break;                                                                        // Vector 12: ADC12IFG12
    case  32: break;                                                                        // Vector 12: ADC12IFG13
    case  34: break;                                                                        // Vector 12: ADC12IFG14
    default: break;                                                                         // Default
    }
}
