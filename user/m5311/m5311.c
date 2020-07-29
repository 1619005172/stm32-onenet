#include "m5311.h"
#include "delay.h"
#include "sys.h"
#include "dht11.h"
#include <stdlib.h>
#include "stdio.h"
#include "tim.h "

char NB_UpdataBuffer[150];
char NB_UpdataBuffer_res[150];
const char str[20];

uint16_t get_i;
char *objectid_T = "3303";
char *objectid_H = "3304";
char *objectid_I = "3301";
//const char s[2] = ",";//�����ָ���յ���atָ��ز���
char *token;//�ڶ���ֵ��msgid��
char *token_d;//ƽ̨�·�ָ��
char *msgid;//ƽ̨�·�ָ��
long out[9];
uint16_t rt_i;
uint16_t rt_out_i;
uint16_t t;
uint16_t rw;//��д��־λ

//�ϵ縴λnbģ��
void res(){
	memset(Usart2type.Usart2RecBuffer,0,USART2_REC_SIE);
	sprintf(NB_UpdataBuffer_res,"AT+CMRB\r\n");
  HAL_UART_Transmit(&huart2,(uint8_t*)NB_UpdataBuffer_res,strlen(NB_UpdataBuffer_res),0xff);
	HAL_Delay(500);
	printf("��������:%s",NB_UpdataBuffer_res);
  if(Usart2type.UsartRecFlag == 1)
  {
    printf("��������:%s\r\n",Usart2type.Usart2RecBuffer);
    Usart2type.UsartRecFlag = 0;  //��ս��ձ�־λ
    Usart2type.UsartRecLen = 0;   //��ս��ճ���
		char *p = strstr((char *)Usart2type.Usart2RecBuffer,"REBOOTING..");//��ȡ���������е����ַ���
		//����ǿմ�������ֵͬ���������ɹ������г�ʼ��
		//���Ϊ��������ٴθ�λ�ж�
		if(p != NULL)
		{
			HAL_Delay(10000);//��ʱ10��ȴ�ģ������
			NB_M5311_Init();//ִ�г�ʼ��
		}else
		{
			printf("��λʧ�ܣ����¸�λ");
			res();
		}
  }
}

void NB_M5311_Init(){
	sprintf(NB_UpdataBuffer,"AT+MIPLCREATE=56,130038F10003F2002A04001100000000000010196E62696F7462742E6865636C6F7564732E636F6D3A35363833000131F300080000000000,0,56,0\r\n");
	up(NB_UpdataBuffer,"OK");
	delay_ms(500);
	//����¶ȴ�����
	sprintf(NB_UpdataBuffer,"AT+MIPLADDOBJ=0,3303,1,\"1\",0,0\r\n");
  up(NB_UpdataBuffer,"OK");
	delay_ms(500);
	//����������
	sprintf(NB_UpdataBuffer,"AT+MIPLADDOBJ=0,3200,1,\"1\",0,0\r\n");
	up(NB_UpdataBuffer,"OK");
	delay_ms(500);
	//���ʪ�ȴ�����
	sprintf(NB_UpdataBuffer,"AT+MIPLADDOBJ=0,3304,1,\"1\",0,0\r\n");
	up(NB_UpdataBuffer,"OK");
	delay_ms(500);
	//��ӹ��մ�����
	sprintf(NB_UpdataBuffer,"AT+MIPLADDOBJ=0,3301,1,\"1\",0,0\r\n");
	up(NB_UpdataBuffer,"OK");
	delay_ms(500);
	//�¶ȴ������������
	sprintf(NB_UpdataBuffer,"AT+MIPLDISCOVERRSP=0,3303,1,4,\"5700\"\r\n");
	up(NB_UpdataBuffer,"OK");
	delay_ms(500);
	//�������
	sprintf(NB_UpdataBuffer,"AT+MIPLDISCOVERRSP=0,3200,1,4,\"5750\"\r\n");
	up(NB_UpdataBuffer,"OK");
	delay_ms(500);
		//ʪ�ȴ������������
	sprintf(NB_UpdataBuffer,"AT+MIPLDISCOVERRSP=0,3304,1,4,\"5700\"\r\n");
	up(NB_UpdataBuffer,"OK");
	delay_ms(500);
	//���մ������������
	sprintf(NB_UpdataBuffer,"AT+MIPLDISCOVERRSP=0,3301,1,4,\"5700\"\r\n");
	up(NB_UpdataBuffer,"OK");
	delay_ms(500);
	//��¼ƽ̨
	sprintf(NB_UpdataBuffer,"AT+MIPLOPEN=0,3600\r\n");
	int i=up(NB_UpdataBuffer,"OK");//��֤�Ƿ��¼ƽ̨
	printf("%d\r\n",i);
	if(i == 1)//�ɹ���
	{
		HAL_TIM_Base_Start_IT(&htim2);//������ʱ��2ÿ��5���ϱ�һ������
		HAL_TIM_Base_Start_IT(&htim3);//������ʱ��3ÿ��30���ϱ�һ��������
	}
	else//���ɹ������³�ʼ��
	{
		res();
	}
}




//��������������֤���Ӳ���
void time()
{
	sprintf(NB_UpdataBuffer,"AT+MIPLUPDATE=0,300,0\r\n");
	up(NB_UpdataBuffer,"OK");
}


//��ȡƽ̨�·�����
//����ֵΪ���ձ�־λ
void getdata(uint16_t recflag){
//	const char t[2] = ",";
	if(recflag == 1)//�ж�ÿһ���������ݰ�
	{
		char *recbuffer_AT = (char *)Usart2type.Usart2RecBuffer;
		//�ر�Ҫע��ָ���ԭ�ַ�����䣬��ɵ�һ�����ַ���
		char *recbuffer_DT = (char *)Usart2type.Usart2RecBuffer;
		char *p=strstr(recbuffer_AT,"+MIPLWRITE:");//���ݷ���ֵͷ���ж��Ƿ��ǽ��յ�ƽ̨�·�����
		if(p != NULL)
		{
			rw = 1;
			printf("�жϺ�����:%s\r\n",p);
			jx(recbuffer_DT);//�������ݰ�
			if(out[7] == 31)
			{
				rt_out_i = 1;
			}
			else if(out[7] == 32)
			{
				rt_out_i = 1;
			}
			memset(Usart2type.Usart2RecBuffer,0,USART2_REC_SIE);//��ս��ջ���
			Usart2type.UsartRecFlag = 0;  //��ս��ձ�־λ
			Usart2type.UsartRecLen = 0;   //��ս��ճ���
			
		}
	}
}


//��ȡƽ̨�·�������ָ��
//����ֵΪ���ձ�־λ
void MIPLREADRSP(uint16_t recflag){
//	const char t[2] = ",";
	if(recflag == 1)//�ж�ÿһ���������ݰ�
	{
		char *recbuffer_AT = (char *)Usart2type.Usart2RecBuffer;
		//�ر�Ҫע��ָ���ԭ�ַ�����䣬��ɵ�һ�����ַ���
		char *recbuffer_DT = (char *)Usart2type.Usart2RecBuffer;
		char *p=strstr(recbuffer_AT,"+MIPLREAD:");//���ݷ���ֵͷ���ж��Ƿ��ǽ��յ�ƽ̨�·�����
		if(p != NULL)
		{
			rw = 0;
			printf("�жϺ�����:%s\r\n",p);
			jx(recbuffer_DT);//�������ݰ�
			if(out[2] == 3303)
			{
				t =3303;
			}
			else if(out[2] == 3304)
			{
				t =3304;
			}
			else if(out[2] == 3301)
			{
				t =3301;
			}
			memset(Usart2type.Usart2RecBuffer,0,USART2_REC_SIE);//��ս��ջ���
			Usart2type.UsartRecFlag = 0;  //��ս��ձ�־λ
			Usart2type.UsartRecLen = 0;   //��ս��ճ���
		}
	}
}





//��ƽ̨�ظ��������
void msg(long out,uint16_t rt_i,uint16_t rw){
	char msg_at[30];
	sprintf(msg_at,"AT+MIPLWRITERSP=0,%ld,%d\r\n",out,rt_i);//ʹ�ý�������msgid����ƴװ
	int i=up(msg_at,"OK");//���÷��ͺ���
	if(i == 0)
	{
		
	}
//	memset(Usart2type.Usart2RecBuffer,0,USART2_REC_SIE);
//	printf("�������� %s\r\n",Usart2type.Usart2RecBuffer);
}

//���ͺ���
//����Ϊ��װ�õ�atָ���
uint16_t up(char resp_buf[150],char *rt){
	memset(Usart2type.Usart2RecBuffer,0,USART2_REC_SIE);
  HAL_UART_Transmit(&huart2,(uint8_t*)resp_buf,strlen(resp_buf),0xff);
//	memset(Usart2type.Usart2RecBuffer,0,USART2_REC_SIE);
	printf("��������:%s",resp_buf);
  delay_ms(300);
  if(Usart2type.UsartRecFlag == 1)
  {
    printf("��������:%s\r\n",Usart2type.Usart2RecBuffer);
		char *recbuffer_rt = (char *)Usart2type.Usart2RecBuffer;
		char *p=strstr(recbuffer_rt,rt);//���ݷ���ֵͷ���ж��Ƿ��ǽ��յ�ƽ̨�·�����
		Usart2type.UsartRecFlag = 0;  //��ս��ձ�־λ
    Usart2type.UsartRecLen = 0;   //��ս��ճ���
		if(p != NULL)
		{
			return 1;
		}
		else
		{
			return 0;
		}
  }
//	return 2;
}
//�ַ�������
void jx(char *recbuffer_DT)
{
	char delims[] = ",";
	char *result = NULL;
	result = strtok(recbuffer_DT,delims);
	int i=0;
	while( result != NULL ){
		i++;
//		if(i==2){
////			printf( "%s\n", result );//out
//			out[0]=atol(result);
//		}else if(i==8){
////			printf( "%s\n", result );//out
//			out[1]=atol(result);
//		}
		switch(i)
		{
			case 1:
				out[0]=atol(result);
			break;
			case 2:
				out[1]=atol(result);//msgid
			break;
			case 3:
				out[2]=atol(result);
			break;
			case 4:
				out[3]=atol(result);
			break;
			case 5:
				out[4]=atol(result);
			break;
			case 6:
				out[5]=atol(result);
			break;
			case 7:
				out[6]=atol(result);
			break;
			case 8:
				out[7]=atol(result);//data
			break;
			case 9:
				out[8]=atol(result);
		}
	result = strtok( NULL, delims );
	}
	for(i=0;i<8;i++){
		printf( "����������:%ld\r\n", out[i] );//out
	}
}




