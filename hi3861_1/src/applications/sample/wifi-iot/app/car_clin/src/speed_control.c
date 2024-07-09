#include <stdio.h>
#include <unistd.h>

#include "ohos_init.h"
#include "cmsis_os2.h"
#include "iot_pwm.h"
#include "iot_gpio.h"
#include "iot_gpio_ex.h"
#include "io_init.h"

#include "iot_uart.h"
#include "hi_io.h"
#include "hi_pwm.h"
//1号轮子
void LF_control_due(void)
{
        IoTGpioSetOutputVal(IOT_IO_NAME_GPIO_5, IOT_GPIO_VALUE1);
        IoTGpioSetOutputVal(IOT_IO_NAME_GPIO_7, IOT_GPIO_VALUE0);
}

void LF_control_opposite(void)
{
         IoTGpioSetOutputVal(IOT_IO_NAME_GPIO_5, IOT_GPIO_VALUE0);
         IoTGpioSetOutputVal(IOT_IO_NAME_GPIO_7, IOT_GPIO_VALUE1);
}

void LF_control_stop(void)
{
         IoTGpioSetOutputVal(IOT_IO_NAME_GPIO_5, IOT_GPIO_VALUE0);
         IoTGpioSetOutputVal(IOT_IO_NAME_GPIO_7, IOT_GPIO_VALUE0);
}
//2号轮子
void LB_control_due(void)
{
        IoTGpioSetOutputVal(IOT_IO_NAME_GPIO_10, IOT_GPIO_VALUE0);
        IoTGpioSetOutputVal(IOT_IO_NAME_GPIO_8, IOT_GPIO_VALUE1);
}

void LB_control_opposite(void)
{
        IoTGpioSetOutputVal(IOT_IO_NAME_GPIO_10, IOT_GPIO_VALUE1);
        IoTGpioSetOutputVal(IOT_IO_NAME_GPIO_8, IOT_GPIO_VALUE0);
}

void LB_control_stop(void)
{
        IoTGpioSetOutputVal(IOT_IO_NAME_GPIO_10, IOT_GPIO_VALUE0);
        IoTGpioSetOutputVal(IOT_IO_NAME_GPIO_8, IOT_GPIO_VALUE0);
}
//3号轮子
void RB_control_due(void)
{
        IoTGpioSetOutputVal(IOT_IO_NAME_GPIO_9, IOT_GPIO_VALUE0);
        IoTGpioSetOutputVal(IOT_IO_NAME_GPIO_14, IOT_GPIO_VALUE1);
}

void RB_control_opposite(void)
{
         IoTGpioSetOutputVal(IOT_IO_NAME_GPIO_9, IOT_GPIO_VALUE1);
        IoTGpioSetOutputVal(IOT_IO_NAME_GPIO_14, IOT_GPIO_VALUE0);
 }

 void RB_control_stop(void)
{
         IoTGpioSetOutputVal(IOT_IO_NAME_GPIO_9, IOT_GPIO_VALUE0);
        IoTGpioSetOutputVal(IOT_IO_NAME_GPIO_14, IOT_GPIO_VALUE0);
 }
//4号轮子
void RF_control_due(void)
{
         IoTGpioSetOutputVal(IOT_IO_NAME_GPIO_6, IOT_GPIO_VALUE0);
        IoTGpioSetOutputVal(IOT_IO_NAME_GPIO_13, IOT_GPIO_VALUE1);
}

void RF_control_opposite(void)
{
         IoTGpioSetOutputVal(IOT_IO_NAME_GPIO_6, IOT_GPIO_VALUE1);
        IoTGpioSetOutputVal(IOT_IO_NAME_GPIO_13, IOT_GPIO_VALUE0);
}

void RF_control_stop(void)
{
         IoTGpioSetOutputVal(IOT_IO_NAME_GPIO_6, IOT_GPIO_VALUE0);
        IoTGpioSetOutputVal(IOT_IO_NAME_GPIO_13, IOT_GPIO_VALUE0);
}


