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
#ifndef __BSP_SHT20_H__
#define __BSP_SHT20_H__

#include "stm32f4xx.h"


#define RCU_SCL  	RCC_AHB1Periph_GPIOB
#define PORT_SCL 	GPIOB
#define GPIO_SCL 	GPIO_Pin_6

#define RCU_SDA  	RCC_AHB1Periph_GPIOB
#define PORT_SDA 	GPIOB
#define GPIO_SDA 	GPIO_Pin_7


#define SDA_IN()  {SHT20_MODE_SET( GPIO_Mode_IN  );}  //SDA����ģʽ
#define SDA_OUT() {SHT20_MODE_SET( GPIO_Mode_OUT );}  //SDA���ģʽ

#define SCL(BIT)  GPIO_WriteBit(PORT_SCL, GPIO_SCL, BIT)
#define SDA(BIT)  GPIO_WriteBit(PORT_SDA, GPIO_SDA, BIT)
#define SDA_GET() GPIO_ReadInputDataBit(PORT_SDA, GPIO_SDA)


void SHT20_GPIO_INIT(void);
float SHT20_Read(unsigned char regaddr);
void SHT20_MODE_SET( uint8_t __mode );



#endif
