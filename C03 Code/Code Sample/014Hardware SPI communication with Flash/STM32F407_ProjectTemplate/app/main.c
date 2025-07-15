/*
 * ������������Ӳ�������������չ����Ӳ�����Ϲ���ȫ����Դ
 * �����������www.lckfb.com
 * ����֧�ֳ�פ��̳���κμ������⻶ӭ��ʱ����ѧϰ
 * ������̳��https://oshwhub.com/forum
 * ��עbilibili�˺ţ������������塿���������ǵ����¶�̬��
 * ��������׬Ǯ���������й�����ʦΪ����
 * Change Logs:
 * Date           Author       Notes
 * 2024-08-02     LCKFB-LP    first version
 */
#include "board.h"
#include "bsp_uart.h"
#include <stdio.h>
#include "spi_flash.h"
#include <string.h>

int main(void)
{
	
	board_init();
	uart1_init(115200U);
	
	/* SPI��ʼ�� */
	bsp_spi_init();
	
	// ���������鶨��
	unsigned char buff[20] = {0};
	
	printf("\r\n=========����ʼ��========\r\n");
	
	// ����Flash����0
	printf("\r\n��1������Flash����0......\r\n");
	W25Q128_erase_sector(0);
	printf("Flash����0������ɣ���\r\n");
	delay_ms(200);
	
	//��ȡW25Q128���豸ID
	printf("\r\n��2����ȡ�豸��id......\r\n");
	printf("�豸ID = %X\r\n",W25Q128_readID());
			
	//��ȡ0��ַ����Ϊ10���ֽڵ����ݵ�buff
	printf("\r\n��3����ȡ0��ַ����Ϊ10���ֽڵ����ݵ�buff......\r\n");
	W25Q128_read(buff, 0, 10);
	//�����ȡ��������
	printf("��ȡ��������= %s\r\n",buff);
	delay_ms(200);
	
	//��0��ַд��10���ֽڵ����� �����������塱
	printf("\r\n��4����0��ַд��10���ֽڵ����� �����������塱......\r\n");
	W25Q128_write((uint8_t *)"����������", 0, 10);
	// �ȴ�д�����
	delay_ms(200);
	printf("����д��ɹ���\r\n");

	//��ȡ0��ַ����Ϊ10���ֽڵ����ݵ�buff
	printf("\r\n��5����ȡ0��ַ����Ϊ10���ֽڵ����ݵ�buff......\r\n");
	W25Q128_read(buff, 0, 10);
	//�����ȡ��������
	printf("��ȡ��������= %s\r\n",buff);
	delay_ms(1000);
		
	// ��Ϊ�Ǵ�0��ַд�ģ����Բ�����0������
	printf("\r\n��6�����ܲ�����ϲ���д�������......\r\n");
	W25Q128_erase_sector(0);
	delay_ms(200);

	// ���������
	memset(buff,0,sizeof(buff));
	
	printf("\r\n=========��������========\r\n");

	while(1) 
	{
		delay_ms(100);			
	}
	

}
