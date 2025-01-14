/**
 * @author : Adam Strömbom
 * @brief : functions interfacing directly with the HAL library 
*/


#include "shield.h"



/**
 * @brief	:	sends data over SPI to the shift registers
 * @return	:	void
 * @param	:	uint8_t bytes[], buffer containing at least 3 bytes
 * 					bytes[0] respresents Shift regsiter3
 * 					bytes[1] respresents Shift regsiter2
 * 					bytes[2] respresents Shift regsiter1
 *
 */
int8_t SR_Send(uint8_t bytes[]){
	HAL_StatusTypeDef status;
	status = HAL_SPI_Transmit(&hspi3, bytes, 3, 1000);

	if(status == HAL_OK){
		HAL_GPIO_WritePin(SR_STCP_GPIO_Port, SR_STCP_Pin, GPIO_PIN_SET); //toggle stcp to put it to the output
		HAL_GPIO_WritePin(SR_STCP_GPIO_Port, SR_STCP_Pin, GPIO_PIN_RESET);
		return 0;
	}

	return -1;
}


/**
 * @brief	:	function for initializing shift register
 * @param	:	void
 * @return	:	void
 */
void SR_Init(void){
	//set SR_reset high (active low)
	HAL_GPIO_WritePin(SR_RESET_GPIO_PORT, SR_RESET_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(SR_RESET_GPIO_PORT, SR_RESET_Pin, GPIO_PIN_SET);

	//set SR_ENABLE low (output enable is active low)
	HAL_GPIO_WritePin(SR_ENABLE_GPIO_Port, SR_ENABLE_Pin, GPIO_PIN_RESET);

	//set SR_STCP low (data transferred to storage register on rising edge)
	HAL_GPIO_WritePin(SR_STCP_GPIO_Port, SR_STCP_Pin, GPIO_PIN_RESET);
}


///**
// * @brief: initial redefinition of callback for external interrupt, later overwritten within freertos.c
// * @param: uint16_t GPIO_Pin, which gpio pin is responsible for invoking the interrupt
// * @return: void
// */
//void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
//
//	switch( GPIO_Pin ){
//		case PL1_Switch_Pin:
//			HAL_GPIO_TogglePin(USR_LED1_GPIO_Port, USR_LED1_Pin);
//
//
//
//			break;
//		case PL2_Switch_Pin:
//			HAL_GPIO_TogglePin(USR_LED2_GPIO_Port, USR_LED2_Pin);
//
//			if( xMessageBufferIsEmpty( message_pedestrian_north ) == pdTRUE ){
//				//do something
//			}
//
//			break;
//		default:
//			break;
//	}
//
//}
