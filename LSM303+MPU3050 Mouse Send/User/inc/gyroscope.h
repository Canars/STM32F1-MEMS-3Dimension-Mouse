/**************************************************************
*������ʹ�õ���GY-83ģ�飬����������ģ����MPU3050
*�ʴ��ļ����MPU3050������
***************************************************************/
#ifndef _GYROSCOPE_H
#define _GYROSCOPE_H
#include "stm32f10x.h"
#define	MPU3050_ADDR  0xD0	      //����������IIC�����еĴӵ�ַ,����ALT  ADDRESS��ַ���Ų�ͬ�޸�
																	//MPU30X0�Ĵ��豸��ַ��110100x b������x��ADO���ŵĵ�ƽ����
//����MPU3050�ڲ���ַ********************
#define WHO_AM_I	  	0x00
#define PRODUCT_ID  	0x01

#define X_OFFS_USRH 	0x0C
#define X_OFFS_USRL		0x0D
#define Y_OFFS_USRH		0x0E
#define Y_OFFS_USRL   0x0F
#define Z_OFFS_USRH		0x10
#define Z_OFFS_USRL		0x11
#define FIFO_EN				0x12

#define AUX_VDDIO     0x13
#define AUX_SLV_ADDR  0x14
#define	SMPLRT_DIV   	0x15
#define DLPF_FS_SYNC	0x16
#define INT_CFG     	0x17
#define AUX_ADDR      0x18
#define INT_STATUS   	0x1A

#define	TEMP_OUT_H	  0x1B
#define	TEMP_OUT_L	  0x1C

#define	GYRO_XOUT_H	  0x1D
#define	GYRO_XOUT_L		0x1E
#define	GYRO_YOUT_H		0x1F
#define	GYRO_YOUT_L		0x20
#define GYRO_ZOUT_H		0x21
#define GYRO_ZOUT_L		0x22

#define AUX_XOUT_H    0x23
#define AUX_XOUT_L		0x24
#define AUX_YOUT_H    0x25
#define AUX_YOUT_L		0x26
#define AUX_ZOUT_H		0x27
#define AUX_ZOUT_L    0x28

#define FIFO_COUNTH   0x3A
#define FIFO_COUNTL   0x3B
#define FIFO_R        0x3C
#define USER_CTRL     0x3D
#define PWR_MGM	      0x3E
//****************************************
//��������
void Gyro_Init(void);
void Gyro_Read(s16 Gyro_Data[3]);

#endif






