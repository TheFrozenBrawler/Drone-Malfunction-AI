/*
 * motor_handler.h
 *
 *  Created on: May 13, 2024
 *      Author: macie
 */

#ifndef SRC_MOTOR_HANDLER_H_
#define SRC_MOTOR_HANDLER_H_

#include "stm32h7xx_hal.h"

void motor_init(TIM_HandleTypeDef *hTIM_pwm, uint32_t Channel);
void set_motor_speed (uint8_t speed);	// percent 0-100

#endif /* SRC_MOTOR_HANDLER_H_ */
