/**
 *	@brief: shield_abstraction.c, functions for abstracting from the shield hardware logic into more human readable logic
 *	@author: Adam Strömbom
 */

#include "shield_abstraction.h"

/**
 * @brief : led_registers_init, initialization of led register struct
 * @param : led_registers_t* registers, pointer to led state object to be initialized
 * @return : void
 */
void led_registers_init(led_registers_t* registers){
	registers->sr1 = 0x00;
	registers->sr2 = 0x00;
	registers->sr3 = 0x00;
}

/**
 * @brief : update_leds, re-package contents of a ledregister struct and send it to shift registers
 * @param : led_registers_t* registers, pointer to led state object to be initialized
 * @return : void
 */
void update_leds(led_registers_t* registers){
	uint8_t buf[3];

	buf[0] = registers->sr3;
	buf[1] = registers->sr2;
	buf[2] = registers->sr1;
	SR_Send(buf);
}

/**
 * @brief : traffic_state_init, initialization of a traffic state
 * @params : traffic_state_t* state, pointer to the trafffic state to be initialized
 * @return : int8_t, 0 if correctly initialized
 */
int8_t traffic_state_init(traffic_state_t* state){
	state->north_pedestrian = false;
	state->west_pedestrian = false;

	state->north_car = check_car(NORTH);
	state->west_car = check_car(WEST);
	state->south_car = check_car(SOUTH);
	state->east_car = check_car(EAST);

	return 0;
}

/**
 * @brief : check_car, check switch status at a given position in the junction
 * @param : junction_position_t position, position in junction
 * @return : boolean, true if switch active
 */
bool check_car(junction_position_t position){
	switch (position){
		case NORTH:
			if( HAL_GPIO_ReadPin(TL4_CAR_GPIO_Port, TL4_CAR_Pin) == GPIO_PIN_SET ){
				return true;
			}
			break;
		case WEST:
			if( HAL_GPIO_ReadPin(TL1_CAR_GPIO_Port, TL1_CAR_Pin) == GPIO_PIN_SET ){
				return true;
			}
			break;
		case SOUTH:
			if( HAL_GPIO_ReadPin(TL2_CAR_GPIO_Port, TL2_CAR_Pin) == GPIO_PIN_SET ){
				return true;
			}
			break;
		case EAST:
			if( HAL_GPIO_ReadPin(TL3_CAR_GPIO_Port, TL3_CAR_Pin) == GPIO_PIN_SET ){
				return true;
			}
			break;
		default:
			return false;
			break;
	}
	return false;
}


/**
 * @brief : 	set_led_all, set all leds to specified state of a specific led state object
 * @param : 	led_registers_t* leds, pointer to led state object to modify
 * 				led_state_t led_state, state to set ledst to (ON or OFF)
 * @return : 	void
 */
void set_led_all(led_registers_t* leds, led_state_t led_state){
	switch (led_state){
		case ON:
			leds->sr3 = 0xff;
			leds->sr2 = 0xff;
			leds->sr1 = 0xff;
			break;
		case OFF:
			leds->sr3 = 0x00;
			leds->sr2 = 0x00;
			leds->sr1 = 0x00;
			break;
		default:
			break;
	}
}

/**
 * @brief	:	set_led, Set specific led of a specific led state object to ON or OFF, also able to toggle by passing TOGGLE
 * @return	:	void
 * @param	: 	led_registers_t* leds, pointer to led state to be modified
 * 				junction_position_t position, position in junction (NORTH, WEST, SOUTH, EAST)
 * 				light_type_t light_type, type of traffic light, TRAFFIC_LIGHT for actual traffic light (for cars) and PEDESTRIAN_LIGHT for pedestrian crossing lights an indicator led
 * 				light_color_t color, color of led to be modified RED, YELLOW, GREEN, ORANGE=YELLOW for traffic type TRAFFIC_LIGHT, RED, GREEN, INDICATOR for PEDESTRIAN_LIGHT
 * 				led_state_t led_state, state to set specific led to, ON, OFF or TOGGLE (will flip specific bit)
 */
void set_led(led_registers_t* leds, junction_position_t position, light_type_t light_type, light_color_t color, led_state_t led_state){

	//reroute to help functions instead of indentation hell
	switch (position){
		case NORTH:
			north_leds(leds, light_type, color, led_state);
			break;
		case WEST:
			west_leds(leds, light_type, color, led_state);
			break;
		case SOUTH:
			south_leds(leds, light_type, color, led_state);
			break;
		case EAST:
			east_leds(leds, light_type, color, led_state);
			break;
		default:
			break;
	}
}

/**
 * @brief	:	help function for set_led(5), navigating closer to determine which led bit needs to be modified in a specified led state
 * @return	:	void
 * @param	: 	led_registers_t* leds, pointer to led state to be modified
 * 				light_type_t light_type, type of traffic light, TRAFFIC_LIGHT for actual traffic light (for cars) and PEDESTRIAN_LIGHT for pedestrian crossing lights an indicator led
 * 				light_color_t color, color of led to be modified RED, YELLOW, GREEN, ORANGE=YELLOW for traffic type TRAFFIC_LIGHT, RED, GREEN, INDICATOR for PEDESTRIAN_LIGHT
 * 				led_state_t led_state, state to set specific led to, ON, OFF or TOGGLE (will flip specific bit)
 */
void north_leds(led_registers_t* leds, light_type_t light_type, light_color_t color, led_state_t led_state){
	switch (light_type){
		case TRAFFIC_LIGHT:
			north_traffic_light(leds, color, led_state);
			break;
		case PEDESTRIAN_LIGHT:
			north_pedestrian_light(leds, color, led_state);
			break;
		default:
			break;
	}
}
/**
 * @brief	:	help function for set_led(5), navigating closer to determine which led bit needs to be modified in a specified led state
 * @return	:	void
 * @param	: 	led_registers_t* leds, pointer to led state to be modified
 * 				light_type_t light_type, type of traffic light, TRAFFIC_LIGHT for actual traffic light (for cars) and PEDESTRIAN_LIGHT for pedestrian crossing lights an indicator led
 * 				light_color_t color, color of led to be modified RED, YELLOW, GREEN, ORANGE=YELLOW for traffic type TRAFFIC_LIGHT, RED, GREEN, INDICATOR for PEDESTRIAN_LIGHT
 * 				led_state_t led_state, state to set specific led to, ON, OFF or TOGGLE (will flip specific bit)
 */
void west_leds(led_registers_t* leds, light_type_t light_type, light_color_t color, led_state_t led_state){
	switch (light_type){
		case TRAFFIC_LIGHT:
			west_traffic_light(leds, color, led_state);
			break;
		case PEDESTRIAN_LIGHT:
			west_pedestrian_light(leds, color, led_state);
			break;
		default:
			break;
	}
}
/**
 * @brief	:	help function for set_led(5), navigating closer to determine which led bit needs to be modified in a specified led state
 * @return	:	void
 * @param	: 	led_registers_t* leds, pointer to led state to be modified
 * 				light_type_t light_type, type of traffic light, TRAFFIC_LIGHT for actual traffic light (for cars) and PEDESTRIAN_LIGHT for pedestrian crossing lights an indicator led
 * 				light_color_t color, color of led to be modified RED, YELLOW, GREEN, ORANGE=YELLOW for traffic type TRAFFIC_LIGHT, RED, GREEN, INDICATOR for PEDESTRIAN_LIGHT
 * 				led_state_t led_state, state to set specific led to, ON, OFF or TOGGLE (will flip specific bit)
 */
void south_leds(led_registers_t* leds, light_type_t light_type, light_color_t color, led_state_t led_state){
	switch (light_type){
		case TRAFFIC_LIGHT:
			south_traffic_light(leds, color, led_state);
			break;
		case PEDESTRIAN_LIGHT:
			//do nothing (no pedestrian lights)
			break;
		default:
			break;
	}
}
/**
 * @brief	:	help function for set_led(5), navigating closer to determine which led bit needs to be modified in a specified led state
 * @return	:	void
 * @param	: 	led_registers_t* leds, pointer to led state to be modified
 * 				light_type_t light_type, type of traffic light, TRAFFIC_LIGHT for actual traffic light (for cars) and PEDESTRIAN_LIGHT for pedestrian crossing lights an indicator led
 * 				light_color_t color, color of led to be modified RED, YELLOW, GREEN, ORANGE=YELLOW for traffic type TRAFFIC_LIGHT, RED, GREEN, INDICATOR for PEDESTRIAN_LIGHT
 * 				led_state_t led_state, state to set specific led to, ON, OFF or TOGGLE (will flip specific bit)
 */
void east_leds(led_registers_t* leds, light_type_t light_type, light_color_t color, led_state_t led_state){
	switch (light_type){
		case TRAFFIC_LIGHT:
			east_traffic_light(leds, color, led_state);
			break;
		case PEDESTRIAN_LIGHT:
			//do nothing (no pedestrian lights)
			break;
		default:
			break;
	}
}


/**
 * @brief	:	help function north_leds(4), navigating even closer to determine and modifying specific led bit of a TRAFFIC_LIGHT in a specified led state
 * @return	:	void
 * @param	: 	led_registers_t* leds, pointer to led state to be modified
 * 				light_color_t color, color of led to be modified RED, YELLOW, GREEN, ORANGE=YELLOW for traffic type TRAFFIC_LIGHT, RED, GREEN, INDICATOR for PEDESTRIAN_LIGHT
 * 				led_state_t led_state, state to set specific led to, ON, OFF or TOGGLE (will flip specific bit)
 */
void north_traffic_light(led_registers_t* leds, light_color_t color, led_state_t led_state){
	switch (color){
		case RED:
			switch (led_state){
				case ON:
					leds->sr3 = leds->sr3 | TL4_RED;
					break;
				case OFF:
					leds->sr3 = leds->sr3 & ~TL4_RED;
					break;
				case TOGGLE:
					leds->sr3 = leds->sr3 ^ TL4_RED;
					break;
				default:
					break;
			}
			break;
		case YELLOW:
			switch (led_state){
				case ON:
					leds->sr3 = leds->sr3 | TL4_YELLOW;
					break;
				case OFF:
					leds->sr3 = leds->sr3 & ~TL4_YELLOW;
					break;
				case TOGGLE:
					leds->sr3 = leds->sr3 ^ TL4_YELLOW;
					break;
				default:
					break;
			}
			break;
		case GREEN:
			switch (led_state){
				case ON:
					leds->sr3 = leds->sr3 | TL4_GREEN;
					break;
				case OFF:
					leds->sr3 = leds->sr3 & ~TL4_GREEN;
					break;
				case TOGGLE:
					leds->sr3 = leds->sr3 ^ TL4_GREEN;
					break;
				default:
					break;
			}
			break;
		case INDICATOR:
			//do nothing (no indicator for traffic light)
			break;
		default:
			break;
	}
}
/**
 * @brief	:	help function west_leds(4), navigating even closer to determine and modifying specific led bit of a TRAFFIC_LIGHT in a specified led state
 * @return	:	void
 * @param	: 	led_registers_t* leds, pointer to led state to be modified
 * 				light_color_t color, color of led to be modified RED, YELLOW, GREEN, ORANGE=YELLOW for traffic type TRAFFIC_LIGHT, RED, GREEN, INDICATOR for PEDESTRIAN_LIGHT
 * 				led_state_t led_state, state to set specific led to, ON, OFF or TOGGLE (will flip specific bit)
 */
void west_traffic_light(led_registers_t* leds, light_color_t color, led_state_t led_state){
	switch (color){
		case RED:
			switch (led_state){
				case ON:
					leds->sr1 = leds->sr1 | TL1_RED;
					break;
				case OFF:
					leds->sr1 = leds->sr1 & ~TL1_RED;
					break;
				case TOGGLE:
					leds->sr1 = leds->sr1 ^ TL1_RED;
					break;
				default:
					break;
			}
			break;
		case YELLOW:
			switch (led_state){
				case ON:
					leds->sr1 = leds->sr1 | TL1_YELLOW;
					break;
				case OFF:
					leds->sr1 = leds->sr1 & ~TL1_YELLOW;
					break;
				case TOGGLE:
					leds->sr1 = leds->sr1 ^ TL1_YELLOW;
					break;
				default:
					break;
			}
			break;
		case GREEN:
			switch (led_state){
				case ON:
					leds->sr1 = leds->sr1 | TL1_GREEN;
					break;
				case OFF:
					leds->sr1 = leds->sr1 & ~TL1_GREEN;
					break;
				case TOGGLE:
					leds->sr1 = leds->sr1 ^ TL1_GREEN;
					break;
				default:
					break;
			}
			break;
		case INDICATOR:
			//do nothing (no indicator for traffic light)
			break;
		default:
			break;
	}
}
/**
 * @brief	:	help function south_leds(4), navigating even closer to determine and modifying specific led bit of a TRAFFIC_LIGHT in a specified led state
 * @return	:	void
 * @param	: 	led_registers_t* leds, pointer to led state to be modified
 * 				light_color_t color, color of led to be modified RED, YELLOW, GREEN, ORANGE=YELLOW for traffic type TRAFFIC_LIGHT, RED, GREEN, INDICATOR for PEDESTRIAN_LIGHT
 * 				led_state_t led_state, state to set specific led to, ON, OFF or TOGGLE (will flip specific bit)
 */
void south_traffic_light(led_registers_t* leds, light_color_t color, led_state_t led_state){
	switch (color){
		case RED:
			switch (led_state){
				case ON:
					leds->sr2 = leds->sr2 | TL2_RED;
					break;
				case OFF:
					leds->sr2 = leds->sr2 & ~TL2_RED;
					break;
				case TOGGLE:
					leds->sr2 = leds->sr2 ^ TL2_RED;
					break;
				default:
					break;
			}
			break;
		case YELLOW:
			switch (led_state){
				case ON:
					leds->sr2 = leds->sr2 | TL2_YELLOW;
					break;
				case OFF:
					leds->sr2 = leds->sr2 & ~TL2_YELLOW;
					break;
				case TOGGLE:
					leds->sr2 = leds->sr2 ^ TL2_YELLOW;
					break;
				default:
					break;
			}
			break;
		case GREEN:
			switch (led_state){
				case ON:
					leds->sr2 = leds->sr2 | TL2_GREEN;
					break;
				case OFF:
					leds->sr2 = leds->sr2 & ~TL2_GREEN;
					break;
				case TOGGLE:
					leds->sr2 = leds->sr2 ^ TL2_GREEN;
					break;
				default:
					break;
			}
			break;
		case INDICATOR:
			//do nothing (no indicator for traffic light)
			break;
		default:
			break;
	}
}
/**
 * @brief	:	help function east_leds(4), navigating even closer to determine and modifying specific led bit of a TRAFFIC_LIGHT in a specified led state
 * @return	:	void
 * @param	: 	led_registers_t* leds, pointer to led state to be modified
 * 				light_color_t color, color of led to be modified RED, YELLOW, GREEN, ORANGE=YELLOW for traffic type TRAFFIC_LIGHT, RED, GREEN, INDICATOR for PEDESTRIAN_LIGHT
 * 				led_state_t led_state, state to set specific led to, ON, OFF or TOGGLE (will flip specific bit)
 */
void east_traffic_light(led_registers_t* leds, light_color_t color, led_state_t led_state){
	switch (color){
		case RED:
			switch (led_state){
				case ON:
					leds->sr3 = leds->sr3 | TL3_RED;
					break;
				case OFF:
					leds->sr3 = leds->sr3 & ~TL3_RED;
					break;
				case TOGGLE:
					leds->sr3 = leds->sr3 ^ TL3_RED;
					break;
				default:
					break;
			}
			break;
		case YELLOW:
			switch (led_state){
				case ON:
					leds->sr3 = leds->sr3 | TL3_YELLOW;
					break;
				case OFF:
					leds->sr3 = leds->sr3 & ~TL3_YELLOW;
					break;
				case TOGGLE:
					leds->sr3 = leds->sr3 ^ TL3_YELLOW;
					break;
				default:
					break;
			}
			break;
		case GREEN:
			switch (led_state){
				case ON:
					leds->sr3 = leds->sr3 | TL3_GREEN;
					break;
				case OFF:
					leds->sr3 = leds->sr3 & ~TL3_GREEN;
					break;
				case TOGGLE:
					leds->sr3 = leds->sr3 ^ TL3_GREEN;
					break;
				default:
					break;
			}
			break;
		case INDICATOR:
			//do nothing (no indicator for traffic light)
			break;
		default:
			break;
	}
}
/**
 * @brief	:	help function north_leds(4), navigating even closer to determine and modifying specific led bit of a PEDESTRIAN_LIGHT in a specified led state
 * @return	:	void
 * @param	: 	led_registers_t* leds, pointer to led state to be modified
 * 				light_color_t color, color of led to be modified RED, YELLOW, GREEN, ORANGE=YELLOW for traffic type TRAFFIC_LIGHT, RED, GREEN, INDICATOR for PEDESTRIAN_LIGHT
 * 				led_state_t led_state, state to set specific led to, ON, OFF or TOGGLE (will flip specific bit)
 */
void north_pedestrian_light(led_registers_t* leds, light_color_t color, led_state_t led_state){
	switch (color){
		case RED:
			switch (led_state){
				case ON:
					leds->sr2 = leds->sr2 | PL2_RED;
					break;
				case OFF:
					leds->sr2 = leds->sr2 & ~PL2_RED;
					break;
				case TOGGLE:
					leds->sr2 = leds->sr2 ^ PL2_RED;
					break;
				default:
					break;
			}
			break;
		case YELLOW:
			//do nothing
			break;
		case GREEN:
			switch (led_state){
				case ON:
					leds->sr2 = leds->sr2 | PL2_GREEN;
					break;
				case OFF:
					leds->sr2 = leds->sr2 & ~PL2_GREEN;
					break;
				case TOGGLE:
					leds->sr2 = leds->sr2 ^ PL2_GREEN;
					break;
				default:
					break;
			}
			break;
		case INDICATOR:
			switch (led_state){
				case ON:
					leds->sr2 = leds->sr2 | PL2_BLUE;
					break;
				case OFF:
					leds->sr2 = leds->sr2 & ~PL2_BLUE;
					break;
				case TOGGLE:
					leds->sr2 = leds->sr2 ^ PL2_BLUE;
					break;
				default:
					break;
			}
			break;
		default:
			break;
	}
}
/**
 * @brief	:	help function west_leds(4), navigating even closer to determine and modifying specific led bit of a PEDESTRIAN_LIGHT in a specified led state
 * @return	:	void
 * @param	: 	led_registers_t* leds, pointer to led state to be modified
 * 				light_color_t color, color of led to be modified RED, YELLOW, GREEN, ORANGE=YELLOW for traffic type TRAFFIC_LIGHT, RED, GREEN, INDICATOR for PEDESTRIAN_LIGHT
 * 				led_state_t led_state, state to set specific led to, ON, OFF or TOGGLE (will flip specific bit)
 */
void west_pedestrian_light(led_registers_t* leds, light_color_t color, led_state_t led_state){
	switch (color){
		case RED:
			switch (led_state){
				case ON:
					leds->sr1 = leds->sr1 | PL1_RED;
					break;
				case OFF:
					leds->sr1 = leds->sr1 & ~PL1_RED;
					break;
				case TOGGLE:
					leds->sr1 = leds->sr1 ^ PL1_RED;
					break;
				default:
					break;
			}
			break;
		case YELLOW:
			//do nothing
			break;
		case GREEN:
			switch (led_state){
				case ON:
					leds->sr1 = leds->sr1 | PL1_GREEN;
					break;
				case OFF:
					leds->sr1 = leds->sr1 & ~PL1_GREEN;
					break;
				case TOGGLE:
					leds->sr1 = leds->sr1 ^ PL1_GREEN;
					break;
				default:
					break;
			}
			break;
		case INDICATOR:
			switch (led_state){
				case ON:
					leds->sr1 = leds->sr1 | PL1_BLUE;
					break;
				case OFF:
					leds->sr1 = leds->sr1 & ~PL1_BLUE;
					break;
				case TOGGLE:
					leds->sr1 = leds->sr1 ^ PL1_BLUE;
					break;
				default:
					break;
			}
			break;
		default:
			break;
	}
}
