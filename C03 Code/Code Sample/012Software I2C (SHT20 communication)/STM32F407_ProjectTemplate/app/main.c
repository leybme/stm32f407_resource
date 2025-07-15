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
#include "bsp_uart.h"
#include <stdio.h>
#include "bsp_sht20.h"

int main(void)
{
	
	board_init();
	
	uart1_init(115200U);

	//���ų�ʼ��
	SHT20_GPIO_INIT();
	
    //�ȴ��������ϵ��ʼ�����
    delay_ms(20);
	
	while(1)
	{
        //�ɼ��¶�
        printf("temp = %.2f\r\n", SHT20_Read(0xf3) );
        //�ɼ�ʪ��
        printf("humi = %.2f\r\n", SHT20_Read(0xf5) );
		printf("\r\n");
        delay_ms(500);
	}
	

}
