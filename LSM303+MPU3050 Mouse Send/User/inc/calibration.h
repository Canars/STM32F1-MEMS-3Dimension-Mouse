/*********************************************************
*�ļ����ƣ�calibration.h
*������������calibration.c���ú�������
**********************************************************/
#ifndef _CALIBRATION_H
#define _CALIBRATION_H
/*******************INCLUDES******************************/
#include "stm32f10x.h"
/*********************************************************/

/*******************DECLARE******************************/
void Sensor_Calibration(void);	//����������������У׼����
void Gyro_Calibration(u32 m);			//�����ǵ�У׼
void Acc_Calibration(u32 m);				//���ٶȼƵ�У׼
void Mag_Calibration(u32 m);       //���̵�У׼
void Get_New_SensorData(void);				 //��ȡ����������
void Mag_Zero_Test(void);					//������У׼

/********************************************************/

#endif
