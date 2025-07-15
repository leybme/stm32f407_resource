/*
 * 立创开发板软硬件资料与相关扩展板软硬件资料官网全部开源
 * 开发板官网：www.lckfb.com
 * 技术支持常驻论坛，任何技术问题欢迎随时交流学习
 * 立创论坛：https://oshwhub.com/forum
 * 关注bilibili账号：【立创开发板】，掌握我们的最新动态！
 * 不靠卖板赚钱，以培养中国工程师为己任
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
	
	PWR_BackupAccessCmd(ENABLE);//使能备份寄存器操作

	RCC_LSEConfig(RCC_LSE_ON); // 打开外部低速晶振

	
	//选择LSE作为RTC时钟 LSE为外部低速晶振
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
	RCC_RTCCLKCmd(ENABLE); // 使能RTC时钟
	RTC_WaitForSynchro();

	if(RTC_ReadBackupRegister(RTC_BKP_DR0) != 0x2002)   //一个变量，看看RTC初始化没
	{
		printf("Set Time and Date.....\r\n");
		RtcTimeConfig(24,3,11,1,18,10,1, RTC_H12_AM);
		printf("Set End!!\r\n");
	}
	PWR_BackupAccessCmd(DISABLE); // 关闭备份寄存器
}

/***********************************
 * 函数名  ：RtcTimeConfig
 * 函数功能：设置日期时间
 * @defgroup RTC_AM_PM_Definitions 
 *
 *  #define RTC_H12_AM   ((uint8_t)0x00)
 *  #define RTC_H12_PM   ((uint8_t)0x40)
 ***********************************/
void RtcTimeConfig(uint8_t year, uint8_t month, uint8_t date, uint8_t week, \
uint8_t hour, uint8_t minute, uint8_t second, uint8_t RTC_H12)
{
		RTC_WriteProtectionCmd(DISABLE); // 打开写保护
		
		RTC_InitTypeDef RTC_InitStructure;
		
		RTC_EnterInitMode(); // 使能RTC编辑模式
		RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24; // 设置为24小时制
		RTC_InitStructure.RTC_AsynchPrediv = 0x7F;
		RTC_InitStructure.RTC_SynchPrediv = 0xFF;
		RTC_Init(&RTC_InitStructure);

		 // 时间设定
		RTC_TimeTypeDef RTC_TimeStructure;
		
		RTC_TimeStructure.RTC_Seconds = second;
		RTC_TimeStructure.RTC_Minutes = minute;
		RTC_TimeStructure.RTC_Hours = hour;
		RTC_TimeStructure.RTC_H12 = RTC_H12;
		RTC_SetTime(RTC_Format_BIN,&RTC_TimeStructure);

		// 日期设定
		RTC_DateTypeDef RTC_DateStructure;
		
		RTC_DateStructure.RTC_Date = date;
		RTC_DateStructure.RTC_Month = month;
		RTC_DateStructure.RTC_WeekDay= week;
		RTC_DateStructure.RTC_Year = year;
		RTC_SetDate(RTC_Format_BIN,&RTC_DateStructure);

		RTC_ExitInitMode(); // 关闭RTC编辑模式
		RTC_WriteBackupRegister(RTC_BKP_DR0,0x2002);//初始化完成，设置标志
		RTC_WriteProtectionCmd(ENABLE);

}

/**********************************************************
 * 函 数 名 称：BcdToDecimal
 * 函 数 功 能：BCD转10进制
 * 传 入 参 数：bcd = BCD码
 * 函 数 返 回：转换完成的10进制
 * 作       者：LCKFB
 * 备       注：无
**********************************************************/
int BcdToDecimal(int bcd) 
{
	int decimal = 0;
	int temp = 1;
	int number = 0;
		
	if( bcd >= 0x0A ) // 如果大于或等于10
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
		// 时间结构体
		RTC_TimeTypeDef RTC_TimeStructure;
		// 日期结构体
		RTC_DateTypeDef RTC_DateStructure;	
		
        //	获取RTC日期
        RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);  
		// 获取RTC时间
		RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);
                
        //将获取到的RTC时间BIN二进制再通过串口输出
        printf("Current time: %d:%d:%d  ", \
        RTC_TimeStructure.RTC_Hours,  
        RTC_TimeStructure.RTC_Minutes, 
        RTC_TimeStructure.RTC_Seconds);
                
        printf("%d-%d-%d\n\r", \
        RTC_DateStructure.RTC_Year, 
        RTC_DateStructure.RTC_Month, 
        RTC_DateStructure.RTC_Date);
}
