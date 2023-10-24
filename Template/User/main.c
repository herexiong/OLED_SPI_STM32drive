/********************************************************************************
 * @file     main.c
 * @author   herexiong
 * @version  V1.0
 * @date     2023-10-24
 * @brief    OLED����ʾ��
 *******************************************************************************/

#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LED/led.h"
#include "./BSP/SPI_OLED/oled_spi.h"
#include "./BSP/SPI_OLED/oled_front.h"

int main(void) {
    uint16_t t = 0; /* LED��˸���� */
	
    HAL_Init(); /* ��ʼ��HAL�� */
    sys_stm32_clock_init(RCC_PLL_MUL9); /* ����ʱ��, 72Mhz ,9��Ƶ��8*9=72 */
    delay_init(72); /* ��ʱ��ʼ�� */
    led_init(); /* ��ʼ��LED */
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
			LED2_TOGGLE(); /* ÿ200ms���һ�ν��,����˸LED0,��ʾ�������� LED2(GREEN)��˸ */
		}	
	}
}
