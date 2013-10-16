/**
  ******************************************************************************
  * @file    lab2_filter.h
  * @author  Group 6
  * @version V1.0.0
  * @date    18-October-2013
  * @brief   This defines public functions for a simple Linear Moving Average Filter:
	*           - Insert value into buffer
	*           - Calculate average of all elements in buffer
	*          It also defines a Moving Average Filter struct
  */

/* Includes ------------------------------------------------------------------*/

#include "stm32f4xx.h"

/* Exported Types ---------------------------------------------------------*/

/** @defgroup Structs
  * @{
  */ 

struct Moving_Average {
	float moving_values[100];
	uint32_t index;
	float average;
};

/**
  * @}
  */

/* Moving Average Filter Public Functions ---------------------------------------------------------*/

/** @defgroup Moving_Average_Filter_Public_Functions
  * @{
  */ 

void insert_value(struct Moving_Average *moving_average, float new_value);
void calculate_average(struct Moving_Average *moving_average);

/**
  * @}
  */
