/***********************************************************************************************************************************
**�ļ���.convert.c
**���ܣ���Ԫ����̬���ݵ������Ҫ������ƫ������ת���Լ����ݸ�ʽ�ķ�װ
***********************************************************************************************************************************/
#include "data_convert.h"
#include "math.h"
#include "ahrs_states.h"
#include "24l01.h"
#include "usart.h"
#include "key.h"
 
//����Ԫ��ת��Ϊŷ���ǣ��ԽǶ�ƫ��ֵ��������ƫ��ֵ
//�ʵ����ǶȷŴ�ӳ�䵽������Ļ
//���㷨�μ����������Ե���������(David H.Titterton John L.Weston)ŷ���ǡ���Ԫ�����������ҵ��໥ת������
//�㷨���Էɻ�����̬����ϵչ������������ʱ��Ӧ����������ϵ��
//�ɻ�����̬����ϵ�μ�AN3192
int quat2Eular(float *quat,float *euler)
{
	u8 i;
	u8 flag = 1;	//���ڿ������״̬ʱ����������Phi��Psi�ı�־λ
	static float PI = 3.141593;	
	float a,b,c,d;	//a,b,c,d��Ӧ��Ԫ�����ĸ�ֵ
	float Phi,			//��д��Phi���գ��ɻ���x����ת��ʾ�����
				Theta,		//��д��Theta���ȣ��ɻ���y����ת��ʾ������
				Psi;			//��д��Psi���ף��ɻ���z����ת��ʾƫ����
	float phi,			//Сд��phi��ʾƫ����
				theta,
				psi;
	float avg;
	avg=sqrt(quat[0]*quat[0]+quat[1]*quat[1]+quat[2]*quat[2]+quat[3]*quat[3]);
	for(i=0;i<4;i++)
	{
		quat[i]=quat[i]/avg;
	}
	
	//��quat[4]�ֱ�ֵ��a,b,c,d
	a=quat[0];
	b=quat[1];
	c=quat[2];
	d=quat[3];
	//����ŷ���ǣ�ͬʱ�ж��Ƿ�ﵽ�������
	//�ɻ���y��ת����ӦTheta
	Theta=asin(2*a*c-2*b*d);  
	//�����ж�Ϊ��ʹTheta��+-90�ȸ���������ͻ��
	if(fabs(PI/2 - Theta) < 0.1 || fabs(-PI/2 - Theta) < 0.1)
	{
		//�������¦׺ͦ�,Phi��Psi
		if(flag)
		{
			Phi = Phi;
			Psi = atan((2*c*d-2*a*b+2*b*c-2*a*d)/(2*(b*d+a*c)-(a*a-b*b+c*c-d*d))) - Phi;	//��=arctan[(c23+c12)/(c13-c22)]-��
		}
		else 
		{
			Psi = Psi ;
			Phi = atan((2*c*d-2*a*b+2*b*c-2*a*d)/(2*(b*d+a*c)-(a*a-b*b+c*c-d*d))) - Psi;	//��=arctan[(c23+c12)/(c13-c22)]-��
		}
		flag = !flag ; //flagȡ��
	}
	else 	//(fabs(PI/2 - Theta) >= 0.1 && fabs(-PI/2 - Theta) >= 0.1)	�Ȳ��ӽ�+-90�ȣ��༴�Ȳ����ڿ����������
	{
		//�ɻ�������תʱ��ӦPsi
		Psi=atan(2*(b*c+a*d)/(a*a+b*b-c*c-d*d));
		//�ɻ��������ʱ��ӦPhi
		Phi=atan(2*(c*d+a*b)/(a*a-b*b-c*c+d*d));
	}

	//����Ƕ�����ϴε�ƫ����
	theta = Theta - gStateData.theta;
	psi   = Psi   - gStateData.psi;
	phi   = Phi   - gStateData.phi;
	
	//���ǶȷŴ�ӳ�䵽��Ļ�ϵ��ƶ�
	//�˴�������Ҫ��Ӧ����������ϵ�ϣ��˴���Ҫ��ɻ�������ϵһһ��Ӧ
	euler[0] = psi   * 1000;				//���������ת��Ӧ�ɻ���Psi
	euler[1] = -phi  * 1000;				//��������ƶ���Ӧ�ɻ���Phi   
	euler[2] = theta * 1000;				//������δ����
	
	//�˴�����С�������ԣ�������겻��ʱָ�붶��
	for(i=0;i<2;i++)
	{
		if(fabs(euler[i]) < 3)
			euler[i] = 0;
	}
	
	
	//�Խṹ����ŷ���Ǹ�ֵ��������һ�μ���
	gStateData.theta = Theta;
	gStateData.psi   = Psi;
	gStateData.phi   = Phi;
	
	return 0;
}


void Send_AHRS_Data(void)
{
	int i;
	s8 data[6];
	float quad[4];
 	float move[3];//float����ƫ��
	
	quad[0] = gStateData.qib.a;  
	quad[1] = -gStateData.qib.b; 
	quad[2] = -gStateData.qib.c; 
	quad[3] = gStateData.qib.d;
	
	//quaternion_to_coord(coord1,quad,coord2);
  quat2Eular(quad,move);
	//data[]�Ƿ�װ������ݰ�	
	data[0] = 0x80;
	data[1] = 0x0A;
	data[2] = key_scan();
	data[3] = (s8)move[0];
	data[4] = (s8)move[1];
	data[5] = 0x00;
	
 	NRF24L01_TxPacket((u8*)data);
	for(i=0;i<6;i++)
	{
		printf("%c",data[i]);
	}	
}

