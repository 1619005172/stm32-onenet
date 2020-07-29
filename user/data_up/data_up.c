#include "m5311.h"
#include "delay.h"
#include "sys.h"
#include "dht11.h"
#include "bh1750.h"
#include "data_up.h"
#include "tim.h"
	u8 temperature;  	 //温度缓存   
	u8 humidity; 				//湿度缓存
	char NB_SendData[350];

	int i=0;
	
	
void updata()
{
	DHT11_Read_Data(&temperature,&humidity);//获取温湿度传感器数据
	sprintf(NB_SendData,"AT+MIPLNOTIFY=0,0,3303,0,5700,4,8,\"%d\",0,0\r\n",temperature);//数据拼接
	up(NB_SendData,"OK");//发送温度数据
	sprintf(NB_SendData,"AT+MIPLNOTIFY=0,0,3304,0,5700,4,8,\"%d\",0,0\r\n",humidity);//拼接湿度数据
	up(NB_SendData,"OK");//发送数据
	float lex = getlex();//获取光照数据
	sprintf(NB_SendData,"AT+MIPLNOTIFY=0,0,3301,0,5700,4,8,\"%.2f\",0,0\r\n",lex);//拼接
	up(NB_SendData,"OK");//发送光照数据源
	data_i = 0;//
}
	
	
//回复平台读数据指令
void readrsp(uint16_t rt){
	float lex;
	char NB_UpdataBuffer[150];
	switch(rt){
		case 3303:
			DHT11_Read_Data(&temperature,&humidity);
			sprintf(NB_UpdataBuffer,"AT+MIPLREADRSP=0,%ld,1,%ld,0,5700,4,4,\"%d\",0,0\r\n",out[1],out[2],temperature);
			up(NB_UpdataBuffer,"OK");
			t = 0;
		break;
		case 3301:
			lex = getlex();//获取光照数据
			sprintf(NB_UpdataBuffer,"AT+MIPLREADRSP=0,%ld,1,%ld,0,5700,4,4,\"%.2f\",0,0\r\n",out[1],out[2],lex);
			up(NB_UpdataBuffer,"OK");
			t = 0;
		break;
		case 3304:
			DHT11_Read_Data(&temperature,&humidity);
			sprintf(NB_UpdataBuffer,"AT+MIPLREADRSP=0,%ld,1,%ld,0,5700,4,4,\"%d\",0,0\r\n",out[1],out[2],humidity);
			up(NB_UpdataBuffer,"OK");
			t = 0;
		break;
	}
}



	
	
	
	
	
	
	
	
	
	
	