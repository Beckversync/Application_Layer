/*
 * I2C_LCD.h
 *
 *  Created on: Oct 20, 2024
 *      Author: ASUS
 */

#ifndef INC_I2C_LCD_H_
#define INC_I2C_LCD_H_

#include "stm32f1xx_hal.h"

#define RS 0x1
#define RW 0x2
#define En 0x4
#define ON 0x1
#define OFF 0x0

extern uint8_t data[1];
extern uint8_t _backlightval;

extern I2C_HandleTypeDef * i2c;

void I2C_Send(I2C_HandleTypeDef *hi2c, uint8_t DevAddress, uint8_t pData, uint8_t rw);
void backlight_On();
void backlight_Off();
void LCD_Init();
void printLCD(uint8_t row, uint8_t col, const char * text);
void clearLCD();
void setBlinking (uint8_t blink);
void setCursor (uint8_t cursor);

#endif /* INC_I2C_LCD_H_ */
