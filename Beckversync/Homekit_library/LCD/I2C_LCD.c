/*
 * I2C_LCD.c
 *
 *  Created on: Oct 20, 2024
 *      Author: ASUS
 */
#include "I2C_LCD.h"

uint8_t data[1];
uint8_t _backlightval = 0x8;
uint8_t D /*Display*/ = 1,
		N /*Number of lines*/ = 1,
		C /*Cursor*/ = 0,
		B /*Blinking*/ = 0;

I2C_HandleTypeDef * lcd_i2c;

void I2C_Send(I2C_HandleTypeDef *i2c, uint8_t DevAddress, uint8_t pData, uint8_t rs) {
	uint8_t data[1] = {(pData & 0xF0) | _backlightval | En | rs};
	HAL_I2C_Master_Transmit(i2c, (0x27 << 1), data, sizeof(data), 10);
	HAL_Delay(1);

	data[0] = (pData & 0xF0) | _backlightval | rs;
	HAL_I2C_Master_Transmit(i2c, (0x27 << 1), data, sizeof(data), 10);
	HAL_Delay(1);

	data[0] = (pData << 4) | _backlightval | rs | En;
	HAL_I2C_Master_Transmit(i2c, (0x27 << 1), data, sizeof(data), 10);
	HAL_Delay(1);

	data[0] = (pData << 4) | _backlightval | rs;
	HAL_I2C_Master_Transmit(i2c, (0x27 << 1), data, sizeof(data), 10);
	HAL_Delay(1);
}

void LCD_Init(I2C_HandleTypeDef * i2c){
	lcd_i2c = i2c;

//	Set up for 8 bits interface
	I2C_Send(lcd_i2c, (0x27 << 1), (3 << 4), 0);
	I2C_Send(lcd_i2c, (0x27 << 1), (3 << 4), 0);
	I2C_Send(lcd_i2c, (0x27 << 1), (3 << 4), 0);

//	Change into 4 bits interface - 2 display lines - font 5x8 dots
	I2C_Send(lcd_i2c, (0x27 << 1), 0x8, 0);

//	Display on - cursor off - blinking on
	I2C_Send(lcd_i2c, (0x27 << 1), 0xD, 0);

//  Entry mode set
	I2C_Send(lcd_i2c, (0x27 << 1), 0x6, 0);

	clearLCD();
};

void backlight_On(){
	_backlightval = 0x8;
	uint8_t data[1] = {_backlightval};
	HAL_I2C_Master_Transmit(lcd_i2c, (0x27 << 1), data, sizeof(data), 50);
	HAL_I2C_Master_Transmit(lcd_i2c, (0x27 << 1), data, sizeof(data), 50);
}

void backlight_Off(){
	_backlightval = 0x0;
	uint8_t data[1] = {_backlightval};
	HAL_I2C_Master_Transmit(lcd_i2c, (0x27 << 1), data, sizeof(data), 50);
	HAL_I2C_Master_Transmit(lcd_i2c, (0x27 << 1), data, sizeof(data), 50);
}

void printLCD(uint8_t row, uint8_t col, const char * text){
	if (text[0] == '\0')			return;
	if (row > 2 || row < 1 || col > 16 || col < 1)				return;
	I2C_Send(lcd_i2c, (0x27 << 1), 0x80 | ((row - 1)*64 + (col - 1)), 0);
	for (int idx = 0; text[idx] != '\0' && col + idx < 17; idx++){
		I2C_Send(lcd_i2c, (0x27 << 1), text[idx], RS);
	}
}

void clearLCD(){
	//  Clear display
		I2C_Send(lcd_i2c, (0x27 << 1), 0x1, 0);
	//  Home return
		I2C_Send(lcd_i2c, (0x27 << 1), 0x2, 0);
}

void setBlinking (uint8_t blink) {
	B = blink;
	I2C_Send(lcd_i2c, (0x27 << 1), 0x0, 0);
	I2C_Send(lcd_i2c, (0x27 << 1), ((1 << 3) | (D << 2) | (C << 1) | B), 0);
}

void setCursor (uint8_t cursor) {
	C = cursor;
	I2C_Send(lcd_i2c, (0x27 << 1), 0x0, 0);
	I2C_Send(lcd_i2c, (0x27 << 1), ((1 << 3) | (D << 2) | (C << 1) | B), 0);
}
