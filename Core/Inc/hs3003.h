
#ifndef INC_HS3003V1_H_
#define INC_HS3003V1_H_
#ifndef HS3003_I2C_ADDR
#define HS3003_I2C_ADDR 0x44
#endif


#include "i2c.h"


void HS3003_Init(void);
void HS3003_Read(float *temperature, float *humidity);


#endif /* INC_HS3003V1_H_ */
