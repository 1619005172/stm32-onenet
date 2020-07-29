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
uint16_t        resp_time100ms;//���ͺ��ѯ������Ϣ����ʱ��100msΪ��λ������Ϊָ�������Ӧʱ�䡣
uint8_t         try_delay1ms; //����ʧ�ܺ��ٴη���ʱ����ʱ��1msΪ��λ
uint8_t         max_try_times; //������Դ���
uint8_t         max_reset_times; //�����������
} stcATConfig;

void time(void);//��������������
void NB_M5311_Init(void);//ģ���ʼ������
//void NB_updata(char NB_up_data[150]);//
void getdata(uint16_t recflag);//����ƽ̨�·�atָ���
uint16_t up(char resp_buf[150],char *rt);//��ƽ̨������Ϣ����
void msg(long out,uint16_t rt_i,uint16_t rw);//��ƽ̨�ظ��������
void res(void);//ģ���ϵ縴λ����
void jx(char *recbuffer_DT);//�������ݽ���
void MIPLREADRSP(uint16_t recflag);//����ƽ̨�·�������ָ��
#endif

