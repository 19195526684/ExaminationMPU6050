#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "lcd.h"
#include "MPU6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 

int main(void)
{
	short aacx,aacy,aacz;			
	short gyrox,gyroy,gyroz;		
	short AX, AY, AZ, GX, GY, GZ;
	short t;
	float pitch,roll,yaw; 	
	
	//初始化
	HAL_Init();                   	
    Stm32_Clock_Init(336,8,2,7);  	
	delay_init(168);               	
	uart_init(500000);   		 
 	LCD_Init();
	MPU6050_Init();
	mpu_dmp_init();
	
	//显示常显文字
 	POINT_COLOR=RED;
	LCD_ShowString(30,220,200,16,16,"Pitch:    . ");	
	LCD_ShowString(30,240,200,16,16," Roll:    . ");	 
	LCD_ShowString(30,260,200,16,16," Yaw :    . ");	 
	LCD_ShowString(30,30,200,16,16,"AX =  ");
	LCD_ShowString(30,60,200,16,16,"AY =  ");
	LCD_ShowString(30,90,200,16,16,"AZ =  ");
	LCD_ShowString(30,120,200,16,16,"GX =  ");
	LCD_ShowString(30,150,200,16,16,"GY =  ");
	LCD_ShowString(30,180,200,16,16,"GZ =  ");
	
	while(1)
	{
        if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)			
        {
			MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//获取陀螺仪数据
			MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//获取加速度数据
			AX = aacx;
			AY = aacy;
			AZ = aacz;
			GX = gyrox;
			GY = gyroy;
			GZ = gyroz;
			
			//显示六轴数据
			if(AX<0)
			{
				AX = (~AX+1);
				LCD_ShowString(70,30,200,16,16,"-");
			}
			else
				LCD_ShowString(70,30,200,16,16," ");
			LCD_ShowNum(78,30,AX,5,16);
			
			if(AY<0)
			{
				AY = (~AY+1);
				LCD_ShowString(70,60,200,16,16,"-");
			}
			else
				LCD_ShowString(70,60,200,16,16," ");
			LCD_ShowNum(78,60,AY,5,16);
			
			if(AZ<0)
			{
				AZ = (~AZ+1);
				LCD_ShowString(70,90,200,16,16,"-");
			}
			else
				LCD_ShowString(70,90,200,16,16," ");
			LCD_ShowNum(78,90,AZ,5,16);
		
			if(GX<0)
			{
				GX = (~GX+1);
				LCD_ShowString(70,120,200,16,16,"-");
			}
			else
				LCD_ShowString(70,120,200,16,16," ");
			LCD_ShowNum(78,120,GX,5,16);
			
			if(GY<0)
			{
				GY = (~GY+1);
				LCD_ShowString(70,150,200,16,16,"-");
			}
			else
				LCD_ShowString(70,150,200,16,16," ");
			LCD_ShowNum(78,150,GY,5,16);
			
			if(GZ<0)
			{
				GZ = (~GZ+1);
				LCD_ShowString(70,180,200,16,16,"-");
			}
			else
				LCD_ShowString(70,180,200,16,16," ");
			LCD_ShowNum(78,180,GZ,5,16);
			
			
			//显示姿态数据
			t=pitch*10;
			if(t<0)
			{
				LCD_ShowChar(78,220,'-',16,0);		
				t=-t;		
			}else LCD_ShowChar(78,220,' ',16,0);	
			LCD_ShowNum(86,220,t/10,3,16);		   
			LCD_ShowNum(118,220,t%10,1,16);	
			
			t=roll*10;
			if(t<0)
			{
				LCD_ShowChar(78,240,'-',16,0);		
				t=-t;		
			}else LCD_ShowChar(78,240,' ',16,0);	
			LCD_ShowNum(86,240,t/10,3,16);		    
			LCD_ShowNum(118,240,t%10,1,16);		
			
			t=yaw*10;
			if(t<0)
			{
				LCD_ShowChar(78,260,'-',16,0);		
				t=-t;		
			}else LCD_ShowChar(78,260,' ',16,0);		
			LCD_ShowNum(86,260,t/10,3,16);		  
			LCD_ShowNum(118,260,t%10,1,16);		
		}
	}
}

