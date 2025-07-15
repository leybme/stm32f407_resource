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


// λ�������ĺ궨�壬ֱ��ʹ�����ṩ��BIT_ADDR��
#define BIT_ADDR(byte_offset,bitnum)  (volatile unsigned long*)(0x42000000 + (byte_offset * 32) + (bitnum * 4))

// ����GPIOB�Ĵ�����λ����������ַ
#define GPIOB_OCTL_OFFSET ((GPIOB_BASE + 0x14) - 0x40000000)
#define GPIOB_ISTAT_OFFSET ((GPIOB_BASE + 0x10) - 0x40000000)

// ����PBout��PBin��
#define PBout(n)    *(BIT_ADDR(GPIOB_OCTL_OFFSET,n))
#define PBin(n)     *(BIT_ADDR(GPIOB_ISTAT_OFFSET,n))

int main(void)
{
	
	board_init();
	
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	while(1)
	{
//		GPIO_SetBits(GPIOB, GPIO_Pin_2);
		PBout(2) = 1;
		delay_ms(500);
		
//		GPIO_ResetBits(GPIOB, GPIO_Pin_2);
		PBout(2) = 0;
		delay_ms(500);
	}
	

}
