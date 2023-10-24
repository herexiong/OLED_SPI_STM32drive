#ifndef __LED_H
#define __LED_H
#include "./SYSTEM/sys/sys.h"

/******************************************************************************************/
/* LED引脚定义 */
#define GPIO_LED_PORT GPIOB //定义端口
#define GPIO_LED_PIN GPIO_PIN_5 //定义引脚
/* PWM端口定义 */
#define GPIO_PWM_PORT GPIOB //定义PWM端口
#define GPIO_PWM_PIN GPIO_PIN_10 //定义PWM引脚
/* nPWM端口定义 */
#define GPIO_nPWM_PORT GPIOB //定义PWM端口
#define GPIO_nPWM_PIN GPIO_PIN_5 //定义PWM引脚

/* 设置时钟 */
#ifndef GPIOC_CLK_ENABLE
#define GPIOC_CLK_ENABLE do{__HAL_RCC_GPIOC_CLK_ENABLE() ; }while(0) 
#endif

#ifndef GPIOB_CLK_ENABLE
#define GPIOB_CLK_ENABLE do{__HAL_RCC_GPIOB_CLK_ENABLE() ; }while(0) 
#endif

#ifndef GPIOA_CLK_ENABLE
#define GPIOA_CLK_ENABLE do{__HAL_RCC_GPIOA_CLK_ENABLE() ; }while(0) 
#endif

/* LED取反定义 */
#define LED2_TOGGLE()   do{ HAL_GPIO_TogglePin(GPIO_LED_PORT, GPIO_LED_PIN); }while(0)        /* 翻转LED2 */
/******************************************************************************************/
void led_init(void);
void led_input_init(void);
void pwm_init(void);
void atim_npwm_init(void);
#endif
