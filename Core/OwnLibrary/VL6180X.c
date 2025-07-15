#include "vl6180x.h"
#include <stdio.h>

// I2C instantie van je project, standaard hi2c1 (pas aan indien nodig)
extern I2C_HandleTypeDef hi2c1;

#define VL6180X_I2C_ADDR      (0x29 << 1)
#define SYSRANGE_START        0x0018
#define RESULT_RANGE_STATUS   0x004f
#define RESULT_RANGE_VAL      0x0062
#define SYSTEM_INTERRUPT_CLEAR 0x0015
#define RESULT_STATUS_FINAL    0x0063

static VL6180X_State_t sensor = {0};

static bool measurementInProgress = false;
static uint8_t laatsteMeetafstand = 255;



// Init sequentie op basis van datasheet
void VL6180X_SimpleInit(void)
{
    VL6180X_WriteReg(0x0207, 0x01);
    VL6180X_WriteReg(0x0208, 0x01);
    VL6180X_WriteReg(0x0096, 0x00);
    VL6180X_WriteReg(0x0097, 0xfd);
    VL6180X_WriteReg(0x00e3, 0x00);
    VL6180X_WriteReg(0x00e4, 0x04);
    VL6180X_WriteReg(0x00e5, 0x02);
    VL6180X_WriteReg(0x00e6, 0x01);
    VL6180X_WriteReg(0x00e7, 0x03);
    VL6180X_WriteReg(0x00f5, 0x02);
    VL6180X_WriteReg(0x00d9, 0x05);
    VL6180X_WriteReg(0x00db, 0xce);
    VL6180X_WriteReg(0x00dc, 0x03);
    VL6180X_WriteReg(0x00dd, 0xf8);
    VL6180X_WriteReg(0x009f, 0x00);
    VL6180X_WriteReg(0x00a3, 0x3c);
    VL6180X_WriteReg(0x00b7, 0x00);
    VL6180X_WriteReg(0x00bb, 0x3c);
    VL6180X_WriteReg(0x00b2, 0x09);
    VL6180X_WriteReg(0x00ca, 0x09);
    VL6180X_WriteReg(0x0198, 0x01);
    VL6180X_WriteReg(0x01b0, 0x17);
    VL6180X_WriteReg(0x01ad, 0x00);
    VL6180X_WriteReg(0x00ff, 0x05);
    VL6180X_WriteReg(0x0100, 0x05);
    VL6180X_WriteReg(0x00ff, 0x00);
    VL6180X_WriteReg(0x010a, 0x30);
    VL6180X_WriteReg(0x003f, 0x46);
    VL6180X_WriteReg(0x01e3, 0xff);
    VL6180X_WriteReg(0x01e4, 0x01);
    VL6180X_WriteReg(0x01e5, 0x00);

    VL6180X_WriteReg(0x0011, 0x10);
    VL6180X_WriteReg(0x010a, 0x30);
    VL6180X_WriteReg(0x003f, 0x46);
    VL6180X_WriteReg(0x0031, 0xFF);
    VL6180X_WriteReg(0x0041, 0x63);
    VL6180X_WriteReg(0x002e, 0x01);

    VL6180X_WriteReg(0x001b, 0x09);
    VL6180X_WriteReg(0x003e, 0x31);
    VL6180X_WriteReg(0x0014, 0x24);
}

void VL6180X_WriteReg(uint16_t reg, uint8_t val)
{
    HAL_I2C_Mem_Write(&hi2c1, VL6180X_I2C_ADDR, reg, I2C_MEMADD_SIZE_16BIT, &val, 1, HAL_MAX_DELAY);
}

uint8_t VL6180X_ReadReg(uint16_t reg)
{
    uint8_t val;
    HAL_StatusTypeDef status = HAL_I2C_Mem_Read(&hi2c1, VL6180X_ADDR, reg, I2C_MEMADD_SIZE_16BIT, &val, 1, HAL_MAX_DELAY);

    return val;
}

// Initialiseer de state machine
void VL6180X_InitStateMachine(uint8_t mux_channel)
{
    sensor.state = SENSOR_IDLE;
    sensor.channel = mux_channel;
    sensor.result = 0;
    sensor.ready = false;
}

// Geef resultaat als beschikbaar
bool VL6180X_IsReady(void)
{
    return sensor.ready;
}

uint8_t VL6180X_GetResult(void)
{
    sensor.ready = false;
    return sensor.result;
}

// Moet regelmatig aangeroepen worden
void VL6180X_Process(void)
{
    switch (sensor.state)
    {
        case SENSOR_IDLE:
            VL6180X_WriteReg(0x0018, 0x01);  // Start meting
            sensor.startTick = HAL_GetTick();
            sensor.state = SENSOR_WAIT;
            break;

        case SENSOR_WAIT:
            if ((HAL_GetTick() - sensor.startTick) > 20) {
                sensor.state = SENSOR_TIMEOUT;
                break;
            }

            if ((VL6180X_ReadReg(0x004F) & 0x07) == 0x04) {
                sensor.state = SENSOR_DONE;
            }
            break;

        case SENSOR_DONE:
            sensor.result = VL6180X_ReadReg(0x0062);
            VL6180X_WriteReg(0x0015, 0x07); // Clear interrupt
            sensor.ready = true;
            sensor.state = SENSOR_IDLE;
            break;

        case SENSOR_TIMEOUT:
            sensor.result = 255;
            sensor.ready = true;
            sensor.state = SENSOR_IDLE;
            break;

        default:
            sensor.state = SENSOR_IDLE;
            break;
    }
}

uint8_t VL6180X_ReadRange(void)
{
    VL6180X_WriteReg(0x0018, 0x01);  // SYSRANGE__START

    uint8_t status;
    uint32_t timeout = 1000;

    uint32_t startTick = HAL_GetTick();
    while ((VL6180X_ReadReg(0x004F) & 0x07) != 0x04) {
        if (HAL_GetTick() - startTick > 100) {
            return -1;
        }
    }



    uint8_t range = VL6180X_ReadReg(0x0062);  // RESULT__RANGE_VAL

    VL6180X_WriteReg(0x0015, 0x07);          // SYSTEM__INTERRUPT_CLEAR

//    uint8_t rangeStatus = VL6180X_ReadReg(0x0063);
//    if (rangeStatus != 0x00) {
////        printf("Foutstatus: 0x%02X\r\n", rangeStatus);
//    }

    return range;
}
