
/**
 * @file	:	shield.h
 * @brief 	:	contains definitions of functions for the specific traffic light shield used in the project
*/
#include "gpio.h"
#include "spi.h"
#include "stm32l4xx_hal_def.h"

#define SR_RESET_GPIO_PORT GPIOA


//bit masks for SR1
#define TL1_RED 0x01
#define TL1_YELLOW 0x02
#define TL1_GREEN 0x04
#define PL1_RED 0x08
#define PL1_GREEN 0x10
#define PL1_BLUE 0x20

//bit masks for SR2
#define TL2_RED 0x01
#define TL2_YELLOW 0x02
#define TL2_GREEN 0x04
#define PL2_RED 0x08
#define PL2_GREEN 0x10
#define PL2_BLUE 0x20

//bit masks for SR3
#define TL3_RED 0x01
#define TL3_YELLOW 0x02
#define TL3_GREEN 0x04
#define TL4_RED 0x08
#define TL4_YELLOW 0x10
#define TL4_GREEN 0x20

void SR_Init(void);
int8_t SR_Send(uint8_t bytes[]);
