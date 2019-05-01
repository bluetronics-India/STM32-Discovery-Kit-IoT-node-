#include "stm32l4xx_hal.h"
#include "LCD_util.h"
#include "USART1_util.h"

//Commands
#define LCD_WRITE_ADDRESS 0x4E
#define LCD_CLEAR_DISPLAY 0x01
#define LCD_RETURN_HOME 0x02
#define LCD_CURSOR_LEFT 0x04
#define LCD_CURSOR_RIGHT 0x06
#define LCD_SHIFT_DISPLAY_RIGHT 0x05
#define LCD_SIFT_DISPLAY_LEFT 0x07
#define LCD_DISPLAY_OFF_CURSOR_OFF 0x08
#define LCD_DISPLAY_OFF_CURSOR_ON 0x0A
#define LCD_DISPLAY_ON_CURSOR_OFF 0x0C
#define LCD_DISPLAY_ON_CURSOR_BLINKING1 0x0E
#define LCD_DISPLAY_ON_CURSOR_BLINKING2 0x0F
#define LCD_SHIFT_CURSOR_LEFT 0x10
#define LCD_SHIFT_CURSOR_RIGHT 0x14
#define LCD_ENTIRE_DISPLAY_LEFT 0x18
#define LCD_ENTIRE_DISPLAY_RIGHT 0x1C
#define LCD_FORCE_CURSOR_BEGINNING_1 0x80
#define LCD_FORCE_CURSOR_BEGINNING_2 0xC0
#define LCD_5x7_4BITMODE 0x28
#define LCD_5x7_8BITMODE 0x38


uint8_t buffer[4];

void LCD_init(I2C_HandleTypeDef i2cHandle) {
	LCD_clear(i2cHandle);
	LCD_send_cmd(i2cHandle, LCD_RETURN_HOME); //DB1 - Cursor Home
	LCD_send_cmd(i2cHandle, LCD_5x7_4BITMODE); //DB5, DB3 
	LCD_send_cmd(i2cHandle, LCD_DISPLAY_ON_CURSOR_OFF); //
	LCD_send_cmd(i2cHandle, LCD_FORCE_CURSOR_BEGINNING_1); //Force cursor to 1st line
}

void LCD_clear(I2C_HandleTypeDef i2cHandle) {
	LCD_send_cmd(i2cHandle, LCD_CLEAR_DISPLAY);
}

void LCD_send_string(I2C_HandleTypeDef i2cHandle, char* str) {
	while(*str) {
		putchUSART1(*str);
		LCD_send_data(i2cHandle, *str++);
	}
	newlineUSART1();
}

void LCD_send_cmd(I2C_HandleTypeDef i2cHandle, char cmd) {
	char data_u, data_l;    
	data_u = cmd & 0xf0;
	data_l = (cmd<<4) & 0xf0;
	buffer[0] = data_u | 0x0C;
	buffer[1] = data_u | 0x08;
	buffer[2] = data_l | 0x0C;
	buffer[3] = data_l | 0x08; 
	//buffer[0] = cmd;
	HAL_I2C_Master_Transmit (&i2cHandle, LCD_WRITE_ADDRESS, (uint8_t *)buffer, 4, 100);
   
}

void LCD_send_data(I2C_HandleTypeDef i2cHandle, char data) {
	char data_u, data_l;
	data_u = data & 0xf0;
	data_l = (data<<4) & 0xf0;
	buffer[0] = data_u | 0x0D;
	buffer[1] = data_u | 0x09;
	buffer[2] = data_l | 0x0D;
	buffer[3] = data_l | 0x09;
	HAL_I2C_Master_Transmit(&i2cHandle, LCD_WRITE_ADDRESS, (uint8_t *)buffer, 4, 100);
}
