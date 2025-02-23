#ifndef APPLICATION_LAYER_H
#define APPLICATION_LAYER_H

#include "global.h"
#include "stm32f1xx_hal.h"
#include <stdbool.h>

typedef struct {
    int32_t id;
    float temperature;
    float humidity;
    uint64_t timestamp;
} sensor_data_t;

typedef struct {
    uint8_t sync;
    uint16_t length;
    uint16_t checksum;
} bkit_header_t;

bool bkit_send_message(const sensor_data_t* data);
bool bkit_receive_message(sensor_data_t* data);

#endif // APPLICATION_LAYER_H
