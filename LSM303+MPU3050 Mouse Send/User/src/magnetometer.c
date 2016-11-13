#include "analog_iic.h"
#include "magnetometer.h"
/**************GLOBAL VARIETIES***************/
extern  u8 mag_zero_test_flag;							//���̳�ʼУ׼����λ
extern s32 s32_x_mid, s32_y_mid, s32_z_mid;	//������Ӧ��У׼��ֵ
/**********************************************
*�������ƣ�Mag_Init
*�������ܣ����̳�ʼ��
*��ڲ�������
*���ڲ�������
**********************************************/
void Mag_Init(void)
{
	I2C_WriteByte(MAG_ADDR,CRA_REG_M,0x14);		//Minimum data output rate 30MHz
	I2C_WriteByte(MAG_ADDR,MR_REG_M,CC_MODE);	//Continuous-conversion mode
}
/**********************************************
*�������ƣ�Mag_Read
*��������:��ȡ��������
*��ڲ�����s16 Mag_Data[3]
*���ڲ�������
**********************************************/
void Mag_Read(s16 Mag_Data[3])										 
{	
	unsigned char X_L, X_H, Y_L,Y_H, Z_L, Z_H;
														 
	X_L = I2C_ReadByte(MAG_ADDR,OUT_X_L_M);
	X_H = I2C_ReadByte(MAG_ADDR,OUT_X_H_M);
	Y_L = I2C_ReadByte(MAG_ADDR,OUT_Y_L_M);
	Y_H = I2C_ReadByte(MAG_ADDR,OUT_Y_H_M);
	Z_L = I2C_ReadByte(MAG_ADDR,OUT_Z_L_M);
	Z_H = I2C_ReadByte(MAG_ADDR,OUT_Z_H_M);

	Mag_Data[0]=(X_H << 8) + X_L;
	Mag_Data[1]=(Y_H << 8) + Y_L;
	Mag_Data[2]=(Z_H << 8) + Z_L;
	
	if(!mag_zero_test_flag)
	{
		Mag_Data[0] -= s32_x_mid;
		Mag_Data[1] -= s32_y_mid;
		Mag_Data[2] -= s32_z_mid;
	}
}
