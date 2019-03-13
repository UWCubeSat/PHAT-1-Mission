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

#define GAS_CONSTANT 8.3144598f
#define G_0 9.80665f
#define AIR_MOLAR_MASS 0.0289644f

#define H_1 11000
#define H_2 20000
#define H_3 32000
#define H_4 47000

typedef struct {
    float altitude;
    float pressure;
    float temperature;
} AltimeterData;

void baromInit(bus_instance_i2c bus);
uint16_t readPROMWord(int8_t reg);
void d1Conversion(int8_t bits);
void d2Conversion(int8_t bits);
float readTemperature(int8_t bits);
float readPressure(int8_t bits);
float calculateAltitude();
AltimeterData *readAltitudeData(int8_t bits);
void wait(int ms);

#endif /* DSBASE_SENSORS_BAROMETER_H_ */
