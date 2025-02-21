/*
 * Kita_Oled_DisplayKit.c
 *
 *  Created on: Oct 11, 2024
 *      Author: ASUS
 */
#include "OLED.h"

I2C_HandleTypeDef * i2c;
uint8_t * buffer;

void init_Oled(I2C_HandleTypeDef * ti) {
  i2c = ti;

  uint8_t data[] = {0x00, //Control byte
					0xAE,//Turn off display
					0xD5, 0x80,//Display clock divide: Fosc = 8, Divide ratio = 1
					0xA8, 0x3F,//Multiplex ratio = 63
					0xD3,0x00,//No display offset
					0x40,//Display start line = 0
					0x8D, 0x14,//Enable change pump
					0x20, 0x00,//Horizontal addressing Mode
					0xA0,//Column address 0 is mapped to SEG0
					0xC0,//COM output, scan direction = remapped mode
					0xDA, 0x12,//COM Pins hardware configuration: Sequential
					0x81, 0xCF,//Set contrast step = 100
//					0xD9, 0xF1,//Set precharge
//					0xDB, 0x40,
					0xA4,//Display from RAM
					0xA6,//Normal display
//					0x2E,
					0xAF,//Turn on display
					0x22, 0x00, 0x07,//Set page address from 0 to 7
					0x21, 0x00, 0x7F};//Set column address from 0 to 127

  if (HAL_I2C_Master_Transmit(i2c, (0x3C << 1), data, sizeof(data), 100) == HAL_BUSY)
	  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, 0);
  else
	  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, 1);

  buffer = (uint8_t *)calloc(128 * 64/8, sizeof(uint8_t));//Allocate memory with initial value of 0
}

void destructor_Oled() {
	free(buffer);
	buffer = NULL;
}

void displayString (char c[], int size, int startPage, int startCol, int color) {
	for (int idx = 0; idx < size; idx++) {
	  if (startCol + 6 >= 128) {
		  startCol = 0;
		  if (++startPage >= 8)
			  startPage = 0;
	  }
	  displayChar(c[idx], startPage, startCol, color);
	  startCol += 6;
	}
}

void displayChar (char c, int startPage, int startCol, int color) {
	if (startPage < 0 || startPage > 7 || startCol < 0 || startCol > 127)
		return;
	int nbOB = 7;
	uint8_t *ptr = &font_6x8[6*(c - ' ')];

	int currentIdx = startPage * 128 + startCol;
	while (nbOB--) {
		if (currentIdx >= 1024)
			currentIdx = 0;
		if (color)//Color = white
			buffer[currentIdx++] = *ptr++;
		else
			buffer[currentIdx++] = ~*ptr++;
	}
}

void displayIcon (int iconIdx, int startPage, int startCol, int color) {
	int nbOB = 32;
	uint8_t *ptr = &battery[iconIdx*32];

	int currentIdx = startPage * 128 + startCol;
	while (nbOB--) {
		if (nbOB == 15) {
			currentIdx = (startPage + 1)*128 + startCol;
		}
		if (currentIdx >= 1024)
			currentIdx = startCol;
		if (color)
			buffer[currentIdx++] = *ptr++;
		else
			buffer[currentIdx++] = ~*ptr++;
	}
}

void displayBitmap (int imgIdx) {
	for (int idx = 0; idx < 1024; idx++) {
		buffer[idx] = OLED_img[imgIdx] [idx];
	}
}

void display() {
	int nbOB = 1030;
	uint8_t *ptr = buffer;

	uint8_t pointerset[] = {0x00,
							0x22, 0x00, 0x07,
							0x21, 0x00, 0x7F};

	HAL_I2C_Master_Transmit(i2c, (0x3C << 1), pointerset, sizeof(pointerset), 50);

	while (nbOB--) {
		uint8_t data[200], count;
		data[0] = 0x40;
		if (nbOB < sizeof(data)/sizeof(data[0]) - 1) {
			count = nbOB;
		}
		for (int idx = 1; idx < sizeof(data)/sizeof(data[0]) && nbOB > 0;) {
			data[idx++] = *ptr++;
			nbOB--;
		}
		HAL_I2C_Master_Transmit(i2c, (0x3C << 1), data, ((nbOB > 0)? sizeof(data)/sizeof(data[0]) : count + 1), 50);
	}
}

void clearDisplay (int color) {
	if (color != 0 && color != 1)
		return;
	int nbOB = 1030;
	uint8_t pointerset[] = {0x00,
							0x22, 0x00, 0x07,
							0x21, 0x00, 0x7F};

	HAL_I2C_Master_Transmit(i2c, (0x3C << 1), pointerset, sizeof(pointerset), 50);

	while (nbOB--) {
		uint8_t data[200], count;
		data[0] = 0x40;
		if (nbOB < sizeof(data)/sizeof(data[0]) - 1) {
			count = nbOB;
		}
		for (int idx = 1; idx < sizeof(data)/sizeof(data[0]) && nbOB > 0;) {
			data[idx++] = (color == WHITE ? 0xFF : 0x00);
			nbOB--;
		}
		HAL_I2C_Master_Transmit(i2c, (0x3C << 1), data, ((nbOB > 0) ? sizeof(data)/sizeof(data[0]) : count + 1), 50);
	}
}


