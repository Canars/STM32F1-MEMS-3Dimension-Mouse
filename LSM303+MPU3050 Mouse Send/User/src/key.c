#include "key.h"
#include "delay.h"



/*********************************************************************************
**������:key_init(void);
**�������ܣ�������ʼ��
**���ڲ�������
**********************************************************************************/
void key_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	//PA0---Key1,PA1---Key2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/*********************************************************************************
**������:key_scan(void);
**�������ܣ�������ʼ��
**���ڲ�������
**********************************************************************************/
u8 key_scan(void)
{
	static u8 key_up=1;
	if(key_up && (KEY1==0 || KEY2==0))
	{
		delay_ms(10);
		if(KEY1==0)
			return KEY1_DOWN;
		else if(KEY2==0)
			return KEY2_DOWN;
		else 
			return 0;
	}
	return 0;
}
