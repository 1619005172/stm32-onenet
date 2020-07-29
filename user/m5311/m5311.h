#ifndef __M5311_H_
#define __M5311_H_



#include "usart.h"
#include "stdio.h"
#include "string.h"
#include <ctype.h>
#include <stdlib.h>
#include "gpio.h"
#define SENDDATATIME 10*1000


extern uint16_t rt_i;
extern uint16_t rt_out_i;
extern long out[9];
extern uint16_t t;
extern uint16_t rw;



typedef struct
{
uint16_t        resp_time100ms;//发送后查询返回信息的延时，100ms为单位。可设为指令最大响应时间。
uint8_t         try_delay1ms; //发送失败后再次发送时的延时，1ms为单位
uint8_t         max_try_times; //最大重试次数
uint8_t         max_reset_times; //最大重启次数
} stcATConfig;

void time(void);//发送心跳包函数
void NB_M5311_Init(void);//模组初始化函数
//void NB_updata(char NB_up_data[150]);//
void getdata(uint16_t recflag);//解析平台下发at指令函数
uint16_t up(char resp_buf[150],char *rt);//向平台发送信息函数
void msg(long out,uint16_t rt_i,uint16_t rw);//向平台回复操作结果
void res(void);//模组上电复位函数
void jx(char *recbuffer_DT);//接收数据解析
void MIPLREADRSP(uint16_t recflag);//接收平台下发读数据指令
#endif

