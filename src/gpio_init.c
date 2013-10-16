/**
  ******************************************************************************
  * @file    gpio_init.c
  * @author  Group 6
  * @version V1.0.0
  * @date    18-October-2013
  * @brief   This file provides initialization functions for the following:
	*           - USER Pushbutton
	*           - On-board LEDs
	*           - ADC1 (for temperature sensor)
	*           - Built-in temperature sensor
  */

/* Includes ------------------------------------------------------------------*/

#include <stdio.h>
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"

/* GPIO Init Public Functions ---------------------------------------------------------*/

/** @defgroup GPIO_Init_Public_Functions
  * @{
  */

/**
  * @brief  Initialize USER pushbutton
	* @note   This function sets up the input GPIO for the USER pushbutton.
	*         The GPIO speed and direction are set.
	* @param  None
  * @retval None
  */
void init_pushbutton() {
	GPIO_InitTypeDef gpio_init_s;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); //Enable clock to Pushbutton
	
	GPIO_StructInit(&gpio_init_s);
	gpio_init_s.GPIO_Pin = GPIO_Pin_0;
	gpio_init_s.GPIO_Mode = GPIO_Mode_IN;
	gpio_init_s.GPIO_Speed = GPIO_Speed_50MHz;
	gpio_init_s.GPIO_OType = GPIO_OType_PP;
	gpio_init_s.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOA, &gpio_init_s);
}

/**
  * @brief  Initialize on-board LEDs
	* @note   This function initializes the four on-board LEDs for use.
	*         It sets pin direction and speed. The four LEDs are:
	*         GPIO_Pin_12, GPIO_Pin_13, GPIO_Pin_14 & GPIO_Pin_15
	* @param  None
  * @retval None
  */
void init_leds() {
	GPIO_InitTypeDef gpio_init_s;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); //Enable clock to LEDs
	
	GPIO_StructInit(&gpio_init_s);
	gpio_init_s.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	gpio_init_s.GPIO_Mode = GPIO_Mode_OUT;
	gpio_init_s.GPIO_Speed = GPIO_Speed_50MHz;
	gpio_init_s.GPIO_OType = GPIO_OType_PP;
	gpio_init_s.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &gpio_init_s);
}

/**
  * @brief  Initialize ADC
	* @note   This function sets up all ADCs for use and configures ADC1 for
	*         reading the built-in temperature sensor.
	* @param  None
  * @retval None
  */
void init_adc() {
	ADC_InitTypeDef adc_init_s;
	ADC_CommonInitTypeDef adc_common_init_s;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //Enable clock to ADC
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	adc_common_init_s.ADC_Mode = ADC_Mode_Independent;
	adc_common_init_s.ADC_Prescaler = ADC_Prescaler_Div2;
	adc_common_init_s.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	adc_common_init_s.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonInit(&adc_common_init_s);
	
	adc_init_s.ADC_Resolution = ADC_Resolution_12b;
	adc_init_s.ADC_ScanConvMode = DISABLE;
	adc_init_s.ADC_ContinuousConvMode = ENABLE;
	adc_init_s.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	adc_init_s.ADC_DataAlign = ADC_DataAlign_Right;
	adc_init_s.ADC_NbrOfConversion = 1;
	ADC_Init(ADC1, &adc_init_s);
	
	ADC_Cmd(ADC1, ENABLE); //Enable ADC
}

/**
  * @brief  Initialize built-in temperature sensor
	* @note   This function initializes the built in temperature sensor on
	*         ADC1 at channel ADC_Channel_16 for use
	* @param  None
  * @retval None
  */
void init_temp_sensor() {
	ADC_TempSensorVrefintCmd(ENABLE); //Enable temperature sensor
	ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 1, ADC_SampleTime_480Cycles); //Configure temperature sensor sample time
}

/**
  * @}
  */

