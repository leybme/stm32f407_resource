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
#include "rtc.h"
#include "bsp_uart.h"
#include "stdio.h"
void bsp_rtc_init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	
	PWR_BackupAccessCmd(ENABLE);//ʹ�ܱ��ݼĴ�������

	RCC_LSEConfig(RCC_LSE_ON); // ���ⲿ���پ���

	
	//ѡ��LSE��ΪRTCʱ�� LSEΪ�ⲿ���پ���
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
	RCC_RTCCLKCmd(ENABLE); // ʹ��RTCʱ��
	RTC_WaitForSynchro();

	if(RTC_ReadBackupRegister(RTC_BKP_DR0) != 0x2002)   //һ������������RTC��ʼ��û
	{
		printf("Set Time and Date.....\r\n");
		RtcTimeConfig(24,3,11,1,18,10,1, RTC_H12_AM);
		printf("Set End!!\r\n");
	}
	PWR_BackupAccessCmd(DISABLE); // �رձ��ݼĴ���
}

/***********************************
 * ������  ��RtcTimeConfig
 * �������ܣ���������ʱ��
 * @defgroup RTC_AM_PM_Definitions 
 *
 *  #define RTC_H12_AM   ((uint8_t)0x00)
 *  #define RTC_H12_PM   ((uint8_t)0x40)
 ***********************************/
void RtcTimeConfig(uint8_t year, uint8_t month, uint8_t date, uint8_t week, \
uint8_t hour, uint8_t minute, uint8_t second, uint8_t RTC_H12)
{
		RTC_WriteProtectionCmd(DISABLE); // ��д����
		
		RTC_InitTypeDef RTC_InitStructure;
		
		RTC_EnterInitMode(); // ʹ��RTC�༭ģʽ
		RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24; // ����Ϊ24Сʱ��
		RTC_InitStructure.RTC_AsynchPrediv = 0x7F;
		RTC_InitStructure.RTC_SynchPrediv = 0xFF;
		RTC_Init(&RTC_InitStructure);

		 // ʱ���趨
		RTC_TimeTypeDef RTC_TimeStructure;
		
		RTC_TimeStructure.RTC_Seconds = second;
		RTC_TimeStructure.RTC_Minutes = minute;
		RTC_TimeStructure.RTC_Hours = hour;
		RTC_TimeStructure.RTC_H12 = RTC_H12;
		RTC_SetTime(RTC_Format_BIN,&RTC_TimeStructure);

		// �����趨
		RTC_DateTypeDef RTC_DateStructure;
		
		RTC_DateStructure.RTC_Date = date;
		RTC_DateStructure.RTC_Month = month;
		RTC_DateStructure.RTC_WeekDay= week;
		RTC_DateStructure.RTC_Year = year;
		RTC_SetDate(RTC_Format_BIN,&RTC_DateStructure);

		RTC_ExitInitMode(); // �ر�RTC�༭ģʽ
		RTC_WriteBackupRegister(RTC_BKP_DR0,0x2002);//��ʼ����ɣ����ñ�־
		RTC_WriteProtectionCmd(ENABLE);

}

/**********************************************************
 * �� �� �� �ƣ�BcdToDecimal
 * �� �� �� �ܣ�BCDת10����
 * �� �� �� ����bcd = BCD��
 * �� �� �� �أ�ת����ɵ�10����
 * ��       �ߣ�LCKFB
 * ��       ע����
**********************************************************/
int BcdToDecimal(int bcd) 
{
	int decimal = 0;
	int temp = 1;
	int number = 0;
		
	if( bcd >= 0x0A ) // ������ڻ����10
	{
		while(bcd > 0) 
		{
			number = bcd % 16;
			decimal += number * temp;
			temp *= 10;
			bcd /= 16;
		}
		return decimal;
	}
	return bcd;

}


void RtcShowTime(void)
{
		// ʱ��ṹ��
		RTC_TimeTypeDef RTC_TimeStructure;
		// ���ڽṹ��
		RTC_DateTypeDef RTC_DateStructure;	
		
        //	��ȡRTC����
        RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);  
		// ��ȡRTCʱ��
		RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);
                
        //����ȡ����RTCʱ��BIN��������ͨ���������
        printf("Current time: %d:%d:%d  ", \
        RTC_TimeStructure.RTC_Hours,  
        RTC_TimeStructure.RTC_Minutes, 
        RTC_TimeStructure.RTC_Seconds);
                
        printf("%d-%d-%d\n\r", \
        RTC_DateStructure.RTC_Year, 
        RTC_DateStructure.RTC_Month, 
        RTC_DateStructure.RTC_Date);
}
