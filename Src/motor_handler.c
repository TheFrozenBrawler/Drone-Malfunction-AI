#include "motor_handler.h"

#include "stm32h7xx_hal.h"

#include "globals.h"

#define SPEED_RATIO 	MOTOR_TIM_HZ/100

struct motor_handler_context {		// object to keep all the motor data
	TIM_HandleTypeDef *pwm_handler;
	uint32_t pwm_channel;
	uint8_t speed_now;	// %
};

static struct motor_handler_context ctx = {};

void motor_init(TIM_HandleTypeDef *pwm_handler, uint32_t Channel) {
	ctx.pwm_handler = pwm_handler;
	ctx.pwm_channel = Channel;
	ctx.speed_now = 0;
	 __HAL_TIM_SET_COMPARE(ctx.pwm_handler, ctx.pwm_channel, 0);
}

void set_motor_speed (uint8_t speed) {
//	uint16_t motor_speed = speed * SPEED_RATIO;
//	ctx.speed_now = (uint8_t)(round((float)speed * 40.0 / 100.0));
	ctx.speed_now = (uint8_t)(round((float)speed / 100.0)); // full speed management
	__HAL_TIM_SET_COMPARE(ctx.pwm_handler, ctx.pwm_channel, ctx.speed_now);
}
