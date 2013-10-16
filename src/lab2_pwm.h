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
#define PULSE_SPEED 500
#define PWM_FREQUENCY 1000
#if (PWM_FREQUENCY * (PULSE_SPEED / 1000) / MAX_INTENSITY) % MAX_INTENSITY != 0
	#define REAL_PWM_FREQUENCY PWM_FREQUENCY
#else
	#define REAL_PWM_FREQUENCY 100000
#endif

/* PWM Public Functions ---------------------------------------------------------*/

/** @defgroup PWM_Public_Functions
  * @{
  */

void do_pwm(void);

/**
  * @}
  */
