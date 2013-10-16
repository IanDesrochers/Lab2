/**
  ******************************************************************************
  * @file    lab2_pwm.h
  * @author  Group 6
  * @version V1.0.0
  * @date    18-October-2013
  * @brief   This defines a PWM function for the second operation mode for
	*          all four onboard LEDs. It should be called at a given frequency
	*          defined by PWM_FREQUENCY.
  */

/* Includes ------------------------------------------------------------------*/

#include "stm32f4xx.h"

/* Defines ------------------------------------------------------------------*/

#define MAX_INTENSITY 100
#define PULSE_SPEED 2000
#define PWM_FREQUENCY 100000

/* PWM Public Functions ---------------------------------------------------------*/

/** @defgroup PWM_Public_Functions
  * @{
  */

void do_pwm(void);

/**
  * @}
  */
