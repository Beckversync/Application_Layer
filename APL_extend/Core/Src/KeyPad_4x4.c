#include "Keypad_4x4.h"

volatile int wakeup = 0;

GPIO_TypeDef * KeypadPort [4];
int KeypadPin [4];
int col, row;
int pass[6];
int step = 0,
	stepVal[3] = {0, 0, 0};
struct keypad Keypad_Controller;

void setPass(uint32_t pas[]) {
	for (int index = 0; index < 6; index++) {
		pass[index] = pas[index];
	}
}

int checkPass (uint32_t pa[]) {
	for (int idx = 0; idx < 6; idx++) {
		if (pa[idx] != pass[idx])			return 0;
	}
	return 1;
}

//void Keypad_init(struct keypad * pad,
//				 struct pin c1, struct pin c2, struct pin c3, struct pin c4,
//				 struct pin r1, struct pin r2, struct pin r3, struct pin r4){
//	struct pin c [] = {c1, c2, c3, c4},
//						 r [] = {r1, r2, r3, r4};
//
//		for (int index = 0; index < 4; index++) {
//			pad->C[index] = c[index];
//			pad->R[index] = r[index];
//
//			KeypadPin[index] = pinArr[r[index].pin];
//			KeypadPort [index] = r[index].p;
//		}
//
//		for (int idx = 0; idx < 4; idx++) {
//
//			if (pad->C[idx].p == GPIOA)
//				RCC->APB2ENR |= (1 << 2) | 1;
//			else if (pad->C[idx].p == GPIOB)
//				RCC->APB2ENR |= (1 << 3) | 1;
//			else if (pad->C[idx].p == GPIOC)
//				RCC->APB2ENR |= (1 << 4) | 1;
//
//			int Exti_port, Exti_x;
//			if (pad->R[idx].p == GPIOA){
//				Exti_port = 0;
//				RCC->APB2ENR |= (1 << 2);
//			}
//			else if (pad->R[idx].p == GPIOB){
//				Exti_port = 1;
//				RCC->APB2ENR |= (1 << 3);
//			}
//			else if (pad->R[idx].p == GPIOC){
//				Exti_port = 2;
//				RCC->APB2ENR |= (1 << 4);
//			}
//
//			if (pad->C[idx].pin < 8){
//				pad->C[idx].p->CRL &= ~(0xF << (pad->C[idx].pin*4));
//				pad->C[idx].p->CRL |= (0x3 << (pad->C[idx].pin*4));
//				}
//			else {
//				pad->C[idx].p->CRH &= ~(0xF << ((pad->C[idx].pin - 8)*4));
//				pad->C[idx].p->CRH |= (0x3 << ((pad->C[idx].pin - 8)*4));
//				}
//
//			pad->C[idx].p->ODR |= (1 << pad->C[idx].pin);
//
//			if (pad->R[idx].pin < 8){
//				pad->R[idx].p->CRL &= ~(0xF << (pad->R[idx].pin*4));
//				pad->R[idx].p->CRL |= (0x8 << (pad->R[idx].pin*4));
//				}
//			else {
//				pad->R[idx].p->CRH &= ~(0xF << ((pad->R[idx].pin - 8)*4));
//				pad->R[idx].p->CRH |= (0x8 << ((pad->R[idx].pin - 8)*4));
//				}
//
//			pad->R[idx].p->ODR &= ~(1 << pad->R[idx].pin);//Pull down for input line
//
//			if (pad->R[idx].pin < 4) {
//				__disable_irq();
//				Exti_x = EXTI0_IRQn;
//				AFIO->EXTICR[0] &= ~(0xFF << pad->R[idx].pin*4);
//				AFIO->EXTICR[0] |= (Exti_port << pad->R[idx].pin*4);
//				EXTI->IMR |= (1 << pad->R[idx].pin);
//				EXTI->RTSR |= (1 << pad->R[idx].pin);
//				NVIC_EnableIRQ(Exti_x);
//				__enable_irq();
//			}
//		}
//}

void Keypad_Init(struct pin c1, struct pin c2, struct pin c3, struct pin c4,
								 struct pin r1, struct pin r2, struct pin r3, struct pin r4) {
	struct pin c [] = {c1, c2, c3, c4},
						 r [] = {r1, r2, r3, r4};

	for (int index = 0; index < 4; index++) {
		Keypad_Controller.C[index] = c[index];
		Keypad_Controller.R[index] = r[index];

		KeypadPin[index] = pinArr[r[index].pin];
		KeypadPort [index] = r[index].p;
	}

	for (int idx = 0; idx < 4; idx++) {

		if (Keypad_Controller.C[idx].p == GPIOA)
			RCC->APB2ENR |= (1 << 2) | 1;
		else if (Keypad_Controller.C[idx].p == GPIOB)
			RCC->APB2ENR |= (1 << 3) | 1;
		else if (Keypad_Controller.C[idx].p == GPIOC)
			RCC->APB2ENR |= (1 << 4) | 1;

		int Exti_port, Exti_x;
		if (Keypad_Controller.R[idx].p == GPIOA){
			Exti_port = 0;
			RCC->APB2ENR |= (1 << 2);
		}
		else if (Keypad_Controller.R[idx].p == GPIOB){
			Exti_port = 1;
			RCC->APB2ENR |= (1 << 3);
		}
		else if (Keypad_Controller.R[idx].p == GPIOC){
			Exti_port = 2;
			RCC->APB2ENR |= (1 << 4);
		}

		if (Keypad_Controller.C[idx].pin < 8){
			Keypad_Controller.C[idx].p->CRL &= ~(0xF << (Keypad_Controller.C[idx].pin*4));
			Keypad_Controller.C[idx].p->CRL |= (0x3 << (Keypad_Controller.C[idx].pin*4));
			}
		else {
			Keypad_Controller.C[idx].p->CRH &= ~(0xF << ((Keypad_Controller.C[idx].pin - 8)*4));
			Keypad_Controller.C[idx].p->CRH |= (0x3 << ((Keypad_Controller.C[idx].pin - 8)*4));
			}

		Keypad_Controller.C[idx].p->ODR |= (1 << Keypad_Controller.C[idx].pin);

		if (Keypad_Controller.R[idx].pin < 8){
			Keypad_Controller.R[idx].p->CRL &= ~(0xF << (Keypad_Controller.R[idx].pin*4));
			Keypad_Controller.R[idx].p->CRL |= (0x8 << (Keypad_Controller.R[idx].pin*4));
			}
		else {
			Keypad_Controller.R[idx].p->CRH &= ~(0xF << ((Keypad_Controller.R[idx].pin - 8)*4));
			Keypad_Controller.R[idx].p->CRH |= (0x8 << ((Keypad_Controller.R[idx].pin - 8)*4));
			}

		Keypad_Controller.R[idx].p->ODR &= ~(1 << Keypad_Controller.R[idx].pin);//Pull down for input line

		if (Keypad_Controller.R[idx].pin < 4) {
			__disable_irq();
			Exti_x = EXTI0_IRQn;
			AFIO->EXTICR[0] &= ~(0xFF << Keypad_Controller.R[idx].pin*4);
			AFIO->EXTICR[0] |= (Exti_port << Keypad_Controller.R[idx].pin*4);
			EXTI->IMR |= (1 << Keypad_Controller.R[idx].pin);
			EXTI->RTSR |= (1 << Keypad_Controller.R[idx].pin);
			NVIC_EnableIRQ(Exti_x);
			__enable_irq();
		}
	}
}

struct pin Pin_Init(GPIO_TypeDef * p, int pi) {
	struct pin Pin;
	Pin.p = p;
	Pin.pin = pi;
	return Pin;
}


int Keypad_Read (void) {
	Keypad_Controller.C[0].p->ODR &= ~(1 << Keypad_Controller.C[0].pin);
	Keypad_Controller.C[1].p->ODR &= ~(1 << Keypad_Controller.C[1].pin);
	Keypad_Controller.C[2].p->ODR &= ~(1 << Keypad_Controller.C[2].pin);
	Keypad_Controller.C[3].p->ODR &= ~(1 << Keypad_Controller.C[3].pin);

	int push_pos[2] = {-2, 0};
	uint8_t pushed_flag = 0;
	for (int c = 0; c < 4 && !pushed_flag; c++) {
		Keypad_Controller.C[c].p->ODR |= (1 << Keypad_Controller.C[c].pin);
		HAL_Delay(2);
		for (int r = 0; r < 4 && !pushed_flag; r++) {
			if (Keypad_Controller.R[r].p->IDR & (1 << Keypad_Controller.R[r].pin)) {
				push_pos[0] = c;//Capture pushing button
				push_pos[1] = r;
				col = c;
				row = r;
				pushed_flag = 1;
			}
		}
		Keypad_Controller.C[c].p->ODR &= ~(1 << Keypad_Controller.C[c].pin);
		HAL_Delay(2);
	}
	//Wait till the pushed button is released
	if (pushed_flag) {
		Keypad_Controller.C[push_pos[0]].p->ODR |= (1 << Keypad_Controller.C[push_pos[0]].pin);
		HAL_Delay(2);
		uint32_t start_time = HAL_GetTick();
		while (1) {
			if (HAL_GetTick() - start_time >= 20) {
				if ((Keypad_Controller.R[push_pos[1]].p->IDR & (1 << Keypad_Controller.R[push_pos[1]].pin))) {
					stepVal[step] = 1;
				}
				if (++step == 3) {
					if ((stepVal[0] | (stepVal[1] << 1) | (stepVal[2] << 2)) == 7) {
						while ((Keypad_Controller.R[push_pos[1]].p->IDR & (1 << Keypad_Controller.R[push_pos[1]].pin))) {}
						Keypad_Controller.C[push_pos[0]].p->ODR &= ~(1 << Keypad_Controller.C[push_pos[0]].pin);
						HAL_Delay(2);
						break;
					} else if ((stepVal[0] | (stepVal[1] << 1) | (stepVal[2] << 2)) == 0) {
						Keypad_Controller.C[push_pos[0]].p->ODR &= ~(1 << Keypad_Controller.C[push_pos[0]].pin);
						push_pos[0] = -2;
						push_pos[1] = 0;
						HAL_Delay(2);
						break;
					} else {
						step = 0;
					}
				}
				start_time = HAL_GetTick();
			}
		}
	}
	return push_pos[0] + 1 + 4 * push_pos[1];
}

int Keypad_read (struct keypad * pad) {
		HAL_GPIO_WritePin(pad->C[0].p, pad->C[0].pin, 0);
		HAL_GPIO_WritePin(pad->C[1].p, pad->C[1].pin, 0);
		HAL_GPIO_WritePin(pad->C[2].p, pad->C[2].pin, 0);
		HAL_GPIO_WritePin(pad->C[3].p, pad->C[3].pin, 0);
		int push_pos[2] = {-2, 0};
		uint8_t pushed_flag = 0;
		for (int c = 0; c < 4 && !pushed_flag; c++) {
			HAL_GPIO_WritePin(pad->C[c].p, pad->C[c].pin, 1);
			for (int r = 0; r < 4 && !pushed_flag; r++) {
				if (pad->R[r].p->IDR & (1 << pad->R[r].pin)) {
					push_pos[0] = c;//Capture pushing button
					push_pos[1] = r;
					col = c;
					row = r;
					pushed_flag = 1;
				}
			}
			HAL_GPIO_WritePin(pad->C[c].p, pad->C[c].pin, 0);
		}
		//Wait till the pushed button is released
		if (pushed_flag) {
			HAL_GPIO_WritePin(pad->C[push_pos[0]].p, pad->C[push_pos[0]].pin, 1);
			uint32_t start_time = HAL_GetTick(), release_flag = 0;
			while (1) {
				if (HAL_GetTick() - start_time >= 90) {
					if (pad->R[push_pos[1]].p->IDR & (1 << pad->R[push_pos[1]].pin)) {
						release_flag = 0;
					} else {
						if (release_flag) {//If the second time capture low input
							break;
						} else {
							release_flag = 1;
						}
					}
					start_time = HAL_GetTick();
				}
			}
		}
		return push_pos[0] + 1 + 4 * push_pos[1];
}
