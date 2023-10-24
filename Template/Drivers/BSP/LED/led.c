#include "./BSP/LED/led.h"

void led_init(){
	GPIO_InitTypeDef gpio_init_struct;
	//����ʱ��
	GPIOB_CLK_ENABLE;
	//�ṹ������
	gpio_init_struct.Pin = GPIO_LED_PIN;
	gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;													
	gpio_init_struct.Speed = GPIO_SPEED_FREQ_LOW ;
	//��������
	HAL_GPIO_Init(GPIO_LED_PORT,&gpio_init_struct);
	//Ĭ������͵�ƽ
	HAL_GPIO_WritePin(GPIO_LED_PORT,GPIO_LED_PIN,GPIO_PIN_RESET);
}

void led_input_init(){
	GPIO_InitTypeDef gpio_init_struct;
	//����ʱ��
	GPIOC_CLK_ENABLE;
	//�ṹ������
	gpio_init_struct.Pin = GPIO_LED_PIN;
	gpio_init_struct.Mode = GPIO_MODE_INPUT;													
	gpio_init_struct.Speed = GPIO_SPEED_FREQ_LOW ;
	//��������
	HAL_GPIO_Init(GPIO_LED_PORT,&gpio_init_struct);
	//Ĭ������͵�ƽ
	HAL_GPIO_WritePin(GPIO_LED_PORT,GPIO_LED_PIN,GPIO_PIN_RESET);
}

void pwm_init(){
	GPIO_InitTypeDef gpio_init_struct;
	//����ʱ��
	GPIOB_CLK_ENABLE;
	//�ṹ������
	gpio_init_struct.Pin = GPIO_PWM_PIN;
	gpio_init_struct.Mode = GPIO_MODE_AF_PP;
	gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH ;
	gpio_init_struct.Pull = GPIO_PULLUP;
	//��������
	HAL_GPIO_Init(GPIO_PWM_PORT,&gpio_init_struct);
	HAL_GPIO_WritePin(GPIO_PWM_PORT,GPIO_PWM_PIN,GPIO_PIN_SET);
}

void atim_npwm_init(){
	GPIO_InitTypeDef gpio_init_struct;
	//����ʱ��
	GPIOB_CLK_ENABLE;
	//�ṹ������
	gpio_init_struct.Pin = GPIO_nPWM_PIN;
	gpio_init_struct.Mode = GPIO_MODE_AF_PP;
	gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH ;
	gpio_init_struct.Pull = GPIO_PULLUP;
	//��������
	HAL_GPIO_Init(GPIO_PWM_PORT,&gpio_init_struct);
	HAL_GPIO_WritePin(GPIO_nPWM_PORT,GPIO_nPWM_PIN,GPIO_PIN_SET);
}
