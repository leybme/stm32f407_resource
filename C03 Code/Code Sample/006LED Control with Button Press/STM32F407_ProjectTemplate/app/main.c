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
		/* 延迟消抖 */
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

		while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == SET) // 等待按键松开
		{

		}

		/* 延迟消抖 */
		delay_ms(20);
		printf("Key Release!!\r\n");
	}	
}
