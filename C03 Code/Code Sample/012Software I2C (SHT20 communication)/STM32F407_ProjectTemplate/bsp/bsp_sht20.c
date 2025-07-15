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
#include "bsp_sht20.h"
#include "board.h"
#include "stdio.h"

void SHT20_GPIO_INIT(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_SCL;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(PORT_SCL, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_SDA;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(PORT_SDA, &GPIO_InitStructure);
}

void SHT20_MODE_SET( uint8_t __mode )
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_SDA;
	GPIO_InitStructure.GPIO_Mode = __mode;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(PORT_SDA, &GPIO_InitStructure);
}

/******************************************************************
 * �� �� �� �ƣ�IIC_Start
 * �� �� ˵ ����IIC��ʼ�ź�
 * �� �� �� �Σ���
 * �� �� �� �أ���
 * ��       �ߣ�LC
 * ��       ע����
******************************************************************/
void IIC_Start(void)
{
	SDA_OUT();
	
	SCL(0);
	SDA(1);
	SCL(1);
	
	delay_us(5);
	
	SDA(0);
	delay_us(5);
	SCL(0);
	delay_us(5);
	
	
}

/******************************************************************
 * �� �� �� �ƣ�IIC_Stop
 * �� �� ˵ ����IICֹͣ�ź�
 * �� �� �� �Σ���
 * �� �� �� �أ���
 * ��       �ߣ�LC
 * ��       ע����
******************************************************************/
void IIC_Stop(void)
{
	SDA_OUT();
	
	SCL(0);
	SDA(0);
	
	SCL(1);
	delay_us(5);
	SDA(1);
	delay_us(5);
	
}

/******************************************************************
 * �� �� �� �ƣ�IIC_Send_Ack
 * �� �� ˵ ������������Ӧ��
 * �� �� �� �Σ�0Ӧ��  1��Ӧ��
 * �� �� �� �أ���
 * ��       �ߣ�LC
 * ��       ע����
******************************************************************/
void IIC_Send_Ack(uint8_t ack)
{
	SDA_OUT();
	SCL(0);
	SDA(0);
	delay_us(5);
	if(!ack) SDA(0);
	else	 SDA(1);
	SCL(1);
	delay_us(5);
	SCL(0);
	SDA(1);
}

/******************************************************************
 * �� �� �� �ƣ�IIC_Wait_Ack
 * �� �� ˵ �����ȴ��ӻ�Ӧ��
 * �� �� �� �Σ���
 * �� �� �� �أ�1=��Ӧ��   0=��Ӧ��
 * ��       �ߣ�LC
 * ��       ע����
******************************************************************/
uint8_t IIC_Wait_Ack(void)
{
	char ack = 0;
	unsigned char ack_flag = 10;
	SDA_IN();
    SDA(1);
	delay_us(5);
	SCL(1);
	delay_us(5);
	while( (SDA_GET()==1) && ( ack_flag ) )
	{
		ack_flag--;
		delay_us(5);
	}
	
	if( ack_flag <= 0 )
	{
		IIC_Stop();
		return 1;
	}
	else
	{
		SCL(0);
		SDA_OUT();
	}
	return ack;
}
/******************************************************************
 * �� �� �� �ƣ�IIC_Write
 * �� �� ˵ ����IICдһ���ֽ�
 * �� �� �� �Σ�datд�������
 * �� �� �� �أ���
 * ��       �ߣ�LC
 * ��       ע����
******************************************************************/
void IIC_Write(uint8_t data)
{
	int i = 0;
	SDA_OUT();
	SCL(0);//����ʱ�ӿ�ʼ���ݴ���
	
	for( i = 0; i < 8; i++ )
	{
		SDA( (data & 0x80) >> 7 );
		delay_us(2);
        data<<=1;
        delay_us(6); 
		SCL(1);
		delay_us(4);
		SCL(0);
		delay_us(4);
		
	}
}

/******************************************************************
 * �� �� �� �ƣ�IIC_Read
 * �� �� ˵ ����IIC��1���ֽ�
 * �� �� �� �Σ���
 * �� �� �� �أ�������1���ֽ�����
 * ��       �ߣ�LC
 * ��       ע����
******************************************************************/
uint8_t IIC_Read(void)
{
	unsigned char i,receive=0;
    SDA_IN();//SDA����Ϊ����
    for(i=0;i<8;i++ )
	{
        SCL(0);
        delay_us(5);
        SCL(1);
        delay_us(5);
        receive<<=1;
        if( SDA_GET() )
        {	
            receive|=1;   
        }
        delay_us(5); 
    }		
  return receive;
}


/******************************************************************
 * �� �� �� �ƣ�SHT20_Read
 * �� �� ˵ ����������ʪ��
 * �� �� �� �Σ�regaddr�Ĵ�����ַ regaddr=0xE3�����¶� =0xE5����ʪ�� 
 * �� �� �� �أ�regaddr=0xE3ʱ�����¶ȣ�regaddr=0xE5ʱ����ʪ��
 * ��       �ߣ�LC
 * ��       ע����
******************************************************************/
float SHT20_Read(uint8_t regaddr)
{	
    unsigned char data_H = 0;
    unsigned char data_L = 0;
    float temp = 0;
    IIC_Start();
    IIC_Write(0x80|0);
    if( IIC_Wait_Ack() == 1 ) printf("error -1\r\n");
    IIC_Write(regaddr);
    if( IIC_Wait_Ack() == 1 ) printf("error -2\r\n");
       
    do{
    delay_us(10);
    IIC_Start();
    IIC_Write(0x80|1);
    
    }while( IIC_Wait_Ack() == 1 );

    delay_us(20);
    
    data_H = IIC_Read();
    IIC_Send_Ack(0);
    data_L = IIC_Read();
    IIC_Send_Ack(1);
    IIC_Stop();
    
    if( regaddr == 0xf3 )
    {
        temp = ((data_H<<8)|data_L) / 65536.0 * 175.72 - 46.85;
    }
    if( regaddr == 0xf5 )
    {
        temp = ((data_H<<8)|data_L) / 65536.0 * 125.0 - 6;
    }
   return temp;

}
