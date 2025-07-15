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
#include "rtc.h"

int main(void)
{
	board_init();
	uart1_init(115200U);

	printf("RTC Init Start.....\r\n");
	bsp_rtc_init(); // RTC ��ʼ��
	printf("RTC Init Successful!!\r\n");
	
	while(1)
	{
		//��ȡRTCʱ�䲢ͨ���������
		RtcShowTime();    
		delay_ms(1000);
	}
	

}
