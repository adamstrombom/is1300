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
#include "semphr.h"
#include "shield_abstraction.h"
#include "message_buffer.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

typedef enum{
	START,
	CAR_GO, //cars have green light
	CAR_TO_PEDESTRIAN,
	PEDESTRIAN_GO, //pedestrians have green light
	PEDESTRIAN_TO_CAR
} task1_state_t;


/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

led_registers_t leds;
traffic_state_t traffic;
task1_state_t state, nextState;

SemaphoreHandle_t ledBlock, trafficBlock, northPedestrianBlinkFlagBlock, westPedestrianBlinkFlagBlock;

MessageBufferHandle_t message_pedestrian_west, message_pedestrian_north;


//project task specific variables
uint16_t toggleFreq = 250; //[ms]
uint16_t pedestrianDelay = 5000; //[ms]
uint16_t walkingDelay = 3000; //[ms]
uint16_t orangeDelay = 1000; //[ms]

bool northPedestrianBlinkFlag = false;
bool westPedestrianBlinkFlag = false;

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for pollSwitchesTas */
osThreadId_t pollSwitchesTasHandle;
const osThreadAttr_t pollSwitchesTas_attributes = {
  .name = "pollSwitchesTas",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for updateLedsTask */
osThreadId_t updateLedsTaskHandle;
const osThreadAttr_t updateLedsTask_attributes = {
  .name = "updateLedsTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for logicTask */
osThreadId_t logicTaskHandle;
const osThreadAttr_t logicTask_attributes = {
  .name = "logicTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for blinkNorthTask */
osThreadId_t blinkNorthTaskHandle;
const osThreadAttr_t blinkNorthTask_attributes = {
  .name = "blinkNorthTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void task1_logic(void);
void test_logic(void);
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void PollSwitchesTask(void *argument);
void UpdateLedsTask(void *argument);
void LogicTask(void *argument);
void BlinkNorthTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
	led_registers_init(&leds);
	traffic_state_init(&traffic);
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
	ledBlock = xSemaphoreCreateMutex();
	trafficBlock = xSemaphoreCreateMutex();
	northPedestrianBlinkFlagBlock = xSemaphoreCreateMutex();
	westPedestrianBlinkFlagBlock = xSemaphoreCreateMutex();
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */

	/*using message buffer to be able to get message from ISR*/
	message_pedestrian_north = xMessageBufferCreate( 10 );
	message_pedestrian_west = xMessageBufferCreate( 10 );

  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of pollSwitchesTas */
  pollSwitchesTasHandle = osThreadNew(PollSwitchesTask, NULL, &pollSwitchesTas_attributes);

  /* creation of updateLedsTask */
  updateLedsTaskHandle = osThreadNew(UpdateLedsTask, NULL, &updateLedsTask_attributes);

  /* creation of logicTask */
  logicTaskHandle = osThreadNew(LogicTask, NULL, &logicTask_attributes);

  /* creation of blinkNorthTask */
  blinkNorthTaskHandle = osThreadNew(BlinkNorthTask, NULL, &blinkNorthTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_PollSwitchesTask */
/**
* @brief 	Function implementing the pollSwitchesTas thread. At regualr intervals, poll car switches and modify traffic state accordingly
* 			also check for messages in message buffer from ISR and modify traffic state to indicate pedestrian presence
* @param 	argument: Not used
* @retval 	None
*/
/* USER CODE END Header_PollSwitchesTask */
void PollSwitchesTask(void *argument)
{
  /* USER CODE BEGIN PollSwitchesTask */

	TickType_t xLastWakeTime;
	const TickType_t xPeriod = pdMS_TO_TICKS(10);
	xLastWakeTime = xTaskGetTickCount();

	uint8_t north_buf[10];
	uint8_t west_buf[10];


	/* Infinite loop */
	for(;;)
	{

		if( xSemaphoreTake(trafficBlock, (TickType_t)10) == pdTRUE ){

			//actually pool switches
			traffic.north_car = check_car(NORTH);
			traffic.west_car = check_car(WEST);
			traffic.south_car = check_car(SOUTH);
			traffic.east_car = check_car(EAST);

			//check message buffers from ISR (we don't care about the message itself just whether or not there is a message in the queue)
			if( xMessageBufferIsEmpty( message_pedestrian_north ) == pdFALSE ){
				xMessageBufferReceive( message_pedestrian_north, north_buf, 10, 1);
				traffic.north_pedestrian = true;
//				xMessageBufferReset( message_pedestrian_north );
			}

			if( xMessageBufferIsEmpty( message_pedestrian_west ) == pdFALSE ){
				xMessageBufferReceive( message_pedestrian_west, west_buf, 10, 1);
				traffic.west_pedestrian = true;
//				xMessageBufferReset( message_pedestrian_west );
			}



			xSemaphoreGive(trafficBlock);
		}

		vTaskDelayUntil(&xLastWakeTime, xPeriod);
	}
  /* USER CODE END PollSwitchesTask */
}

/* USER CODE BEGIN Header_UpdateLedsTask */
/**
* @brief Function implementing the updateLedsTask thread. Periodically send state of led over SPI to shift registers
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_UpdateLedsTask */
void UpdateLedsTask(void *argument)
{
  /* USER CODE BEGIN UpdateLedsTask */

	TickType_t xLastWakeTime;
	const TickType_t xPeriod = pdMS_TO_TICKS(20);
	xLastWakeTime = xTaskGetTickCount();

  /* Infinite loop */
	for(;;)
	{

		if( xSemaphoreTake(ledBlock, (TickType_t)10) == pdTRUE ){
			update_leds(&leds);
			xSemaphoreGive(ledBlock);
		}

		vTaskDelayUntil(&xLastWakeTime, xPeriod);

	}
  /* USER CODE END UpdateLedsTask */
}

/* USER CODE BEGIN Header_LogicTask */
/**
* @brief Function implementing the logicTask thread. Handles actual logic of the junction by invoking different logic functions for each project task (currently just task1 implenmented)
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_LogicTask */
void LogicTask(void *argument)
{
  /* USER CODE BEGIN LogicTask */

	TickType_t xLastWakeTime;
	const TickType_t xPeriod = pdMS_TO_TICKS(20);
	xLastWakeTime = xTaskGetTickCount();

	//initialize state for state machine
	state = START;
	nextState = START;


  /* Infinite loop */
	for(;;)
	{
//		test_logic(); //function used to test functionality of different tasks
		task1_logic();
		vTaskDelayUntil(&xLastWakeTime, xPeriod);
	}
  /* USER CODE END LogicTask */
}

/* USER CODE BEGIN Header_BlinkNorthTask */
/**
* @brief Function implementing the blinkNorthTask thread. Blinks the north pedestrian indicator led with a period of toggleFreq [ms] as long as the corresponding flag is true
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_BlinkNorthTask */
void BlinkNorthTask(void *argument)
{
  /* USER CODE BEGIN BlinkNorthTask */
	TickType_t xLastWakeTime;
	const TickType_t xPeriod = pdMS_TO_TICKS(toggleFreq);
	xLastWakeTime = xTaskGetTickCount();
	bool tempFlag = false;

  /* Infinite loop */
	for(;;)
	{
		//check flag (use tempFlag to avoid a little bit of nesting)
		if( xSemaphoreTake(northPedestrianBlinkFlagBlock, (TickType_t)10) == pdTRUE ){
			tempFlag = northPedestrianBlinkFlag;
			xSemaphoreGive(northPedestrianBlinkFlagBlock);
		}

		//if flag true -> toggle led
		if(tempFlag == true){
			if( xSemaphoreTake(ledBlock, (TickType_t)10) == pdTRUE ){
				set_led(&leds, NORTH, PEDESTRIAN_LIGHT, INDICATOR, TOGGLE);
				xSemaphoreGive(ledBlock);
			}
		}

		vTaskDelayUntil(&xLastWakeTime, xPeriod);

	}
  /* USER CODE END BlinkNorthTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/**
 * @brief : function containing the state machine used to handle the logic of task 1
 * @param : void
 * @return : void
 */
void task1_logic(void){

	//used to delay within state machine
	TickType_t time;
	TickType_t period;

	state = nextState;

	switch (state){
		case START:
			//init for CAR_GO state
			if( xSemaphoreTake(ledBlock, (TickType_t)10) == pdTRUE ){
				set_led_all(&leds, OFF);
				set_led(&leds, NORTH, TRAFFIC_LIGHT, GREEN, ON);
				set_led(&leds, SOUTH, TRAFFIC_LIGHT, GREEN, ON);
				set_led(&leds, NORTH, PEDESTRIAN_LIGHT, RED, ON);
				xSemaphoreGive(ledBlock);
			}
			nextState = CAR_GO;
			break;
		case CAR_GO:
			if( xSemaphoreTake(ledBlock, (TickType_t)10) == pdTRUE ){
				set_led_all(&leds, OFF);
				set_led(&leds, NORTH, TRAFFIC_LIGHT, GREEN, ON);
				set_led(&leds, SOUTH, TRAFFIC_LIGHT, GREEN, ON);
				set_led(&leds, NORTH, PEDESTRIAN_LIGHT, RED, ON);
				xSemaphoreGive(ledBlock);
			}

			//if pedestrian button has been pressed, go to CAR_TO_PEDESTRIAN state
			if( xSemaphoreTake(trafficBlock, (TickType_t)10) == pdTRUE ){
				if(traffic.north_pedestrian == true){
					nextState = CAR_TO_PEDESTRIAN;
				}

				xSemaphoreGive(trafficBlock);
			}
			break;
		case CAR_TO_PEDESTRIAN:
			//start blinking pedestrian indicator
			if( xSemaphoreTake(northPedestrianBlinkFlagBlock, (TickType_t)10) == pdTRUE ){
				northPedestrianBlinkFlag = true;
				xSemaphoreGive(northPedestrianBlinkFlagBlock);
			}

			//delay for  pedestrianDelay-orangeDelay ms
			period = pdMS_TO_TICKS(pedestrianDelay - orangeDelay);
			time = xTaskGetTickCount();
			vTaskDelayUntil(&time, period);

			//light orange car led, turn off red car led
			if( xSemaphoreTake(ledBlock, (TickType_t)10) == pdTRUE ){
				set_led(&leds, NORTH, TRAFFIC_LIGHT, GREEN, OFF);
				set_led(&leds, SOUTH, TRAFFIC_LIGHT, GREEN, OFF);
				set_led(&leds, NORTH, TRAFFIC_LIGHT, ORANGE, ON);
				set_led(&leds, SOUTH, TRAFFIC_LIGHT, ORANGE, ON);
				xSemaphoreGive(ledBlock);
			}

			//delay for orangeDelay ms
			period = pdMS_TO_TICKS(orangeDelay);
			time = xTaskGetTickCount();
			vTaskDelayUntil(&time, period);

			//stop blinking pedestrian indicator
			if( xSemaphoreTake(northPedestrianBlinkFlagBlock, (TickType_t)10) == pdTRUE ){
				northPedestrianBlinkFlag = false;
				xSemaphoreGive(northPedestrianBlinkFlagBlock);
			}

			//go to next state
			nextState = PEDESTRIAN_GO;
			break;
		case PEDESTRIAN_GO:

			//light correct leds
			if( xSemaphoreTake(ledBlock, (TickType_t)10) == pdTRUE ){
				set_led_all(&leds, OFF);
				set_led(&leds, NORTH, TRAFFIC_LIGHT, RED, ON);
				set_led(&leds, SOUTH, TRAFFIC_LIGHT, RED, ON);
				set_led(&leds, NORTH, PEDESTRIAN_LIGHT, GREEN, ON);
				xSemaphoreGive(ledBlock);
			}

			//delay for walkingDelay ms
			period = pdMS_TO_TICKS(walkingDelay);
			time = xTaskGetTickCount();
			vTaskDelayUntil(&time, period);

			nextState = PEDESTRIAN_TO_CAR;
			break;
		case PEDESTRIAN_TO_CAR:

			//turn off pedestrian green leds and light orange car leds and red pedestrian leds
			if( xSemaphoreTake(ledBlock, (TickType_t)10) == pdTRUE ){
				set_led(&leds, NORTH, TRAFFIC_LIGHT, ORANGE, ON);
				set_led(&leds, SOUTH, TRAFFIC_LIGHT, ORANGE, ON);
				set_led(&leds, NORTH, PEDESTRIAN_LIGHT, GREEN, OFF);
				set_led(&leds, NORTH, PEDESTRIAN_LIGHT, RED, ON);
				xSemaphoreGive(ledBlock);
			}

			/*
			assert that pedestrian flag is cleared
			effectively disregard any button presses made
			when pedestrian light was green or waiting to turn green
			*/
			if( xSemaphoreTake(trafficBlock, (TickType_t)10) == pdTRUE ){
				traffic.north_pedestrian = false;
				xSemaphoreGive(trafficBlock);
			}

			//delay for orangeDelay ms
			period = pdMS_TO_TICKS(orangeDelay);
			time = xTaskGetTickCount();
			vTaskDelayUntil(&time, period);

			nextState = CAR_GO;
			break;
		default:
			break;
	}

}


/**
 * @brief 	:	function for initial testing(/playing around) with previously implemented modules
 * @param	:	void
 * @return	:	void
 */
void test_logic(void){
	traffic_state_t temp_traffic;

	if( xSemaphoreTake(trafficBlock, (TickType_t)10) == pdTRUE ){
		temp_traffic = traffic;
		xSemaphoreGive(trafficBlock);
	}


	if( xSemaphoreTake(ledBlock, (TickType_t)10) == pdTRUE ){


		if(temp_traffic.north_pedestrian == true){
			if(xSemaphoreTake(northPedestrianBlinkFlagBlock, (TickType_t)10) == pdTRUE){
				northPedestrianBlinkFlag = true;
				xSemaphoreGive(northPedestrianBlinkFlagBlock);
			}
		}
		else{
			if(xSemaphoreTake(northPedestrianBlinkFlagBlock, (TickType_t)10) == pdTRUE){
				northPedestrianBlinkFlag = false;
				xSemaphoreGive(northPedestrianBlinkFlagBlock);
			}
		}

//		if(temp_traffic.west_pedestrian == true){
//			set_led(&leds, WEST, PEDESTRIAN_LIGHT, INDICATOR, ON);
//
//			if( xSemaphoreTake(trafficBlock, (TickType_t)10) == pdTRUE ){
//				traffic.west_pedestrian = false,
//				xSemaphoreGive(trafficBlock);
//			}
//
//		}
//		else{ set_led(&leds, WEST, PEDESTRIAN_LIGHT, INDICATOR, OFF); }



		if(temp_traffic.north_car == true){
			set_led(&leds, NORTH, TRAFFIC_LIGHT, GREEN, ON);
		}
		else{ set_led(&leds, NORTH, TRAFFIC_LIGHT, GREEN, OFF); }

		if(temp_traffic.west_car == true){
			set_led(&leds, WEST, TRAFFIC_LIGHT, GREEN, ON);
		}
		else{ set_led(&leds, WEST, TRAFFIC_LIGHT, GREEN, OFF); }

		if(temp_traffic.south_car == true){
			set_led(&leds, SOUTH, TRAFFIC_LIGHT, GREEN, ON);
		}
		else{ set_led(&leds, SOUTH, TRAFFIC_LIGHT, GREEN, OFF); }

		if(temp_traffic.east_car == true){
			set_led(&leds, EAST, TRAFFIC_LIGHT, GREEN, ON);
		}
		else{ set_led(&leds, EAST, TRAFFIC_LIGHT, GREEN, OFF); }

		xSemaphoreGive(ledBlock);
	}
}



/**
 * @brief: redefinition of callback for external interrupt
 * @param: uint16_t GPIO_Pin, pin identifier of pin invoking the interrupt
 * @return: void
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	uint8_t message[1];
	message[0] = 0x1;

	switch( GPIO_Pin ){
		case PL1_Switch_Pin:
			//HAL_GPIO_TogglePin(USR_LED1_GPIO_Port, USR_LED1_Pin);

			if( xMessageBufferIsEmpty( message_pedestrian_west ) == pdTRUE ){
				xMessageBufferSendFromISR( message_pedestrian_west, message, 1 , NULL);
			}

			break;
		case PL2_Switch_Pin:
			//HAL_GPIO_TogglePin(USR_LED2_GPIO_Port, USR_LED2_Pin);

			if( xMessageBufferIsEmpty( message_pedestrian_north ) == pdTRUE ){
				xMessageBufferSendFromISR( message_pedestrian_north, message, 1 , NULL);
			}

			break;
		default:
			break;
	}

}

/* USER CODE END Application */

