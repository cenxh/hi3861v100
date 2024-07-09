#ifndef _TEMPERATURE_H
#define _TEMPERATURE_H
void DS18B20_IO_IN(void);
uint8_t GPIO_GetInputValue(IotIoName id,IotGpioValue *val);
void DS18B20_Res(void);
uint8_t DS18B20_Check(void);
uint8_t DS18B20_Init(void);
void DS18B20_Write_Byte(uint8_t byte);
uint8_t DS18B20_Read_Bit(void);
uint8_t DS18B20_Read_Byte(void);
void DS18B20_Change_Temperature(void);
float DS18B20_Read_Temperature(void);
#endif