#include <stdio.h>
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"

#define V25 .760f
#define AVSLOPE .0025f


uint32_t ticks = 0;

struct Moving_Average {
	float moving_values[100];
	uint32_t index;
	float average;
};

void insert_value(struct Moving_Average *moving_average, uint16_t new_value);
void calculate_average(struct Moving_Average *moving_average);
float get_temperature(uint16_t raw_value);

int main()
{

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	GPIO_InitTypeDef gpio_init_s;
	
	GPIO_StructInit(&gpio_init_s);
	gpio_init_s.GPIO_Pin = GPIO_Pin_12;
	gpio_init_s.GPIO_Mode = GPIO_Mode_OUT;
	gpio_init_s.GPIO_Speed = GPIO_Speed_100MHz;
	gpio_init_s.GPIO_OType = GPIO_OType_PP;
	gpio_init_s.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &gpio_init_s);
	gpio_init_s.GPIO_Pin = GPIO_Pin_13;
	GPIO_Init(GPIOD, &gpio_init_s);
	gpio_init_s.GPIO_Pin = GPIO_Pin_14;
	GPIO_Init(GPIOD, &gpio_init_s);
	gpio_init_s.GPIO_Pin = GPIO_Pin_15;
	GPIO_Init(GPIOD, &gpio_init_s);
	
	SysTick_Config(SystemCoreClock / 20);
	
	ADC_InitTypeDef adc_init_s;
	ADC_CommonInitTypeDef adc_common_init_s;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	
	ADC_TempSensorVrefintCmd(ENABLE);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	adc_common_init_s.ADC_Mode = ADC_Mode_Independent;
	adc_common_init_s.ADC_Prescaler = ADC_Prescaler_Div2;
	adc_common_init_s.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	adc_common_init_s.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonInit(&adc_common_init_s);
	
	adc_init_s.ADC_Resolution = ADC_Resolution_12b;
	adc_init_s.ADC_ScanConvMode = DISABLE;
	adc_init_s.ADC_ContinuousConvMode = DISABLE;
	adc_init_s.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	adc_init_s.ADC_DataAlign = ADC_DataAlign_Right;
	adc_init_s.ADC_NbrOfConversion = 1;
	ADC_Init(ADC1, &adc_init_s);
	
	ADC_Cmd(ADC1, ENABLE);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 1, ADC_SampleTime_480Cycles);
	
	ADC_SoftwareStartConv(ADC1);
	
	ADC_ContinuousModeCmd(ADC1, ENABLE);
	
	struct Moving_Average Temperature_Moving_Average;
	
	while(1) {
		
		if (ticks) {
			ticks = 0;
			float current = get_temperature(ADC_GetConversionValue(ADC1));
			insert_value(&Temperature_Moving_Average, current);
			calculate_average(&Temperature_Moving_Average);
			uint32_t led_number = (uint32_t)Temperature_Moving_Average.average % 8;
			GPIO_Write(GPIOD, 0x0);
			switch (led_number) {
				case 0:
					GPIO_SetBits(GPIOD, GPIO_Pin_12);
					break;
				case 1:
					GPIO_SetBits(GPIOD, GPIO_Pin_12);
					break;
				case 2:
					GPIO_SetBits(GPIOD, GPIO_Pin_13);
					break;
				case 3:
					GPIO_SetBits(GPIOD, GPIO_Pin_13);
					break;
				case 4:
					GPIO_SetBits(GPIOD, GPIO_Pin_14);
					break;
				case 5:
					GPIO_SetBits(GPIOD, GPIO_Pin_14);
					break;
				case 6:
					GPIO_SetBits(GPIOD, GPIO_Pin_15);
					break;
				default:
					GPIO_SetBits(GPIOD, GPIO_Pin_15);
					break;
			}
					
			printf("Current: %f\tAverage: %f\n", current, Temperature_Moving_Average.average);
		}
		
	}
}

void insert_value(struct Moving_Average *moving_average, uint16_t new_value) {
	if (moving_average->index == sizeof(moving_average->moving_values)/sizeof(moving_average->average)) {
		moving_average->index = 0;
	} else {
		moving_average->index++;
	}
	moving_average->moving_values[moving_average->index-1] = new_value;
}

void calculate_average(struct Moving_Average *moving_average) {
	uint32_t i;
	double sum;
	for (i=0; i<sizeof(moving_average->moving_values)/sizeof(moving_average->average); i++) {
		sum += moving_average->moving_values[i];
	}
	moving_average->average = sum/(sizeof(moving_average->moving_values)/sizeof(moving_average->average));
}

float get_temperature(uint16_t raw_value) {
	return((((3*((float)raw_value)/4096 - V25)/AVSLOPE)+25));
}

void SysTick_Handler(void) {
	ticks++;
}
