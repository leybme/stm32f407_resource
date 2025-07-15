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
