


/**
 * @brief	:	definitions for shield_abstraction.c
 * @author	:	Adam Strömbom
 * @date	:	December 2024
 */

#include <stdint.h>
#include "shield.h"
#include <stdbool.h>

typedef struct led_registers{
	uint8_t sr1;
	uint8_t sr2;
	uint8_t sr3;
} led_registers_t;

typedef enum {
	WEST,
	SOUTH,
	EAST,
	NORTH
} junction_position_t;

typedef enum {
	TRAFFIC_LIGHT,
	PEDESTRIAN_LIGHT,
} light_type_t;

typedef enum {
	RED,
	YELLOW,
	GREEN,
	INDICATOR,
	ORANGE = YELLOW //either orange or yellow works for the same lights
} light_color_t;

typedef enum {
	ON = 1,
	OFF = 0,
	TOGGLE = -1
} led_state_t;

typedef struct traffic_state{
	bool north_car;
	bool west_car;
	bool south_car;
	bool east_car;
	bool north_pedestrian;
	bool west_pedestrian;
} traffic_state_t;

void led_registers_init(led_registers_t* registers);
void update_leds(led_registers_t* registers);
void set_led_all(led_registers_t* leds, led_state_t led_state);
void set_led(led_registers_t* leds, junction_position_t position, light_type_t light_type, light_color_t color, led_state_t led_state);
int8_t traffic_state_init(traffic_state_t* state);
bool check_car(junction_position_t position);

/*help functions (for set led)*/
void north_leds(led_registers_t* leds, light_type_t light_type, light_color_t color, led_state_t led_state);
void west_leds(led_registers_t* leds, light_type_t light_type, light_color_t color, led_state_t led_state);
void south_leds(led_registers_t* leds, light_type_t light_type, light_color_t color, led_state_t led_state);
void east_leds(led_registers_t* leds, light_type_t light_type, light_color_t color, led_state_t led_state);

void north_traffic_light(led_registers_t* leds, light_color_t color, led_state_t led_state);
void west_traffic_light(led_registers_t* leds, light_color_t color, led_state_t led_state);
void south_traffic_light(led_registers_t* leds, light_color_t color, led_state_t led_state);
void east_traffic_light(led_registers_t* leds, light_color_t color, led_state_t led_state);

void north_pedestrian_light(led_registers_t* leds, light_color_t color, led_state_t led_state);
void west_pedestrian_light(led_registers_t* leds, light_color_t color, led_state_t led_state);
