#include "oled.h"

uint8_t oled_ram[128 * 8]; // 128列 * 8页（每页代表8行像素）
static SPI_HandleTypeDef *OLED_SPI_Handle;

// 宏定义
#define OLED_CS_LOW()   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET)
#define OLED_CS_HIGH()  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET)
#define OLED_DC_LOW()   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET)
#define OLED_DC_HIGH()  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET)



void OLED_SendCommand(uint8_t cmd) {
    OLED_CS_LOW();
    OLED_DC_LOW(); // 设置为命令模式
    HAL_SPI_Transmit(OLED_SPI_Handle, &cmd, 1, HAL_MAX_DELAY);
    OLED_CS_HIGH();
}


void OLED_SendData(uint8_t *data, uint16_t size) {
    OLED_CS_LOW();
    OLED_DC_HIGH(); // 设置数据
    HAL_SPI_Transmit(OLED_SPI_Handle, data, size, HAL_MAX_DELAY);
    OLED_CS_HIGH();
}


void OLED_Init(SPI_HandleTypeDef *hspi) {
    OLED_SPI_Handle = hspi;

    OLED_SendCommand(0xAE); // 关闭显示
    HAL_Delay(100);

    OLED_SendCommand(0x15); // 列
    uint8_t columnArgs[] = {0x00, OLED_WIDTH - 1};
    OLED_SendData(columnArgs, sizeof(columnArgs));

    OLED_SendCommand(0x75); // 行
    uint8_t rowArgs[] = {0x00, OLED_HEIGHT - 1};
    OLED_SendData(rowArgs, sizeof(rowArgs));

    OLED_SendCommand(0xA0); // 重映射
    OLED_SendCommand(0x74); // RGB
    OLED_SendCommand(0xA1); // 起始行
    OLED_SendCommand(0x00);
    OLED_SendCommand(0xA2); // 偏移
    OLED_SendCommand(0x00);
    OLED_SendCommand(0xAF); // 开启显示
    HAL_Delay(100);

    OLED_ClearScreen(0x0000); // 清屏
}

// 清屏（批量操作）
void OLED_ClearScreen(uint16_t color) {
    OLED_SendCommand(0x15); // 列
    OLED_SendCommand(0x00);
    OLED_SendCommand(0x7F);

    OLED_SendCommand(0x75); // 行
    OLED_SendCommand(0x00);
    OLED_SendCommand(0x7F);

    OLED_SendCommand(0x5C); // 写入内存

    uint8_t colorData[256 * 2];
    for (uint16_t i = 0; i < 256; i++) {
        colorData[i * 2] = color >> 8; // 高
        colorData[i * 2 + 1] = color & 0xFF; // 低
    }

    for (uint16_t i = 0; i < 128 * 128 / 256; i++) {
        OLED_SendData(colorData, sizeof(colorData));
    }
}

// 绘制单个像素点
void OLED_DrawPixel(uint8_t x, uint8_t y, uint16_t color) {
    OLED_SendCommand(0x15); // 列
    OLED_SendCommand(x);
    OLED_SendCommand(x);

    OLED_SendCommand(0x75); // 行
    OLED_SendCommand(y);
    OLED_SendCommand(y);

    uint8_t data[] = {color >> 8, color & 0xFF}; // 高低字节
    OLED_SendCommand(0x5C); // 写入内存
    OLED_SendData(data, sizeof(data));
}

// 绘制
void OLED_DrawChar(uint8_t x, uint8_t y, const uint8_t *char_data, uint16_t color) {
    for (uint8_t i = 0; i < 8; i++) {
        for (uint8_t j = 0; j < 8; j++) {
            if (char_data[i] & (1 << (7 - j))) {
                OLED_DrawPixel(x + j, y + i, color);
            }
        }
    }
}

// 显示
void OLED_ShowString(uint8_t x, uint8_t y, char *str, uint16_t color) {
    while (*str) {
        OLED_DrawChar(x, y, Font8x8[*str - ' '], color);
        x += 8; // 宽度8像素
        if (x >= OLED_WIDTH) { // 换行
            x = 0;
            y += 8;
        }
        str++;
    }
}

void OLED_Refresh(void) {

    for (uint8_t page = 0; page < 8; page++) {
        OLED_SetPos(0, page);
        for (uint8_t col = 0; col < 128; col++) {
            OLED_SendData(&oled_ram[page * 128 + col], 1);
        }
    }
}
void OLED_DrawString(uint8_t x, uint8_t y, unsigned char *str, uint16_t color) {
    while (*str) {
        OLED_DrawChar(x, y, *str, color);
        x += 8;
        if (x >= OLED_WIDTH) {  // 超出屏幕宽度换行
            x = 0;
            y += 8;
        }
        str++;
    }
}

void OLED_SetPos(uint8_t x, uint8_t y) {
    OLED_SendCommand(0x15);
    OLED_SendData((uint8_t[]){x, 127}, 2);
    OLED_SendCommand(0x75);
    OLED_SendData((uint8_t[]){y, 127}, 2);
}
