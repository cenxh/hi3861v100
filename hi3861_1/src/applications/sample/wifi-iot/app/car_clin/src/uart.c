// // #include <stdio.h>
// // #include <string.h>
// // #include <unistd.h>
// // #include <stdint.h>

// // #include "ohos_init.h"
// // #include "cmsis_os2.h"
// // #include "wifiiot_errno.h"
// // #include "wifiiot_gpio.h"
// // #include "wifiiot_gpio_ex.h"
// // #include "wifiiot_adc.h"
// // #include "wifiiot_uart.h"
// // #include "hi_isr.h"
// // #include "hi3861.h"
// // #include "uart.h"


// // #define UART_TASK_STACK_SIZE 1024 * 8
// // #define UART_TASK_PRIO 25
// // #define UART_BUFF_SIZE 1000

// // static unsigned char counter,sign;
// // static unsigned char  Wireless[6],WirelessBuff[6];

// // Wireless_TypeDef Remote;


// // // 全局变量用于接收串口数据
// // static uint8_t WirelessBuff[4];
// // static int counter = 0;

// // void WirelessDataDecode(void)
// // {
// //     // 解析接收到的数据，假设在这里完成了数据的处理
// //     Remote.Run=WirelessBuff[1];
// // }
// // void WirelessData_Get(float Data)
// // {
// //     WirelessBuff[counter] = Data;
// //     counter++;
// //     if (counter == 4)
// //     {
// //         // 处理接收到的数据
// //         counter = 0;
// //         WirelessDataDecode();
// //     }
// // }

// // void UART_init(void)
// // {
// //     uint8_t uart_buff[UART_BUFF_SIZE] = {0};
// //     uint32_t ret;

// //     WifiIotUartAttribute uart_attr = {
// //         .baudRate = 115200,
// //         .dataBits = 8,
// //         .stopBits = 1,
// //         .parity = 0,
// //     };

// //     ret = UartInit(WIFI_IOT_UART_IDX_1, &uart_attr, NULL);
// //     if (ret != WIFI_IOT_SUCCESS)
// //     {
// //         printf("Failed to init uart! Err code = %d\n", ret);
// //         return;
// //     }

// // }

// #include <stdio.h>
// #include <unistd.h>
// #include <string.h>

// #include "io_init.h"
// #include "speed_control.h"
// #include "E53_IS1.h"
// //#include "uart.h"
// #include "ohos_init.h"
// #include "cmsis_os2.h"
// #include "wifiiot_pwm.h"
// #include "wifiiot_gpio.h"
// #include "wifiiot_gpio_ex.h"
// #include "wifiiot_uart.h"

// #include "wifiiot_errno.h"
// #include "wifiiot_adc.h"

// #define UART_BUFF_SIZE 1000
// #define TASK_STACK_SIZE 1024 * 8
// #define TASK_PRIO 25
// #define FLAGS_MSK1 0x00000001U

// typedef struct Wireless_TypeDef
// {
// 	int Run;
// 	int Mode;
// 	int Circle;
// 	int Crossing;
// }Wireless_TypeDef;

// osEventFlagsId_t evt_id;
// Wireless_TypeDef Remote;
// //static const char *data = "Hello, BearPi!\r\n";

// // 全局变量用于接收串口数据
// static uint8_t WirelessBuff[4];
// static int counter = 0;

// void WirelessDataDecode(void)
// {
//     // 解析接收到的数据，假设在这里完成了数据的处理
//     Remote.Run=WirelessBuff[1];
// }
// void WirelessData_Get(float Data)
// {
//     WirelessBuff[counter] = Data;
//     counter++;
//     if (counter == 4)
//     {
//         // 处理接收到的数据
//         counter = 0;
//         WirelessDataDecode();
//     }
// }

// static void Beep_Alarm(char *arg)
// {
//     (void)arg;
//     osEventFlagsSet(evt_id, FLAGS_MSK1);
// }

//  void PWMTask(void)
// {
//     uint8_t uart_buff[UART_BUFF_SIZE] = {0};
//     uint32_t ret;

//     PWM_init();
//     E53_IS1_Init();
//     E53_IS1_Read_Data(Beep_Alarm);

//     WifiIotUartAttribute uart_attr = {
//         .baudRate = 9600,
//         .dataBits = 8,
//         .stopBits = 1,
//         .parity = 0,
//     };

//     ret = UartInit(WIFI_IOT_UART_IDX_1, &uart_attr, NULL);
//     if (ret != WIFI_IOT_SUCCESS)
//     {
//         printf("Failed to init uart! Err code = %d\n", ret);
//         return;
//     }
//     printf("UART Test Start\n");

//     while (1)
//     {
//         PwmStart(WIFI_IOT_PWM_PORT_PWM2, 3000, 6000);
//         osEventFlagsWait(evt_id, FLAGS_MSK1, osFlagsWaitAny, osWaitForever);
//         Beep_StatusSet(OFF);
//         osDelay(300);
//         Beep_StatusSet(OFF);

//         // 接收串口数据
//         ret = UartRead(WIFI_IOT_UART_IDX_1, uart_buff, UART_BUFF_SIZE);
//         if (ret > 0)
//         {
//             // 处理接收到的数据
//             for (uint32_t i = 0; i < ret; i++)
//             {
//                 // 将接收到的数据转换为浮点数，并调用处理函数
//                 float received_data = (float)uart_buff[i];
//                 WirelessData_Get(received_data);
//             }
//         }

//         // 发送数据
//         // 假设你想要将接收到的数据再发送回去
//         ret = UartWrite(WIFI_IOT_UART_IDX_1, uart_buff, ret);
//         if (ret != WIFI_IOT_SUCCESS)
//         {
//             printf("Failed to write data to UART! Err code = %d\n", ret);
//         }

//         if(Remote.Run==1)
//         {
//             LF_control_due();
//         }
//         else if(Remote.Run==2)
//         {
//             LF_control_opposite();
//         }
//     }
// }

// void PWMExampleEntry(void)
// {
//     osThreadAttr_t attr;

//     evt_id = osEventFlagsNew(NULL);
//     if (evt_id == NULL)
//     {
//         printf("Falied to create EventFlags!\n");
//     }

//     attr.name = "PWMTask";
//     attr.attr_bits = 0U;
//     attr.cb_mem = NULL;
//     attr.cb_size = 0U;
//     attr.stack_mem = NULL;
//     attr.stack_size = TASK_STACK_SIZE;
//     attr.priority = 25;

//     if (osThreadNew((osThreadFunc_t)PWMTask, NULL, &attr) == NULL)
//     {
//         printf("Falied to create Example_Task!\n");
//     }
// }

// APP_FEATURE_INIT(PWMExampleEntry);

