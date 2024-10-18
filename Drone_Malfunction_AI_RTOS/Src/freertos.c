/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
// vibration sensor library
#include "mpu6050.h"
#include "motor_handler.h"
#include "measure_handler.h"
#include "globals.h"
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define FLAG_MEAS 0x0001
#define FLAG_SEND 0x0010
#define FLAG_END  0x0100
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

struct measure_data {
	uint16_t sample_ms;
	uint16_t AC1_mv;
	uint16_t AC2_mv;
	uint16_t AC3_mv;
	MPU6050_t MPU6050;
};

static struct measure_data measure_data_set [NB_OF_SAMPLES];
static uint16_t sample_nb;

extern UART_HandleTypeDef huart3;

extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;
extern ADC_HandleTypeDef hadc3;
extern I2C_HandleTypeDef hi2c2;

/* USER CODE END Variables */
/* Definitions for ControlTask */
osThreadId_t ControlTaskHandle;
const osThreadAttr_t ControlTask_attributes = {
  .name = "ControlTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for UARTTask */
osThreadId_t UARTTaskHandle;
const osThreadAttr_t UARTTask_attributes = {
  .name = "UARTTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for MeasureTask */
osThreadId_t MeasureTaskHandle;
const osThreadAttr_t MeasureTask_attributes = {
  .name = "MeasureTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for sample_timer */
osTimerId_t sample_timerHandle;
const osTimerAttr_t sample_timer_attributes = {
  .name = "sample_timer"
};
/* Definitions for uart_send_event */
osEventFlagsId_t uart_send_eventHandle;
const osEventFlagsAttr_t uart_send_event_attributes = {
  .name = "uart_send_event"
};
/* Definitions for measure_event */
osEventFlagsId_t measure_eventHandle;
const osEventFlagsAttr_t measure_event_attributes = {
  .name = "measure_event"
};
/* Definitions for cycle_end_event */
osEventFlagsId_t cycle_end_eventHandle;
const osEventFlagsAttr_t cycle_end_event_attributes = {
  .name = "cycle_end_event"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void control_task(void *argument);
void uart_task(void *argument);
void measure_task(void *argument);
void sample_timer_cb(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* Create the timer(s) */
  /* creation of sample_timer */
  sample_timerHandle = osTimerNew(sample_timer_cb, osTimerPeriodic, NULL, &sample_timer_attributes);

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of ControlTask */
  ControlTaskHandle = osThreadNew(control_task, NULL, &ControlTask_attributes);

  /* creation of UARTTask */
  UARTTaskHandle = osThreadNew(uart_task, NULL, &UARTTask_attributes);

  /* creation of MeasureTask */
  MeasureTaskHandle = osThreadNew(measure_task, NULL, &MeasureTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* Create the event(s) */
  /* creation of uart_send_event */
  uart_send_eventHandle = osEventFlagsNew(&uart_send_event_attributes);

  /* creation of measure_event */
  measure_eventHandle = osEventFlagsNew(&measure_event_attributes);

  /* creation of cycle_end_event */
  cycle_end_eventHandle = osEventFlagsNew(&cycle_end_event_attributes);

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */

  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_control_task */
/**
  * @brief  Function implementing the ControlTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_control_task */
void control_task(void *argument)
{
  /* USER CODE BEGIN control_task */
  /* Infinite loop */
	for(uint8_t meas=0; meas<NB_OF_MEAS; meas++)
  {
	  set_motor_speed(MOTOR_SPEED);
	  sample_nb = 0;
	  osEventFlagsClear(cycle_end_eventHandle, FLAG_END);
	  osTimerStart(sample_timerHandle, TP_MS);

	  osEventFlagsWait(cycle_end_eventHandle, FLAG_END, osFlagsWaitAny,  osWaitForever);
	  osTimerStop(sample_timerHandle);
	  set_motor_speed(0);
	  osEventFlagsSet(uart_send_eventHandle, FLAG_SEND);
	  osDelay(WAIT_TIME_MS);
  }

	for (;;) {
		HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
		osDelay(500);
	}
  /* USER CODE END control_task */
}

/* USER CODE BEGIN Header_uart_task */
/**
* @brief Function implementing the UARTTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_uart_task */
void uart_task(void *argument)
{
  /* USER CODE BEGIN uart_task */
  /* Infinite loop */
  char msg [200];
  int msg_len = 0;
  for(;;)
  {
	  //wait for event
	  osEventFlagsWait(uart_send_eventHandle, FLAG_SEND, osFlagsWaitAny,  osWaitForever);
	  // send data in loop
	  uint16_t lines_nb = sample_nb;
	  HAL_GPIO_WritePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin, 1);
	  for (uint16_t line=0; line <= lines_nb; line++) {
		  msg_len = sprintf(msg, "%ld;%d;%d;%d;%f;%f;%f;line;",
				  measure_data_set[line].sample_ms,
				  measure_data_set[line].AC1_mv,
				  measure_data_set[line].AC2_mv,
				  measure_data_set[line].AC3_mv,
				  measure_data_set[line].MPU6050.Ax,
				  measure_data_set[line].MPU6050.Ay,
				  measure_data_set[line].MPU6050.Az);

		  HAL_UART_Transmit(&huart3, (uint8_t*)msg, strlen(msg), msg_len);
	  }
	  msg_len = sprintf(msg, "end;");
	  HAL_UART_Transmit(&huart3, (uint8_t*)msg, strlen(msg), msg_len);
	  HAL_GPIO_WritePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin, 0);
  }
  /* USER CODE END uart_task */
}

/* USER CODE BEGIN Header_measure_task */
/**
* @brief Function implementing the MeasureTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_measure_task */
void measure_task(void *argument)
{
  /* USER CODE BEGIN measure_task */
  /* Infinite loop */
  for(;;)
  {
	  osEventFlagsWait(measure_eventHandle, FLAG_MEAS, osFlagsWaitAny,  osWaitForever);

	  struct measure_data data;
	  data.AC1_mv = ADC_take_measure(&hadc1);
	  data.AC2_mv = ADC_take_measure(&hadc2);
	  data.AC3_mv = ADC_take_measure(&hadc3);

	  // wibracje
	  MPU6050_Read_Accel(&hi2c2, &data.MPU6050);

	  data.sample_ms = sample_nb * TP_MS;
	  measure_data_set[sample_nb] = data;
	  sample_nb++;
  }
  /* USER CODE END measure_task */
}

/* sample_timer_cb function */
void sample_timer_cb(void *argument)
{
  /* USER CODE BEGIN sample_timer_cb */
	if (sample_nb == NB_OF_SAMPLES) {
		osEventFlagsSet(cycle_end_eventHandle, FLAG_END);
	} else {
		osEventFlagsSet(measure_eventHandle, FLAG_MEAS);
	}
  /* USER CODE END sample_timer_cb */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

