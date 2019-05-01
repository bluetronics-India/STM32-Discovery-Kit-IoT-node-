#include "stm32l4xx_hal.h"
#include "LIS3MDL_util.h"

#define LIS3MDL_WRITE_ADDRESS 0x3C
#define LIS3MDL_READ_ADDRESS 0x3D
#define WHO_AM_I 0x0F
#define CTRL_REG1 0x20
#define CTRL_REG2 0x21
#define CTRL_REG3 0x22
#define CTRL_REG4 0x23
#define CTRL_REG5 0x24
#define STATUS_REG 0x27
#define OUT_X_L 0x28
#define OUT_X_H 0x29
#define OUT_Y_L 0x2A
#define OUT_Y_H 0x2B
#define OUT_Z_L 0x2C
#define OUT_Z_H 0x2D
#define TEMP_OUT_L 0x2E
#define TEMP_OUT_H 0x2F
#define INT_CFG 0x30
#define INT_SRC 0x31
#define INT_THS_L 0x32
#define INT_THS_H 0x33

//GN values +/- 4 = 6842, +/- 8 = 3421, +/- 12 2281, +/- 16 1711
uint16_t GN = 6842;

unsigned char buffer[4];

void LIS3MDL_init_default(I2C_HandleTypeDef i2cHandle) {
	LIS3MDL_Write_CTRL_REG1(i2cHandle, 0x70);
	LIS3MDL_Write_CTRL_REG2(i2cHandle, 0x00);
	LIS3MDL_Write_CTRL_REG3(i2cHandle, 0x00);
	LIS3MDL_Write_CTRL_REG4(i2cHandle, 0x0C);
}

void LIS3MDL_Write_CTRL_REG1(I2C_HandleTypeDef i2cHandle, unsigned char value) {
	LIS3MDL_Write8bit(i2cHandle, CTRL_REG1, value);
}

void LIS3MDL_Write_CTRL_REG2(I2C_HandleTypeDef i2cHandle, unsigned char value) {
	unsigned char FS = (value & 0x60)>>5;
	switch(FS) {
		case 0x01:
			GN = 3421;
			break;
		case 0x02:
			GN = 2281;
			break;
		case 0x03:
			GN = 1711;
			break;
		default:
			GN = 6842;
			break;
	}
	
	LIS3MDL_Write8bit(i2cHandle, CTRL_REG2, value);
}

void LIS3MDL_Write_CTRL_REG3(I2C_HandleTypeDef i2cHandle, unsigned char value) {
	LIS3MDL_Write8bit(i2cHandle, CTRL_REG3, value);
}

void LIS3MDL_Write_CTRL_REG4(I2C_HandleTypeDef i2cHandle, unsigned char value) {
	LIS3MDL_Write8bit(i2cHandle, CTRL_REG4, value);
}

void LIS3MDL_Write_CTRL_REG5(I2C_HandleTypeDef i2cHandle, unsigned char value) {
	LIS3MDL_Write8bit(i2cHandle, CTRL_REG5, value);
}

void LIS3MDL_Write_INT_CFG(I2C_HandleTypeDef i2cHandle, unsigned char value) {
	LIS3MDL_Write8bit(i2cHandle, INT_CFG, value);
}

void LIS3MDL_Write_INT_THS_L(I2C_HandleTypeDef i2cHandle, unsigned char value) {
	LIS3MDL_Write8bit(i2cHandle, INT_THS_L, value);
}

void LIS3MDL_Write_INT_THS_H(I2C_HandleTypeDef i2cHandle, unsigned char value) {
	LIS3MDL_Write8bit(i2cHandle, INT_THS_H, value);
}

float LIS3MDL_Read_OUT_X(I2C_HandleTypeDef i2cHandle) {
	uint8_t out_x_l = LIS3MDL_Read8bit(i2cHandle, OUT_X_L);
	uint8_t out_x_h = LIS3MDL_Read8bit(i2cHandle, OUT_X_H);
	
	uint16_t z = 0;
	int value;
	value = ((out_x_h | z)<<8 | out_x_l);
	
	//Shift value to (-) value 
	if (value > 32768) {
		value = value - 65536;
	}
	
	return (float)value / (float)GN;
}

float LIS3MDL_Read_OUT_Y(I2C_HandleTypeDef i2cHandle) {
	uint8_t out_y_l = LIS3MDL_Read8bit(i2cHandle, OUT_Y_L);
	uint8_t out_y_h = LIS3MDL_Read8bit(i2cHandle, OUT_Y_H);
	
	uint16_t z = 0;
	int value;
	value  = ((out_y_h | z)<<8 | out_y_l);
	
	//Shift value to (-) value
	if (value > 32768) {
		value = value - 65536;
	}
	
	return (float)value / (float)GN;
}

float LIS3MDL_Read_OUT_Z(I2C_HandleTypeDef i2cHandle) {
	uint8_t out_z_l = LIS3MDL_Read8bit(i2cHandle, OUT_Z_L);
	uint8_t out_z_h = LIS3MDL_Read8bit(i2cHandle, OUT_Z_H);
	
	uint16_t z = 0;
	int value;
	value = ((out_z_h | z)<<8 | out_z_l);
	
	//Shift value to (-) value
	if (value > 32768) {
		value = value - 65536;
	}
	
	return (float)value / (float)GN;
}

void LIS3MDL_Write8bit(I2C_HandleTypeDef i2cHandle, unsigned char reg, unsigned char value) {
	buffer[0] = reg;
	buffer[1] = value;
	HAL_I2C_Master_Transmit(&i2cHandle, LIS3MDL_WRITE_ADDRESS, buffer, 2, 100);
	HAL_Delay(20);
}

uint8_t LIS3MDL_Read8bit(I2C_HandleTypeDef i2cHandle, unsigned char reg) {
	buffer[0] = reg;
	HAL_I2C_Master_Transmit(&i2cHandle, LIS3MDL_WRITE_ADDRESS, buffer, 1, 100);
	HAL_Delay(20);
	HAL_I2C_Master_Receive(&i2cHandle, LIS3MDL_READ_ADDRESS, buffer, 1, 100);
	
	return buffer[0];
}
