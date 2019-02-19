#ifndef _INA219_CURRENT_SENSOR_H
#define _INA219_CURRENT_SENSOR_H

// Address
#define INA219_CURRENT_SENSOR_I2C_ADDRESS 0x40 // A1: GND, A0: GND
#define INA219_CALIBRATION_SET_CONSTANT     0.04096
#define INA219_CALIBRATION_READ_CONSTANT    4096

#define INA219_CURRENTLSB_FACTOR                32768
#define INA219_BUS_VOLTAGE_CONVERSION_FACTOR    0.004f
#define INA219_SHUNT_VOLTAGE_CONVERSION_FACTOR  0.00001f


// register addresses
#define CUR_INA219_REG_ADDR_CONFIG  0x00
#define CUR_INA219_REG_ADDR_SHUNT_V 0x01
#define CUR_INA219_REG_ADDR_BUS_V   0x02
#define CUR_INA219_REG_ADDR_POWER   0x03
#define CUR_INA219_REG_ADDR_CURRENT 0x04
#define CUR_INA219_REG_ADDR_CALIB   0x05

// configuration register
#define CUR_INA219_REG_CONFIG_RESET_LSB 0x00
#define CUR_INA219_REG_CONFIG_RESET_MSB 0x80

#define CUR_INA219_REG_CONFIG_SADC_LSB 0x78
#define CUR_INA219_REG_CONFIG_SADC_MSB 0x00

#define CUR_INA219_REG_CONFIG_BADC_LSB 0x80
#define CUR_INA219_REG_CONFIG_BADC_MSB 0x07

// Shortcut register values for specific configurations
#define CUR_INA219_CONFIG_MAX_GAIN_MAX_SAMPLING_LSB 0xFF
#define CUR_INA219_CONFIG_MAX_GAIN_MAX_SAMPLING_MSB 0x3F


#endif
