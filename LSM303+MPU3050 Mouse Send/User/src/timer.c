#include "timer.h"


void TIM2_Configuration(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;


	NVIC_InitTypeDef NVIC_InitStructure;	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		//�������ȼ����飺��ռ���ȼ�2λ,�����ȼ�2λ
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn; //TIM2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;  //
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	/* ---------------------------------------------------------------
	TIM2 
	--------------------------------------------------------------- */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	TIM_DeInit(TIM2);
	//TIM_InternalClockConfig(TIM2);

	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 500 - 1; 
	TIM_TimeBaseStructure.TIM_Prescaler = 4800 - 1;  
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	TIM_ARRPreloadConfig(TIM2, DISABLE);//��ֹARRԤװ�ػ�����
	TIM_ClearFlag(TIM2,TIM_FLAG_Update);
 		
	/* TIM IT enable */
	TIM_ITConfig(  //�������жϱ�־
		TIM2, //TIM2
		TIM_IT_Update,  //TIM �ж�Դ
		ENABLE  //ʹ��
		);
	
	/* TIM3 enable counter */
	TIM_Cmd(TIM2, ENABLE);  //ʹ��TIMx����
}

