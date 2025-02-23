/*
 * Keypad_4x4_Kit.h
 *
 *  Created on: Sep 16, 2024
 *      Author: ASUS
 */

#ifndef INC_KEYPAD_4X4_KIT_H_
#define INC_KEYPAD_4X4_KIT_H_
#include "stm32f1xx_hal.h"

extern volatile int wakeup;

extern GPIO_TypeDef * KeypadPort [4];
extern int KeypadPin [4],
		   pass[6],
		   step;
//extern int col, row;

static int pinArr [] = {GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_2, GPIO_PIN_3, GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_6, GPIO_PIN_7, GPIO_PIN_8, GPIO_PIN_9, GPIO_PIN_10, GPIO_PIN_11, GPIO_PIN_12, GPIO_PIN_13, GPIO_PIN_14, GPIO_PIN_15};

enum port {PortA, PortB, PortC};
struct pin {
	GPIO_TypeDef * p;
	int pin;
};

struct keypad {
	struct pin C[4], R[4];
};

extern struct keypad Keypad_Controller;

struct pin Pin_Init(GPIO_TypeDef * p, int pi);

void setPass(uint32_t pas[]);

int checkPass (uint32_t pa[]);

void Keypad_Init(struct pin c1, struct pin c2, struct pin c3, struct pin c4,
				 struct pin r1, struct pin r2, struct pin r3, struct pin r4);

int Keypad_Read (void);

#endif /* INC_KEYPAD_4X4_KIT_H_ */
