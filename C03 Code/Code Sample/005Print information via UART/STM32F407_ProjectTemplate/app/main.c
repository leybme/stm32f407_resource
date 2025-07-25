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
#include "bsp_uart.h"
#include <stdio.h>


int main(void)
{
	float t = 0.0;
	int count = 0;
	
	board_init();
	
	uart1_init(115200U);
	
	printf("USART Init!!\r\n");
	
	while(1)
	{
		printf("count = %d, t = %.2f\r\n",++count,t += 0.11);
		delay_ms(1000);
	}
	

}
