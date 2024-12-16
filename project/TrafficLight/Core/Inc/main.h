/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA
#define TL1_CAR_Pin GPIO_PIN_4
#define TL1_CAR_GPIO_Port GPIOC
#define USR_LED1_Pin GPIO_PIN_2
#define USR_LED1_GPIO_Port GPIOB
#define SR_STCP_Pin GPIO_PIN_12
#define SR_STCP_GPIO_Port GPIOB
#define TL2_CAR_Pin GPIO_PIN_13
#define TL2_CAR_GPIO_Port GPIOB
#define TL3_CAR_Pin GPIO_PIN_14
#define TL3_CAR_GPIO_Port GPIOB
#define USR_LED2_Pin GPIO_PIN_15
#define USR_LED2_GPIO_Port GPIOB
#define SR_ENABLE_Pin GPIO_PIN_7
#define SR_ENABLE_GPIO_Port GPIOC
#define SR_RESET_Pin GPIO_PIN_9
#define SR_RESET_GPIO_Port GPIOA
#define TL4_CAR_Pin GPIO_PIN_10
#define TL4_CAR_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define PL1_Switch_Pin GPIO_PIN_15
#define PL1_Switch_GPIO_Port GPIOA
#define PL1_Switch_EXTI_IRQn EXTI15_10_IRQn
#define SR_SHCP_Pin GPIO_PIN_10
#define SR_SHCP_GPIO_Port GPIOC
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
#define SR_DS_Pin GPIO_PIN_5
#define SR_DS_GPIO_Port GPIOB
#define PL2_Switch_Pin GPIO_PIN_7
#define PL2_Switch_GPIO_Port GPIOB
#define PL2_Switch_EXTI_IRQn EXTI9_5_IRQn

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
