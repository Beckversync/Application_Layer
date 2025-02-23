/*
 * global.h
 *
 *  Created on: Feb 15, 2025
 *      Author: Dell
 */

#ifndef INC_GLOBAL_H_
#define INC_GLOBAL_H_

#include <stdint.h>
#define BKIT_MAX_SIZE 256

// Định nghĩa các giao thức hỗ trợ
typedef enum {
    PROTOCOL_UART = 0x01,
    PROTOCOL_I2C  = 0x02,
    PROTOCOL_SPI  = 0x03
} bkit_protocol_t;


#endif /* INC_GLOBAL_H_ */
