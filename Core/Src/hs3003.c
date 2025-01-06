#include "hs3003.h"

#include "i2c.h"
#define HS3003_ADDRESS 0x88
void HS3003_Init(void) {

    printf("HS3003 initialized successfully.\n");
}
void HS3003_Read(float *temperature, float *humidity) {
    uint8_t readBuffer[4];
    uint16_t rawHumidity, rawTemperature;


    HAL_I2C_Master_Receive(&hi2c1, HS3003_I2C_ADDR << 1, readBuffer, 4, HAL_MAX_DELAY);


    rawHumidity = (readBuffer[0] << 8) | readBuffer[1];
    rawTemperature = (readBuffer[2] << 8) | readBuffer[3];

    *humidity = 100.0f * rawHumidity / 65535.0f;
    *temperature = -45.0f + 175.0f * rawTemperature / 65535.0f;
}
