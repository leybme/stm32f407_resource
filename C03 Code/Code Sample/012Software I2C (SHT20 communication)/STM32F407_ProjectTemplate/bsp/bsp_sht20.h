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


#define SDA_IN()  {SHT20_MODE_SET( GPIO_Mode_IN  );}  //SDA输入模式
#define SDA_OUT() {SHT20_MODE_SET( GPIO_Mode_OUT );}  //SDA输出模式

#define SCL(BIT)  GPIO_WriteBit(PORT_SCL, GPIO_SCL, BIT)
#define SDA(BIT)  GPIO_WriteBit(PORT_SDA, GPIO_SDA, BIT)
#define SDA_GET() GPIO_ReadInputDataBit(PORT_SDA, GPIO_SDA)


void SHT20_GPIO_INIT(void);
float SHT20_Read(unsigned char regaddr);
void SHT20_MODE_SET( uint8_t __mode );



#endif
