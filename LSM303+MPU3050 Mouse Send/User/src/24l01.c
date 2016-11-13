#include "24l01.h"
//#include "lcd.h"
//#include "delay.h"
//#include "usart.h"
//Mini STM32������
//NRF24L01 �������� 
	 

extern u8 TX_ADDRESS[TX_ADR_WIDTH]; //���͵�ַ
extern u8 RX_ADDRESS[RX_ADR_WIDTH]; //���յ�ַ

//SPI��ʼ��
void SPI1_Init(void)
{	 
	//GPIO�����ýṹ��
	GPIO_InitTypeDef GPIO_InitStructure;
	//SPI�����ýṹ��
	SPI_InitTypeDef  SPI_InitStructure;
	//PORTA�ں�SPI1��ʱ��ʹ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_SPI1 | RCC_APB2Periph_AFIO,ENABLE);	   
	//SPI1�ڳ�ʼ��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;//��Ӧ���ۢ�ŵ�PA5--SCK��PA6--MISO��PA7--MOSI
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA,&GPIO_InitStructure);

  SPI_InitStructure.SPI_Direction=SPI_Direction_2Lines_FullDuplex;		//ȫ˫��ģʽ
	SPI_InitStructure.SPI_Mode=SPI_Mode_Master;													//SPI����
	SPI_InitStructure.SPI_DataSize=SPI_DataSize_8b;											//8bit���ݸ�ʽ
	SPI_InitStructure.SPI_CPOL=SPI_CPOL_Low;														//����״̬ʱ��SCK=0
	SPI_InitStructure.SPI_CPHA=SPI_CPHA_1Edge;													//���ݲ����ӵ�һ��ʱ�ӱ��ؿ�ʼ
	SPI_InitStructure.SPI_NSS=SPI_NSS_Soft;															//����������豸����
	SPI_InitStructure.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_8; 	  //8��Ƶ��spi�ٶ�Ϊ9Mhz��24L01�����SPIʱ��Ϊ10Mhz��
																																			//ԭ�����õ�256��Ƶ�ٶ�̫�����Ӳ�������
	SPI_InitStructure.SPI_FirstBit=SPI_FirstBit_MSB;										//�ȷ���MSB
	SPI_InitStructure.SPI_CRCPolynomial=7;															//CRCУ��
	SPI_Init(SPI1,&SPI_InitStructure);
	SPI_Cmd(SPI1,ENABLE);	
}   
							    
//��ʼ��24L01��IO��
void NRF24L01_Init(void)
{
//	u8 i,buf[5];
	//GPIO�����ýṹ��
	GPIO_InitTypeDef GPIO_InitStructure;
	//ʹ��GPIOA��GPIOB��ʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB,ENABLE);
	
	//PA3����24L01��CE��Ϊ���
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4;//��Ӧ���ۢ�ŵ�PA3--CE,PA4-NSS
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//PB4�������Ӧ24L01��CSN
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;//��Ӧ���ۢ�ŵ�PB4--CSN
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	SPI1_Init();    //��ʼ��SPI1
	NRF24L01_it_irq();//NRF24L01�ж�����
	
	CE_0; 	//chip enable
	CSN_1;	//spi disable
}
//24L01���ж�����
void NRF24L01_it_irq(void)
{
	//GPIO�����ýṹ��
	GPIO_InitTypeDef GPIO_InitStructure;
	//NVIC���ýṹ��
	NVIC_InitTypeDef NVIC_InitStructure;
	//�ⲿ�ж����ýṹ��
	EXTI_InitTypeDef EXTI_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	//GPIOA_2���룬����24L01��IRQ�ж�����,����PA2���ó����ж�2
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;//��Ӧ���ۢ�ŵ�PA2--IRQ
  GPIO_Init(GPIOA, &GPIO_InitStructure);  
	//GPIO_SetBits(GPIOA,GPIO_Pin_2);//��ʼ״̬��IRQΪ��
	
	#ifdef VECT_TAB_FLASH
		/*����������ʼ��ַ����Ϊ0x20000000*/
		NVIC_SetVectorTable(NVIC_VectTab_RAM,0x0);
	#else			/*VECT_TAB_FLASH*/
		/*����������ʼ��ַ����Ϊ0x08000000*/
		NVIC_SetVectorTable(NVIC_VectTab_FLASH,0x0);
	#endif
	
	/*������1λ��ʾ���ȼ�*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	/*����EXTI2�ж�*/
	NVIC_InitStructure.NVIC_IRQChannel=EXTI2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource2);
	EXTI_InitStructure.EXTI_Line = EXTI_Line2;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			//�ⲿ�ж�
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//�����½��ش���
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
}
//SPIд�Ĵ���
//reg:ָ���Ĵ�����ַ
//value:д���ֵ
u8 NRF24L01_Write_Reg(u8 reg,u8 value)
{
	u8 status;	
   	CSN_0;                 //ʹ��SPI����
  	status =SPI1_ReadWriteByte(reg);//���ͼĴ����� 
  	SPI1_ReadWriteByte(value);      //д��Ĵ�����ֵ
  	CSN_1;                 //��ֹSPI����	   
  	return(status);       			//����״ֵ̬
}
//��ȡSPI�Ĵ���ֵ
//reg:Ҫ���ļĴ���
u8 NRF24L01_Read_Reg(u8 reg)
{
	  u8 reg_val;	    
 	  CSN_0;          //ʹ��SPI����		
  	SPI1_ReadWriteByte(reg);   //���ͼĴ�����
  	reg_val=SPI1_ReadWriteByte(0xFF);//��ȡ�Ĵ�������
  	CSN_1;          //��ֹSPI����		    
  	return(reg_val);           //����״ֵ̬
}	
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
u8 SPI1_ReadWriteByte(u8 TxData)
{		
	u8 retry=0;				 
	while((SPI1->SR&1<<1)==0)//�ȴ���������	
	{
		retry++;
		if(retry>200)return 0;
	}			  
	SPI1->DR=TxData;	 	  //����һ��byte 
	retry=0;
	while((SPI1->SR&1<<0)==0) //�ȴ�������һ��byte  (�ȴ����ջ�����Ϊ��)
	{
		retry++;
		if(retry>200)return 0;
	}	  						    
	return SPI1->DR;          //�����յ�������				
}
//��ָ��λ�ö���ָ�����ȵ�����
//reg:�Ĵ���(λ��)
//*pBuf:����ָ��
//len:���ݳ���
//����ֵ,�˴ζ�����״̬�Ĵ���ֵ 
u8 NRF24L01_Read_Buf(u8 reg,u8 *pBuf,u8 len)
{
	u8 status,u8_ctr;	       
  	CSN_0;           //ʹ��SPI����
  	status=SPI1_ReadWriteByte(reg);//���ͼĴ���ֵ(λ��),����ȡ״ֵ̬   	   
		for(u8_ctr=0;u8_ctr<len;u8_ctr++)
				pBuf[u8_ctr]=SPI1_ReadWriteByte(0xFF);//��������
  	CSN_1;       //�ر�SPI����
  	return status;        //���ض�����״ֵ̬
}
//��ָ��λ��дָ�����ȵ�����
//reg:�Ĵ���(λ��)
//*pBuf:����ָ��
//len:���ݳ���
//����ֵ,�˴ζ�����״̬�Ĵ���ֵ
u8 NRF24L01_Write_Buf(u8 reg, u8 *pBuf, u8 len)
{
		u8 status,u8_ctr;	    
		CSN_0;          //ʹ��SPI����
  	status = SPI1_ReadWriteByte(reg);//���ͼĴ���ֵ(λ��),����ȡ״ֵ̬
  	for(u8_ctr=0; u8_ctr<len; u8_ctr++)SPI1_ReadWriteByte(*pBuf++); //д������	 
  	CSN_1;       //�ر�SPI����
  	return status;          //���ض�����״ֵ̬
}				   
//����NRF24L01����һ������
//txbuf:�����������׵�ַ
//����ֵ:�������״��
u8 NRF24L01_TxPacket(u8 *txbuf)
{
	u8 sta;   
	CE_0;
  	NRF24L01_Write_Buf(WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH);//д���ݵ�TX BUF  32���ֽ�
  CE_1;//��������	   
	while(NRF24L01_IRQ!=0) ;//�ȴ��������
	sta=NRF24L01_Read_Reg(STATUS);  //��ȡ״̬�Ĵ�����ֵ	   
	NRF24L01_Write_Reg(SPI_WRITE_REG+STATUS,sta); //���TX_DS��MAX_RT�жϱ�־
	if(sta&MAX_TX)//�ﵽ����ط�����
	{
		NRF24L01_Write_Reg(FLUSH_TX,0xff);//���TX FIFO�Ĵ��� 
		return MAX_TX; 
	}
	if(sta&TX_OK)//�������
	{
		return TX_OK;
	}
	return 0xff;//����ԭ����ʧ��
}
//����NRF24L01����һ������
//rxbuf:�����������׵�ַ
//����ֵ:0��������ɣ��������������
u8 NRF24L01_RxPacket(u8 *rxbuf)
{
	u8 sta;		    							    
	sta=NRF24L01_Read_Reg(STATUS);  //��ȡ״̬�Ĵ�����ֵ    	 
	NRF24L01_Write_Reg(SPI_WRITE_REG+STATUS,sta); //���TX_DS��MAX_RT�жϱ�־
	if(sta&RX_OK)//���յ�����
	{
		CE_0;
		NRF24L01_Read_Buf(RD_RX_PLOAD,rxbuf,RX_PLOAD_WIDTH);//��ȡ����
		NRF24L01_Write_Reg(FLUSH_RX,0xff);//���RX FIFO�Ĵ��� 
		CE_1;
		return 0; 
	}	   
	return 1;//û�յ��κ�����
}					    
//�ú�����ʼ��NRF24L01��RXģʽ
//����RX��ַ,дRX���ݿ��,ѡ��RFƵ��,�����ʺ�LNA HCURR
//��CE��ߺ�,������RXģʽ,�����Խ���������		   
void RX_Mode(void)
{	
	  CE_0;
		NRF24L01_Write_Reg(SPI_WRITE_REG+STATUS,0x70);				//clear RX_OK or TX_OK or MAX_TX interrupt flag
		NRF24L01_Write_Reg(FLUSH_TX,0);		//���TX_FIFO
		NRF24L01_Write_Reg(FLUSH_RX,0);		//���RX_FIFO
  	NRF24L01_Write_Buf(SPI_WRITE_REG+RX_ADDR_P0,(u8*)RX_ADDRESS,RX_ADR_WIDTH);//дRX�ڵ��ַ
		NRF24L01_Write_Reg(SPI_WRITE_REG+EN_AA,0x01);    //ʹ��ͨ��0���Զ�Ӧ�� 
		NRF24L01_Write_Reg(SPI_WRITE_REG+EN_RXADDR,0x01);//ʹ��ͨ��0�Ľ��յ�ַ  	 
		NRF24L01_Write_Reg(SPI_WRITE_REG+RF_CH,0);	     //����RFͨ��Ƶ��		  
		NRF24L01_Write_Reg(SPI_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);//ѡ��ͨ��0����Ч���ݿ�� 		
  	NRF24L01_Write_Reg(SPI_WRITE_REG+RF_SETUP,0x07);//PLL_LOCK����,���ݴ�����1Mbps,���书��0dBm,�������Ŵ������濪��   
  	NRF24L01_Write_Reg(SPI_WRITE_REG+CONFIG, 0x0f);//���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ 
  	CE_1; //CEΪ��,�������ģʽ 
}						 
//�ú�����ʼ��NRF24L01��TXģʽ
//����TX��ַ,дTX���ݿ��,����RX�Զ�Ӧ��ĵ�ַ,���TX��������,ѡ��RFƵ��,�����ʺ�LNA HCURR
//PWR_UP,CRCʹ��
//��CE��ߺ�,������RXģʽ,�����Խ���������		   
//CEΪ�ߴ���10us,����������.	 
void TX_Mode(void)
{													 
	  CE_0;	    
  	NRF24L01_Write_Buf(SPI_WRITE_REG+TX_ADDR,(u8*)TX_ADDRESS,TX_ADR_WIDTH);//дTX�ڵ��ַ 
  	NRF24L01_Write_Reg(SPI_WRITE_REG+EN_AA,0x01);     //ʹ��ͨ��0���Զ�Ӧ��    
  	NRF24L01_Write_Reg(SPI_WRITE_REG+EN_RXADDR,0x01); //ʹ��ͨ��0�Ľ��յ�ַ  
  	NRF24L01_Write_Reg(SPI_WRITE_REG+SETUP_RETR,0x32);//�����Զ��ط����ʱ��:500us + 86us;����Զ��ط�����:10��
  	NRF24L01_Write_Reg(SPI_WRITE_REG+RF_CH,0);       //����RFͨ��Ϊ40
  	NRF24L01_Write_Reg(SPI_WRITE_REG+RF_SETUP,0x07);  //����TX�������,0db����,2Mbps,���������濪��   
  	NRF24L01_Write_Reg(SPI_WRITE_REG+CONFIG,0x0e);    //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ,���������ж�
	  CE_1;//CEΪ��,10us����������
}		  

//���24L01�Ƿ����
//����ֵ:0���ɹ�;1��ʧ��	
u8 NRF24L01_Check(void)
{
	u8 buf[5]={0XA5,0XA5,0XA5,0XA5,0XA5};
	u8 buf1[5];
	u8 i; 
	NRF24L01_Write_Buf(SPI_WRITE_REG+TX_ADDR,buf,5);//д��5���ֽڵĵ�ַ
	NRF24L01_Read_Buf(TX_ADDR,buf1,5); //����д��ĵ�ַ  	
	for(i=0;i<5;i++)
	{
		if(buf1[i]!=0XA5)break;
		//USART_SendData(USART1,buf1[i]);
	}
	if(i!=5)return 1;//���24L01����
	return 0;		 //��⵽24L01
}	





