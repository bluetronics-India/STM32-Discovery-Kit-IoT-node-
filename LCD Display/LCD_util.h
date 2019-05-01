void LCD_init(I2C_HandleTypeDef i2cHandle);
void LCD_clear(I2C_HandleTypeDef i2cHandle);
void LCD_send_string(I2C_HandleTypeDef i2cHandle, char* str);
void LCD_send_cmd(I2C_HandleTypeDef i2cHandle, char cmd);
void LCD_send_data(I2C_HandleTypeDef i2cHandle, char data);
