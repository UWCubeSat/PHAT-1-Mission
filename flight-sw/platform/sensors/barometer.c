/*
 * barometer.c
 *
 *  Created on: Jan 8, 2019
 *      Author: Eli R.
 */

#include "sensors/barometer.h"
#include "core/timer.h"

#define MAX_BUFF_SIZE 0x04
#define PROM_WORD_MAX_VALUE 65535

FILE_STATIC uint8_t i2cBuff[MAX_BUFF_SIZE];
FILE_STATIC hDev hBarom;
FILE_STATIC BarometerData baromData;
FILE_STATIC ThermometerData tempData;
FILE_STATIC uint16_t prom[8];

void baromInit(bus_instance_i2c bus)
{
    i2cEnable(bus);
    hBarom = i2cInit(bus, MS5611_I2C_7BIT_ADDRESS);
    // send RESET sequence
    i2cBuff[0] = MS5611_RESET;
    i2cMasterWrite(hBarom, i2cBuff, 1);

    // read calibration coefficients from PROM (do something with CRC/vendor data?)
    int8_t reg;
    for (reg = 7; reg >= 0; reg--) {
        prom[reg] = readPROMWord(reg);
    }
}

uint16_t readPROMWord(int reg)
{
    uint16_t word = 0;
    reg = reg * 2;
    uint8_t addr = MS5611_C1_REG_ADDR + reg;
    i2cMasterRegisterRead(hBarom, addr, i2cBuff, 2);
    word = (uint16_t)(i2cBuff[1]) | ((uint16_t)(i2cBuff[0]) << 8);   // MSB first
    return word;
}

/*
 * Initiates the D1 conversion sequence for the MS5611, which populates 0x00 registry value
 * (ADC yield) with raw digital pressure value.
 * int bits: sensitivity of conversion, between 8 and 12
 */
void d1Conversion(int bits)
{
    if (bits > 12 || bits < 8) {
        // needs some kind of error reporting here, not sure where that would happen
        printf("Poorly formed input");
        return;
    }

    int offset = (bits - 8) * 2;
    uint8_t addr = MS5611_D1_OSR256 + offset;
    i2cBuff[0] = addr;
    i2cMasterWrite(hBarom, i2cBuff, 1);
    return;
}

/*
 * Initiates the D2 conversion sequence for the MS5611, which populates 0x00 registry value
 * (ADC yield) with raw digital temperature value.
 * int bits: sensitivity of conversion, between 8 and 12
 */
void d2Conversion(int bits)
{
    if (bits > 12 || bits < 8) {
        // needs some kind of error reporting here, not sure where that would happen
        printf("Poorly formed input");
        return;
    }

    int offset = (bits - 8) * 2;
    uint8_t addr = MS5611_D2_OSR256 + offset;
    i2cBuff[0] = addr;
    i2cMasterWrite(hBarom, i2cBuff, 1);
    return;
}

uint *readPressureData(int bits)
{
    BarometerData *bData = &baromData;
    d1Conversion(bits);
    TIMER_HANDLE tHandle = timerPollInitializer(10);
    while(!checkTimer(tHandle)) {}
    // read from ADC register
    i2cMasterRegisterRead(hBarom, MS5611_ADC_ADDR, i2cBuff, 2);
    bData->
    return bData;
}

ThermometerData *readTemperatureData(int bits)
{
    ThermometerData *tData = &tempData;
    d2Conversion(bits);
    return tData;
}
