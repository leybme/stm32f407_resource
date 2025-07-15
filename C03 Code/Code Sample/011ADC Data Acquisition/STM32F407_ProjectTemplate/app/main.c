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

void Adc_Init(void);
uint16_t Get_Adc_Average(uint8_t CHx,uint8_t times);

int main(void)
{
	
	board_init();
	
	uart1_init(115200U);

	Adc_Init();
	
	while(1)
	{
		uint16_t value = Get_Adc_Average(ADC_Channel_5,20);
		
		printf("value = %d\r\n", value);
		delay_ms(1000);
	}
	

}
/**********************************************************
 * �� �� �� �ƣ�Adc_Init
 * �� �� �� �ܣ���ʼ��ADC
 * �� �� �� ������
 * �� �� �� �أ���
 * ��       �ߣ�LC
 * ��       ע��LP
**********************************************************/
void Adc_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStruct;
	ADC_InitTypeDef ADC_InitStruct;

	RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);  

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//ģ������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	ADC_DeInit();//ADC��λ

	ADC_CommonInitStruct.ADC_DMAAccessMode=ADC_DMAAccessMode_Disabled;
	ADC_CommonInitStruct.ADC_Mode=ADC_Mode_Independent;
	ADC_CommonInitStruct.ADC_Prescaler=ADC_Prescaler_Div4;
	ADC_CommonInitStruct.ADC_TwoSamplingDelay=ADC_TwoSamplingDelay_5Cycles;

	ADC_CommonInit(&ADC_CommonInitStruct);


	ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStruct.ADC_DataAlign=ADC_DataAlign_Right;
	ADC_InitStruct.ADC_ExternalTrigConvEdge=ADC_ExternalTrigConvEdge_None;
	ADC_InitStruct.ADC_NbrOfConversion=1;
	ADC_InitStruct.ADC_Resolution=ADC_Resolution_12b;
	ADC_InitStruct.ADC_ScanConvMode = DISABLE;

	ADC_Init(ADC1, &ADC_InitStruct);

	ADC_Cmd(ADC1, ENABLE);
}
/**********************************************************
 * �� �� �� �ƣ�Get_Adc
 * �� �� �� �ܣ����ĳ��ͨ��ֵ
 * �� �� �� ����CHx��ͨ������
 * �� �� �� �أ���
 * ��       �ߣ�LC
 * ��       ע��LP
**********************************************************/
uint16_t Get_Adc(uint8_t CHx) 	 
{
	
	ADC_RegularChannelConfig(ADC1, CHx, 1, ADC_SampleTime_480Cycles );			    
  
	ADC_SoftwareStartConv(ADC1);	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));
 
	return ADC_GetConversionValue(ADC1);
}
 
 /**********************************************************
 * �� �� �� �ƣ�Get_Adc_Average
 * �� �� �� �ܣ�����ó�ĳ��ͨ����������������ƽ��ֵ 
 * �� �� �� ����CHx		��ͨ������
 *				times	���ɼ�����
 * �� �� �� �أ���
 * ��       �ߣ�LC
 * ��       ע��LP
**********************************************************/
uint16_t Get_Adc_Average(uint8_t CHx,uint8_t times)// 
{
	
	uint32_t value = 0;
	uint8_t t;
	
	for(t=0;t<times;t++)
	{
		value += Get_Adc(CHx);
		delay_ms(5);
	}
	return value/times;
}



