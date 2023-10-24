#ifndef __LED_H
#define __LED_H
#include "./SYSTEM/sys/sys.h"

/******************************************************************************************/
/* LED���Ŷ��� */
#define GPIO_LED_PORT GPIOB //����˿�
#define GPIO_LED_PIN GPIO_PIN_5 //��������
/* PWM�˿ڶ��� */
#define GPIO_PWM_PORT GPIOB //����PWM�˿�
#define GPIO_PWM_PIN GPIO_PIN_10 //����PWM����
/* nPWM�˿ڶ��� */
#define GPIO_nPWM_PORT GPIOB //����PWM�˿�
#define GPIO_nPWM_PIN GPIO_PIN_5 //����PWM����

/* ����ʱ�� */
#ifndef GPIOC_CLK_ENABLE
#define GPIOC_CLK_ENABLE do{__HAL_RCC_GPIOC_CLK_ENABLE() ; }while(0) 
#endif

#ifndef GPIOB_CLK_ENABLE
#define GPIOB_CLK_ENABLE do{__HAL_RCC_GPIOB_CLK_ENABLE() ; }while(0) 
#endif

#ifndef GPIOA_CLK_ENABLE
#define GPIOA_CLK_ENABLE do{__HAL_RCC_GPIOA_CLK_ENABLE() ; }while(0) 
#endif

/* LEDȡ������ */
#define LED2_TOGGLE()   do{ HAL_GPIO_TogglePin(GPIO_LED_PORT, GPIO_LED_PIN); }while(0)        /* ��תLED2 */
/******************************************************************************************/
void led_init(void);
void led_input_init(void);
void pwm_init(void);
void atim_npwm_init(void);
#endif
