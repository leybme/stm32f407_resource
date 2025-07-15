/*
 * 立创开发板软硬件资料与相关扩展板软硬件资料官网全部开源
 * 开发板官网：www.lckfb.com
 * 技术支持常驻论坛，任何技术问题欢迎随时交流学习
 * 立创论坛：https://oshwhub.com/forum
 * 关注bilibili账号：【立创开发板】，掌握我们的最新动态！
 * 不靠卖板赚钱，以培养中国工程师为己任
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
	
	/* SPI初始化 */
	bsp_spi_init();
	
	// 缓存区数组定义
	unsigned char buff[20] = {0};
	
	printf("\r\n=========【开始】========\r\n");
	
	// 擦除Flash扇区0
	printf("\r\n【1】擦除Flash扇区0......\r\n");
	W25Q128_erase_sector(0);
	printf("Flash扇区0擦除完成！！\r\n");
	delay_ms(200);
	
	//获取W25Q128的设备ID
	printf("\r\n【2】读取设备的id......\r\n");
	printf("设备ID = %X\r\n",W25Q128_readID());
			
	//读取0地址长度为10个字节的数据到buff
	printf("\r\n【3】读取0地址长度为10个字节的数据到buff......\r\n");
	W25Q128_read(buff, 0, 10);
	//输出读取到的数据
	printf("读取到的数据= %s\r\n",buff);
	delay_ms(200);
	
	//往0地址写入10个字节的数据 “立创开发板”
	printf("\r\n【4】往0地址写入10个字节的数据 “立创开发板”......\r\n");
	W25Q128_write((uint8_t *)"立创开发板", 0, 10);
	// 等待写入完成
	delay_ms(200);
	printf("数据写入成功！\r\n");

	//读取0地址长度为10个字节的数据到buff
	printf("\r\n【5】读取0地址长度为10个字节的数据到buff......\r\n");
	W25Q128_read(buff, 0, 10);
	//输出读取到的数据
	printf("读取到的数据= %s\r\n",buff);
	delay_ms(1000);
		
	// 因为是从0地址写的，所以擦除第0个扇区
	printf("\r\n【6】功能测试完毕擦除写入的数据......\r\n");
	W25Q128_erase_sector(0);
	delay_ms(200);

	// 清除缓存区
	memset(buff,0,sizeof(buff));
	
	printf("\r\n=========【结束】========\r\n");

	while(1) 
	{
		delay_ms(100);			
	}
	

}
