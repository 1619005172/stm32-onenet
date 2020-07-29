#ifndef __DHT11_H
#define __DHT11_H

#include "main.h"
#include "sys.h"

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;

//IO方向设置
#define DHT11_IO_IN()  {GPIOC->CRH&=0XFFF0FFFF;GPIOC->CRH|=8<<16;}
#define DHT11_IO_OUT() {GPIOC->CRH&=0XFFF0FFFF;GPIOC->CRH|=3<<16;}

////IO操作函数
#define	DHT11_DQ_OUT PCout(12) //数据端口	PC12
#define	DHT11_DQ_IN  PCin(12)  //数据端口	PC12

u8 DHT11_Init(void);//初始化DHT11
u8 DHT11_Read_Data(u8 *temp, u8 *humi); //读取温湿度
u8 DHT11_Read_Byte(void);//读出一个字节
u8 DHT11_Read_Bit(void);//读出一个位
u8 DHT11_Check(void);//检测是否存在DHT11
void DHT11_Rst(void);//复位DHT11
u8 DHT11_Read_Data_Float(float *temp,float *humi);

#endif

