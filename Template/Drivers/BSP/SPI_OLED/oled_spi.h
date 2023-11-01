/****************************************************************************************
OLED驱动程序
 by herexiong
基于SPI通讯协议
使用与七线OLED模块，分辨率128*64
使用4线SPI通讯
修改历史
* V1.1 2023/11/01
	添加了画点函数，可以使用画点函数在对应位置画点，然后将其转换为显示数组显示
	先通过画点函数操作屏幕上的像素，然后将像素转换通过显示BMP图像的方式显示
	现在可以按坐标随意操作屏幕像素
****************************************************************************************/
#ifndef __SPI_OLED_H
#define __SPI_OLED_H
#include "stm32f1xx.h"

static const unsigned char  OLED_init_cmd[25]=
{
  /*0xae,0X00,0X10,0x40,0X81,0XCF,0xff,0xa1,0xa4,
  0xA6,0xc8,0xa8,0x3F,0xd5,0x80,0xd3,0x00,0XDA,0X12,
  0x8d,0x14,0xdb,0x40,0X20,0X02,0xd9,0xf1,0xAF*/
       0xAE,//关闭显示
       0xD5,//设置时钟分频因子,震荡频率
       0x80,  //[3:0],分频因子;[7:4],震荡频率
       0xA8,//设置驱动路数
       0X3F,//默认0X3F(1/64)
       0xD3,//设置显示偏移
       0X00,//默认为0
       0x40,//设置显示开始行 [5:0],行数.                              
       0x8D,//电荷泵设置
       0x14,//bit2，开启/关闭
       0x20,//设置内存地址模式
       0x02,//[1:0],00，列地址模式;01，行地址模式;10,页地址模式;默认10;
       0xA1,//段重定义设置,bit0:0,0->0;1,0->127;
       0xC8,//设置COM扫描方向;bit3:0,普通模式;1,重定义模式 COM[N-1]->COM0;N:驱动路数
       0xDA,//设置COM硬件引脚配置
       0x12,//[5:4]配置            
       0x81,//对比度设置
       0xEF,//1~255;默认0X7F (亮度设置,越大越亮)
       0xD9,//设置预充电周期
       0xf1,//[3:0],PHASE 1;[7:4],PHASE 2;
       0xDB,//设置VCOMH 电压倍率
       0x30,//[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;
       0xA4,//全局显示开启;bit0:1,开启;0,关闭;(白屏/黑屏)
       0xA6,//设置显示方式;bit0:1,反相显示;0,正常显示        
       0xAF,//开启显示     
};

/************************等待超时时间*******************************/
#define SPIT_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define SPIT_LONG_TIMEOUT         ((uint32_t)(10 * SPIT_FLAG_TIMEOUT))

/*************************SPI时钟定义-开始****************************/
#define SPIx                             SPI1
#define SPIx_CLK_ENABLE()                __HAL_RCC_SPI1_CLK_ENABLE()
#define SPIx_SCK_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOA_CLK_ENABLE()
#define SPIx_MISO_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE() 
#define SPIx_MOSI_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE() 
#define SPIx_CS_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOA_CLK_ENABLE() 
#define SPIx_DC_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOB_CLK_ENABLE() 

#define SPIx_FORCE_RESET()               __HAL_RCC_SPI1_FORCE_RESET()
#define SPIx_RELEASE_RESET()             __HAL_RCC_SPI1_RELEASE_RESET()

/******************************SPI接口定义-开头****************************/
/*
D0->CLK
D1->MOSI
DC-> LOW-commd HIGH-data
*/
#define SPIx_SCK_PIN                     GPIO_PIN_5
#define SPIx_SCK_GPIO_PORT               GPIOA

#define SPIx_MISO_PIN                    GPIO_PIN_6
#define SPIx_MISO_GPIO_PORT              GPIOA

#define SPIx_MOSI_PIN                    GPIO_PIN_7
#define SPIx_MOSI_GPIO_PORT              GPIOA

#define FLASH_CS_PIN                     GPIO_PIN_4               
#define FLASH_CS_GPIO_PORT               GPIOA 

#define OLED_DC_PIN                     GPIO_PIN_1              
#define OLED_DC_GPIO_PORT               GPIOB 

#define OLED_RES_PIN                     GPIO_PIN_0              
#define OLED_RES_GPIO_PORT               GPIOB 

#define	digitalHi(p,i)			    {p->BSRR=i;}			  //设置为高电平		
#define digitalLo(p,i)			    {p->BSRR=(uint32_t)i << 16;}	//输出低电平
#define SPI_FLASH_CS_LOW()      digitalLo(FLASH_CS_GPIO_PORT,FLASH_CS_PIN )
#define SPI_FLASH_CS_HIGH()     digitalHi(FLASH_CS_GPIO_PORT,FLASH_CS_PIN )
#define SPI_OLED_DC_LOW()      digitalLo(OLED_DC_GPIO_PORT,OLED_DC_PIN )
#define SPI_OLED_DC_HIGH()     digitalHi(OLED_DC_GPIO_PORT,OLED_DC_PIN )
/********************************SPI接口定义-结尾*******************************************/
/*信息输出*/
#define OLED_DEBUG_ON         1

#define OLED_INFO(fmt,arg...)           printf("<<-OLED-INFO->> "fmt"\n",##arg)
#define OLED_ERROR(fmt,arg...)          printf("<<-OLED-ERROR->> "fmt"\n",##arg)
#define OLED_DEBUG(fmt,arg...)          do{\
                                          if(OLED_DEBUG_ON)\
                                          printf("<<-OLED-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
                                          }while(0)
/******************************************缓冲区**************************************************/
//uint8_t OLED_buffer[8192];
//uint8_t OLED_display[1024];				
extern uint8_t OLED_buffer[8192];
extern uint8_t OLED_display[1024];										  
/*******************************************函数声明***********************************************/
void OLED_SPI_init(void);
										  
uint8_t OLED_WriteCMD(uint8_t byte);
uint8_t OLED_WriteDATA(uint8_t byte);
uint8_t SPI_SendByte(uint8_t byte);	
										  
void OLED_SPI_set_page(unsigned char page);
void OLED_SPI_set_column(unsigned char column);
void OLED_SPI_set_pos(uint8_t x, uint8_t y);
										  
void OLED_SPI_dis_char(uint8_t x, uint8_t y, uint8_t str,uint8_t size);
void OLED_SPI_dis_str(uint8_t x, uint8_t y, uint8_t *str, uint8_t size);
void OLED_SPI_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);								  
										  
void OLED_SPI_full(void);
void OLED_SPI_clear(void);										  
void OLED_SPI_on(void);
void OLED_SPI_off(void);	

void drawPoint(uint8_t *OLED_buffer,uint8_t x,uint8_t y);
void drawNode(uint8_t *OLED_buffer,uint8_t x,uint8_t y);
uint8_t drawAllNode(uint8_t *temp,uint8_t *OLED_buffer);
void oledTrans(uint8_t *OLED_buffer,uint8_t *OLED_display);										  
#endif
