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
#include "spi_flash.h"
#include "board.h"

/**********************************************************
 * �� �� �� �ƣ�bsp_spi_init
 * �� �� �� �ܣ���ʼ��SPI
 * �� �� �� ������
 * �� �� �� �أ���
 * ��       �ߣ�LC
 * ��       ע����
**********************************************************/
void bsp_spi_init(void)
{
    SPI_InitTypeDef  SPI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    /* ʹ�� GPIO ʱ�� */
    RCC_AHB1PeriphClockCmd (BSP_GPIO_RCU, ENABLE);

    /* SPIʱ��ʹ�� */
    RCC_APB2PeriphClockCmd(BSP_SPI_RCU, ENABLE);

    /* �������Ÿ��� */
    GPIO_PinAFConfig(BSP_GPIO_PORT, BSP_SPI_SCK_PINSOURCE, BSP_GPIO_AF); 
    GPIO_PinAFConfig(BSP_GPIO_PORT, BSP_SPI_MISO_PINSOURCE, BSP_GPIO_AF); 
    GPIO_PinAFConfig(BSP_GPIO_PORT, BSP_SPI_MOSI_PINSOURCE, BSP_GPIO_AF); 

    /* ����SPI��������SCK */
    GPIO_InitStructure.GPIO_Pin = BSP_SPI_SCK;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;  

    GPIO_Init(BSP_GPIO_PORT, &GPIO_InitStructure);

    /* ����SPI����MISO */
    GPIO_InitStructure.GPIO_Pin = BSP_SPI_MISO;
    GPIO_Init(BSP_GPIO_PORT, &GPIO_InitStructure);

    /* ����SPI����MOSI */
    GPIO_InitStructure.GPIO_Pin = BSP_SPI_MOSI;
    GPIO_Init(BSP_GPIO_PORT, &GPIO_InitStructure);  

    /* ����SPI����CS */
    GPIO_InitStructure.GPIO_Pin = BSP_SPI_NSS;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_Init(BSP_GPIO_PORT, &GPIO_InitStructure);

    /* CS���Ÿߵ�ƽ */
    W25QXX_CS_ON(1);

    /* FLASH_SPI ģʽ���� */
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; // ����ģʽȫ˫��
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;                // ����Ϊ����
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;            // 8λ����
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;                // ������λ
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;                   // ���cs
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2; // SPIʱ��Ԥ������Ϊ2
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;        //��λ��ǰ
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(BSP_SPI, &SPI_InitStructure);

    /* ʹ�� FLASH_SPI  */
    SPI_Cmd(BSP_SPI, ENABLE);
	
	W25QXX_CS_ON(1); 	// Ƭѡ����
}


uint8_t spi_read_write_byte(uint8_t dat)
{
    //�ȴ����ͻ�����Ϊ��
    while(RESET == SPI_I2S_GetFlagStatus(BSP_SPI,  SPI_I2S_FLAG_TXE) );
    //ͨ��SPI4����һ���ֽ����� 
    SPI_I2S_SendData(BSP_SPI, dat);
 
    //�ȴ����ջ��������ձ�־
    while(RESET == SPI_I2S_GetFlagStatus(BSP_SPI,  SPI_I2S_FLAG_RXNE) );
    //��ȡ��������SPI��ȡ���ĵ��ֽ�����
    return SPI_I2S_ReceiveData(BSP_SPI);
}

//��ȡоƬID          
//��ȡ�豸ID
uint16_t W25Q128_readID(void)
{
    uint16_t  temp = 0;     
    //��CS������Ϊ�͵�ƽ     
    W25QXX_CS_ON(0);        
    //����ָ��90h    
    spi_read_write_byte(0x90);//���Ͷ�ȡID����      
    //���͵�ַ  000000H    
    spi_read_write_byte(0x00);             
    spi_read_write_byte(0x00);             
    spi_read_write_byte(0x00); 
        
    //��������
    //����������ID
    temp |= spi_read_write_byte(0xFF)<<8;  
    //�����豸ID
    temp |= spi_read_write_byte(0xFF);        
    //�ָ�CS��Ϊ�ߵ�ƽ
    W25QXX_CS_ON(1);      
    //����ID                  
    return temp;
}

//����дʹ��
void W25Q128_write_enable(void)   
{
    //����CS��Ϊ�͵�ƽ
    W25QXX_CS_ON(0);                          
    //����ָ��06h
    spi_read_write_byte(0x06);                  
    //����CS��Ϊ�ߵ�ƽ
    W25QXX_CS_ON(1);
}

/**********************************************************
 * �� �� �� �ƣ�W25Q128_wait_busy
 * �� �� �� �ܣ������·�Ƿ�æ
 * �� �� �� ������
 * �� �� �� �أ���
 * ��       �ߣ�LC
 * ��       ע����
**********************************************************/
void W25Q128_wait_busy(void)   
{   
    unsigned char byte = 0;
    do
     { 
        //����CS��Ϊ�͵�ƽ
        W25QXX_CS_ON(0); 
        //����ָ��05h                           
        spi_read_write_byte(0x05);                
        //����״̬�Ĵ���ֵ
        byte = spi_read_write_byte(0Xff);       
        //�ָ�CS��Ϊ�ߵ�ƽ
        W25QXX_CS_ON(1);      
     //�ж�BUSYλ�Ƿ�Ϊ1 ���Ϊ1˵����æ�����¶�дBUSYλֱ��Ϊ0   
     }while( ( byte & 0x01 ) == 1 );  
}

/**********************************************************
 * �� �� �� �ƣ�W25Q128_erase_sector
 * �� �� �� �ܣ�����һ������
 * �� �� �� ����addr=������������
 * �� �� �� �أ���
 * ��       �ߣ�LC
 * ��       ע��addr=�����������ţ���Χ=0~4096��

W25Q128��16M��������Ϊ256���飨Block),ÿ�����СΪ64K��64000�����ֽڣ�ÿ�����ַ�Ϊ16��������Sector),ÿ������4K���ֽڡ�
W25Q128����С������λΪһ��������Ҳ����ÿ�α������4K���ֽڡ�
**********************************************************/
void W25Q128_erase_sector(uint32_t addr)   
{
        //���������ţ�һ������4KB=4096
        addr *= 4096;
        W25Q128_write_enable();  //дʹ��   
        W25Q128_wait_busy();     //�ж�æ�����æ��һֱ�ȴ�
        //����CS��Ϊ�͵�ƽ
        W25QXX_CS_ON(0);  
        //����ָ��20h                                     
        spi_read_write_byte(0x20);
        //����24λ������ַ�ĸ�8λ                
        spi_read_write_byte((uint8_t)((addr)>>16));      
        //����24λ������ַ����8λ    
        spi_read_write_byte((uint8_t)((addr)>>8));   
        //����24λ������ַ�ĵ�8λ    
        spi_read_write_byte((uint8_t)addr);
        //�ָ�CS��Ϊ�ߵ�ƽ  
        W25QXX_CS_ON(1);                  
        //�ȴ��������                                                  
        W25Q128_wait_busy();   
}

/**********************************************************
 * �� �� �� �ƣ�W25Q128_write
 * �� �� �� �ܣ�д���ݵ�W25Q128���б���
 * �� �� �� ����buffer=д�����������  addr=д���ַ  numbyte=д�����ݵĳ���
 * �� �� �� �أ���
 * ��       �ߣ�LC
 * ��       ע����
**********************************************************/
void W25Q128_write(uint8_t* buffer, uint32_t addr, uint16_t numbyte)
{    
    unsigned int i = 0;
    //������������
    W25Q128_erase_sector(addr/4096);
    //дʹ�� 
    W25Q128_write_enable();  
    //æ���  
    W25Q128_wait_busy();    
    //д������
    //����CS��Ϊ�͵�ƽ
    W25QXX_CS_ON(0);         
    //����ָ��02h                              
    spi_read_write_byte(0x02);                 
    //����д���24λ��ַ�еĸ�8λ   
    spi_read_write_byte((uint8_t)((addr)>>16));  
    //����д���24λ��ַ�е���8λ
    spi_read_write_byte((uint8_t)((addr)>>8));   
    //����д���24λ��ַ�еĵ�8λ
    spi_read_write_byte((uint8_t)addr);   
    //����д����ֽڳ�������д������buffer
    for(i=0;i<numbyte;i++)
    {
        spi_read_write_byte(buffer[i]);  
    }
    //�ָ�CS��Ϊ�ߵ�ƽ
    W25QXX_CS_ON(1);
    //æ��� 
    W25Q128_wait_busy();      
}

/**********************************************************
 * �� �� �� �ƣ�W25Q128_read
 * �� �� �� �ܣ���ȡW25Q128������
 * �� �� �� ����buffer=�������ݵı����ַ  read_addr=��ȡ��ַ   read_length=��ȥ����
 * �� �� �� �أ���
 * ��       �ߣ�LC
 * ��       ע����
**********************************************************/
void W25Q128_read(uint8_t* buffer,uint32_t read_addr,uint16_t read_length)   
{ 
        uint16_t i;                   
        //����CS��Ϊ�͵�ƽ
        W25QXX_CS_ON(0);    
        //����ָ��03h        
        spi_read_write_byte(0x03);  
        //����24λ��ȡ���ݵ�ַ�ĸ�8λ                         
        spi_read_write_byte((uint8_t)((read_addr)>>16));     
        //����24λ��ȡ���ݵ�ַ����8λ      
        spi_read_write_byte((uint8_t)((read_addr)>>8));   
        //����24λ��ȡ���ݵ�ַ�ĵ�8λ
        spi_read_write_byte((uint8_t)read_addr);   
        //���ݶ�ȡ���ȶ�ȡ����ַ���浽buffer��
        for(i=0;i<read_length;i++)
        { 
            buffer[i]= spi_read_write_byte(0XFF);  
        }
        //�ָ�CS��Ϊ�ߵ�ƽ
        W25QXX_CS_ON(1);                                    
}
