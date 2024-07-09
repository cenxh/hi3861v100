#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <cmsis_os2.h>
#include "iot_gpio.h"
#include "iot_gpio_ex.h"

#define DS18B20_PIN 13 // 假设DS18B20连接到GPIO 13号引脚

#define DS18B20_SKIP_ROM 0xCC
#define DS18B20_CONVERT_T 0x44
#define DS18B20_READ_SCRATCHPAD 0xBE

// 定义DS18B20初始化、复位和通信函数

void DS18B20_SetPinOpenDrain(void) 
{
    IoTGpioSetDir(DS18B20_PIN, IOT_GPIO_DIR_OUT);
    IoTGpioSetOutputVal(DS18B20_PIN, 1);
    IoTGpioSetPull(DS18B20_PIN, IOT_IO_PULL_UP);
    IoTGpioSetDrv(DS18B20_PIN, IOT_IO_DRIVER_STRENGTH_MAX, IOT_IO_DRIVER_STRENGTH_MAX);
}

void DS18B20_Init(void) 
{
    IoTGpioInit(DS18B20_PIN);
    DS18B20_SetPinOpenDrain();
}

int DS18B20_Reset(void) {
    DS18B20_SetPinOutput();
    IoTGpioSetOutputVal(DS18B20_PIN, 0);
    usleep(480);
    DS18B20_SetPinInput();
    usleep(70);
    IotGpioValue presence;
    IoTGpioGetInputVal(DS18B20_PIN, &presence);
    usleep(410);
    return presence == IOT_GPIO_VALUE0;
}

void DS18B20_WriteByte(uint8_t data) {
    for (int i = 0; i < 8; i++) {
        if (data & 0x01) {
            DS18B20_SetPinOutput();
            IoTGpioSetOutputVal(DS18B20_PIN, 0);
            usleep(2);
            DS18B20_SetPinOpenDrain();
            usleep(70);
        } else {
            DS18B20_SetPinOutput();
            IoTGpioSetOutputVal(DS18B20_PIN, 0);
            usleep(70);
            DS18B20_SetPinOpenDrain();
        }
        data >>= 1;
    }
}

uint8_t DS18B20_ReadByte(void) {
    uint8_t data = 0;
    IotGpioValue val;
    for (int i = 0; i < 8; i++) {
        DS18B20_SetPinOutput();
        IoTGpioSetOutputVal(DS18B20_PIN, 0);
        usleep(2);
        DS18B20_SetPinInput();
        usleep(1);
        IoTGpioGetInputVal(DS18B20_PIN, &val);
        data >>= 1;
        data |= (val << 7);
        usleep(50);
    }
    return data;
}

float DS18B20_ReadTemperature(void) {
    if (DS18B20_Reset() == 0) {
        DS18B20_WriteByte(DS18B20_SKIP_ROM);
        DS18B20_WriteByte(DS18B20_CONVERT_T);
        usleep(750000); // 等待温度转换完成

        if (DS18B20_Reset() == 0) {
            DS18B20_WriteByte(DS18B20_SKIP_ROM);
            DS18B20_WriteByte(DS18B20_READ_SCRATCHPAD);
            
            uint8_t temp_lsb = DS18B20_ReadByte();
            uint8_t temp_msb = DS18B20_ReadByte();
            int16_t temp = (temp_msb << 8) | temp_lsb;
            
            if (temp == 0x0000 || temp == 0xFFFF) {
                return -1000.0; // 表示读取失败
            }
            return temp * 0.0625;
        }
    }
    return -1000.0; // 表示读取失败
}

// 任务函数，每秒读取一次温度
void TemperatureTask(void) {
    DS18B20_Init();
    while (1) {
        float temperature = DS18B20_ReadTemperature();
        if (temperature > -1000.0) {
            printf("Temperature: %.2f°C\n", temperature);
        } else {
            printf("Failed to read temperature from DS18B20\n");
        }
        sleep(1); // 延时一秒
    }
}

// 应用程序入口初始化
static void UART_ExampleEntry(void) 
{
    osThreadAttr_t attr ;
    attr.name = "TemperatureTask";
    attr.attr_bits = 0U;
    attr.cb_mem = NULL;
    attr.cb_size = 0U;
    attr.stack_mem = NULL;
    attr.priority = osPriorityNormal;
    attr.stack_size = 1024;

     if (osThreadNew((osThreadFunc_t)TemperatureTask, NULL, &attr) == NULL) {
        printf("Failed to create TempTask!\n");
    }
}

SYS_RUN(UART_ExampleEntry);
