#include "analog_iic.h"
#include "gyroscope.h"
/**********************************************
*�������ƣ�Gyro_Init
*�������ܣ������ǳ�ʼ��
*��ڲ�������
*���ڲ�������
**********************************************/
void Gyro_Init(void)
{
   I2C_WriteByte(MPU3050_ADDR,PWR_MGM, 0x80);  		 					//��Դ����ʹ���ڲ�����
   I2C_WriteByte(MPU3050_ADDR,SMPLRT_DIV, 0x07);    				//Fsample=1KHZ/(7+1)=125Hz
   //I2C_WriteByte(MPU3050_ADDR,DLPF_FS_SYNC, 0x0E);				//��500��,�ڲ�ģ�����Ƶ��Ϊ1KHz
   I2C_WriteByte(MPU3050_ADDR,DLPF_FS_SYNC, 0x1E);    			//��2000��,�ڲ�ģ�����Ƶ��1KHz
}
/**********************************************
*�������ƣ�Gyro_Read
*�������ܣ���ȡ����������
*��ڲ�����s16 Gyro_Data[2]
*���ڲ�������
**********************************************/
void Gyro_Read(s16 Gyro_Data[3])									 
{
	unsigned char X_L, X_H, Y_L,Y_H, Z_L, Z_H;
														 
	X_L = I2C_ReadByte(MPU3050_ADDR,GYRO_XOUT_L);
	X_H = I2C_ReadByte(MPU3050_ADDR,GYRO_XOUT_H);
	Y_L = I2C_ReadByte(MPU3050_ADDR,GYRO_YOUT_L);
	Y_H = I2C_ReadByte(MPU3050_ADDR,GYRO_YOUT_H);
	Z_L = I2C_ReadByte(MPU3050_ADDR,GYRO_ZOUT_L);
	Z_H = I2C_ReadByte(MPU3050_ADDR,GYRO_ZOUT_H);

	Gyro_Data[0]=(X_H << 8) + X_L;//�����ǲ����X��ԭʼ����
	Gyro_Data[1]=(Y_H << 8) + Y_L;//�����ǲ����Y��ԭʼ����
	Gyro_Data[2]=(Z_H << 8) + Z_L;//�����ǲ����Z��ԭʼ����
}
