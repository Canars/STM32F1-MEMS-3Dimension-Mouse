#include "usart.h"
//����1��ʼ��			    

//V1.3
//֧����Ӧ��ͬƵ���µĴ��ڲ���������.
//�����˶�printf��֧��
//�����˴��ڽ��������.

//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
	/* Whatever you require here. If the only file you are using is */ 
	/* standard output using printf() for debugging, no file handling */ 
	/* is required. */ 
}; 
/* FILE is typedef�� d in stdio.h. */ 
FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
_sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	USART1->DR = (u8) ch;      
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
	return ch;
}
#endif 
//end
//////////////////////////////////////////////////////////////////										 
//��ʼ������1
void usart_init(void)
{
	//GPIOA�����ýṹ��/*���ۢ�ŵ�PA9��ӦUSART1_TX*/
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//USART�����ýṹ��
	USART_InitTypeDef USART_InitStructure;
	/* USARTx configured as follow:
        - BaudRate = 115200 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
  */
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	//ʹ��PORTA�ں�USART1��ʱ��
				      /*
								//ʹ��PORTA��ʱ�Ӻ�USART1��ʱ�ӣ������ֱ��ʽһ����
								RCC->APB2ENR |=RCC_APB2Periph_GPIOA;
								RCC->APB2ENR |=RCC_APB2Periph_USART1;
								//���������ϱ��ǵȼ۵�
							*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1, ENABLE);
	
	/* Configure USART Tx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;//��Ӧ����һ�ŵ�PA9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	/* Configure USART Rx as input floating */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//��Ӧ����һ�ŵ�PA10
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	

	USART_Init(USART1,&USART_InitStructure);
	//����USART1
	USART_Cmd(USART1,ENABLE);
}

