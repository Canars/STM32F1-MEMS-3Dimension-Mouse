/**************************************************************************
*------------------------STM32������������--------------------------------*
*-------------------------------------------------------------------------*/
 #include "stm32f10x.h"
#include "stm32f10x_it.h"
#include "usart.h"
#include "24l01.h"
#include <stdio.h> 

#include "usb_lib.h"
#include "hw_config.h"
#include "led.h"
//#include "sys.h"

//JTAGģʽ���ö���
#define JTAG_SWD_DISABLE   0X02
#define SWD_ENABLE         0X01
#define JTAG_SWD_ENABLE    0X00	


int data_ready=0;							//���յ����ݵı�־λ
u8 recv[TX_PLOAD_WIDTH];		//���յ�����������

//��ͷ�ļ��ж�����������Ҫ���͵����ݣ�����������������ٷ��ͣ���ֹ�����������
u8 TX_ADDRESS[TX_ADR_WIDTH]= {0x34,0x43,0x10,0x10,0x01};	//���ص�ַ
u8 RX_ADDRESS[RX_ADR_WIDTH]= {0x34,0x43,0x10,0x10,0x01};	//���յ�ַ
/**************************************************************************
*------------------------STM32������������--------------------------------*
*-------------------------------------------------------------------------*/


__IO uint8_t PrevXferComplete = 1;	                     //�Ƿ��͵ı�־

u8 PreButton;                                              //��¼ǰһʱ�����İ���״̬

u8 Mouse_Buffer[4]={0,0,0,0};

void Delay(vu32 nCount)
{
  for(; nCount != 0; nCount--);
}


void JTAG_Set(u8 mode)
{
	u32 temp;
	temp=mode;
	temp<<=25;//���mode=SWD_ENABLE=0x01,0x01<<25λ
	RCC->APB2ENR|=1<<0;     //��������ʱ��	AFIO   
	AFIO->MAPR&=0XF8FFFFFF; //���MAPR��[26:24]
	AFIO->MAPR|=temp;       //����jtagģʽ
} 

int main()
{
//	int i;
	JTAG_Set(SWD_ENABLE);//SWD_ENABLE
	
	//LED_Init();       //LED�˿ڳ�ʼ��
	//LED1_0;	           //PA2            (LED3--PA3 ��24101��CE )	
  Set_System();     //USB�����˿ڳ�ʼ������   
	
  USB_Interrupts_Config();  //USB�ж�ʹ��    
  Set_USBClock();     //USBʱ������
  USB_Init();         //�豸��س�ʼ��  
	
	usart_init();//����1��ʼ��
		//printf("\n\r���ڳɹ���\n\r");
	NRF24L01_Init();//NRF24L01��ʼ��
	//while(NRF24L01_Check())			//NRF24L01�ļ�⺯�����������ԡ��õ�ʱ����24l01.h��24l01.c�а�ע��ȡ������
		//	printf("δ��⵽24L01");
	RX_Mode();//���óɽ���ģʽ

	while(1)
	{	
	
	if( data_ready && PrevXferComplete )
	 {			
			data_ready = 0;		
			Joystick_Send(recv);		 
	 }	 
   }	 
}



/*************��껭�����δ���******************************/
//   u8 X=0x05;
//   u8 Y=0x00;
//   u8 count=0;
//   LED_Init();

//   Set_System(); 
//      
//   USB_Interrupts_Config();
//     
//   Set_USBClock();
//   
//   USB_Init();
//   LED0_0;

//   while (1)
//   {	
//     if ((Joystick_data(X,Y))&&PrevXferComplete)
//     {
// 	  LED0_1;
// 	  //Joystick_data();	  
// 	  Joystick_Send(Mouse_Buffer);	  	  
// 	  LED0_0;
// 	  LED1_0;
// 	  LED2_0;
// 	if(count<=50)
// 	{
// 	   X=0x05;
// 	   Y=0x00;
// 	}
// 	if(count >50&& count<= 100)
// 	{	
// 	   X=0x00;
// 	   Y=0x05;		
// 	}
// 	if(count >100&& count<=150)
// 	{
// 	   X=0xfb;
// 	   Y=0x00;

// 	}
// 	if(count >150&& count<=200)
// 	{
// 	   X=0x00;
// 	   Y=0xfb;	   
// 	}
// 	if(count> 200)
// 	{
// 	  count=0;
// 	}
// 	count++; 
//     }		
//   }	
/*****************************************************/



// int main()
// {
// 	int i=0;
// 	JTAG_Set(SWD_ENABLE);//SWD_ENABLE
// 	usart_init();//����1��ʼ��
// 		//printf("\n\r���ڳɹ���\n\r");
// 	NRF24L01_Init();//NRF24L01��ʼ��
// //  	while(NRF24L01_Check())			//NRF24L01�ļ�⺯�����������ԡ��õ�ʱ����24l01.h��24l01.c�а�ע��ȡ������
// //  			printf("δ��⵽24L01");
// 	RX_Mode();
// 	while(1)
// 	{
// 	 	if(data_ready==1)
// 		{
// 			data_ready = 0;
// 			for(i=0;i<6;i++)
// 				printf("%c",recv[i]);
// 		}
// 	}
// }





