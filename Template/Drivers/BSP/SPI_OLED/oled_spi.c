#include "./BSP/SPI_OLED/oled_spi.h"

static SPI_HandleTypeDef SpiHandle;
static __IO uint32_t  SPITimeout = SPIT_LONG_TIMEOUT;   
static uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode);
extern const unsigned char F6x8[];
extern const unsigned char F8X16[];

static  uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode)
{
  /* 等待超时后的处理,输出错误信息 */
  OLED_ERROR("SPI 等待超时!errorCode = %d",errorCode);
  return 0;
}

/*******************************************************
*函数名：OLED_SPI_init
*功  能：初始化OLED，即SPI和GPIO接口
*参  数：无   
*返  回：无
*备  注：使用OLED前需要使用初始化函数
********************************************************/
void OLED_SPI_init(void)
  {
	GPIO_InitTypeDef  GPIO_InitStruct={0};
	/*启用SPI和GPIO的时钟*/
	SPIx_CLK_ENABLE(); 
	SPIx_SCK_GPIO_CLK_ENABLE();
	SPIx_MISO_GPIO_CLK_ENABLE();
	SPIx_MOSI_GPIO_CLK_ENABLE();
	SPIx_CS_GPIO_CLK_ENABLE();
	SPIx_DC_GPIO_CLK_ENABLE();
	/* 配置SPI */ 
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
	/* SPI SCK GPIO pin 配置  */
	GPIO_InitStruct.Pin       = SPIx_SCK_PIN;
	GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull      = GPIO_PULLUP;
	GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(SPIx_SCK_GPIO_PORT, &GPIO_InitStruct);

	/* SPI MISO GPIO pin 配置  */
	GPIO_InitStruct.Pin = SPIx_MISO_PIN;  
	HAL_GPIO_Init(SPIx_MISO_GPIO_PORT, &GPIO_InitStruct);

	/* SPI MOSI GPIO pin 配置  */
	GPIO_InitStruct.Pin = SPIx_MOSI_PIN; 
	HAL_GPIO_Init(SPIx_MOSI_GPIO_PORT, &GPIO_InitStruct);   

	/* SPI CS GPIO pin 配置  */
	GPIO_InitStruct.Pin = FLASH_CS_PIN ;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	HAL_GPIO_Init(FLASH_CS_GPIO_PORT, &GPIO_InitStruct); 

	/* SPI DC GPIO pin 配置  */
	GPIO_InitStruct.Pin = OLED_DC_PIN ;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	HAL_GPIO_Init(OLED_DC_GPIO_PORT, &GPIO_InitStruct); 

	/* SPI RES GPIO pin 配置  */
	GPIO_InitStruct.Pin = OLED_RES_PIN ;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	HAL_GPIO_Init(OLED_RES_GPIO_PORT, &GPIO_InitStruct);
	/************复位OLED***************/
	digitalLo(OLED_RES_GPIO_PORT,OLED_RES_PIN);
	digitalHi(OLED_RES_GPIO_PORT,OLED_RES_PIN);	
	/************发送OLED命令***********/
	for(unsigned char i=0;i<25;i++)
	  {		
		OLED_WriteCMD(OLED_init_cmd[i]);		
	  }
}
/*******************************************************
*函数名：OLED_SPI_set_column
*功  能：设置显存的行数
*参  数：无   
*返  回：无
*备  注：OLED的分辨率是128*64，128的横向像素被分为128行，每Column占1px
********************************************************/
void OLED_SPI_set_column(unsigned char column)
{	  
	OLED_WriteCMD(0x10|(column>>4));    //设置列地址高位
	OLED_WriteCMD(0x00|(column&0x0f));   //设置列地址低位         	 
}
/*******************************************************
*函数名：OLED_SPI_set_page
*功  能：设置显存的页数
*参  数：无   
*返  回：无
*备  注：OLED的分辨率是128*64，64的竖直像素被分为8page，每page占8px
********************************************************/
void OLED_SPI_set_page(unsigned char page)
{
OLED_WriteCMD(0xb0+page);
}
/*******************************************************
*函数名：OLED_SPI_clear
*功  能：将OLED清屏
*参  数：无   
*返  回：无
*备  注：
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
*函数名：OLED_SPI_full
*功  能：将OLED整个填满
*参  数：无   
*返  回：无
*备  注：
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
*函数名：OLED_WriteCMD
*功  能：往OLED写入指令
*参  数：byte:要写入命令   
*返  回：接受到的数据
*备  注：
********************************************************/
uint8_t OLED_WriteCMD(uint8_t byte)
{
	uint8_t RxData = 0;
	SPI_FLASH_CS_LOW();//片选
	SPI_OLED_DC_LOW();//数据指令线拉低，表示往CR写入指令
	SPI_SendByte(byte);//传输一个字节数据
	SPI_FLASH_CS_HIGH();//取消片选
	return RxData;
}

/*******************************************************
*函数名：OLED_WriteDATA
*功  能：往OLED写入指令
*参  数：byte:要写入的数据   
*返  回：接受到的数据
*备  注：
********************************************************/
uint8_t OLED_WriteDATA(uint8_t byte)
{
	uint8_t RxData = 0;
	SPI_FLASH_CS_LOW();//片选
	SPI_OLED_DC_HIGH();//数据指令线拉高，表示往DR写入数据
	RxData = SPI_SendByte(byte);//传输一个字节数据
	SPI_FLASH_CS_HIGH();//取消片选
	return RxData;
}
/*******************************************************
*函数名：SPI_SendByte
*功  能：SPI发送函数（一字节）
*参  数：byte:要写入的数据   
*返  回：接受到的数据
*备  注：SPI是双工通讯，因此发送一字节的同时也会接收到一字节
*备  注：不要调用此函数发送数据，请使用OLED_WriteDATA或OLED_WriteCMD
********************************************************/
uint8_t SPI_SendByte(uint8_t byte)
{
  SPITimeout = SPIT_FLAG_TIMEOUT;

  /* 等待发送缓冲区为空，TXE事件 */
  while (__HAL_SPI_GET_FLAG( &SpiHandle, SPI_FLAG_TXE ) == RESET)
   {
    if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(0);
   }

  /* 写入数据寄存器，把要写入的数据写入发送缓冲区 */
  WRITE_REG(SpiHandle.Instance->DR, byte);

  SPITimeout = SPIT_FLAG_TIMEOUT;

  /* 等待接收缓冲区非空，RXNE事件 */
  while (__HAL_SPI_GET_FLAG( &SpiHandle, SPI_FLAG_RXNE ) == RESET)
   {
    if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(1);
   }

  /* 读取数据寄存器，获取接收缓冲区数据 */
  return READ_REG(SpiHandle.Instance->DR);
}

/*******************************************************
*函数名：OLED_SPI_set_pos
*功  能：oled 显示坐标设置
*参  数：x:横坐标;y：纵坐标
*返  回：无
********************************************************/
void OLED_SPI_set_pos(uint8_t x, uint8_t y)
{
	OLED_SPI_set_page(y);	//设置页(page)地址
	OLED_SPI_set_column(x);//设置行(column)地址
}

/*******************************************************
*函数名：OLED_SPI_dis_char
*功  能：oled 显示单个字符
*参  数：x：横坐标(列);y:纵坐标(行);str：显示字符;size:字符大小，8或16
*返  回：无
********************************************************/
void OLED_SPI_dis_char(uint8_t x, uint8_t y, uint8_t str,uint8_t size)
{
	uint8_t i = 0 , ret = 0;
	ret = str - ' ';//得到偏移后的值,对ASCLL码进行一个减法.即在二维数组里找它的位置  
	if (x > (128 - 1))	//列超过最大,更换到下一行
	{
		x = 0;
		if (size == 8 )
		{
			y = y + 1;//针对8号的字符
		}
		if (size == 16 )
		{
			y = y + 2;//针对16号的字符
		}
	}
	if (size == 16 )
	{
		OLED_SPI_set_pos(x, y);
		//16的字体分成两部分写入
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
*函数名：OLED_SPI_dis_str
*功  能：oled 显示多个字符
*参  数：x：横坐标(列);y:纵坐标(行);str：显示字符串;size:字符大小，8或16
*返  回：无
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
*函数名：oled_dis_on
*功  能：oled 打开显示
*参  数：无
*返  回：无
********************************************************/
void OLED_SPI_on(void)
{
	OLED_WriteCMD(0x8d);	//打开DCDC
	OLED_WriteCMD(0x14);	//打开DCDC
	OLED_WriteCMD(0xaf);	//打开OLED
}
/*******************************************************
*函数名：oled_dis_off
*功  能：oled 关闭显示
*参  数：无
*返  回：无
********************************************************/
void OLED_SPI_off(void)
{
	OLED_WriteCMD(0x8d);//设置DCDC
	OLED_WriteCMD(0x10);//关闭DCDC
	OLED_WriteCMD(0xae);//关闭OLED
}
