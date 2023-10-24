/********************************************************************************
 * @file     main.c
 * @author   herexiong
 * @version  V1.0
 * @date     2023-10-24
 * @brief    OLED驱动示例
 *******************************************************************************/

#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LED/led.h"
#include "./BSP/SPI_OLED/oled_spi.h"
#include "./BSP/SPI_OLED/oled_front.h"

int main(void) {
    uint16_t t = 0; /* LED闪烁变量 */
	
    HAL_Init(); /* 初始化HAL库 */
    sys_stm32_clock_init(RCC_PLL_MUL9); /* 设置时钟, 72Mhz ,9倍频，8*9=72 */
    delay_init(72); /* 延时初始化 */
    led_init(); /* 初始化LED */
	OLED_SPI_init();
	delay_ms(1000);
	OLED_SPI_clear();
	OLED_SPI_dis_str(0,0,"hello world",16);
	OLED_SPI_dis_str(0,2,"hello world",16);
//	OLED_SPI_dis_str(0,2,"hello world",8);
//	OLED_SPI_dis_str(0,3,"hello world",8);
	OLED_SPI_dis_str(0,4,"hello world",16);
//	OLED_SPI_dis_str(0,5,"hello world",8);
	OLED_SPI_dis_str(0,6,"hello world",16);
//	OLED_SPI_dis_str(0,7,"hello world",8);
	delay_ms(1000);
	OLED_SPI_off();
	delay_ms(1000);
	OLED_SPI_on();
    while(1){
        delay_ms(10);
        t++;
		
        if (t>=20 && t % 20==0) {
			LED2_TOGGLE(); /* 每200ms输出一次结果,并闪烁LED0,提示程序运行 LED2(GREEN)闪烁 */
		}	
	}
}
