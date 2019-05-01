#include "stm32l4xx_hal.h"
void LSM6DSL_acc_init_default(I2C_HandleTypeDef i2cHandle);
void LSM6DSL_Write_FIFO_CTRL1(I2C_HandleTypeDef i2cHandle, unsigned char value);
void LSM6DSL_Write_FIFO_CTRL2(I2C_HandleTypeDef i2cHandle, unsigned char value);
void LSM6DSL_Write_FIFO_CTRL3(I2C_HandleTypeDef i2cHandle, unsigned char value);
void LSM6DSL_Write_FIFO_CTRL4(I2C_HandleTypeDef i2cHandle, unsigned char value);
void LSM6DSL_Write_FIFO_CTRL5(I2C_HandleTypeDef i2cHandle, unsigned char value);
void LSM6DSL_Write_INT1_CTRL(I2C_HandleTypeDef i2cHandle, unsigned char value);
void LSM6DSL_Write_INT2_CTRL(I2C_HandleTypeDef i2cHandle, unsigned char value);
void LSM6DSL_Write_CTRL1_XL(I2C_HandleTypeDef i2cHandle, unsigned char value);
float LSM6DSL_acc_Read_OUT_X(I2C_HandleTypeDef i2cHandle);
float LSM6DSL_acc_Read_OUT_Y(I2C_HandleTypeDef i2cHandle);
float LSM6DSL_acc_Read_OUT_Z(I2C_HandleTypeDef i2cHandle);
void LSM6DSL_Write8bit(I2C_HandleTypeDef i2cHandle, unsigned char reg, unsigned char value);
uint8_t LSM6DSL_Read8bit(I2C_HandleTypeDef i2cHandle, unsigned char reg); 
