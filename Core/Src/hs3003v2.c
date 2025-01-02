#include "hs3003v2.h"

// 定义 HS3003 的 I2C 地址（注意左移形成 8 位地址）
#define HS3003_ADDRESS (0x44 << 1)
  // I2C 7-bit 地址

// 定义 HS3003 的寄存器地址
#define HS3003_STATUS_REGISTER 0x06    // 状态字寄存器地址
#define HS3003_HUMIDITY_REGISTER 0x11  // 湿度数据寄存器地址
#define HS3003_TEMPERATURE_REGISTER 0x12  // 温度数据寄存器地址

// 初始化函数：校验状态寄存器是否正常，若未校准，则发送初始化命令
void HS3003_Init(void) {
	printf("HS3003 initialized successfully.\n");
    uint8_t readBuffer[1];   // 用于接收状态字
    uint8_t sendBuffer[2];   // 用于发送初始化命令

    // 等待上电稳定时间（根据手册，0.1 秒即可）
    HAL_Delay(100);

    // 读取状态寄存器
    if (HAL_I2C_Master_Receive(&hi2c1, HS3003_ADDRESS, readBuffer, 1, HAL_MAX_DELAY) != HAL_OK) {
        Error_Handler(); // 错误处理
    }



        if (HAL_I2C_Master_Transmit(&hi2c1, HS3003_ADDRESS, sendBuffer, 2, HAL_MAX_DELAY) != HAL_OK) {
            Error_Handler(); // 错误处理
        }

        // 等待初始化完成
        HAL_Delay(10); // 等待 10ms 完成校准
    }

// 从 HS3003 读取数据（温度和湿度），并转换为浮点数格式
    HS3003_Read(float *temperature, float *humidity) {
    uint8_t readBuffer[4]; // 接收湿度和温度的原始数据
    uint32_t rawHumidity, rawTemperature;

    // 从湿度寄存器开始读取连续的 4 个字节（湿度和温度各 2 字节）
    if (HAL_I2C_Master_Receive(&hi2c1, HS3003_ADDRESS | HS3003_HUMIDITY_REGISTER, readBuffer, 4, HAL_MAX_DELAY) != HAL_OK) {
        return HAL_ERROR; // 读取失败
    }

    // 解析湿度和温度原始数据
    rawHumidity = (readBuffer[0] << 8) | readBuffer[1];     // 湿度高 16 位
    rawTemperature = (readBuffer[2] << 8) | readBuffer[3]; // 温度高 16 位

    // 转换为实际湿度和温度值（根据手册的转换公式）
    *humidity = ((float)rawHumidity / 65536.0f) * 100.0f;    // 湿度百分比
    *temperature = ((float)rawTemperature / 65536.0f) * 165.0f - 40.0f; // 温度（摄氏度）

    return HAL_OK; // 读取成功
}
