/*
 * measure_handler.c
 *
 *  Created on: May 12, 2024
 *      Author: macie
 */
#include "measure_handler.h"
#include "globals.h"
#include "stm32h7xx_hal.h"


uint16_t ADC_take_measure(ADC_HandleTypeDef *adc_handler) {
	uint16_t AC_analog;
	uint16_t AC_voltage;
	HAL_ADC_Start(adc_handler);
	HAL_ADC_PollForConversion(adc_handler, 1000);
	AC_analog = HAL_ADC_GetValue(adc_handler);

	AC_voltage = (AC_analog * PWR_V) / ADC_RES;

	HAL_ADC_Stop(adc_handler);

	return AC_voltage;
}


