#include "stm32l4xx_hal.h"
#include "LSM6DSL_util.h"

#define LSM6DSL_WRITE_ADDRESS 0xD4
#define LSM6DSL_READ_ADDRESS 0xD5
#define FUNC_CFG_ACCESS 0x01
#define SENSOR_SYNC_TIME_FRAME 0x04
#define SENSOR_SYNC_RES_RATIO 0x05
#define FIFO_CTRL1 0x06
#define FIFO_CTRL2 0x07
#define FIFO_CTRL3 0x08
#define FIFO_CTRL4 0x09
#define FIFO_CTRL5 0x0A
#define DRDY_PULSE_CFG_G 0x0B
#define INT1_CTRL 0x0D
#define INT2_CTRL 0x0E
#define WHO_AM_I 0x0F
#define CTRL1_XL 0x10
#define CTRL2_G 0x11
#define CTRL3_C 0x12
#define CTRL4_C 0x13
#define CTRL5_C 0x14
#define CTRL6_C 0x15
#define CTRL7_G 0x16
#define CTRL8_XL 0x17
#define CTRL9_XL 0x18
#define CTRL10_C 0x19
#define MASTER_CONFIG 0x1A
#define WAKE_UP_SRC 0x1B
#define TAP_SRC 0x1C
#define D6D_SRC 0x1D
#define STATUS_REG 0x1E
#define OUT_TEMP_L 0x20
#define OUT_TEMP_H 0x21
//Output for Gyro X,Y,Z
#define OUTX_L_G 0x22
#define OUTX_H_G 0x23
#define OUTY_L_G 0x24
#define OUTY_H_G 0x25
#define OUTZ_L_G 0x26
#define OUTZ_H_G 0x27
//Accelerometer OUT X,Y,Z
#define OUTX_L_XL 0x28
#define OUTX_H_XL 0x29
#define OUTY_L_XL 0x2A
#define OUTY_H_XL 0x2B
#define OUTZ_L_XL 0x2C
#define OUTZ_H_XL 0x2D
double linear_sensitivity = 0.061;
unsigned char buffer[4];

void LSM6DSL_acc_init_default(I2C_HandleTypeDef i2cHandle) {
	LSM6DSL_Write_FIFO_CTRL5(i2cHandle, 0x26);
	LSM6DSL_Write_CTRL1_XL(i2cHandle, 0x40);
}

void LSM6DSL_Write_FIFO_CTRL1(I2C_HandleTypeDef i2cHandle, unsigned char value) {
	LSM6DSL_Write8bit(i2cHandle, FIFO_CTRL1, value);
}

void LSM6DSL_Write_FIFO_CTRL2(I2C_HandleTypeDef i2cHandle, unsigned char value) {
	LSM6DSL_Write8bit(i2cHandle, FIFO_CTRL2, value);
}

void LSM6DSL_Write_FIFO_CTRL3(I2C_HandleTypeDef i2cHandle, unsigned char value) {
	LSM6DSL_Write8bit(i2cHandle, FIFO_CTRL3, value);
}

void LSM6DSL_Write_FIFO_CTRL4(I2C_HandleTypeDef i2cHandle, unsigned char value) {
	LSM6DSL_Write8bit(i2cHandle, FIFO_CTRL4, value);
}

void LSM6DSL_Write_FIFO_CTRL5(I2C_HandleTypeDef i2cHandle, unsigned char value) {
	LSM6DSL_Write8bit(i2cHandle, FIFO_CTRL5, value);
}

void LSM6DSL_Write_INT1_CTRL(I2C_HandleTypeDef i2cHandle, unsigned char value) {
	LSM6DSL_Write8bit(i2cHandle, INT1_CTRL, value);
}

void LSM6DSL_Write_INT2_CTRL(I2C_HandleTypeDef i2cHandle, unsigned char value) {
	LSM6DSL_Write8bit(i2cHandle, INT2_CTRL, value);
}

void LSM6DSL_Write_CTRL1_XL(I2C_HandleTypeDef i2cHandle, unsigned char value) {
	int FS = (value & 0x0C)>>2;
	
	switch(FS) {
		case 1: // +/- 16g
			linear_sensitivity = 0.488;
			break;
		case 2: // +/- 4g
			linear_sensitivity = 0.122;
			break;
		case 3: // +/- 8g
			linear_sensitivity = 0.244;
			break;
		default: // +/- 2g
			linear_sensitivity = 0.061;
			break;
	}
	
	LSM6DSL_Write8bit(i2cHandle, CTRL1_XL, value);
}

//Accelerometer
float LSM6DSL_acc_Read_OUT_X(I2C_HandleTypeDef i2cHandle) {
	int outx_l = LSM6DSL_Read8bit(i2cHandle, OUTX_L_XL);
	int outx_h = LSM6DSL_Read8bit(i2cHandle, OUTX_H_XL);
	
	int out_x = outx_h<<8 | outx_l;
	if (out_x > 23768) {
		out_x -= 65536;
	}
	
	return out_x * linear_sensitivity;
}

float LSM6DSL_acc_Read_OUT_Y(I2C_HandleTypeDef i2cHandle) {
	int outy_l = LSM6DSL_Read8bit(i2cHandle, OUTY_L_XL);
	int outy_h = LSM6DSL_Read8bit(i2cHandle, OUTY_H_XL);
	
	int out_y = outy_h<<8 | outy_l;
	if (out_y > 23768) {
		out_y -= 65536;
	}
	
	return out_y * linear_sensitivity;
}

float LSM6DSL_acc_Read_OUT_Z(I2C_HandleTypeDef i2cHandle) {
	int outz_l = LSM6DSL_Read8bit(i2cHandle, OUTZ_L_XL);
	int outz_h = LSM6DSL_Read8bit(i2cHandle, OUTZ_H_XL);
	
	int out_z = outz_h<<8 | outz_l;
	if (out_z > 23768) {
		out_z -= 65536;
	}
	
	return out_z * linear_sensitivity;
}

void LSM6DSL_Write8bit(I2C_HandleTypeDef i2cHandle, unsigned char reg, unsigned char value) { 
	buffer[0] = reg; 
	buffer[1] = value; 
	HAL_I2C_Master_Transmit(&i2cHandle, LSM6DSL_WRITE_ADDRESS, buffer, 2, 100); 
	HAL_Delay(20); 
} 

 
uint8_t LSM6DSL_Read8bit(I2C_HandleTypeDef i2cHandle, unsigned char reg) { 
	buffer[0] = reg; 
	HAL_I2C_Master_Transmit(&i2cHandle, LSM6DSL_WRITE_ADDRESS, buffer, 1, 100); 
	HAL_Delay(20); 
	HAL_I2C_Master_Receive(&i2cHandle, LSM6DSL_READ_ADDRESS, buffer, 1, 100); 

	return buffer[0]; 
} 


