/*
 * barometer.h
 *
 *  Created on: Jan 8, 2019
 *      Author: Eli R.
 */

#ifndef DSBASE_SENSORS_BAROMETER_H_
#define DSBASE_SENSORS_BAROMETER_H_

#include <sensors/MS5611_Barometer.h>
#include <stdint.h>
#include "core/i2c.h"
#include "core/utils.h"

// Encapsulates factory calibrated coefficients read from sensor PROM
typedef struct {
    uint16_t pSens;     // C1
    uint16_t pOff;      // C2
    uint16_t tcs;       // C3
    uint16_t tco;       // C4
    uint16_t tRef;      // C5
    uint16_t tSens;     // C6
} CalibrationData;

// Encapsulates data retrieved from sensor pertaining to pressure
typedef struct {
    uint32_t dPress;            // Digital Pressure from ADC after D1 conversion

    int64_t offAtTempActual;    // Offset at actual temperature = pOff + tco*dTemp
    int64_t sensAtTempActual;   // Sensitivity at actual temperature = pSens + tcs*tDiff
    int32_t pTempComp;          // Temperature compensated pressure = dPress*pSens - pOff
} BarometerData;

// Encapsulated data retrieved from sensor pertaining to temperature
typedef struct {
    uint32_t dTemp;             // Digital Temperature from ADC after D2 conversion

    int32_t tDiff;              // Difference between actual and ref temp = dTemp - tRef
    int32_t tActual;            // Actual temperature = 20 (degrees C) + tDiff*tSens
} ThermometerData;

void baromInit(bus_instance_i2c bus);
uint16_t readPROMWord(int reg);
void d1Conversion(int bits);
void d2Conversion(int bits);
uint32_t readPressureData(int bits);
uint32_t readTemperatureData(int bits);
float calculatedTempCompensatedPressure(BarometerData, ThermometerData);
float calculateActualTemperature();

#endif /* DSBASE_SENSORS_BAROMETER_H_ */
