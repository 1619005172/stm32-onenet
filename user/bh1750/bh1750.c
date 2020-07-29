#include "bh1750.h"


float getlex(void)
{
	uint8_t  BUF[2]; 
	float temp;							 //光照数据
  int dis_data; 
  uint8_t t_Data;
	t_Data=0x01;
  HAL_I2C_Master_Transmit(&hi2c1,0x46,&t_Data,1,0xff);
		 
	t_Data=0x10;
  HAL_I2C_Master_Transmit(&hi2c1,0x46,&t_Data,1,0xff); 
  HAL_Delay(180);  
  HAL_I2C_Master_Receive(&hi2c1, 0x46+1,BUF,2,0xff); 
  dis_data=BUF[0];
  dis_data=(dis_data<<8)+BUF[1];//合成数据，即光照数据 
  temp=(float)dis_data/1.2;		
	return temp;
}

