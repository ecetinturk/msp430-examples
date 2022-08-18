#ifndef DRIVERAPI_CLOCK_CONFIG_H_
#define DRIVERAPI_CLOCK_CONFIG_H_

// #include "driverlib.h"
#include "ucs.h"
#include "sfr.h"
#include "gpio.h"
#include "pmm.h"

/* Target frequency for MCLK in kHz */
#define UCS_MCLK_DESIRED_FREQUENCY_IN_KHZ       12000

/* MCLK/FLLRef Ratio */
#define UCS_MCLK_FLLREF_RATIO       366

void ConfigClockSetting(void);
void ConfigTimer0_A0(void);


#endif /* DRIVERAPI_CLOCK_CONFIG_H_ */
