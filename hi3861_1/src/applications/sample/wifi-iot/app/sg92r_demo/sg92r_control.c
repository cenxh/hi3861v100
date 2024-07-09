/*
 * Copyright (c) 2020 Nanjing Xiaoxiongpai Intelligent Technology Co., Ltd.
 * Copyright (c) 2022 HiSilicon (Shanghai) Technologies CO., LIMITED.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "ohos_init.h"
#include "cmsis_os2.h"
#include "iot_gpio.h"
#include "hi_io.h"
#include "iot_gpio_ex.h"
#include "iot_watchdog.h"
#include "hi_time.h"
#include "iot_uart.h"
#include "hi_uart.h"
#include "iot_errno.h"
#include "E53_IS1.h"
#include "temperature.h"
#include "lwip/sockets.h"
#include "wifi_connect.h"

#define WIFI_ACCOUNT "nova"
#define WIFI_PASSWD "13515072007"
#define PORT 8888

int sock_fd; // 修改为单个变量，因为我们只需要一个socket

char recvBuff[512];
const char *response = "'1'"; // 常量字符串

#define UART_BUFF_SIZE 4
#define COUNT 10
#define FREQ_TIME 20000
#define TASK_STACK_SIZE (1024 * 8)
#define TASK_PRIO 25
#define TASK_DELAY_3S 300
#define FLAGS_MSK1 0x00000005U

osEventFlagsId_t g_eventFlagsId;



void Uart1GpioInit(void)
{
    IoTGpioInit(IOT_IO_NAME_GPIO_0);
    IoSetFunc(IOT_IO_NAME_GPIO_0, IOT_IO_FUNC_GPIO_0_UART1_TXD);
    IoTGpioInit(IOT_IO_NAME_GPIO_1);
    IoSetFunc(IOT_IO_NAME_GPIO_1, IOT_IO_FUNC_GPIO_1_UART1_RXD);
}

void Uart1Config(void)
{
    uint32_t ret;
    IotUartAttribute uart_attr = {
        .baudRate = 115200,
        .dataBits = 8,
        .stopBits = 1,
        .parity = 0,
    };
    ret = IoTUartInit(HI_UART_IDX_1, &uart_attr);
    if (ret != IOT_SUCCESS) {
        printf("Init Uart1 Failed Error No : %d\n", ret);
        return;
    }
}

void SetAngle(unsigned int duty)
{
    unsigned int time = FREQ_TIME;
    IoTGpioSetOutputVal(IOT_IO_NAME_GPIO_2, IOT_GPIO_VALUE1);
    hi_udelay(duty);
    IoTGpioSetOutputVal(IOT_IO_NAME_GPIO_2, IOT_GPIO_VALUE0);
    hi_udelay(time - duty);
}

void RegressMiddle(void)
{
    unsigned int angle = 1500;
    for (int i = 0; i < COUNT; i++) {
        SetAngle(angle);
    }
}

void EngineTurnLeft(void)
{
    unsigned int angle = 500;
    for (int i = 0; i < COUNT; i++) {
        SetAngle(angle);
    }
}

void EngineTurnRight(void)
{
    unsigned int angle = 2500;
    for (int i = 0; i < COUNT; i++) {
        SetAngle(angle);
    }
}

void EngineLeftFront(void)
{
    unsigned int angle = 1000;
    for (int i = 0; i < COUNT; i++) {
        SetAngle(angle);
    }
}

void EngineRightFront(void)
{
    unsigned int angle = 2000;
    for (int i = 0; i < COUNT; i++) {
        SetAngle(angle);
    }
}

void S92RInit(void)
{
    IoTGpioInit(IOT_IO_NAME_GPIO_2);
    IoSetFunc(IOT_IO_NAME_GPIO_2, IOT_IO_FUNC_GPIO_2_GPIO);
    IoTGpioSetDir(IOT_IO_NAME_GPIO_2, IOT_GPIO_DIR_OUT);
}

//任务函数
void TemperatureTask(void *arg)
{
    (void)arg;
    while (1)
    {
        DS18B20_Read_Temperature();
        hi_udelay(1000000); // 每秒采样一次
    }
}

static void BeepAlarm(char *arg)
{
    (void)arg;
    osEventFlagsSet(g_eventFlagsId, FLAGS_MSK1);
}

static void ExampleTask(void)
{
    int ret;
    E53IS1Init();
    ret = E53IS1ReadData(BeepAlarm);
    if (ret != 0) {
        printf("E53_IS1 Read Data failed!\r\n");
        return;
    }
    while (1) {
        osEventFlagsWait(g_eventFlagsId, FLAGS_MSK1, osFlagsWaitAny, osWaitForever);
        BeepStatusSet(OFF);
        osDelay(TASK_DELAY_3S);
        BeepStatusSet(OFF);
    }
}
static void UDPServerTask(void) {
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    ssize_t ret;

    // 初始化WiFi连接
    WifiConnect(WIFI_ACCOUNT, WIFI_PASSWD);

    // 创建UDP socket
    if ((sock_fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket creation failed");
        exit(1);
    }

    // 设置服务器地址信息
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(PORT);

    // 绑定socket
    if (bind(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind failed");
        exit(1);
    }

    printf("UDP server is running, waiting for messages...\n");

    float temperature;
    char tempStr[32]; // 用于存储温度字符串

    while (1) { // 主循环处理接收和发送
        // 清理接收缓冲区
        memset(recvBuff, 0, sizeof(recvBuff));

        // 接收数据
        ret = recvfrom(sock_fd, recvBuff, sizeof(recvBuff), 0, (struct sockaddr *)&client_addr, &client_len);
        if (ret == -1) {
            perror("recvfrom failed");
            continue; // 如果接收失败，跳过本次循环
        }

        // 打印客户端信息和接收到的消息
        printf("Received message from %s:%d: %s\n",
               inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), recvBuff);

                 // 读取温度
        temperature = DS18B20_Read_Temperature();
        // 将温度格式化为字符串，例如 "23.45"
        snprintf(tempStr, sizeof(tempStr), "%.2f", temperature);
        
        // 发送温度响应
        sendto(sock_fd, tempStr, strlen(tempStr) + 1, 0, (struct sockaddr *)&client_addr, client_len);

        // // 发送响应
        // sendto(sock_fd, response, strlen(response) + 1, 0, (struct sockaddr *)&client_addr, client_len);
    }

    // 关闭socket（实际中，这里可能不会执行，因为while(1)是无限循环）
    close(sock_fd);
}


void Sg92RTask(void)
{
    unsigned int time = 200;
    uint32_t len = 0;
    unsigned char uartReadBuff[UART_BUFF_SIZE] = {'1'};
    Uart1GpioInit();
    Uart1Config();
    S92RInit();

    RegressMiddle();
    TaskMsleep(time);

    while (1) {
        len = IoTUartRead(HI_UART_IDX_1, uartReadBuff, UART_BUFF_SIZE);
        if (len > 0) {
            IoTUartWrite(HI_UART_IDX_1, uartReadBuff, UART_BUFF_SIZE);
            switch (uartReadBuff[0]) {
                case '1':
                    EngineTurnLeft();
                    break;
                case '2':
                    EngineLeftFront();
                    break;
                case '3':
                    EngineTurnRight();
                    break;
                case '4':
                    EngineRightFront();
                    break;
                
                default:
                    break;
            }
            TaskMsleep(time);
        }
    }
}

void CombinedEntry(void)
{
    g_eventFlagsId = osEventFlagsNew(NULL);
    if (g_eventFlagsId == NULL) {
        printf("Failed to create EventFlags!\n");
    }
    osThreadAttr_t attr;
    IoTWatchDogDisable();

    attr.name = "ExampleTask";
    attr.attr_bits = 0U;
    attr.cb_mem = NULL;
    attr.cb_size = 0U;
    attr.stack_mem = NULL;
    attr.stack_size = TASK_STACK_SIZE;
    attr.priority = TASK_PRIO;
    if (osThreadNew((osThreadFunc_t)ExampleTask, NULL, &attr) == NULL) {
        printf("Failed to create ExampleTask!\n");
    }

    attr.name = "Sg92RTask";
    attr.stack_size = 1024 * 5; // 堆栈大小为1024*5 stack size 5*1024
    attr.priority = osPriorityNormal;
    if (osThreadNew((osThreadFunc_t)Sg92RTask, NULL, &attr) == NULL) {
        printf("[Sg92RTask] Failed to create Sg92RTask!\n");
    }

    DS18B20_Init();

    attr.name = "TemperatureTask";
    attr.priority = osPriorityNormal;
    attr.stack_size = 1024;

    if (osThreadNew(TemperatureTask, NULL, &attr) == NULL)
    {
        printf("Failed to create TemperatureTask!\n");
    }

    memset(&attr, 0, sizeof(attr)); // 初始化线程属性
    attr.name = "UDPServerTask";
    attr.stack_size = 10240;
    attr.priority = osPriorityNormal; // 设置合适的线程优先级

    if (osThreadNew((osThreadFunc_t)UDPServerTask, NULL, &attr) == NULL) {
        printf("Failed to create UDPServerTask.\n");
    }


}

APP_FEATURE_INIT(CombinedEntry);