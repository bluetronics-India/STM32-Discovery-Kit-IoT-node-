#include "HTS221_util.h"
#include "stm32l4xx_hal.h"

#define HTS221_WRITE_ADDRESS 0xBE
#define HTS221_READ_ADDRESS 0xBF
#define WHO_AM_I 0x0F
#define AV_CONF 0x10
#define CTRL_REG1 0x20
#define CTRL_REG2 0x21
#define CTRL_REG3 0x22
#define STATUS_REG 0x27
#define HUMIDITY_OUT_L 0x28
#define HUMIDITY_OUT_H 0x29
#define TEMP_OUT_L 0x2A
#define TEMP_OUT_H 0x2B
//Calibration Registers
#define H0_rH_x2 0x30
#define H1_rH_x2 0x31
#define T0_degC_x8 0x32
#define T1_degC_x8 0x33
#define T1_T0_msb 0x35
#define H0_T0_OUT_L 0x36
#define H0_T0_OUT_H 0x37
#define H1_T0_OUT_L 0x3A
#define H1_T0_OUT_H 0x3B
#define T0_OUT_L 0x3C
#define T0_OUT_H 0x3D
#define T1_OUT_L 0x3E
#define T1_OUT_H 0x3F

void HTS221_init_default(I2C_HandleTypeDef i2cHandle) {
	HTS221_Write8bit(i2cHandle, AV_CONF, 0x1B);
	HTS221_Write8bit(i2cHandle, CTRL_REG1, 0x83);
}

double HTS221_Read_Temp(I2C_HandleTypeDef i2cHandle) {
	int T0_degCi = HTS221_Read8bit(i2cHandle, T0_degC_x8);
	int T1_degCi = HTS221_Read8bit(i2cHandle, T1_degC_x8);
	int raw = HTS221_Read8bit(i2cHandle, T1_T0_msb);
	
	T0_degCi += ((raw & 0x03) * 256.0); //append the upper bits from the T1/T0 msb callibration reg to make 10 bit data.
	T1_degCi += ((raw & 0x0C) * 64.0);
	
	float T0_degC = T0_degCi / 8.0;
	float T1_degC = T1_degCi / 8.0;
	
	int T0_out = HTS221_Read8bit(i2cHandle, T0_OUT);
	
}

void HTS221_Write8bit(I2C_HandleTypeDef i2cHandle, unsigned char reg, unsigned char value) {
	buffer[0] = reg;
	buffer[1] = value;
	HAL_I2C_Master_Transmit(&i2cHandle, HTS221_WRITE_ADDRESS, buffer, 2, 100);
	HAL_Delay(20);
}

uint8_t HTS221_Read8bit(I2C_HandleTypeDef i2cHandle, unsigned char reg) {
	buffer[0] = reg;
	HAL_I2C_Master_Transmit(&i2cHandle, HTS221_WRITE_ADDRESS, buffer, 1, 100);
	HAL_Delay(20);
	HAL_I2C_Master_Receive(&i2cHandle, HTS221_READ_ADDRESS, buffer, 1, 100);
	
	return buffer[0];
}
 
