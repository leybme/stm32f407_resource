/*
 * ������������Ӳ�������������չ����Ӳ�����Ϲ���ȫ����Դ
 * �����������www.lckfb.com
 * ����֧�ֳ�פ��̳���κμ������⻶ӭ��ʱ����ѧϰ
 * ������̳��https://oshwhub.com/forum
 * ��עbilibili�˺ţ������������塿���������ǵ����¶�̬��
 * ��������׬Ǯ���������й�����ʦΪ����
 * 

 Change Logs:
 * Date           Author       Notes
 * 2024-03-08     LCKFB-LP    first version
 */
#include "board.h"
#include <stdio.h>
#include "string.h"

#define USART_MAX_LEN 400

volatile uint16_t usart1_rx_len = 0;    //����֡���ݵĳ���
volatile uint16_t usart1_tx_len = 0;    //����֡���ݵĳ���
volatile uint8_t usart1_recv_end_flag = 0;//֡���ݽ�����ɱ�־
uint8_t DMA_USART1_RX_BUF[USART_MAX_LEN]={0};   //�������ݻ���
uint8_t DMA_USART1_TX_BUF[USART_MAX_LEN]={0};	//DMA���ͻ���

void DMA_USART1_Send(u8 *data,u16 size);
void uart1_Init(uint32_t __baud);


int main(void)
{
	
	board_init();
	
	uart1_Init(115200U);
	
	DMA_USART1_Send((uint8_t *)"hello world!!\r\n",13);
	
	while(1)
	{
		
	}

}


void uart1_Init(uint32_t __baud)
{
    //GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    USART_DeInit(USART1);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); 		//ʹ��GPIOAʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);		//ʹ��USART1ʱ��
	
    //����1��Ӧ���Ÿ���ӳ��
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); 	//GPIOA9����ΪUSART1
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);	 //GPIOA10����ΪUSART1
	
    //USART1�˿�����
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_9; 	//GPIOA10��GPIOA9
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;				//���ù���
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;			//�ٶ�100MHz
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 				//���츴�����
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 				//����
    GPIO_Init(GPIOA,&GPIO_InitStructure); 						//��ʼ��PA9��PA10
	
    //USART1 ��ʼ������
    USART_InitStructure.USART_BaudRate = __baud;					//����������
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;	//�ֳ�Ϊ8λ���ݸ�ʽ
    USART_InitStructure.USART_StopBits = USART_StopBits_1;		//һ��ֹͣλ
    USART_InitStructure.USART_Parity = USART_Parity_No;			//����żУ��λ
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//��Ӳ������������
    USART_InitStructure.USART_Mode =  USART_Mode_Rx | USART_Mode_Tx;				//�շ�ģʽ
    USART_Init(USART1, &USART_InitStructure); 	//��ʼ������1

    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;       //����1�ж�ͨ��
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2; //��ռ���ȼ�2
    NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;	    //�����ȼ�1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
    NVIC_Init(&NVIC_InitStructure);							//����ָ���Ĳ�����ʼ��VIC�Ĵ���
    
    NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream5_IRQn; //Ƕ��ͨ��ΪDMA2_Stream5_IRQn
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; //��ռ���ȼ�Ϊ 2
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; //��Ӧ���ȼ�Ϊ 2
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //ͨ���ж�ʹ��
    NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream7_IRQn ;//����1�����ж�ͨ��
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;   //��ռ���ȼ�2
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 3;   //�����ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE; //IRQͨ��ʹ��
    NVIC_Init(&NVIC_InitStructure);

    USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);  	//�������ڿ����ж�
    USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);  	// ��������DMA����
    USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);  	// ��������DMA����
	
    /* ���ô���DMA����*/
    DMA_InitTypeDef DMA_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);  					// ����DMAʱ��
    DMA_DeInit(DMA2_Stream5);
    DMA_InitStructure.DMA_Channel = DMA_Channel_4; 							//ͨ��ѡ��
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART1->DR;		//DMA�����ַ
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)DMA_USART1_RX_BUF;	//DMA �洢��0��ַ
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;   				//�洢��������ģʽ
    DMA_InitStructure.DMA_BufferSize = USART_MAX_LEN;						//���ݴ�����
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;		//���������ģʽ
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;					//�洢������ģʽ
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //�������ݳ���:8λ
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;			//�洢�����ݳ���:8λ
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;							//ʹ����ͨģʽ
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;				    //�ߵ����ȼ�
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;                  //������FIFOģʽ
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;           //FIFO��ֵ
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;				//�洢��ͻ�����δ���
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;		//����ͻ�����δ���
    DMA_Init(DMA2_Stream5, &DMA_InitStructure);
    DMA_Cmd(DMA2_Stream5, ENABLE); //ʹ��DMA2_Stream5ͨ��

    DMA_DeInit(DMA2_Stream7);    //��ʼ��DMA Stream
    while (DMA_GetCmdStatus(DMA2_Stream7) != DISABLE);//�ȴ�DMA������
	
    /* ����DMA2 Stream7��USART1���� */
    DMA_InitStructure.DMA_Channel            = DMA_Channel_4;               //ͨ��ѡ��
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART1->DR;            //DMA�����ַ
    DMA_InitStructure.DMA_Memory0BaseAddr    = (u32)DMA_USART1_TX_BUF;      //DMA �洢��0��ַ
    DMA_InitStructure.DMA_DIR                = DMA_DIR_MemoryToPeripheral;  //�洢��������ģʽ
    DMA_InitStructure.DMA_BufferSize         = USART_MAX_LEN;       		//���ݴ�����
    DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;   //���������ģʽ
    DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;        //�洢������ģʽ
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //�������ݳ���:8λ
    DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;     //�洢�����ݳ���:8λ
    DMA_InitStructure.DMA_Mode               = DMA_Mode_Normal;             //ʹ����ͨģʽ
    DMA_InitStructure.DMA_Priority           = DMA_Priority_Medium;         //�е����ȼ�
    DMA_InitStructure.DMA_FIFOMode           = DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_FIFOThreshold      = DMA_FIFOThreshold_1QuarterFull;
    DMA_InitStructure.DMA_MemoryBurst        = DMA_MemoryBurst_Single;      //�洢��ͻ�����δ���
    DMA_InitStructure.DMA_PeripheralBurst    = DMA_PeripheralBurst_Single;  //����ͻ�����δ���
    DMA_Init(DMA2_Stream7, &DMA_InitStructure);                             //��ʼ��DMA Stream7

    DMA_ITConfig(DMA2_Stream7, DMA_IT_TC, ENABLE);							//DMA2��������ж�
    DMA_Cmd(DMA2_Stream7, DISABLE);											//��ʹ��
	
    USART_Cmd(USART1, ENABLE);  //ʹ�ܴ���1

}


void USART1_IRQHandler(void)  //����1�жϷ������
{
    if(USART_GetITStatus(USART1,USART_IT_IDLE)!=RESET) 	//�����жϴ���
    {
    	usart1_recv_end_flag = 1;  	// ������ɱ�־λ��1

    	DMA_Cmd(DMA2_Stream5, DISABLE);  					   /* ��ʱ�ر�dma��������δ���� */
    	usart1_rx_len = USART_MAX_LEN - DMA_GetCurrDataCounter(DMA2_Stream5);/* ��ȡ���յ������ݳ��� ��λΪ�ֽ�*/
    	DMA_ClearFlag(DMA2_Stream5,DMA_FLAG_TCIF5);  		/* ��DMA��־λ */
		
		DMA_USART1_Send(DMA_USART1_RX_BUF, usart1_rx_len); // �����յ����ݻ���
		
    	DMA_SetCurrDataCounter(DMA2_Stream5,USART_MAX_LEN);	/* ���¸�ֵ����ֵ��������ڵ��������ܽ��յ�������֡��Ŀ */
    	DMA_Cmd(DMA2_Stream5, ENABLE);      				/*��DMA*/
		
    	USART_ReceiveData(USART1);   						//��������жϱ�־λ�����պ��������־λ�����ã�
    }

  	if(USART_GetFlagStatus(USART1,USART_IT_TXE)==RESET)	//���ڷ������
  	{
    	USART_ITConfig(USART1,USART_IT_TC,DISABLE);
		usart1_rx_len = 0;
 	}
}

void DMA2_Stream7_IRQHandler(void)
{
	//�����־
	if(DMA_GetFlagStatus(DMA2_Stream7,DMA_FLAG_TCIF7)!=RESET)//�ȴ�DMA2_Steam7�������
	{
		DMA_ClearFlag(DMA2_Stream7,DMA_FLAG_TCIF7); //���DMA2_Steam7������ɱ�־
   		DMA_Cmd(DMA2_Stream7,DISABLE);				//�ر�ʹ��
    	USART_ITConfig(USART1,USART_IT_TC,ENABLE);  //�򿪴��ڷ�������ж�
	}
}

void DMA_USART1_Send(u8 *data,u16 size)
{
	memcpy(DMA_USART1_TX_BUF,data,size);				//�������ݵ�DMA���ͻ�����
	while (DMA_GetCmdStatus(DMA2_Stream7) != DISABLE);	//ȷ��DMA���Ա�����
	DMA_SetCurrDataCounter(DMA2_Stream7,size);			//�������ݴ��䳤��
	DMA_Cmd(DMA2_Stream7,ENABLE);						//��DMA����������ʼ����
}

