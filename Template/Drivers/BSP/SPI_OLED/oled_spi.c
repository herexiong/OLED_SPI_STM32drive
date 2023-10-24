#include "./BSP/SPI_OLED/oled_spi.h"

static SPI_HandleTypeDef SpiHandle;
static __IO uint32_t  SPITimeout = SPIT_LONG_TIMEOUT;   
static uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode);
extern const unsigned char F6x8[];
extern const unsigned char F8X16[];

static  uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode)
{
  /* �ȴ���ʱ��Ĵ���,���������Ϣ */
  OLED_ERROR("SPI �ȴ���ʱ!errorCode = %d",errorCode);
  return 0;
}

/*******************************************************
*��������OLED_SPI_init
*��  �ܣ���ʼ��OLED����SPI��GPIO�ӿ�
*��  ������   
*��  �أ���
*��  ע��ʹ��OLEDǰ��Ҫʹ�ó�ʼ������
********************************************************/
void OLED_SPI_init(void)
  {
	GPIO_InitTypeDef  GPIO_InitStruct={0};
	/*����SPI��GPIO��ʱ��*/
	SPIx_CLK_ENABLE(); 
	SPIx_SCK_GPIO_CLK_ENABLE();
	SPIx_MISO_GPIO_CLK_ENABLE();
	SPIx_MOSI_GPIO_CLK_ENABLE();
	SPIx_CS_GPIO_CLK_ENABLE();
	SPIx_DC_GPIO_CLK_ENABLE();
	/* ����SPI */ 
	SpiHandle.Instance               = SPIx;
	SpiHandle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
	SpiHandle.Init.Direction         = SPI_DIRECTION_2LINES;
	SpiHandle.Init.CLKPhase          = SPI_PHASE_2EDGE;
	SpiHandle.Init.CLKPolarity       = SPI_POLARITY_HIGH;
	SpiHandle.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
	SpiHandle.Init.CRCPolynomial     = 7;
	SpiHandle.Init.DataSize          = SPI_DATASIZE_8BIT;
	SpiHandle.Init.FirstBit          = SPI_FIRSTBIT_MSB;
	SpiHandle.Init.NSS               = SPI_NSS_SOFT;
	SpiHandle.Init.TIMode            = SPI_TIMODE_DISABLE;

	SpiHandle.Init.Mode = SPI_MODE_MASTER;

	HAL_SPI_Init(&SpiHandle); 

	__HAL_SPI_ENABLE(&SpiHandle); 	
	/*##-2- Configure peripheral GPIO ##########################################*/  
	/* SPI SCK GPIO pin ����  */
	GPIO_InitStruct.Pin       = SPIx_SCK_PIN;
	GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull      = GPIO_PULLUP;
	GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(SPIx_SCK_GPIO_PORT, &GPIO_InitStruct);

	/* SPI MISO GPIO pin ����  */
	GPIO_InitStruct.Pin = SPIx_MISO_PIN;  
	HAL_GPIO_Init(SPIx_MISO_GPIO_PORT, &GPIO_InitStruct);

	/* SPI MOSI GPIO pin ����  */
	GPIO_InitStruct.Pin = SPIx_MOSI_PIN; 
	HAL_GPIO_Init(SPIx_MOSI_GPIO_PORT, &GPIO_InitStruct);   

	/* SPI CS GPIO pin ����  */
	GPIO_InitStruct.Pin = FLASH_CS_PIN ;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	HAL_GPIO_Init(FLASH_CS_GPIO_PORT, &GPIO_InitStruct); 

	/* SPI DC GPIO pin ����  */
	GPIO_InitStruct.Pin = OLED_DC_PIN ;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	HAL_GPIO_Init(OLED_DC_GPIO_PORT, &GPIO_InitStruct); 

	/* SPI RES GPIO pin ����  */
	GPIO_InitStruct.Pin = OLED_RES_PIN ;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	HAL_GPIO_Init(OLED_RES_GPIO_PORT, &GPIO_InitStruct);
	/************��λOLED***************/
	digitalLo(OLED_RES_GPIO_PORT,OLED_RES_PIN);
	digitalHi(OLED_RES_GPIO_PORT,OLED_RES_PIN);	
	/************����OLED����***********/
	for(unsigned char i=0;i<25;i++)
	  {		
		OLED_WriteCMD(OLED_init_cmd[i]);		
	  }
}
/*******************************************************
*��������OLED_SPI_set_column
*��  �ܣ������Դ������
*��  ������   
*��  �أ���
*��  ע��OLED�ķֱ�����128*64��128�ĺ������ر���Ϊ128�У�ÿColumnռ1px
********************************************************/
void OLED_SPI_set_column(unsigned char column)
{	  
	OLED_WriteCMD(0x10|(column>>4));    //�����е�ַ��λ
	OLED_WriteCMD(0x00|(column&0x0f));   //�����е�ַ��λ         	 
}
/*******************************************************
*��������OLED_SPI_set_page
*��  �ܣ������Դ��ҳ��
*��  ������   
*��  �أ���
*��  ע��OLED�ķֱ�����128*64��64����ֱ���ر���Ϊ8page��ÿpageռ8px
********************************************************/
void OLED_SPI_set_page(unsigned char page)
{
OLED_WriteCMD(0xb0+page);
}
/*******************************************************
*��������OLED_SPI_clear
*��  �ܣ���OLED����
*��  ������   
*��  �أ���
*��  ע��
********************************************************/
void OLED_SPI_clear(void)
{
unsigned char page,column; 
for(page=0;page<8;page++)             //page loop
  { 
	  OLED_SPI_set_page(page);
	  OLED_SPI_set_column(0);	  
	  for(column=0;column<128;column++)	//column loop
		{
		  OLED_WriteDATA(0x00);
		}
  }	  
}
/*******************************************************
*��������OLED_SPI_full
*��  �ܣ���OLED��������
*��  ������   
*��  �أ���
*��  ע��
********************************************************/ 
void OLED_SPI_full(void)
{
unsigned char page,column;	  	  
for(page=0;page<8;page++)             //page loop
  { 
	OLED_SPI_set_page(page);
	OLED_SPI_set_column(0);	  
	for(column=0;column<128;column++)	//column loop
	  {
		OLED_WriteDATA(0xff);
	  }
  }  
}
/*******************************************************
*��������OLED_WriteCMD
*��  �ܣ���OLEDд��ָ��
*��  ����byte:Ҫд������   
*��  �أ����ܵ�������
*��  ע��
********************************************************/
uint8_t OLED_WriteCMD(uint8_t byte)
{
	uint8_t RxData = 0;
	SPI_FLASH_CS_LOW();//Ƭѡ
	SPI_OLED_DC_LOW();//����ָ�������ͣ���ʾ��CRд��ָ��
	SPI_SendByte(byte);//����һ���ֽ�����
	SPI_FLASH_CS_HIGH();//ȡ��Ƭѡ
	return RxData;
}

/*******************************************************
*��������OLED_WriteDATA
*��  �ܣ���OLEDд��ָ��
*��  ����byte:Ҫд�������   
*��  �أ����ܵ�������
*��  ע��
********************************************************/
uint8_t OLED_WriteDATA(uint8_t byte)
{
	uint8_t RxData = 0;
	SPI_FLASH_CS_LOW();//Ƭѡ
	SPI_OLED_DC_HIGH();//����ָ�������ߣ���ʾ��DRд������
	RxData = SPI_SendByte(byte);//����һ���ֽ�����
	SPI_FLASH_CS_HIGH();//ȡ��Ƭѡ
	return RxData;
}
/*******************************************************
*��������SPI_SendByte
*��  �ܣ�SPI���ͺ�����һ�ֽڣ�
*��  ����byte:Ҫд�������   
*��  �أ����ܵ�������
*��  ע��SPI��˫��ͨѶ����˷���һ�ֽڵ�ͬʱҲ����յ�һ�ֽ�
*��  ע����Ҫ���ô˺����������ݣ���ʹ��OLED_WriteDATA��OLED_WriteCMD
********************************************************/
uint8_t SPI_SendByte(uint8_t byte)
{
  SPITimeout = SPIT_FLAG_TIMEOUT;

  /* �ȴ����ͻ�����Ϊ�գ�TXE�¼� */
  while (__HAL_SPI_GET_FLAG( &SpiHandle, SPI_FLAG_TXE ) == RESET)
   {
    if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(0);
   }

  /* д�����ݼĴ�������Ҫд�������д�뷢�ͻ����� */
  WRITE_REG(SpiHandle.Instance->DR, byte);

  SPITimeout = SPIT_FLAG_TIMEOUT;

  /* �ȴ����ջ������ǿգ�RXNE�¼� */
  while (__HAL_SPI_GET_FLAG( &SpiHandle, SPI_FLAG_RXNE ) == RESET)
   {
    if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(1);
   }

  /* ��ȡ���ݼĴ�������ȡ���ջ��������� */
  return READ_REG(SpiHandle.Instance->DR);
}

/*******************************************************
*��������OLED_SPI_set_pos
*��  �ܣ�oled ��ʾ��������
*��  ����x:������;y��������
*��  �أ���
********************************************************/
void OLED_SPI_set_pos(uint8_t x, uint8_t y)
{
	OLED_SPI_set_page(y);	//����ҳ(page)��ַ
	OLED_SPI_set_column(x);//������(column)��ַ
}

/*******************************************************
*��������OLED_SPI_dis_char
*��  �ܣ�oled ��ʾ�����ַ�
*��  ����x��������(��);y:������(��);str����ʾ�ַ�;size:�ַ���С��8��16
*��  �أ���
********************************************************/
void OLED_SPI_dis_char(uint8_t x, uint8_t y, uint8_t str,uint8_t size)
{
	uint8_t i = 0 , ret = 0;
	ret = str - ' ';//�õ�ƫ�ƺ��ֵ,��ASCLL�����һ������.���ڶ�ά������������λ��  
	if (x > (128 - 1))	//�г������,��������һ��
	{
		x = 0;
		if (size == 8 )
		{
			y = y + 1;//���8�ŵ��ַ�
		}
		if (size == 16 )
		{
			y = y + 2;//���16�ŵ��ַ�
		}
	}
	if (size == 16 )
	{
		OLED_SPI_set_pos(x, y);
		//16������ֳ�������д��
		for (i = 0; i < 8; i++)
		{
			OLED_WriteDATA(F8X16[ret*16+i]);
		}
		
		OLED_SPI_set_pos(x, y + 1);
		for	(i = 0; i < 8; i++)
		{
			OLED_WriteDATA(F8X16[ret*16+i+8]);
		}
	}
	else
	{
		OLED_SPI_set_pos(x, y);
		for(i = 0; i < 6; i++)
		{
			OLED_WriteDATA(F6x8[ret*6+i]);
		}
	}
}

/*******************************************************
*��������OLED_SPI_dis_str
*��  �ܣ�oled ��ʾ����ַ�
*��  ����x��������(��);y:������(��);str����ʾ�ַ���;size:�ַ���С��8��16
*��  �أ���
********************************************************/
void OLED_SPI_dis_str(uint8_t x, uint8_t y, uint8_t *str, uint8_t size)
{
	uint8_t i = 0;	
	while(str[i] != '\0')
	{
		OLED_SPI_dis_char(x, y, str[i], size);
		x += 8;		
		if(x > 120)
		{
			x = 0;
			y += 2;
		}
		i++;
	}
}

/*******************************************************
*��������oled_dis_on
*��  �ܣ�oled ����ʾ
*��  ������
*��  �أ���
********************************************************/
void OLED_SPI_on(void)
{
	OLED_WriteCMD(0x8d);	//��DCDC
	OLED_WriteCMD(0x14);	//��DCDC
	OLED_WriteCMD(0xaf);	//��OLED
}
/*******************************************************
*��������oled_dis_off
*��  �ܣ�oled �ر���ʾ
*��  ������
*��  �أ���
********************************************************/
void OLED_SPI_off(void)
{
	OLED_WriteCMD(0x8d);//����DCDC
	OLED_WriteCMD(0x10);//�ر�DCDC
	OLED_WriteCMD(0xae);//�ر�OLED
}
