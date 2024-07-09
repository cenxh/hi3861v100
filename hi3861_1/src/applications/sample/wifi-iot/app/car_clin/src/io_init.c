//引脚：2、3、4、5、6、8、9、10、11、14
#include <stdio.h>
#include <unistd.h>

#include "ohos_init.h"
#include "cmsis_os2.h"
#include "iot_pwm.h"
#include "iot_gpio.h"
#include "iot_gpio_ex.h"
#include "iot_uart.h"
#include "hi_io.h"
#include "hi_pwm.h"
#include "io_init.h"

void PWM_init(void)
{
    //初始化GPIO
    //GpioInit();
    //1号电机（2，5，11）
    //设置GPIO_2引脚复用功能为PWM
    IoTGpioInit(IOT_IO_NAME_GPIO_2);
    hi_io_set_func(IOT_IO_NAME_GPIO_2, HI_IO_FUNC_GPIO_2_PWM2_OUT);
    IoTGpioSetDir(IOT_IO_NAME_GPIO_2, IOT_GPIO_DIR_OUT);
    IoTPwmInit(HI_PWM_PORT_PWM2);
    //设置GPIO_5的复用功能为普通GPIO
    IoTGpioInit(IOT_IO_NAME_GPIO_5);
    hi_io_set_func(IOT_IO_NAME_GPIO_5, HI_IO_FUNC_GPIO_5_GPIO);
    IoTGpioSetDir(IOT_IO_NAME_GPIO_5, IOT_GPIO_DIR_OUT);
     //设置GPIO_11的复用功能为普通GPIO
    IoTGpioInit(IOT_IO_NAME_GPIO_7);
    hi_io_set_func(IOT_IO_NAME_GPIO_7, HI_IO_FUNC_GPIO_7_GPIO);
    IoTGpioSetDir(IOT_IO_NAME_GPIO_7, IOT_GPIO_DIR_OUT);

    IoTGpioSetOutputVal(IOT_IO_NAME_GPIO_5, IOT_GPIO_VALUE0);
    IoTGpioSetOutputVal(IOT_IO_NAME_GPIO_7, IOT_GPIO_VALUE0);

    //2号电机
    //设置GPIO_4引脚复用功能为PWM
    IoTGpioInit(IOT_IO_NAME_GPIO_4);
    hi_io_set_func(IOT_IO_NAME_GPIO_4, HI_IO_FUNC_GPIO_4_PWM1_OUT);
    IoTGpioSetDir(IOT_IO_NAME_GPIO_4, IOT_GPIO_DIR_OUT);
    IoTPwmInit(HI_PWM_PORT_PWM1);
    //设置GPIO_5的复用功能为普通GPIO
    IoTGpioInit(IOT_IO_NAME_GPIO_8);
    hi_io_set_func(IOT_IO_NAME_GPIO_8, HI_IO_FUNC_GPIO_8_GPIO);
    IoTGpioSetDir(IOT_IO_NAME_GPIO_8, IOT_GPIO_DIR_OUT);
    //设置GPIO_11的复用功能为普通GPIO
    IoTGpioInit(IOT_IO_NAME_GPIO_10);
    hi_io_set_func(IOT_IO_NAME_GPIO_10, HI_IO_FUNC_GPIO_10_GPIO);
    IoTGpioSetDir(IOT_IO_NAME_GPIO_10, IOT_GPIO_DIR_OUT);

    IoTGpioSetOutputVal(IOT_IO_NAME_GPIO_8, IOT_GPIO_VALUE0);
    IoTGpioSetOutputVal(IOT_IO_NAME_GPIO_10, IOT_GPIO_VALUE0);


    //3号电机
    //设置GPIO_3引脚复用功能为PWM
    IoTGpioInit(IOT_IO_NAME_GPIO_3);
    hi_io_set_func(IOT_IO_NAME_GPIO_3, HI_IO_FUNC_GPIO_3_PWM5_OUT);
    IoTGpioSetDir(IOT_IO_NAME_GPIO_3, IOT_GPIO_DIR_OUT);
    IoTPwmInit(HI_PWM_PORT_PWM5);
    //设置GPIO_5的复用功能为普通GPIO
    IoTGpioInit(IOT_IO_NAME_GPIO_9);
    hi_io_set_func(IOT_IO_NAME_GPIO_9, HI_IO_FUNC_GPIO_9_GPIO);
    IoTGpioSetDir(IOT_IO_NAME_GPIO_9, IOT_GPIO_DIR_OUT);
    //设置GPIO_11的复用功能为普通GPIO
    IoTGpioInit(IOT_IO_NAME_GPIO_14);
    hi_io_set_func(IOT_IO_NAME_GPIO_14, HI_IO_FUNC_GPIO_14_GPIO);
    IoTGpioSetDir(IOT_IO_NAME_GPIO_14, IOT_GPIO_DIR_OUT);

    IoTGpioSetOutputVal(IOT_IO_NAME_GPIO_9, IOT_GPIO_VALUE0);
    IoTGpioSetOutputVal(IOT_IO_NAME_GPIO_14, IOT_GPIO_VALUE0);

   //4号电机
     //设置GPIO_7引脚复用功能为PWM
    // IoTGpioInit(IOT_IO_NAME_GPIO_7);
    // hi_io_set_func(IOT_IO_NAME_GPIO_7, HI_IO_FUNC_GPIO_7_PWM0_OUT);
    // IoTGpioSetDir(IOT_IO_NAME_GPIO_7, IOT_GPIO_DIR_OUT);
    // IoTPwmInit(HI_PWM_PORT_PWM0);
    //设置GPIO_5的复用功能为普通GPIO
    IoTGpioInit(IOT_IO_NAME_GPIO_6);
    hi_io_set_func(IOT_IO_NAME_GPIO_6, HI_IO_FUNC_GPIO_6_GPIO);
    IoTGpioSetDir(IOT_IO_NAME_GPIO_6, IOT_GPIO_DIR_OUT);
    //设置GPIO_11的复用功能为普通GPIO
    IoTGpioInit(IOT_IO_NAME_GPIO_13);
    hi_io_set_func(IOT_IO_NAME_GPIO_13, HI_IO_FUNC_GPIO_13_GPIO);
    IoTGpioSetDir(IOT_IO_NAME_GPIO_13, IOT_GPIO_DIR_OUT);

    IoTGpioSetOutputVal(IOT_IO_NAME_GPIO_6, IOT_GPIO_VALUE0);
    IoTGpioSetOutputVal(IOT_IO_NAME_GPIO_13, IOT_GPIO_VALUE0);

    //  //uart1
    // IoTGpioInit(IOT_IO_NAME_GPIO_0);  
    // hi_io_set_func(IOT_IO_NAME_GPIO_0, HI_IO_FUNC_GPIO_0_UART1_TXD);
    // IoTGpioInit(IOT_IO_NAME_GPIO_1);  
    // hi_io_set_func(IOT_IO_NAME_GPIO_1, HI_IO_FUNC_GPIO_1_UART1_RXD);

}


