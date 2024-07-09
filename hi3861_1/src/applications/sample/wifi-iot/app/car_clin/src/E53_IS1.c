#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include "cmsis_os2.h"
#include "E53_IS1.h"
#include "iot_errno.h"
#include "iot_gpio.h"
#include "iot_gpio_ex.h"
#include "iot_pwm.h"
#include "ohos_init.h"
#include "iot_uart.h"
#include "hi_io.h"
#include "hi_pwm.h"



/***************************************************************
* 函数名称: E53_SC2_IO_Init
* 说    明: E53_SC2_GPIO初始化
* 参    数: 无
* 返 回 值: 无
***************************************************************/
static void E53_IS1_IO_Init(void)
{
    //  IoTGpioInit(IOT_IO_NAME_GPIO_10);  
    // hi_io_set_func(IOT_IO_NAME_GPIO_10, HI_IO_FUNC_GPIO_10_PWM1_OUT);
    // IoTGpioSetDir(IOT_IO_NAME_GPIO_10, IOT_GPIO_DIR_OUT);
    //IoTPwmInit(IOT_PWM_PORT_PWM1);
    //PwmInit(IOT_PWM_PORT_PWM1);//初始化PWM5端口
    /*****初始化F1按键，设置为下降沿触发中断*****/
    // IoTGpioInit(IOT_IO_NAME_GPIO_11); 
    // hi_io_set_func(IOT_IO_NAME_GPIO_11, IOT_IO_FUNC_GPIO_11_GPIO);
    // IoTGpioSetDir(IOT_IO_NAME_GPIO_11, IOT_GPIO_DIR_IN);
    //IoSetPull(IOT_IO_NAME_GPIO_11, IOT_IO_PULL_UP);
    
}

/***************************************************************
* 函数名称: E53_IS1_Init
* 说    明: 初始化E53_IS1
* 参    数: 无
* 返 回 值: 无
***************************************************************/
void E53_IS1_Init(void)
{
    E53_IS1_IO_Init();

}
/***************************************************************
* 函数名称: E53_IS1_Read_Data
* 说    明: 读取数据
* 参    数: 无
* 返 回 值: 无
***************************************************************/
void E53_IS1_Read_Data(E53_IS1_CallbackFunc func)
{
	//GpioRegisterIsrFunc(IOT_IO_NAME_GPIO_11, IOT_INT_TYPE_EDGE, IOT_GPIO_EDGE_RISE_LEVEL_HIGH,func, NULL);
}
/***************************************************************
* 函数名称: Beep_StatusSet
* 说    明: 蜂鸣器报警与否
* 参    数: status,ENUM枚举的数据
*									OFF,蜂鸣器
*									ON,开蜂鸣器
* 返 回 值: 无
***************************************************************/
void Beep_StatusSet(E53_IS1_Status_ENUM status)
{
	// if(status == ON)
	// 	PwmStart(IOT_PWM_PORT_PWM1, 20000, 40000); //输出不同占空比的PWM波
	// if(status == OFF)
	// 	PwmStop(IOT_PWM_PORT_PWM1);
}
