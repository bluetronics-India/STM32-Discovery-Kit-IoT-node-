#include "stm32l4xx_hal.h"

void LIS3MDL_init_default(I2C_HandleTypeDef i2cHandle);
void LIS3MDL_Write_CTRL_REG1(I2C_HandleTypeDef i2cHandle, unsigned char value);
void LIS3MDL_Write_CTRL_REG2(I2C_HandleTypeDef i2cHandle, unsigned char value);
void LIS3MDL_Write_CTRL_REG3(I2C_HandleTypeDef i2cHandle, unsigned char value);
void LIS3MDL_Write_CTRL_REG4(I2C_HandleTypeDef i2cHandle, unsigned char value);
void LIS3MDL_Write_CTRL_REG5(I2C_HandleTypeDef i2cHandle, unsigned char value);
void LIS3MDL_Write_INT_CFG(I2C_HandleTypeDef i2cHandle, unsigned char value);
void LIS3MDL_Write_INT_THS_L(I2C_HandleTypeDef i2cHandle, unsigned char value);
void LIS3MDL_Write_INT_THS_H(I2C_HandleTypeDef i2cHandle, unsigned char value);
float LIS3MDL_Read_OUT_X(I2C_HandleTypeDef i2cHandle);
float LIS3MDL_Read_OUT_Y(I2C_HandleTypeDef i2cHandle);
float LIS3MDL_Read_OUT_Z(I2C_HandleTypeDef i2cHandle);
void LIS3MDL_Write8bit(I2C_HandleTypeDef i2cHandle, unsigned char reg, unsigned char value);
uint8_t LIS3MDL_Read8bit(I2C_HandleTypeDef i2cHandle, unsigned char reg);

