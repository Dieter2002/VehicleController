#pragma once

#include "stm32h5xx_hal.h"
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define VL6180X_ADDR  (0x29 << 1)  // 7-bit adres + write bit (8-bit format)

typedef enum {
    SENSOR_IDLE,
    SENSOR_START,
    SENSOR_WAIT,
    SENSOR_DONE,
    SENSOR_TIMEOUT
} SensorState_t;

typedef struct {
    SensorState_t state;
    uint32_t startTick;
    uint8_t result;        // Laatste afstandsmeting
    uint8_t channel;       // TCA9548A kanaal
    bool ready;            // True zodra meting klaar is
} VL6180X_State_t;

void      VL6180X_WriteReg(uint16_t reg, uint8_t val);
uint8_t   VL6180X_ReadReg(uint16_t reg);
void     VL6180X_SimpleInit(void);
//uint8_t  VL6180X_ReadRange(void);

void VL6180X_InitStateMachine(uint8_t mux_channel);
void VL6180X_Process(void);
bool VL6180X_IsReady(void);
uint8_t VL6180X_GetResult(void);

#ifdef __cplusplus
}
#endif
