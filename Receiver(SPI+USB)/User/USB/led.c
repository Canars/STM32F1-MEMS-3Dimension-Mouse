#include <stm32f10x.h>	   
#include "led.h"
//Mini STM32������
//LED��������			 


// V1.0
//��ʼ��PA8��PD2Ϊ�����.��ʹ���������ڵ�ʱ��		    
	 
//LED IO��ʼ��
void LED_Init(void)
{
// //   RCC->APB2ENR|=1<<3;    //ʹ��PORTBʱ��
// //   GPIOB->CRL&=0XFFFFF0FF;
// // 	GPIOB->CRL|=0X00000300;//PB.2�������
// // 	GPIOB->ODR|=1<<2;      //PB.2���

	RCC->APB2ENR|=1<<2;    //ʹ��PORTAʱ��
  GPIOA->CRL&=0XFFFF0FFF;
	GPIOA->CRL|=0X00003000;//PA.2,3�������
	GPIOA->ODR|=10<<2;      //PA.2,3���
	
}






