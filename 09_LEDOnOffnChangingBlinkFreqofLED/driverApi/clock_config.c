#include "driverApi/clock_config.h"

/* Variable to store current Clock values */
uint32_t clockValue = 0;

/* Target frequency for MCLK = SMCLK = 12MHz and ACLK = 32,768KHz */

void ConfigClockSetting(void)
{
    /* Set VCore = 1 for 12MHz clock */
    PMM_setVCore(PMM_CORE_LEVEL_1);

    /* ACLK, MCLK, MCLK set out to pins */
    // GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P11, GPIO_PIN0 + GPIO_PIN1 + GPIO_PIN2);

    /* Set DCO FLL reference = REFO */
    UCS_initClockSignal(UCS_ACLK, UCS_REFOCLK_SELECT, UCS_CLOCK_DIVIDER_1);

    /* Set ACLK = REFO */
    UCS_initFLLSettle(UCS_MCLK_DESIRED_FREQUENCY_IN_KHZ, UCS_MCLK_FLLREF_RATIO);

    /* Enable global oscillator fault flag */
    SFR_clearInterrupt(SFR_OSCILLATOR_FAULT_INTERRUPT);
    SFR_enableInterrupt(SFR_OSCILLATOR_FAULT_INTERRUPT);

    /* Verify if the Clock settings are as expected */
    clockValue = UCS_getSMCLK();
    clockValue = UCS_getMCLK();
    clockValue = UCS_getACLK();
}

void ConfigTimer0_A0(void)
{
    TA0CCTL0 = CCIE;                        // CCR0 interrupt enabled
    TA0CCR0 = 30000;                        // 10ms Timer : 30000 / 3MHz
    TA0CTL = TASSEL_2 + MC_1 + ID_2         // SMCLK = 12 MHz, upmode , Input divider: input clock / 4 = 3 MHz
}
