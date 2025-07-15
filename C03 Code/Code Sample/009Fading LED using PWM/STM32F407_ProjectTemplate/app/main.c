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
 * 2024-03-08     LCKFB-LP    first version
 */
#include "board.h"
#include "bsp_uart.h"
#include <stdio.h>

void TIM3_PWM_Init(u16 arr,u16 psc);
void pwm_breathing_lamp(void);

int main(void)
{

	board_init();
	
	uart1_init(115200U);

	TIM3_PWM_Init(1000, 500);
	
	while(1)
	{
		pwm_breathing_lamp();
	}
	

}


void TIM3_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,  ENABLE);	//ʹ�ܶ�ʱ��3ʱ��
 	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);   //ʹ��GPIO����
	   
 
   //���ø�����Ϊ�����������,���TIM3 CH2��PWM���岨��	GPIOB5
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIO
	
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_TIM3); //����
 
   //��ʼ��TIM3
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =psc - 1; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; // ʱ�ӷ�Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	//��ʼ��TIM3 Channel2 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM3 OC2
 
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR2�ϵ�Ԥװ�ؼĴ���
 
	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIM3
	
}

void pwm_breathing_lamp(void)
{
	uint32_t brightness = 0;// ��ǰ����
	int step = 10; 			// ���ȸı�Ĳ���
	
	// ����������
	for(brightness = 0; brightness < 1000; brightness += step)
	{
		TIM_SetCompare2(TIM3, brightness); // ʹ��TIM3��Channel 2
		delay_ms(10);
	}
	
	// �𽥼�������
	for(brightness = 1000; brightness > 10; brightness -= step)
	{
		TIM_SetCompare2(TIM3, brightness); // ʹ��TIM3��Channel 2
		delay_ms(10);
	}
}

