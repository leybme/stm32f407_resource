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
 * 2024-03-07     LCKFB-LP    first version
 */
#include "board.h"
#include "bsp_uart.h"
#include <stdio.h>

uint8_t flag;

void led_init(void);
void key_scan(void);

int main(void)
{
	
	board_init();
	uart1_init(115200U);
	led_init();
	
	printf("Start.....!!\r\n");
	
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef  GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	while(1)
	{
		key_scan();
	}

}

void led_init(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	GPIO_InitTypeDef  GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType 	= GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOB,GPIO_Pin_2);
	
	flag = 0;
}

void key_scan(void)
{
	if( SET == GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) )
	{
		/* �ӳ����� */
		delay_ms(20);

		if( flag )
		{
			GPIO_SetBits(GPIOB, GPIO_Pin_2);
			flag = 0;
		}
		else
		{
			GPIO_ResetBits(GPIOB, GPIO_Pin_2);
			flag = 1;
		}


		printf("Key Press!!\r\n");

		while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == SET) // �ȴ������ɿ�
		{

		}

		/* �ӳ����� */
		delay_ms(20);
		printf("Key Release!!\r\n");
	}	
}
