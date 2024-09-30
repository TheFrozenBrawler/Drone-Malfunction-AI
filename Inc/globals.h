/*
 * globals.h
 *
 *  Created on: May 12, 2024
 *      Author: macie
 */

#ifndef SRC_GLOBALS_H_
#define SRC_GLOBALS_H_

#define PWR_V 			3300	// maximal voltage of ADC
#define ADC_RES 		4096	// resolution of ADC measurements (12 bit is nominal)
#define TP_MS			10		// sampling time

#define ADC_MEAS_TIME	3000	// time of motor activity
#define NB_OF_MEAS		1		// number of measurements
#define WAIT_TIME_MS	3000	// wait time between measurements
#define NB_OF_SAMPLES 	ADC_MEAS_TIME / TP_MS

// --- MOTOR CONFIG ---
#define MOTOR_TIM_HZ	10000	// frequency of PWM output for motor control
#define MOTOR_SPEED		50


#endif /* SRC_GLOBALS_H_ */


// --- UNUSED ---

//  for (int i=0; i<5; i++) {
//	  HAL_ADC_Start(&hadc1);
//	  HAL_ADC_PollForConversion(&hadc1, 1000);
//	  AC1_analog = HAL_ADC_GetValue(&hadc1);
//
//	  AC1_voltage = (AC1_analog * ADC_pwr) / ADC_resolution;
//
////	  send_msg("%d;", AC1_voltage);
//	  msg_len = sprintf(msg, "%d;", AC1_voltage);
//	  HAL_UART_Transmit(&huart3, (uint8_t*)msg, strlen(msg), msg_len);
//
//	  AC1_analog = 0; AC1_voltage = 0;
//	  HAL_ADC_Stop(&hadc1);
//
//	  HAL_Delay(10);
//  }




// send message function

//int send_msg(const char *data_string, ...) {
//	va_list va_ptr;
//	va_start(va_ptr, data_string);
//
//	int msg_len;
//	msg_len = sprintf(msg, data_string, va_ptr);
//	HAL_UART_Transmit(&huart3, (uint8_t*)msg, strlen(msg), msg_len);
//
//	va_end(va_ptr);
//
//	return 1;
//}
