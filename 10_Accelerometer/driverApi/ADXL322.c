#ifndef DRIVERAPI_ADXL322_C_
#define DRIVERAPI_ADXL322_C_

/**************************
 * Libraries
 **************************/
#include "driverApi/ADXL322.h"

int8_t CalibrateADXL322(float* biasPtr)
{
    uint32_t        dataTimeout = 500;
    float           a_Accumulator[2];

    sampleCounter = 0;

    a_Accumulator[0] = 0;                       // a_xAccumulator
    a_Accumulator[1] = 0;                       // a_yAccumulator

    // Accumulate a certain number of samples.
    for (sampleCounter = 0; sampleCounter < CALIBRATION_SAMPLES; sampleCounter++)
    {
        while((dataReadyFlag != true) && (--dataTimeout))
        {
            __delay_cycles(12000);              // ~1ms because MCLK = 12MHz
        }

        if(dataTimeout == 0)
        {
            return ADXL322_DATA_READY_ERROR;
        }

        a_Accumulator[0] += (uint16_t) ADCresults[0];
        a_Accumulator[1] += (uint16_t) ADCresults[1];

        __delay_cycles(10);
    }

    // Average samples
    a_Accumulator[0] /= CALIBRATION_SAMPLES;
    a_Accumulator[1] /= CALIBRATION_SAMPLES;

    // These are our zero g offsets.
    biasPtr[0] = a_Accumulator[0];              // a_xBias
    biasPtr[1] = a_Accumulator[1];              // a_yBias

    // Reset accumulators and readings
    a_Accumulator[0] = 0;                       // a_xAccumulator
    a_Accumulator[1] = 0;                       // a_yAccumulator

    return ADXL322_OPERATION_SUCCESSFUL;
}

int8_t getVoltage(float* voltOutputPtr)
{
    voltOutputPtr[0] = ((((uint16_t) ADCresults[0]) * ((float)SUPPLY_VOLTAGE)) / 4096);                             // Voltage results in x-direction
    voltOutputPtr[1] = ((((uint16_t) ADCresults[1]) * ((float)SUPPLY_VOLTAGE)) / 4096);                             // Voltage results in y-direction

    return ADXL322_OPERATION_SUCCESSFUL;
}

int8_t getAccelerometer(float* accOutputPtr, float* voltOutputPtr, float* biasPtr)
{
    accOutputPtr[0] = (voltOutputPtr[0] - (biasPtr[0] * (((float)SUPPLY_VOLTAGE / 4096))) / G_CONSTANT);            // Acceleration in x-direction in g units.
    accOutputPtr[1] = (voltOutputPtr[1] - (biasPtr[1] * (((float)SUPPLY_VOLTAGE / 4096))) / G_CONSTANT);            // Acceleration in y-direction in g units.

    return ADXL322_OPERATION_SUCCESSFUL;
}

#endif /* DRIVERAPI_ADXL322_C_ */
