/*
 * barometer.c
 *
 *  Created on: Jan 8, 2019
 *      Author: Eli R.
 */

#include "sensors/barometer.h"
#include "core/timer.h"
#include <math.h>

#define MAX_BUFF_SIZE 3
#define PROM_WORD_MAX_VALUE 65535
#define MAX_DIGITAL_VALUES 16777216

FILE_STATIC AltimeterData altData;
FILE_STATIC uint8_t i2cBuff[MAX_BUFF_SIZE];
FILE_STATIC hDev hBarom;
FILE_STATIC int32_t tDiff;
FILE_STATIC int32_t tActual;
FILE_STATIC uint16_t prom[8];
FILE_STATIC lastAltitude;

void baromInit(bus_instance_i2c bus)
{
    i2cEnable(bus);
    hBarom = i2cInit(bus, MS5611_I2C_7BIT_ADDRESS);
    // send RESET sequence
    i2cBuff[0] = MS5611_RESET;
    i2cMasterWrite(hBarom, i2cBuff, 1);
    initializeTimer();
    wait(3);
    lastAltitude = 0;

    // read calibration coefficients from PROM (do something with CRC/vendor data?)
    int8_t reg;
    for (reg = 0; reg < 8; reg++) {
        prom[reg] = readPROMWord(reg);
    }
}

/*
 * Reads a single 16-bit calibration coefficient from the sensor's PROM.
 * int reg: decimal 
 */
uint16_t readPROMWord(int8_t reg)
{
    uint16_t word = 0;
    reg = reg * 2;
    uint8_t addr = MS5611_PROM_START_ADDR + reg;
    i2cMasterRegisterRead(hBarom, addr, i2cBuff, 2);
    word = (uint16_t)(i2cBuff[1]) | ((uint16_t)(i2cBuff[0]) << 8);   // MSB first
    return word;
}

/*
 * Initiates the D1 conversion sequence for the MS5611, which populates 0x00 registry value
 * (ADC yield) with raw digital pressure value.
 * int bits: sensitivity of conversion, between 8 and 12
 */
void d1Conversion(int8_t bits)
{
    int8_t offset = (bits - 8) * 2;
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
void d2Conversion(int8_t bits)
{
    int8_t offset = (bits - 8) * 2;
    uint8_t addr = MS5611_D2_OSR256 + offset;
    i2cBuff[0] = addr;
    i2cMasterWrite(hBarom, i2cBuff, 1);
    return;
}

float readTemperature(int8_t bits)
{
    d2Conversion(bits);
    wait(10);
    // read from ADC register
    i2cMasterRegisterRead(hBarom, MS5611_ADC_ADDR, i2cBuff, 3);
    uint32_t digTemperature = ((uint32_t)(i2cBuff[2]) | ((uint32_t)(i2cBuff[1]) << 8)) | ((uint32_t)(i2cBuff[0]) << 16);

    tDiff = digTemperature - prom[C5] * 256;
    tActual = 2000 + (tDiff * prom[C6]) / 8388608;
    return (float)tActual / 100;
}

float readPressure(int8_t bits)
{
    d1Conversion(bits);
    wait(10);
    // read from ADC register
    i2cMasterRegisterRead(hBarom, MS5611_ADC_ADDR, i2cBuff, 3);
    uint32_t digPressure = ((uint32_t)(i2cBuff[2]) | ((uint32_t)(i2cBuff[1]) << 8)) | ((uint32_t)(i2cBuff[0]) << 16);

    int32_t tDiff2 = 0;
    int64_t off2 = 0;
    int64_t sens2 = 0;
    if (tActual < 2000)
    {
        tDiff2 = pow(tDiff, 2) / pow(2, 31);
        off2 = 5 * pow((tActual - 2000), 2) / 2;
        sens2 = 5 * pow((tActual - 2000), 2) / 4;
        if (tActual < -1500)
        {
            off2 = off2 + 7 * pow((tActual + 1500), 2);
            sens2 = sens2 + (11 * pow((tActual + 1500), 2) / 2);
        }
    }
    tActual = tActual - tDiff2;
    int64_t pOff = prom[C2] * 65536 + (prom[C4] * tDiff) / 128 - off2;
    int64_t pSens = prom[C1] * 32768 + (prom[C3] * tDiff) / 256 - sens2;
    int32_t pActual = (digPressure * pSens / 2097152 - pOff) / 65536;
    return (float)pActual;
}

float calculateAltitude()
{
    float staticPressure;
    float standardTemp;
    float lapseRate;
    float altitude;
    float layerHeight;

    if (lastAltitude < H_4 && lastAltitude >= H_3)
    {
        staticPressure = 868.02f;
        standardTemp = 228.65f;
        lapseRate = 0.0028f;
        layerHeight = H_3;
    }
    else if (lastAltitude < H_3 && lastAltitude >= H_2)
    {
        staticPressure = 5474.89f;
        standardTemp = 216.65f;
        lapseRate = 0.001f;
        layerHeight = H_2;
    }
    else if (lastAltitude < H_2 && lastAltitude >= H_1)
    {
        staticPressure = 22632.10f;
        standardTemp = 216.65f;
        lapseRate = 0.0f;
        layerHeight = H_1;
    }
    else if (lastAltitude < H_1 && lastAltitude >= 0)
    {
        staticPressure = 101325.00f;
        standardTemp = 288.15f;
        lapseRate = 0.0065f;
        layerHeight = 0;
    }

    float exponent =  (GAS_CONSTANT * lapseRate) / (G_0 * AIR_MOLAR_MASS);
    altitude = (standardTemp / lapseRate) * (1 - pow((altData.pressure / staticPressure), exponent)) + layerHeight;
    lastAltitude = altitude;
    return altitude;
}

AltimeterData *readAltitudeData(int8_t bits)
{
    AltimeterData *aData = &altData;
    aData->temperature = readTemperature(bits);
    aData->pressure = readPressure(bits);
    aData->altitude = calculateAltitude();
    return aData;
}

/*
 * Waits 'ms' milliseconds using our timer library.
 * int ms: time to wait
 */
void wait(int ms)
{
	TIMER_HANDLE tHandle = timerPollInitializer(10);
    while(!checkTimer(tHandle)) {}
    endPollingTimer(tHandle);
    return;
}
