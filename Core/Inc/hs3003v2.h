/*
 * hs3003v2.h
 *
 *  Created on: Jan 2, 2025
 *      Author: csl12
 */

#ifndef INC_HS3003V2_H_
#define INC_HS3003V2_H_

#include"i2c.h"

void hs3003_Init(void);
HS3003_Read(float *temperature, float *humidity);

#endif /* INC_HS3003V2_H_ */
