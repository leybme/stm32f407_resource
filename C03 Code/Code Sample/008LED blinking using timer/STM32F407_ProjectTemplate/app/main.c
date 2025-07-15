/*
 * ������������Ӳ�������������չ����Ӳ�����Ϲ���ȫ����Դ
 * �����������www.lckfb.com
 * ����֧�ֳ�פ��̳���κμ������⻶ӭ��ʱ����ѧϰ
 * ������̳��club.szlcsc.com
 * ��עbilibili�˺ţ������������塿���������ǵ����¶�̬��
 * ��������׬Ǯ���������й�����ʦΪ����
 * 
 
 Change Logs:
 * Date           Author       Notes
 * 2024-03-08     LCKFB-LP    first version
 */
#include "board.h"
#include "bsp_uart.h"
#include <stdio.h>

void Timer_init(void);

uint8_t flag = 0;

int main(void)
{
	
	board_init();
	
	uart1_init(115200U);
	
	Timer_init();

	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOB, GPIO_Pin_2);
	
	while(1)
	{

	}
}


void Timer_init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = 10000;
	TIM_TimeBaseStructure.TIM_Prescaler = 8400 - 1;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
	TIM_ITConfig(TIM3,TIM_IT_Update, ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM3,ENABLE);

	
}	

void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) == SET)
	{ // {}��Ϊ�жϴ���
		printf("IRQHandler!!!\r\n");
		
		// ��תLED��
		if( flag == 0 )
		{
			GPIO_SetBits(GPIOB, GPIO_Pin_2);
			flag = 1;
		}
		else
		{
			GPIO_ResetBits(GPIOB, GPIO_Pin_2);
			flag = 0;
		}
	}
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
}
