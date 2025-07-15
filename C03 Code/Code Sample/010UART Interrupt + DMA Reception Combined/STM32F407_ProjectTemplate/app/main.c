/*
 * 立创开发板软硬件资料与相关扩展板软硬件资料官网全部开源
 * 开发板官网：www.lckfb.com
 * 技术支持常驻论坛，任何技术问题欢迎随时交流学习
 * 立创论坛：https://oshwhub.com/forum
 * 关注bilibili账号：【立创开发板】，掌握我们的最新动态！
 * 不靠卖板赚钱，以培养中国工程师为己任
 * 

 Change Logs:
 * Date           Author       Notes
 * 2024-03-08     LCKFB-LP    first version
 */
#include "board.h"
#include <stdio.h>
#include "string.h"

#define USART_MAX_LEN 400

volatile uint16_t usart1_rx_len = 0;    //接收帧数据的长度
volatile uint16_t usart1_tx_len = 0;    //发送帧数据的长度
volatile uint8_t usart1_recv_end_flag = 0;//帧数据接收完成标志
uint8_t DMA_USART1_RX_BUF[USART_MAX_LEN]={0};   //接收数据缓存
uint8_t DMA_USART1_TX_BUF[USART_MAX_LEN]={0};	//DMA发送缓存

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
    //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    USART_DeInit(USART1);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); 		//使能GPIOA时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);		//使能USART1时钟
	
    //串口1对应引脚复用映射
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); 	//GPIOA9复用为USART1
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);	 //GPIOA10复用为USART1
	
    //USART1端口配置
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_9; 	//GPIOA10与GPIOA9
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;				//复用功能
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;			//速度100MHz
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 				//推挽复用输出
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 				//上拉
    GPIO_Init(GPIOA,&GPIO_InitStructure); 						//初始化PA9，PA10
	
    //USART1 初始化设置
    USART_InitStructure.USART_BaudRate = __baud;					//波特率设置
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;	//字长为8位数据格式
    USART_InitStructure.USART_StopBits = USART_StopBits_1;		//一个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;			//无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//无硬件数据流控制
    USART_InitStructure.USART_Mode =  USART_Mode_Rx | USART_Mode_Tx;				//收发模式
    USART_Init(USART1, &USART_InitStructure); 	//初始化串口1

    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;       //串口1中断通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2; //抢占优先级2
    NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;	    //子优先级1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);							//根据指定的参数初始化VIC寄存器
    
    NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream5_IRQn; //嵌套通道为DMA2_Stream5_IRQn
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; //抢占优先级为 2
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; //响应优先级为 2
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //通道中断使能
    NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream7_IRQn ;//串口1发送中断通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;   //抢占优先级2
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 3;   //子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE; //IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);

    USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);  	//开启串口空闲中断
    USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);  	// 开启串口DMA接收
    USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);  	// 开启串口DMA接收
	
    /* 配置串口DMA接收*/
    DMA_InitTypeDef DMA_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);  					// 开启DMA时钟
    DMA_DeInit(DMA2_Stream5);
    DMA_InitStructure.DMA_Channel = DMA_Channel_4; 							//通道选择
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART1->DR;		//DMA外设地址
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)DMA_USART1_RX_BUF;	//DMA 存储器0地址
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;   				//存储器到外设模式
    DMA_InitStructure.DMA_BufferSize = USART_MAX_LEN;						//数据传输量
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;		//外设非增量模式
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;					//存储器增量模式
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //外设数据长度:8位
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;			//存储器数据长度:8位
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;							//使用普通模式
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;				    //高等优先级
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;                  //不开启FIFO模式
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;           //FIFO阈值
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;				//存储器突发单次传输
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;		//外设突发单次传输
    DMA_Init(DMA2_Stream5, &DMA_InitStructure);
    DMA_Cmd(DMA2_Stream5, ENABLE); //使能DMA2_Stream5通道

    DMA_DeInit(DMA2_Stream7);    //初始化DMA Stream
    while (DMA_GetCmdStatus(DMA2_Stream7) != DISABLE);//等待DMA可配置
	
    /* 配置DMA2 Stream7，USART1发送 */
    DMA_InitStructure.DMA_Channel            = DMA_Channel_4;               //通道选择
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART1->DR;            //DMA外设地址
    DMA_InitStructure.DMA_Memory0BaseAddr    = (u32)DMA_USART1_TX_BUF;      //DMA 存储器0地址
    DMA_InitStructure.DMA_DIR                = DMA_DIR_MemoryToPeripheral;  //存储器到外设模式
    DMA_InitStructure.DMA_BufferSize         = USART_MAX_LEN;       		//数据传输量
    DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;   //外设非增量模式
    DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;        //存储器增量模式
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //外设数据长度:8位
    DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;     //存储器数据长度:8位
    DMA_InitStructure.DMA_Mode               = DMA_Mode_Normal;             //使用普通模式
    DMA_InitStructure.DMA_Priority           = DMA_Priority_Medium;         //中等优先级
    DMA_InitStructure.DMA_FIFOMode           = DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_FIFOThreshold      = DMA_FIFOThreshold_1QuarterFull;
    DMA_InitStructure.DMA_MemoryBurst        = DMA_MemoryBurst_Single;      //存储器突发单次传输
    DMA_InitStructure.DMA_PeripheralBurst    = DMA_PeripheralBurst_Single;  //外设突发单次传输
    DMA_Init(DMA2_Stream7, &DMA_InitStructure);                             //初始化DMA Stream7

    DMA_ITConfig(DMA2_Stream7, DMA_IT_TC, ENABLE);							//DMA2传输完成中断
    DMA_Cmd(DMA2_Stream7, DISABLE);											//不使能
	
    USART_Cmd(USART1, ENABLE);  //使能串口1

}


void USART1_IRQHandler(void)  //串口1中断服务程序
{
    if(USART_GetITStatus(USART1,USART_IT_IDLE)!=RESET) 	//空闲中断触发
    {
    	usart1_recv_end_flag = 1;  	// 接受完成标志位置1

    	DMA_Cmd(DMA2_Stream5, DISABLE);  					   /* 暂时关闭dma，数据尚未处理 */
    	usart1_rx_len = USART_MAX_LEN - DMA_GetCurrDataCounter(DMA2_Stream5);/* 获取接收到的数据长度 单位为字节*/
    	DMA_ClearFlag(DMA2_Stream5,DMA_FLAG_TCIF5);  		/* 清DMA标志位 */
		
		DMA_USART1_Send(DMA_USART1_RX_BUF, usart1_rx_len); // 将接收的数据回显
		
    	DMA_SetCurrDataCounter(DMA2_Stream5,USART_MAX_LEN);	/* 重新赋值计数值，必须大于等于最大可能接收到的数据帧数目 */
    	DMA_Cmd(DMA2_Stream5, ENABLE);      				/*打开DMA*/
		
    	USART_ReceiveData(USART1);   						//清除空闲中断标志位（接收函数有清标志位的作用）
    }

  	if(USART_GetFlagStatus(USART1,USART_IT_TXE)==RESET)	//串口发送完成
  	{
    	USART_ITConfig(USART1,USART_IT_TC,DISABLE);
		usart1_rx_len = 0;
 	}
}

void DMA2_Stream7_IRQHandler(void)
{
	//清除标志
	if(DMA_GetFlagStatus(DMA2_Stream7,DMA_FLAG_TCIF7)!=RESET)//等待DMA2_Steam7传输完成
	{
		DMA_ClearFlag(DMA2_Stream7,DMA_FLAG_TCIF7); //清除DMA2_Steam7传输完成标志
   		DMA_Cmd(DMA2_Stream7,DISABLE);				//关闭使能
    	USART_ITConfig(USART1,USART_IT_TC,ENABLE);  //打开串口发送完成中断
	}
}

void DMA_USART1_Send(u8 *data,u16 size)
{
	memcpy(DMA_USART1_TX_BUF,data,size);				//复制数据到DMA发送缓存区
	while (DMA_GetCmdStatus(DMA2_Stream7) != DISABLE);	//确保DMA可以被设置
	DMA_SetCurrDataCounter(DMA2_Stream7,size);			//设置数据传输长度
	DMA_Cmd(DMA2_Stream7,ENABLE);						//打开DMA数据流，开始发送
}

