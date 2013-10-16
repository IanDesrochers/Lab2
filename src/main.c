/**
  ******************************************************************************
  * @file    main.c
  * @author  Group 6
  * @version V1.0.0
  * @date    18-October-2013
  * @brief   Main entry point for embedded temperature sensor/LED PWM controller
  */

/* Includes ------------------------------------------------------------------*/

#include <stdio.h>
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include "gpio_init.h"
#include "lab2_filter.h"
#include "lab2_pwm.h"

/* Defines ------------------------------------------------------------------*/

#define V25 .760f
#define AVSLOPE .0025f
#define VDD 3.0f

/* Private Variables ------------------------------------------------------------------*/

uint32_t ticks = 0;
uint32_t mode = 0;

/* Private Function Definitions ------------------------------------------------------------------*/

static void rotate_led(uint32_t led_number);
static void set_mode(void);
static float get_temperature(uint16_t raw_value);
static void sleep(uint32_t n_sleep);

/* Public Functions ---------------------------------------------------------*/

/** @defgroup Public_Functions
  * @{
  */

/**
  * @brief  Main entry point
	* @param  None
  * @retval int: Error code
  */
int main()
{
	struct Moving_Average Temperature_Moving_Average; //Define moving average filter struct
	Temperature_Moving_Average.index = 0;

	SysTick_Config(SystemCoreClock / 20); //Set default systick interrupt frequency (mode 0 - temperature)
	
	init_pushbutton(); //Initialize pushbutton
	init_leds(); //Initialize LEDs
	init_adc(); //Initialize ADC
	init_temp_sensor(); //Initialize temperature sensor
	
	ADC_SoftwareStartConv(ADC1); //Start ADC conversion

	while(1) {
		set_mode(); //Read pushbutton and set operation mode accordingly
		if (!mode) { //If we're in the default (temperature mode)
			if (ticks) {
				ticks = 0; //Reset interrupt flag
				float current = get_temperature(ADC_GetConversionValue(ADC1)); //Get current temperature value
				insert_value(&Temperature_Moving_Average, current); //Insert newest temperature reading into filter
				calculate_average(&Temperature_Moving_Average); //Calculate averaged temperature
				uint32_t led_number = (uint32_t)Temperature_Moving_Average.average % 8; //Determine which LED should be on depending on temperature
				GPIO_Write(GPIOD, 0x0); //Reset all LEDs
				rotate_led(led_number); //Turn on correct (rotating) LED
						
				printf("Current: %f\tAverage: %f\n", current, Temperature_Moving_Average.average); //Print current and averaged temperature values
			}
		} else { //Otherwise, we're in the PWM mode
			if (ticks) {
				ticks = 0; //Reset interrupt flag
				do_pwm();
			}
		}
	}
}

void rotate_led(uint32_t led_number) {
	switch (led_number) { //Only turn the desired LED on
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
}

/**
  * @brief  SysTick ISR.
	* @note   Sets flag indicating interrupt has occurred.
	* @param  None
  * @retval None
  */

void SysTick_Handler(void) {
	//Increment flag to signal interrupt
	ticks++;
}

/**
  * @}
  */

/* Private Functions ---------------------------------------------------------*/

/** @defgroup Private_Functions
  * @{
  */

/**
  * @brief  Read USER pushbutton and set operation mode.
	* @note   Includes switch debouncing. Mode 0 = Default (temperature).
	*         Mode 1 = PWM LED control.
	* @param  None
  * @retval None
  */

static void set_mode() {
	uint8_t button_state = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0); //Read pushbutton state
	if (button_state) { //If the button is pushed
		if (mode) { //If we're in mode 1 (PWM)
			mode = 0; //Go to mode 0 (temperature)
			SysTick_Config(SystemCoreClock / 20); //Configure systick timer for temperature mode
		} else { //If we're in mode 0 (temperature)
			mode = 1; //Go to mode 1 (PWM)
			SysTick_Config(SystemCoreClock / REAL_PWM_FREQUENCY); //Configure systick timer for PWM mode
			GPIO_Write(GPIOD, 0x0); //Turn off all LEDs
		}
		sleep(250); //Wait for a bit (debounce)
		while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)) {
			sleep(250); //Wait a bit more, until the button is released (debounce pt 2)
		}
	}
}

/**
  * @brief  Converts ADC value to temperature (in degrees C).
	* @param  raw_value: Raw ADC temperature sensor reading.
  * @retval float: Converted temperature in degrees C.
  */

static float get_temperature(uint16_t raw_value) {
	//Take raw ADC value, scale it from 0-4095 to 0-3V
	//Return this scaled value after sensor calibration function
	return((((VDD*((float)raw_value)/4096 - V25)/AVSLOPE)+25));
}

/**
  * @brief  Sleeps for given time.
	* @param  n_sleep: Time to sleep for (in ms)
  * @retval None
  */

static void sleep(uint32_t n_sleep) {
	//Set intermediate number representing clock ticks to sleep for
	n_sleep = SystemCoreClock / 1000;
	//Do nothing for this many ticks
  while(n_sleep--) {
  }
}

/**
  * @}
  */
