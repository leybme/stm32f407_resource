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
#ifndef __SPI_FLASH_H__
#define __SPI_FLASH_H__

#include "stm32f4xx.h"


#define BSP_GPIO_RCU                  RCC_AHB1Periph_GPIOA // GPIOʱ��
#define BSP_SPI_RCU                   RCC_APB2Periph_SPI1  // SPIʱ��
#define BSP_SPI_NSS_RCU               RCC_AHB1Periph_GPIOA // CS����ʱ��

#define BSP_GPIO_PORT                 GPIOA
#define BSP_GPIO_AF                   GPIO_AF_SPI1

#define BSP_SPI                       SPI1

#define BSP_SPI_NSS                   GPIO_Pin_4        // ���CS
        
#define BSP_SPI_SCK                   GPIO_Pin_5
#define BSP_SPI_SCK_PINSOURCE         GPIO_PinSource5

#define BSP_SPI_MISO                  GPIO_Pin_6
#define BSP_SPI_MISO_PINSOURCE        GPIO_PinSource6

#define BSP_SPI_MOSI                  GPIO_Pin_7
#define BSP_SPI_MOSI_PINSOURCE        GPIO_PinSource7


#define W25QXX_CS_ON(x)               GPIO_WriteBit(BSP_GPIO_PORT, BSP_SPI_NSS, x ? Bit_SET : Bit_RESET)


void bsp_spi_init(void);
uint8_t spi_read_write_byte(uint8_t dat);
uint16_t W25Q128_readID(void);
void W25Q128_write_enable(void);
void W25Q128_wait_busy(void);
void W25Q128_erase_sector(uint32_t addr);
void W25Q128_write(uint8_t* buffer, uint32_t addr, uint16_t numbyte);
void W25Q128_read(uint8_t* buffer,uint32_t read_addr,uint16_t read_length);



#endif
