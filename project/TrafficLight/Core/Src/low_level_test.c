/**
 * @brief	:	functions for testing low level features of the traffic shield project
 * @author	:	Adam Strömbom
 */


#include "low_level_test.h"

/**
 * @brief	:	main test function for the low level and abstraction layer functionality
 * @param	:	void
 * @return	:	void
 */
void test_low_level(void){
/*actual low level tests*/
//	SR_test();
//	PB_test();
//	Car_test();

/*abstraction tests*/
//	set_led_test();
//	check_car_test();
	toggle_led_test();
}

/**
 * @brief : function for testing the "toggle" functionality of set_led() function by toggling each led in turn
 *  			north traffic light -> red, yellow, green
 * 				west traffic light -> red, yellow, green
 * 				south traffic light -> red, yellow, green
 * 				east traffic light -> red, yellow, green
 *
 * 				followed by
 *
 * 				north pedestrian light -> red, green, blue
 * 				west pedestrian light -> red, green, blue
 */
void toggle_led_test(void){
	led_registers_t leds;
	led_registers_init(&leds);

	uint32_t delay = 500; //ms
	update_leds(&leds);

	while(1){
		set_led(&leds, NORTH, TRAFFIC_LIGHT, RED, TOGGLE);
		update_leds(&leds);
		HAL_Delay(delay);
		set_led(&leds, NORTH, TRAFFIC_LIGHT, ORANGE, TOGGLE); //either yellow or orange should work
		update_leds(&leds);
		HAL_Delay(delay);
		set_led(&leds, NORTH, TRAFFIC_LIGHT, GREEN, TOGGLE);
		update_leds(&leds);
		HAL_Delay(delay);
		set_led(&leds, WEST, TRAFFIC_LIGHT, RED, TOGGLE);
		update_leds(&leds);
		HAL_Delay(delay);
		set_led(&leds, WEST, TRAFFIC_LIGHT, YELLOW, TOGGLE);
		update_leds(&leds);
		HAL_Delay(delay);
		set_led(&leds, WEST, TRAFFIC_LIGHT, GREEN, TOGGLE);
		update_leds(&leds);
		HAL_Delay(delay);
		set_led(&leds, SOUTH, TRAFFIC_LIGHT, RED, TOGGLE);
		update_leds(&leds);
		HAL_Delay(delay);
		set_led(&leds, SOUTH, TRAFFIC_LIGHT, YELLOW, TOGGLE);
		update_leds(&leds);
		HAL_Delay(delay);
		set_led(&leds, SOUTH, TRAFFIC_LIGHT, GREEN, TOGGLE);
		update_leds(&leds);
		HAL_Delay(delay);
		set_led(&leds, EAST, TRAFFIC_LIGHT, RED, TOGGLE);
		update_leds(&leds);
		HAL_Delay(delay);
		set_led(&leds, EAST, TRAFFIC_LIGHT, YELLOW, TOGGLE);
		update_leds(&leds);
		HAL_Delay(delay);
		set_led(&leds, EAST, TRAFFIC_LIGHT, GREEN, TOGGLE);
		update_leds(&leds);
		HAL_Delay(delay);


		set_led(&leds, NORTH, PEDESTRIAN_LIGHT, RED, TOGGLE);
		update_leds(&leds);
		HAL_Delay(delay);
		set_led(&leds, NORTH, PEDESTRIAN_LIGHT, GREEN, TOGGLE);
		update_leds(&leds);
		HAL_Delay(delay);
		set_led(&leds, NORTH, PEDESTRIAN_LIGHT, INDICATOR, TOGGLE);
		update_leds(&leds);
		HAL_Delay(delay);
		set_led(&leds, WEST, PEDESTRIAN_LIGHT, RED, TOGGLE);
		update_leds(&leds);
		HAL_Delay(delay);
		set_led(&leds, WEST, PEDESTRIAN_LIGHT, GREEN, TOGGLE);
		update_leds(&leds);
		HAL_Delay(delay);
		set_led(&leds, WEST, PEDESTRIAN_LIGHT, INDICATOR, TOGGLE);
		update_leds(&leds);
		HAL_Delay(delay);
	}



}


/**
 * @brief : function to test check_car() function which is an abstraction for checking the switch gpio from the shield
 */
void check_car_test(void){
	led_registers_t leds;
	led_registers_init(&leds);

	traffic_state_t traffic;
	traffic_state_init(&traffic);

	update_leds(&leds);

	while(1){
		if( check_car(NORTH) ){
			set_led(&leds, NORTH, TRAFFIC_LIGHT, GREEN, ON);
		}
		else{
			set_led(&leds, NORTH, TRAFFIC_LIGHT, GREEN, OFF);
		}

		if( check_car(WEST) ){
			set_led(&leds, WEST, TRAFFIC_LIGHT, GREEN, ON);
		}
		else{
			set_led(&leds, WEST, TRAFFIC_LIGHT, GREEN, OFF);
		}

		if( check_car(SOUTH) ){
			set_led(&leds, SOUTH, TRAFFIC_LIGHT, GREEN, ON);
		}
		else{
			set_led(&leds, SOUTH, TRAFFIC_LIGHT, GREEN, OFF);
		}

		if( check_car(EAST) ){
			set_led(&leds, EAST, TRAFFIC_LIGHT, GREEN, ON);
		}
		else{
			set_led(&leds, EAST, TRAFFIC_LIGHT, GREEN, OFF);
		}

		update_leds(&leds);
	}
}


/**
 * @brief : test of set_led() function lighting up leds on the shield in a specific order
 * 				north traffic light -> red, yellow, green
 * 				west traffic light -> red, yellow, green
 * 				south traffic light -> red, yellow, green
 * 				east traffic light -> red, yellow, green
 *
 * 				followed by
 *
 * 				north pedestrian light -> red, green, blue
 * 				west pedestrian light -> red, green, blue
 */
void set_led_test(void){
	led_registers_t test;
	led_registers_init(&test); //initialize all lights OFF
	HAL_Delay(500);

	set_led(&test, NORTH, TRAFFIC_LIGHT, RED, ON);
	update_leds(&test);
	HAL_Delay(500);
	set_led(&test, NORTH, TRAFFIC_LIGHT, YELLOW, ON);
	update_leds(&test);
	HAL_Delay(500);
	set_led(&test, NORTH, TRAFFIC_LIGHT, GREEN, ON);
	update_leds(&test);
	HAL_Delay(500);

	set_led_all(&test, OFF);
	set_led(&test, WEST, TRAFFIC_LIGHT, RED, ON);
	update_leds(&test);
	HAL_Delay(500);
	set_led(&test, WEST, TRAFFIC_LIGHT, YELLOW, ON);
	update_leds(&test);
	HAL_Delay(500);
	set_led(&test, WEST, TRAFFIC_LIGHT, GREEN, ON);
	update_leds(&test);
	HAL_Delay(500);

	set_led_all(&test, OFF);
	set_led(&test, SOUTH, TRAFFIC_LIGHT, RED, ON);
	update_leds(&test);
	HAL_Delay(500);
	set_led(&test, SOUTH, TRAFFIC_LIGHT, YELLOW, ON);
	update_leds(&test);
	HAL_Delay(500);
	set_led(&test, SOUTH, TRAFFIC_LIGHT, GREEN, ON);
	update_leds(&test);
	HAL_Delay(500);

	set_led_all(&test, OFF);
	set_led(&test, EAST, TRAFFIC_LIGHT, RED, ON);
	update_leds(&test);
	HAL_Delay(500);
	set_led(&test, EAST, TRAFFIC_LIGHT, YELLOW, ON);
	update_leds(&test);
	HAL_Delay(500);
	set_led(&test, EAST, TRAFFIC_LIGHT, GREEN, ON);
	update_leds(&test);
	HAL_Delay(500);

	set_led_all(&test, OFF);
	set_led(&test, NORTH, PEDESTRIAN_LIGHT, RED, ON);
	update_leds(&test);
	HAL_Delay(500);
	set_led(&test, NORTH, PEDESTRIAN_LIGHT, GREEN, ON);
	update_leds(&test);
	HAL_Delay(500);
	set_led(&test, NORTH, PEDESTRIAN_LIGHT, INDICATOR, ON);
	update_leds(&test);
	HAL_Delay(500);

	set_led_all(&test, OFF);
	set_led(&test, WEST, PEDESTRIAN_LIGHT, RED, ON);
	update_leds(&test);
	HAL_Delay(500);
	set_led(&test, WEST, PEDESTRIAN_LIGHT, GREEN, ON);
	update_leds(&test);
	HAL_Delay(500);
	set_led(&test, WEST, PEDESTRIAN_LIGHT, INDICATOR, ON);
	update_leds(&test);
	HAL_Delay(500);
}


/**
 * @brief	:	 function for testing that polling the switches work as intended
 */
void Car_test(void){

	uint8_t buf[3];
	uint8_t sr1, sr2, sr3;
	sr1 = 0x00;
	sr2 = 0x00;
	sr3 = 0x00;

	while(1){

		if( HAL_GPIO_ReadPin(TL1_CAR_GPIO_Port, TL1_CAR_Pin) == GPIO_PIN_SET ){
			sr1 = sr1 | TL1_RED | TL1_YELLOW | TL1_GREEN; //turn on leds
		}
		else{
			sr1 = sr1 & ~TL1_RED & ~TL1_YELLOW & ~TL1_GREEN; //turn off leds
		}

		if( HAL_GPIO_ReadPin(TL2_CAR_GPIO_Port, TL2_CAR_Pin) == GPIO_PIN_SET ){
			sr2 = sr2 | TL2_RED | TL2_YELLOW | TL2_GREEN;
		}
		else{
			sr2 = sr2 & ~TL2_RED & ~TL2_YELLOW & ~TL2_GREEN;
		}

		if( HAL_GPIO_ReadPin(TL3_CAR_GPIO_Port, TL3_CAR_Pin) == GPIO_PIN_SET ){
			sr3 = sr3 | TL3_RED | TL3_YELLOW | TL3_GREEN;
		}
		else{
			sr3 = sr3 & ~TL3_RED & ~TL3_YELLOW & ~TL3_GREEN;
		}

		if( HAL_GPIO_ReadPin(TL4_CAR_GPIO_Port, TL4_CAR_Pin) == GPIO_PIN_SET ){
			sr3 = sr3 | TL4_RED | TL4_YELLOW | TL4_GREEN;
		}
		else{
			sr3 = sr3 & ~TL4_RED & ~TL4_YELLOW & ~TL4_GREEN;
		}

		buf[0] = sr3;
		buf[1] = sr2;
		buf[2] = sr1;

		SR_Send(buf);
	}
}

/**
 * function to test functionality of pedestrian buttons
 */
void PB_test(void){
	//handled by interrupt, tested by making interrupt callback toggle USR_LEDs on the shield board
}



/**
 * @brief	:	function for testing SR_Send() function
 * @param	:	void
 * @return	:	void
 * @brief	: 	correct test lights up each traffic light led and pair of pedestrian led one at a time starting with output 5
 * 				from shift register 3 and followed by output 4, 3..0 after which output 5 from shift register 2 is activated and so on...
 * 				LED sequence should look like:
 *
 * 					LED12
 * 					LED11
 * 					LED10
 * 					LED9
 * 					LED8
 * 					LED7
 *
 * 					LED23 and LED24
 * 					LED21 and LED22
 * 					LED19 and LED20
 * 					LED6
 * 					LED5
 * 					LED4
 *
 *					LED17 and LED18
 *					LED15 and LED16
 *					LED13 and LED14
 *					LED3
 *					LED2
 *					LED1
 */
void SR_test(void){
	uint8_t leds[] = {0x20, 0x00, 0x00}; //{shiftregister3, shiftregister2, shiftregister1}

	for(int i=0; i<6; i++){
		SR_Send(leds);
		HAL_Delay(1000);
		leds[0] = leds[0] >> 1;
	}

	leds[0] = 0x00;
	leds[1] = 0x20;

	for(int i=0; i<6; i++){
			SR_Send(leds);
			HAL_Delay(1000);
			leds[1] = leds[1] >> 1;
	}

	leds[1] = 0x00;
	leds[2] = 0x20;

	for(int i=0; i<6; i++){
			SR_Send(leds);
			HAL_Delay(1000);
			leds[2] = leds[2] >> 1;
	}

}
