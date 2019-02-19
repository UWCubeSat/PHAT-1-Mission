/*
 * MS5611_barometer.h
 *
 *  Created on: Jan 8, 2019
 *      Author: Eli R.
 */

#ifndef DSBASE_SENSORS_MS5611_BAROMETER_H_
#define DSBASE_SENSORS_MS5611_BAROMETER_H_

// I2C 7 bit address - complement of CSB pin (tied LOW - 1, tied HIGH - 0)
#define MS5611_I2C_7BIT_ADDRESS             0x77

// Reset Sequence
#define MS5611_RESET                        0x1E

// PROM Read Sequences
#define MS5611_C1_REG_ADDR                  0xA2    // Pressure Sensitivity
//#define MS5611_C2_REG_ADDR                 0xA2    // Pressure Offset
//#define MS5611_C3_REG_ADDR                 0xA4    // Temperature Coefficient of Pressure Sensitivity
//#define MS5611_C4_REG_ADDR                 0xA6    // Temperature Coefficient of Pressure Offset
//#define MS5611_C5_REG_ADDR                 0xA8    // Reference Temperature
//#define MS5611_C6_REG_ADDR                 0xAE    // Temperature Coefficient of the Temperature

// Pressure (D1) Conversion Sequences
#define MS5611_D1_OSR256                    0x40
#define MS5611_D1_OSR512                    0x42
#define MS5611_D1_OSR1024                   0x44
#define MS5611_D1_OSR2048                   0x46
#define MS5611_D1_OSR4096                   0x48

// Temperature (D2) Conversion Sequences
#define MS5611_D2_OSR256                    0x50
#define MS5611_D2_OSR512                    0x52
#define MS5611_D2_OSR1024                   0x54
#define MS5611_D2_OSR2048                   0x56
#define MS5611_D2_OSR4096                   0x58


typedef enum {
    Vendor  = 0,
    C1      = 1,
    C2      = 2,
    C3      = 3,
    C4      = 4,
    C5      = 5,
    C6      = 6,
    CRC     = 7
} PROMData;

// ADC registry address
#define MS5611_ADC_ADDR                 0x00    // After conversion sequence has ended

#define COMPENSATED_PRESSURE_CONVERSION_FACTOR      0.01
#define ACTUAL_TEMPERATURE_CONVERSION_FACTOR        0.01
#endif /* DSBASE_SENSORS_MS5611_BAROMETER_H_ */
