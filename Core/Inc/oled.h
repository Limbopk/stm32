#ifndef __OLED_H
#define __OLED_H

#include "stm32l4xx_hal.h"
#include <stdint.h>


#define OLED_WIDTH  128
#define OLED_HEIGHT 128



#define OLED_CMD_DISPLAY_OFF          0xAE  // 关闭
#define OLED_CMD_DISPLAY_ON           0xAF  // 打开
#define OLED_CMD_SET_COLUMN_ADDRESS   0x15  // 列地址
#define OLED_CMD_SET_ROW_ADDRESS      0x75  // 行地址
#define OLED_CMD_WRITE_RAM            0x5C  // 写入
#define OLED_CMD_READ_RAM             0x5D  // 读取

// 初始化
#define OLED_CMD_SET_REMAP_COLOR      0xA0  //颜色
#define OLED_CMD_SET_START_LINE       0xA1  // 起始行
#define OLED_CMD_SET_DISPLAY_OFFSET   0xA2  // 显示偏移
#define OLED_CMD_SET_NORMAL_DISPLAY   0xA6  // 正常
#define OLED_CMD_SET_INVERSE_DISPLAY  0xA7  // 反转

// 点阵数据
extern const uint8_t Font8x8[][8];


void OLED_Refresh(void);
void OLED_SetPos(uint8_t x, uint8_t y);


void OLED_Init(SPI_HandleTypeDef *hspi);           //  初始化
void OLED_SendCommand(uint8_t cmd);
void OLED_SendData(uint8_t *data, uint16_t size);

void OLED_ClearScreen(uint16_t color);            // 清屏
void OLED_DrawPixel(uint8_t x, uint8_t y, uint16_t color); // 像素点
void OLED_DrawChar(uint8_t x, uint8_t y, const uint8_t *char_data, uint16_t color); // 单个字符
void OLED_ShowString(uint8_t x, uint8_t y, char *str, uint16_t color);              // 显示字符串

void OLED_DrawString(uint8_t x, uint8_t y, unsigned char *str, uint16_t color);

#endif
