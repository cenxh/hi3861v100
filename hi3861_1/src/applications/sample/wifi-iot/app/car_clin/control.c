#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "io_init.h"
#include "speed_control.h"
#include "E53_IS1.h"
#include "ohos_init.h"
#include "cmsis_os2.h"
#include "iot_pwm.h"
#include "iot_gpio.h"
#include "iot_gpio_ex.h"
#include "iot_uart.h"
#include "iot_errno.h"
#include "hi_uart.h"

#include "hi_io.h"
#include "hi_pwm.h"
#include "hiview_util.h"

#define IOT_UART_IDX_1  (1)
#define IOT_UART_IDX_2  (2)
#define STACK_SIZE   (1024)
#define DELAY_US     (20000)
#define IOT_GPIO_0  (0) //uart1TX
#define IOT_GPIO_1  (1) //uart1RX
#define IOT_GPIO_11  (11) //uart2TX
#define IOT_GPIO_12  (12) //uart2RX

#define DTOF_DATA_LEN       137
#define CRC_LEN             133
#define UART_READ_BUFF_SIZE       256

#define PWM_TASK_STACK_SIZE 512
#define PWM_TASK_PRIO 25

unsigned char uartReadBuff[300] = {0};
unsigned char crcBuff[300] = {0};
static unsigned int crc32table[256];

#define UART_TASK_STACK_SIZE 1024 * 8
#define UART_TASK_PRIO 25
#define UART_BUFF_SIZE 1000
#define FLAGS_MSK1 0x00000001U

// 串口2
static uint8_t WirelessBuff[4];
static int counter = 0;
static int i=0;
uint8_t uart_buff[UART_BUFF_SIZE] = {0};

uint8_t buf[128];
uint16_t bufLen;

uint16_t Uart2_buff[]={0xA2, 0x04, 0xAD, 0x04, 0x3F, 0xD0, 0x01, 0x3C, 0x47, 0x3F, 0xD0, 0x01, 0x3C, 0xD1, 0x01, 0x3C, 0x47, 0x3F, 0x47, 0x3F, 0xD0, 0x01, 0x3C, 0x46, 0x3F, 0x47, 0x3F, 0xD0, 0x01, 0x3C, 0x47, 0x3F, 0x47, 0x3F, 0xD0, 0x01, 0x3C, 0xD1, 0x01, 0x3C, 0x47, 0x3F, 0xD0, 0x01, 0x3C, 0x47, 0x3F, 0xD0, 0x01, 0x3C, 0xD0, 0x01, 0x3C, 0xD0, 0x01, 0x3C, 0xD0, 0x01, 0x3C, 0x47, 0x3F, 0xD0, 0x01, 0x3C, 0xD0, 0x01, 0x3C, 0xD0, 0x01, 0x3C, 0x47, 0x3F, 0x47, 0x3F, 0x47, 0x3F, 0x47, 0x3F, 0xD0, 0x01, 0x3C, 0x47, 0x3F, 0x47, 0x3F, 0xD0, 0x01, 0x3C, 0xD0, 0x01, 0x3C, 0xD0, 0x01, 0x3C, 0x47, 0x3F, 0x47, 0x3F, 0x47, 0x3F, 0x47, 0x3F, 0x47, 0x3F, 0x47, 0x3F, 0x47, 0x3F, 0x46, 0x3F, 0xD0, 0x01, 0x3C, 0xD0, 0x01, 0x3C, 0xD0, 0x01, 0x3C, 0xD0, 0x01, 0x3C, 0xD0, 0x01, 0x3C, 0x90, 0x05, 0xA3, 0x04, 0xAD, 0x04, 0x3F, 0xD0, 0x01, 0x3C, 0x47, 0x3F, 0xD0, 0x01, 0x3C, 0xD0, 0x01, 0x3C, 0x46, 0x3F, 0x47, 0x3F, 0xD0, 0x01, 0x3C, 0x47, 0x3F, 0x47, 0x3F, 0xD0, 0x01, 0x3C, 0x47, 0x3F, 0x47, 0x3F, 0xD0, 0x01, 0x3C, 0xD0, 0x01, 0x3C, 0x47, 0x3F, 0xD0, 0x01, 0x3C, 0x47, 0x3F, 0xD0, 0x01, 0x3C, 0xD0, 0x01, 0x3C, 0xD0, 0x01, 0x3C, 0xD0, 0x01, 0x3C, 0x46, 0x3F, 0xD0, 0x01, 0x3C, 0xD0, 0x01, 0x3C, 0xD0, 0x01, 0x3C, 0x47, 0x3F, 0x47, 0x3F, 0x47, 0x3F, 0x47, 0x3F, 0xD0, 0x01, 0x3D, 0x47, 0x3F, 0x47, 0x3F, 0xD0, 0x01, 0x3C, 0xD0, 0x01, 0x3C, 0xD0, 0x01, 0x3C, 0x47, 0x3F, 0x46, 0x3F, 0x46, 0x3F, 0x47, 0x3F, 0x46, 0x3F, 0x46, 0x3F, 0x46, 0x3F, 0x46, 0x30, 0xD0, 0x01, 0x3D, 0xD0, 0x01, 0x3D, 0xD0, 0x01, 0x3D, 0xD0, 0x01, 0x3D, 0xD0, 0x01, 0x3D};
 

typedef struct Wireless_TypeDef
{
	int Run;
	int Mode;
	int Circle;
	int Crossing;
}Wireless_TypeDef;

Wireless_TypeDef Remote;
osEventFlagsId_t evt_id;

// 在所有函数之前添加全局变量声明
osMutexId_t runMutexId;

/*
 * 初始化uart1接口配置
 * Initialize uart1 interface configuration
 */
int usr_uart_config(void)
{
    IotUartAttribute g_uart_cfg = {115200, 8, 1, IOT_UART_PARITY_NONE, 500, 500, 0};
    int ret = IoTUartInit(IOT_UART_IDX_1, &g_uart_cfg);
    if (ret != 0) 
    {
        printf("uart1 init fail\r\n");
    }

    return ret;
}

int usr_uart2_config(void)
{
    IotUartAttribute g_uart_cfg = {115200, 8, 1, IOT_UART_PARITY_NONE, 500, 500, 0};
    int ret = IoTUartInit(IOT_UART_IDX_2, &g_uart_cfg);
    if (ret != 0) 
    {
        printf("uart2 init fail\r\n");
    }

    return ret;
}

static void init_crc_table(void)
{
    unsigned int c;
    unsigned int i, j;
    for (i = 0; i < 256; i++) 
    {
        c = (unsigned int)i;
        for (j = 0; j < 8; j++) 
        {
            if (c & 1)
                c = 0xedb88320L ^ (c >> 1);
            else
                c = c >> 1;
        }                                                                                                                                            
        crc32table[i] = c;
    }
};

/*
 * crc校验
 * crc checking
 */

static unsigned int crc32(const unsigned char *buf, unsigned int size)
{
    unsigned int  i, crc = 0xFFFFFFFF;

    for (i = 0; i < size; i++) 
    {
        crc = crc32table[(crc ^ buf[i]) & 0xff] ^ (crc >> 8); /* 8: 右移8bit */
    }
    return crc ^ 0xFFFFFFFF;
}

//串口2的数据解析
void WirelessDataDecode(void)
{

     // 确保互斥锁已经创建
    if (runMutexId != NULL)
    {
    // 获取互斥锁
        osMutexAcquire(runMutexId, osWaitForever);
    // 安全地修改共享变量
    // Remote.Run = WirelessBuff[0];
        Remote.Mode = WirelessBuff[0];
        Remote.Circle = WirelessBuff[1];
        Remote.Crossing = WirelessBuff[2];
    // 释放互斥锁
    osMutexRelease(runMutexId);
    }

   

    //const unsigned char value[] = {WirelessBuff[0]};
         
    //UartWrite(IOT_UART_IDX_2, value, sizeof(value)); //发送串口收到的第一个值来判断通信是否成功
}

// 串口2数据接收处理函数
void WirelessData_Get(uint8_t Data) {
    if (counter < sizeof(WirelessBuff)) {  // 确保不会数组越界
        WirelessBuff[counter] = Data;
        counter++;
    }

    // 检查是否已接收完整个数据包
    if (counter == sizeof(WirelessBuff)) {
        WirelessDataDecode();  // 解析数据
        counter = 0;  // 重置计数器，准备接收下一组数据
        memset(WirelessBuff, 0, sizeof(WirelessBuff));  // 清空缓冲区
    }
}

void gpio_uart1_init(void)
{
    //uart1
    IoTGpioInit(IOT_GPIO_0);  
    hi_io_set_func(IOT_GPIO_0, HI_IO_FUNC_GPIO_0_UART1_TXD);
    IoTGpioInit(IOT_GPIO_1);  
    hi_io_set_func(IOT_GPIO_1, HI_IO_FUNC_GPIO_1_UART1_RXD);
}

void gpio_uart2_init(void)
{
     //uart2
    IoTGpioInit(IOT_GPIO_11);  
    hi_io_set_func(IOT_GPIO_11, HI_IO_FUNC_GPIO_11_UART2_TXD);
    IoTGpioInit(IOT_GPIO_12);  
    hi_io_set_func(IOT_GPIO_12, HI_IO_FUNC_GPIO_12_UART2_RXD);
}

static void Beep_Alarm(char *arg)
{
    (void)arg;
    osEventFlagsSet(evt_id, FLAGS_MSK1);
}

static void UART1_TaskEntry(void)
{
    static int r = 0; //统计距离小于0.2米的点云数量，满足条件亮红灯
    static int g = 0; //统计距离在0.4米到1米之间的点云数量，满足条件亮绿灯
    static int y = 0; //统计距离在0.2米到0.4米之间的点云数量，满足条件亮黄灯
    static int d = 0; 
    unsigned short int *data = NULL;
    short int i = 0;

    unsigned int len = 0; //接收到的串口数据长度
    unsigned int crc_data = 0; //接收CRC校验后的值

    gpio_uart1_init();
    usr_uart_config();
    init_crc_table();

    while(1)
    {
        //uart1
        len += IoTUartRead(IOT_UART_IDX_1, uartReadBuff+len, DTOF_DATA_LEN);
        if (len >= DTOF_DATA_LEN) 
        {
            if(uartReadBuff[0] == 0xAA && uartReadBuff[1] == 0x55 && uartReadBuff[132] == 0xFF) 
            {
                memcpy(crcBuff,uartReadBuff,CRC_LEN);
                crc_data = crc32(crcBuff,CRC_LEN);

                if(crc_data == *(unsigned int *)(&uartReadBuff[CRC_LEN])) 
                {
                    for(i = 0; i < 64; i++) 
                    {
                        data = (unsigned short int *)(&uartReadBuff[i*2+4]);
                        if((*data) < 200)
                        {
                            r += 1;
                        }
                        else if ((*data) >= 200 && (*data) < 400)
                        {
                            y += 1;
                        }
                        else if((*data) >= 400 && (*data)  < 1000)
                        {
                            g += 1;
                        }

                    }
                    d = d + 1;
                    if (r == 64)
                    { 
                        //IoTPwmStart(HI_PWM_PORT_PWM2, 45, 4000);
                        //Remote.Run=1;
                        if((*data) >= 100 && (*data) < 150)
                        {
                            d = 0;
                        }
                        else if((*data) >= 150 && (*data) < 200 && d >= 3)
                        {
                            d = 0;
                        }
                         // 确保互斥锁已经创建
                         if (runMutexId != NULL)
                        {
                            // 获取互斥锁
                            osMutexAcquire(runMutexId, osWaitForever);
                            // 安全地修改共享变量
                            Remote.Run = 1;
                            // 释放互斥锁
                           osMutexRelease(runMutexId);
                         }
                    }
                    else if(r != 64)
                    {
                         // 确保互斥锁已经创建
                         if (runMutexId != NULL)
                        {
                            // 获取互斥锁
                            osMutexAcquire(runMutexId, osWaitForever);
                            // 安全地修改共享变量
                            Remote.Run = 0;
                            // 释放互斥锁
                           osMutexRelease(runMutexId);
                         }

                    }
                    else if(y >= 32)
                    {
                        //IoTPwmStart(HI_PWM_PORT_PWM2, 10, 4000);

                        if((*data) >= 200 && (*data) < 300 && d >= 6)
                        {
                            d = 0;
                        }
                        else if((*data) >= 300 && (*data) < 400 && d >= 9)
                        {
                            d = 0;
                        }
                        else if((*data) >= 400 && (*data) < 500 && d >= 12)
                        {
                            d = 0;
                        }
                    }
                    else if (g >= 16)
                    {
                        
                    }
                    else
                    {
                    }
                }
                else
                {
                    printf("crc32 fail !!!\r\n");
                }
            }
            len = 0;
            crc_data = 0;
           // printf("Uart read data:r = %d,y = %d,g = %d\r\n", r,y,g);
            //printf("Remote.Run= %d",Remote.Run);
            r = y = g = 0;
            data = NULL;
            memset(uartReadBuff,0,sizeof(uartReadBuff));
            memset(crcBuff,0,sizeof(crcBuff));
        }
        usleep(DELAY_US);
        if(len == 0)
        {
            crc_data = 0;
            //printf("Uart read data:r = %d,y = %d,g = %d\r\n", r,y,g);
            r = y = g = 0;
            data = NULL;
            memset(uartReadBuff,0,sizeof(uartReadBuff));
            memset(crcBuff,0,sizeof(crcBuff));
        }

    }

}

//求校验和
static uint8_t Get_Check(uint8_t *data, uint16_t len) {
	uint8_t sum = 0;
	uint16_t i;
	for (i = 0; i < len; i++) {
		sum += data[i];
	}
	return sum;
}

uint16_t IR_Send_Pack(uint8_t *data,uint8_t index) {
	uint8_t *p = data;
	*p++ = 0x68;
	*p++ = 0x08;
	*p++ = 0x00;
	*p++ = 0x00;
	*p++ = 0x12;
	*p++ = index;
	*p = Get_Check(&data[3], p - data - 3);
	p++;
	*p++ = 0x16;
	return p - data;
}

static void UART2_TaskEntry(void)
{
    unsigned int len2 = 0; //接收到的串口数据长度

    gpio_uart2_init();
    usr_uart2_config();

    while (1)
    {
        len2 = IoTUartRead(IOT_UART_IDX_2, uart_buff, sizeof(uart_buff));  // 读取数据
    if (len2 > 0) {
        for (uint32_t i = 0; i < len2; i++) {
            WirelessData_Get(uart_buff[i]);  // 处理接收到的每个字节
        }
    }
    
        //IoTUartWrite(IOT_UART_IDX_2, (uint8_t *)&Remote.Circle, sizeof(Remote.Circle));
    }

}

void PWM_Task(void)
{

    PWM_init();
    E53_IS1_Init();
    E53_IS1_Read_Data(Beep_Alarm);
    while (1)
    {

       if(Remote.Circle == 6)
       {
        if(i==0)
        {
         bufLen = IR_Send_Pack(buf,0x00);//关
         //SendInfraredCode(irCode, irCodeSize);
        IoTUartWrite(IOT_UART_IDX_2, buf, bufLen); // 发送 WirelessBuff[0]
        i=1;
        }
       }
       else if(Remote.Circle == 5)
       {
        if(i==0)
        {
         bufLen = IR_Send_Pack(buf,0x01);//开
         IoTUartWrite(IOT_UART_IDX_2, buf, bufLen); // 发送 WirelessBuff[0]
         i=1;
        }
       }
        
        IoTPwmStart(HI_PWM_PORT_PWM2, 55, 4000);
        IoTPwmStart(HI_PWM_PORT_PWM1, 60, 4000);
        IoTPwmStart(HI_PWM_PORT_PWM5, 60, 4000);
        // 获取互斥锁
        osMutexAcquire(runMutexId, osWaitForever);
        //没识别到门
        //Remote.Mode == '0'
        //if(Remote.Run == 1 && Remote.Mode == '0' )
        if(Remote.Mode == '4' )//直走
       {
           // 安全地访问共享变量
           LF_control_due();
           RF_control_due();
           LB_control_due();
           RB_control_due();
       }
    //    if(Remote.Run == 0)
    //    {
         //识别到门&&识别到数字1
           if(Remote.Mode == '1') //左转
          {
           // 安全地访问共享变量
           LF_control_opposite();
           RF_control_due();
           LB_control_due();
           RB_control_opposite();
          }
           //识别到门&&没识别到数字
       else if(Remote.Mode == '2')//右转
       {
           // 安全地访问共享变量
           RF_control_opposite();
           LF_control_due();
           RB_control_due();
           LB_control_opposite();
       }
       else if(Remote.Mode == '3')//右转 
       {
            RF_control_stop();
           LF_control_stop();
           RB_control_stop();
           LB_control_stop();
       }
    //    }
       
      
          // 释放互斥锁
        osMutexRelease(runMutexId);
    }
}

// 其他代码保持不变

static void UART_ExampleEntry(void)
{

     // 创建互斥锁
    runMutexId = osMutexNew(NULL);
    if (runMutexId == NULL)
    {
        printf("Failed to create run Mutex!\n");
    }

    osThreadAttr_t attr;
   //串口1
    attr.name = "UART1_TaskEntry";
    attr.attr_bits = 0U;
    attr.cb_mem = NULL;
    attr.cb_size = 0U;
    attr.stack_mem = NULL;
    attr.stack_size = UART_TASK_STACK_SIZE;
    attr.priority = UART_TASK_PRIO;

    if (osThreadNew((osThreadFunc_t)UART1_TaskEntry, NULL, &attr) == NULL)
    {
        printf("[ADCExample] Falied to create UART1_TaskEntry!\n");
    }
    //串口2
    attr.stack_size = UART_TASK_STACK_SIZE;
    attr.priority = UART_TASK_PRIO;
    attr.name = "UART2_TaskEntry";
    if (osThreadNew((osThreadFunc_t)UART2_TaskEntry, NULL, &attr) == NULL) {
        printf("Failed to create UART2_TaskEntry!\n");
    }

    //PWM
    attr.stack_size = PWM_TASK_STACK_SIZE;
    attr.priority = PWM_TASK_PRIO;
    attr.name = "PWM_Task";
    if (osThreadNew((osThreadFunc_t)PWM_Task, NULL, &attr) == NULL) {
        printf("Failed to create PWM_Task!\n");
    }
}

APP_FEATURE_INIT(UART_ExampleEntry);
