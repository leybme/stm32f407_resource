/*
 * 立创开发板软硬件资料与相关扩展板软硬件资料官网全部开源
 * 开发板官网：www.lckfb.com
 * 技术支持常驻论坛，任何技术问题欢迎随时交流学习
 * 立创论坛：club.szlcsc.com
 * 关注bilibili账号：【立创开发板】，掌握我们的最新动态！
 * 不靠卖板赚钱，以培养中国工程师为己任
 * 
 
 Change Logs:
 * Date           Author       Notes
 * 2024-03-07     LCKFB-LP    first version
 */
#include "board.h"
#include <stdio.h>




int main(void)
{
	
	board_init();

	RCC->AHB1ENR |= ( 0x01 << 1 );

	GPIOB->MODER 	|= ( 0x01 << 2 * 2 );
	GPIOB->OSPEEDR	|= ( 0x03 << 2 * 2 );
	GPIOB->PUPDR 	|= ( 0x01 << 2 * 2 );
	GPIOB->ODR  	|= ( 0x01 << 2);
	
	while(1)
	{

	}
	

}
