/****************************************************************************************
OLED��������
 by herexiong
����SPIͨѶЭ��
ʹ��������OLEDģ�飬�ֱ���128*64
ʹ��4��SPIͨѶ
�޸���ʷ
* V1.1 2023/11/01
	����˻��㺯��������ʹ�û��㺯���ڶ�Ӧλ�û��㣬Ȼ����ת��Ϊ��ʾ������ʾ
	��ͨ�����㺯��������Ļ�ϵ����أ�Ȼ������ת��ͨ����ʾBMPͼ��ķ�ʽ��ʾ
	���ڿ��԰��������������Ļ����
****************************************************************************************/
#ifndef __SPI_OLED_H
#define __SPI_OLED_H
#include "stm32f1xx.h"

static const unsigned char  OLED_init_cmd[25]=
{
  /*0xae,0X00,0X10,0x40,0X81,0XCF,0xff,0xa1,0xa4,
  0xA6,0xc8,0xa8,0x3F,0xd5,0x80,0xd3,0x00,0XDA,0X12,
  0x8d,0x14,0xdb,0x40,0X20,0X02,0xd9,0xf1,0xAF*/
       0xAE,//�ر���ʾ
       0xD5,//����ʱ�ӷ�Ƶ����,��Ƶ��
       0x80,  //[3:0],��Ƶ����;[7:4],��Ƶ��
       0xA8,//��������·��
       0X3F,//Ĭ��0X3F(1/64)
       0xD3,//������ʾƫ��
       0X00,//Ĭ��Ϊ0
       0x40,//������ʾ��ʼ�� [5:0],����.                              
       0x8D,//��ɱ�����
       0x14,//bit2������/�ر�
       0x20,//�����ڴ��ַģʽ
       0x02,//[1:0],00���е�ַģʽ;01���е�ַģʽ;10,ҳ��ַģʽ;Ĭ��10;
       0xA1,//���ض�������,bit0:0,0->0;1,0->127;
       0xC8,//����COMɨ�跽��;bit3:0,��ͨģʽ;1,�ض���ģʽ COM[N-1]->COM0;N:����·��
       0xDA,//����COMӲ����������
       0x12,//[5:4]����            
       0x81,//�Աȶ�����
       0xEF,//1~255;Ĭ��0X7F (��������,Խ��Խ��)
       0xD9,//����Ԥ�������
       0xf1,//[3:0],PHASE 1;[7:4],PHASE 2;
       0xDB,//����VCOMH ��ѹ����
       0x30,//[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;
       0xA4,//ȫ����ʾ����;bit0:1,����;0,�ر�;(����/����)
       0xA6,//������ʾ��ʽ;bit0:1,������ʾ;0,������ʾ        
       0xAF,//������ʾ     
};

/************************�ȴ���ʱʱ��*******************************/
#define SPIT_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define SPIT_LONG_TIMEOUT         ((uint32_t)(10 * SPIT_FLAG_TIMEOUT))

/*************************SPIʱ�Ӷ���-��ʼ****************************/
#define SPIx                             SPI1
#define SPIx_CLK_ENABLE()                __HAL_RCC_SPI1_CLK_ENABLE()
#define SPIx_SCK_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOA_CLK_ENABLE()
#define SPIx_MISO_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE() 
#define SPIx_MOSI_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE() 
#define SPIx_CS_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOA_CLK_ENABLE() 
#define SPIx_DC_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOB_CLK_ENABLE() 

#define SPIx_FORCE_RESET()               __HAL_RCC_SPI1_FORCE_RESET()
#define SPIx_RELEASE_RESET()             __HAL_RCC_SPI1_RELEASE_RESET()

/******************************SPI�ӿڶ���-��ͷ****************************/
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

#define	digitalHi(p,i)			    {p->BSRR=i;}			  //����Ϊ�ߵ�ƽ		
#define digitalLo(p,i)			    {p->BSRR=(uint32_t)i << 16;}	//����͵�ƽ
#define SPI_FLASH_CS_LOW()      digitalLo(FLASH_CS_GPIO_PORT,FLASH_CS_PIN )
#define SPI_FLASH_CS_HIGH()     digitalHi(FLASH_CS_GPIO_PORT,FLASH_CS_PIN )
#define SPI_OLED_DC_LOW()      digitalLo(OLED_DC_GPIO_PORT,OLED_DC_PIN )
#define SPI_OLED_DC_HIGH()     digitalHi(OLED_DC_GPIO_PORT,OLED_DC_PIN )
/********************************SPI�ӿڶ���-��β*******************************************/
/*��Ϣ���*/
#define OLED_DEBUG_ON         1

#define OLED_INFO(fmt,arg...)           printf("<<-OLED-INFO->> "fmt"\n",##arg)
#define OLED_ERROR(fmt,arg...)          printf("<<-OLED-ERROR->> "fmt"\n",##arg)
#define OLED_DEBUG(fmt,arg...)          do{\
                                          if(OLED_DEBUG_ON)\
                                          printf("<<-OLED-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
                                          }while(0)
/******************************************������**************************************************/
//uint8_t OLED_buffer[8192];
//uint8_t OLED_display[1024];				
extern uint8_t OLED_buffer[8192];
extern uint8_t OLED_display[1024];										  
/*******************************************��������***********************************************/
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
