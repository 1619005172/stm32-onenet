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
//const char s[2] = ",";//用来分割接收到的at指令返回参数
char *token;//第二个值（msgid）
char *token_d;//平台下发指令
char *msgid;//平台下发指令
long out[9];
uint16_t rt_i;
uint16_t rt_out_i;
uint16_t t;
uint16_t rw;//读写标志位

//上电复位nb模组
void res(){
	memset(Usart2type.Usart2RecBuffer,0,USART2_REC_SIE);
	sprintf(NB_UpdataBuffer_res,"AT+CMRB\r\n");
  HAL_UART_Transmit(&huart2,(uint8_t*)NB_UpdataBuffer_res,strlen(NB_UpdataBuffer_res),0xff);
	HAL_Delay(500);
	printf("发送数据:%s",NB_UpdataBuffer_res);
  if(Usart2type.UsartRecFlag == 1)
  {
    printf("接收数据:%s\r\n",Usart2type.Usart2RecBuffer);
    Usart2type.UsartRecFlag = 0;  //清空接收标志位
    Usart2type.UsartRecLen = 0;   //清空接收长度
		char *p = strstr((char *)Usart2type.Usart2RecBuffer,"REBOOTING..");//提取接受数据中的子字符串
		//如果非空代表有相同值，则启动成功，进行初始化
		//如果为空则进行再次复位判断
		if(p != NULL)
		{
			HAL_Delay(10000);//延时10秒等待模组入网
			NB_M5311_Init();//执行初始化
		}else
		{
			printf("复位失败，重新复位");
			res();
		}
  }
}

void NB_M5311_Init(){
	sprintf(NB_UpdataBuffer,"AT+MIPLCREATE=56,130038F10003F2002A04001100000000000010196E62696F7462742E6865636C6F7564732E636F6D3A35363833000131F300080000000000,0,56,0\r\n");
	up(NB_UpdataBuffer,"OK");
	delay_ms(500);
	//添加温度传感器
	sprintf(NB_UpdataBuffer,"AT+MIPLADDOBJ=0,3303,1,\"1\",0,0\r\n");
  up(NB_UpdataBuffer,"OK");
	delay_ms(500);
	//添加输入对象
	sprintf(NB_UpdataBuffer,"AT+MIPLADDOBJ=0,3200,1,\"1\",0,0\r\n");
	up(NB_UpdataBuffer,"OK");
	delay_ms(500);
	//添加湿度传感器
	sprintf(NB_UpdataBuffer,"AT+MIPLADDOBJ=0,3304,1,\"1\",0,0\r\n");
	up(NB_UpdataBuffer,"OK");
	delay_ms(500);
	//添加光照传感器
	sprintf(NB_UpdataBuffer,"AT+MIPLADDOBJ=0,3301,1,\"1\",0,0\r\n");
	up(NB_UpdataBuffer,"OK");
	delay_ms(500);
	//温度传感器添加属性
	sprintf(NB_UpdataBuffer,"AT+MIPLDISCOVERRSP=0,3303,1,4,\"5700\"\r\n");
	up(NB_UpdataBuffer,"OK");
	delay_ms(500);
	//添加属性
	sprintf(NB_UpdataBuffer,"AT+MIPLDISCOVERRSP=0,3200,1,4,\"5750\"\r\n");
	up(NB_UpdataBuffer,"OK");
	delay_ms(500);
		//湿度传感器添加属性
	sprintf(NB_UpdataBuffer,"AT+MIPLDISCOVERRSP=0,3304,1,4,\"5700\"\r\n");
	up(NB_UpdataBuffer,"OK");
	delay_ms(500);
	//光照传感器添加属性
	sprintf(NB_UpdataBuffer,"AT+MIPLDISCOVERRSP=0,3301,1,4,\"5700\"\r\n");
	up(NB_UpdataBuffer,"OK");
	delay_ms(500);
	//登录平台
	sprintf(NB_UpdataBuffer,"AT+MIPLOPEN=0,3600\r\n");
	int i=up(NB_UpdataBuffer,"OK");//验证是否登录平台
	printf("%d\r\n",i);
	if(i == 1)//成功后
	{
		HAL_TIM_Base_Start_IT(&htim2);//开启定时器2每隔5秒上报一次数据
		HAL_TIM_Base_Start_IT(&htim3);//开启定时器3每隔30秒上报一次心跳包
	}
	else//不成功则重新初始化
	{
		res();
	}
}




//发送心跳包，保证连接不断
void time()
{
	sprintf(NB_UpdataBuffer,"AT+MIPLUPDATE=0,300,0\r\n");
	up(NB_UpdataBuffer,"OK");
}


//获取平台下发数据
//传入值为接收标志位
void getdata(uint16_t recflag){
//	const char t[2] = ",";
	if(recflag == 1)//判断每一个接收数据包
	{
		char *recbuffer_AT = (char *)Usart2type.Usart2RecBuffer;
		//特别要注意分割处理后原字符串会变，变成第一个子字符串
		char *recbuffer_DT = (char *)Usart2type.Usart2RecBuffer;
		char *p=strstr(recbuffer_AT,"+MIPLWRITE:");//根据返回值头部判断是否是接收到平台下发数据
		if(p != NULL)
		{
			rw = 1;
			printf("判断后数据:%s\r\n",p);
			jx(recbuffer_DT);//解析数据包
			if(out[7] == 31)
			{
				rt_out_i = 1;
			}
			else if(out[7] == 32)
			{
				rt_out_i = 1;
			}
			memset(Usart2type.Usart2RecBuffer,0,USART2_REC_SIE);//清空接收缓存
			Usart2type.UsartRecFlag = 0;  //清空接收标志位
			Usart2type.UsartRecLen = 0;   //清空接收长度
			
		}
	}
}


//获取平台下发读数据指令
//传入值为接收标志位
void MIPLREADRSP(uint16_t recflag){
//	const char t[2] = ",";
	if(recflag == 1)//判断每一个接收数据包
	{
		char *recbuffer_AT = (char *)Usart2type.Usart2RecBuffer;
		//特别要注意分割处理后原字符串会变，变成第一个子字符串
		char *recbuffer_DT = (char *)Usart2type.Usart2RecBuffer;
		char *p=strstr(recbuffer_AT,"+MIPLREAD:");//根据返回值头部判断是否是接收到平台下发数据
		if(p != NULL)
		{
			rw = 0;
			printf("判断后数据:%s\r\n",p);
			jx(recbuffer_DT);//解析数据包
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
			memset(Usart2type.Usart2RecBuffer,0,USART2_REC_SIE);//清空接收缓存
			Usart2type.UsartRecFlag = 0;  //清空接收标志位
			Usart2type.UsartRecLen = 0;   //清空接收长度
		}
	}
}





//向平台回复操作结果
void msg(long out,uint16_t rt_i,uint16_t rw){
	char msg_at[30];
	sprintf(msg_at,"AT+MIPLWRITERSP=0,%ld,%d\r\n",out,rt_i);//使用解析到的msgid进行拼装
	int i=up(msg_at,"OK");//调用发送函数
	if(i == 0)
	{
		
	}
//	memset(Usart2type.Usart2RecBuffer,0,USART2_REC_SIE);
//	printf("测试数据 %s\r\n",Usart2type.Usart2RecBuffer);
}

//发送函数
//参数为组装好的at指令包
uint16_t up(char resp_buf[150],char *rt){
	memset(Usart2type.Usart2RecBuffer,0,USART2_REC_SIE);
  HAL_UART_Transmit(&huart2,(uint8_t*)resp_buf,strlen(resp_buf),0xff);
//	memset(Usart2type.Usart2RecBuffer,0,USART2_REC_SIE);
	printf("发送数据:%s",resp_buf);
  delay_ms(300);
  if(Usart2type.UsartRecFlag == 1)
  {
    printf("接收数据:%s\r\n",Usart2type.Usart2RecBuffer);
		char *recbuffer_rt = (char *)Usart2type.Usart2RecBuffer;
		char *p=strstr(recbuffer_rt,rt);//根据返回值头部判断是否是接收到平台下发数据
		Usart2type.UsartRecFlag = 0;  //清空接收标志位
    Usart2type.UsartRecLen = 0;   //清空接收长度
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
//字符串解析
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
		printf( "解析后数据:%ld\r\n", out[i] );//out
	}
}




