#ifndef __LED_H
#define __LED_H	 
//#include "sys.h"
//Mini STM32������
//LED��������			 
//LED�˿ڶ���
#define LED1_0 GPIO_ResetBits(GPIOA,GPIO_Pin_2)
#define LED1_1 GPIO_SetBits(GPIOA,GPIO_Pin_2)
#define LED3_0 GPIO_ResetBits(GPIOA,GPIO_Pin_3)
#define LED3_1 GPIO_SetBits(GPIOA,GPIO_Pin_3)
//#define LED0 PBout(2)// PB2
//#define LED1 PAout(2)// PA2
//#define LED2 PAout(3)// PA3

void LED_Init(void);//��ʼ��		 				    
#endif

















