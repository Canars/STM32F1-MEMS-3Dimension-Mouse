#include "analog_iic.h"
#include "accelerometer.h"
/**********************************************
*�������ƣ�Acc_Init
*�������ܣ����ٶȼƳ�ʼ��
*��ڲ�������
*���ڲ�������
**********************************************/
void Acc_Init(void)
{
	I2C_WriteByte(ACC_ADDR,CTRL_REG1_A,0x27);//Normal Mode,ODR(Output Data Rate) 50Hz
	I2C_WriteByte(ACC_ADDR,CTRL_REG4_A,0x00);//,continuous update,Little endian,data MSB @ Lower Address,������Χ����2g,
}
/**********************************************
*�������ƣ�Acc_Read
*�������ܣ���ȡ���ٶ�����
*��ڲ�����s16 Acc_Data[3]
*���ڲ�������
**********************************************/
void Acc_Read(s16 Acc_Data[3])									 
{		
	unsigned char X_L, X_H, Y_L,Y_H, Z_L, Z_H;
														 
	X_L = I2C_ReadByte(ACC_ADDR,OUT_X_L_A);//X_L_A
	X_H = I2C_ReadByte(ACC_ADDR,OUT_X_H_A);//X_H_A
	Y_L = I2C_ReadByte(ACC_ADDR,OUT_Y_L_A);//Y_L_A
	Y_H = I2C_ReadByte(ACC_ADDR,OUT_Y_H_A);//Y_H_A
	Z_L = I2C_ReadByte(ACC_ADDR,OUT_Z_L_A);//Z_L_A
	Z_H = I2C_ReadByte(ACC_ADDR,OUT_Z_H_A);//Z_H_A
	
	Acc_Data[0]=((X_H << 8) + X_L);
	Acc_Data[1]=((Y_H << 8) + Y_L);
	Acc_Data[2]=((Z_H << 8) + Z_L);
}
