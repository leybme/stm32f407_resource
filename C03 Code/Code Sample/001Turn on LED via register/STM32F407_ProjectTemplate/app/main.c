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
