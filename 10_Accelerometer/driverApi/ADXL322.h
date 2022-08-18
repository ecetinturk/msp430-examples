#ifndef DRIVERAPI_ADXL322_H_
#define DRIVERAPI_ADXL322_H_

/**************************
 * Libraries
 **************************/
#include "gpio.h"

/**************************
 * Binds
 **************************/
#define                 CALIBRATION_SAMPLES     4096.0
#define                 SAMPLES                 4.0
#define                 SUPPLY_VOLTAGE          3.3
#define                 G_CONSTANT              0.462

/**************************
 * Variables
 **************************/
int                     k;
int                     dataTimeout;
int                     sampleCounter;
int                     sampleCounter2;
volatile float          ADCresults[2];          // ADC results
bool                    dataReadyFlag;

typedef enum {
    ADXL322_OPERATION_SUCCESSFUL = 0;
    ADXL322_DATA_READY_ERROR = -1;
} ADXL322_Error_t;

int8_t CalibrateADXL322(float* biasPtr);
int8_t getVoltage(float* voltOutputPtr);
int8_t getAccelerometer(float* accOutputPtr, float* voltOutputPtr, float* biasPtr);

#endif /* DRIVERAPI_ADXL322_H_ */
