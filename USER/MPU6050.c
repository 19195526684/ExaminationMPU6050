#include "mpu6050.h"
#include "sys.h"
#include "delay.h"
#include "usart.h"  

u8 MPU_Write_Byte(u8 reg,u8 data) 		//写入一个字节（地址，数据）		 
{ 
    IIC_Start(); 
	IIC_Send_Byte((MPU_ADDR<<1)|0);
	if(IIC_Wait_Ack())
	{		
		IIC_Stop();		 
		return 1;		
	}
    IIC_Send_Byte(reg);	
    IIC_Wait_Ack();		
	IIC_Send_Byte(data);
	if(IIC_Wait_Ack())	
	{
		IIC_Stop();	 
		return 1;		 
	}		 
    IIC_Stop();	 
	return 0;
}

u8 MPU_Read_Byte(u8 reg)		//读取一个字节
{
	u8 receive;
    IIC_Start(); 
	IIC_Send_Byte((MPU_ADDR<<1)|0);
	IIC_Wait_Ack();		
    IIC_Send_Byte(reg);
	IIC_Wait_Ack();		
	
    IIC_Start();
	IIC_Send_Byte((MPU_ADDR<<1)|1);
    IIC_Wait_Ack();		
	receive=IIC_Read_Byte(0);
    IIC_Stop();			
	return receive;		
}

u8 MPU_Set_Gyro_Fsr(u8 fsr)		//设置陀螺仪量程
{
	return MPU_Write_Byte(MPU_GYRO_CFG_REG,fsr<<3);
}

u8 MPU_Set_Accel_Fsr(u8 fsr)	//设置加速度计量程
{
	return MPU_Write_Byte(MPU_ACCEL_CFG_REG,fsr<<3);  
}

u8 MPU_Set_LPF(u16 lpf)		//设置低通滤波
{
	u8 data=0;
	if(lpf>=188)data=1;
	else if(lpf>=98)data=2;
	else if(lpf>=42)data=3;
	else if(lpf>=20)data=4;
	else if(lpf>=10)data=5;
	else data=6; 
	return MPU_Write_Byte(MPU_CFG_REG,data); 
}

u8 MPU_Set_Rate(u16 rate)		//设置采样速率
{
	u8 data;
	if(rate>1000)rate=1000;
	if(rate<4)rate=4;
	data=1000/rate-1;
	data=MPU_Write_Byte(MPU_SAMPLE_RATE_REG,data);	
 	return MPU_Set_LPF(rate/2);	
}


u8 MPU6050_Init(void)		//初始化
{
	u8 res;
	IIC_Init();
	MPU_Write_Byte(MPU_PWR_MGMT1_REG,0X80);
    delay_ms(100);
	MPU_Write_Byte(MPU_PWR_MGMT1_REG,0X00);	
	MPU_Set_Gyro_Fsr(3);					
	MPU_Set_Accel_Fsr(0);					
	MPU_Set_Rate(50);						
	MPU_Write_Byte(MPU_INT_EN_REG,0X00);	
	MPU_Write_Byte(MPU_USER_CTRL_REG,0X00);	
	MPU_Write_Byte(MPU_FIFO_EN_REG,0X00);	
	MPU_Write_Byte(MPU_INTBP_CFG_REG,0X80);	
	res=MPU_Read_Byte(MPU_DEVICE_ID_REG);
	if(res==MPU_ADDR)
	{
		MPU_Write_Byte(MPU_PWR_MGMT1_REG,0X01);	
		MPU_Write_Byte(MPU_PWR_MGMT2_REG,0X00);
		MPU_Set_Rate(50);						
 	}else return 1;
	return 0;
}	

u8 MPU_Get_Gyroscope(short *gx,short *gy,short *gz)		//获取陀螺仪数据
{
    u8 buf[6],res;  
	res=MPU_Read_Len(MPU_ADDR,MPU_GYRO_XOUTH_REG,6,buf);
	if(res==0)
	{
		*gx=((u16)buf[0]<<8)|buf[1];  
		*gy=((u16)buf[2]<<8)|buf[3];  
		*gz=((u16)buf[4]<<8)|buf[5];
	} 	
    return res;;
}


u8 MPU_Get_Accelerometer(short *ax,short *ay,short *az)		//获取加速度数据
{
    u8 buf[6],res;  
	res=MPU_Read_Len(MPU_ADDR,MPU_ACCEL_XOUTH_REG,6,buf);
	if(res==0)
	{
		*ax=((u16)buf[0]<<8)|buf[1];  
		*ay=((u16)buf[2]<<8)|buf[3];  
		*az=((u16)buf[4]<<8)|buf[5];
	} 	
    return res;;
}


u8 MPU_Write_Len(u8 addr,u8 reg,u8 len,u8 *buf)		//在连续地址连续写入数据（从机地址，从机起始地址，长度，数据）
{
	u8 i; 
    IIC_Start(); 
	IIC_Send_Byte((addr<<1)|0);
	if(IIC_Wait_Ack())	
	{
		IIC_Stop();		 
		return 1;		
	}
    IIC_Send_Byte(reg);
    IIC_Wait_Ack();		
	for(i=0;i<len;i++)
	{
		IIC_Send_Byte(buf[i]);	
		if(IIC_Wait_Ack())		
		{
			IIC_Stop();	 
			return 1;		 
		}		
	}    
    IIC_Stop();	 
	return 0;	
} 

u8 MPU_Read_Len(u8 addr,u8 reg,u8 len,u8 *buf)		//在连续地址连续读取数据（从机地址，从机起始地址，长度，数据）
{ 
 	IIC_Start(); 
	IIC_Send_Byte((addr<<1)|0);
	if(IIC_Wait_Ack())	
	{
		IIC_Stop();		 
		return 1;		
	}
    IIC_Send_Byte(reg);
    IIC_Wait_Ack();		
    IIC_Start();
	IIC_Send_Byte((addr<<1)|1);
    IIC_Wait_Ack();		
	while(len)
	{
		if(len==1)*buf=IIC_Read_Byte(0); 
		else *buf=IIC_Read_Byte(1);		 
		len--;
		buf++; 
	}    
    IIC_Stop();	
	return 0;	
}





