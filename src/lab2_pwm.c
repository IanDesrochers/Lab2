/**
  ******************************************************************************
  * @file    lab2_pwm.c
  * @author  Group 6
  * @version V1.0.0
  * @date    18-October-2013
  * @brief   This defines a PWM function for the second operation mode for
	*          all four onboard LEDs. It should be called at a given frequency
	*          defined by PWM_FREQUENCY.
  */

/* Includes ------------------------------------------------------------------*/

#include "stm32f4xx.h"
#include "lab2_pwm.h"

/* Private variables ---------------------------------------------------------*/

/** @defgroup PWM_Variables
  * @{
  */

uint32_t pwm_count = 0, intensity = 0, up_down = 1, change_intensity_count = 0;

/**
  * @}
  */

/* PWM Public Functions ---------------------------------------------------------*/

/** @defgroup PWM_Public_Functions
  * @{
  */

void do_pwm() {
	
	//Set LED states
	if (pwm_count >= intensity) {
		GPIO_Write(GPIOD, 0x0); //Turn LEDs off if they have been on for specified intensity duration
	} else if (pwm_count == 0) {
		GPIO_Write(GPIOD, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15); //Turn LEDs on at the beginning of each new cycle
	}
	
	//Setting brighness direction mode (increaseing/decreasing)
	if (pwm_count == MAX_INTENSITY) {
		pwm_count = 0; //At the end of each cycle, reset cycle counter
		if (intensity == MAX_INTENSITY) {
			up_down = 0; //If we are at the brightest, start dimming
		} else if (intensity == 0) {
			up_down = 1; //If we are at the dimmest, start brightening
		}
	} else {
		pwm_count++; //Increment cycle counter
	}
	
	//Checking change-brighness flag
	if (!change_intensity_count) { //Check if we need to change the brightness level this iteraiton
		if (up_down) {
			intensity++; //Increment brightness if we're on an upwards metaphorical trajectory
		} else {
			intensity--; //Otherwise, decrement
		}
	}
	
	//Setting change-brighness flag
	if (change_intensity_count == (REAL_PWM_FREQUENCY * ((float)PULSE_SPEED / 1000) / MAX_INTENSITY)) {
		change_intensity_count = 0; //Check if we've stayed at the curent brightness for long enough
	} else {
		change_intensity_count++; //Otherwise, increment counter and check next iteration
	}
}

/**
  * @}
  */
