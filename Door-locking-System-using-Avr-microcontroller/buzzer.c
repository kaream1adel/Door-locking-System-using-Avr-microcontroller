 /******************************************************************************
 *
 * Module: BUZZER
 *
 * File Name: buzzer.c
 *
 * Description: source file for the AVR buzzer driver
 *
 * Author: Kaream adel
 *
 *******************************************************************************/


#include "gpio.h"
#include "buzzer.h"

void BUZZER_init (void){

	GPIO_setupPinDirection(BUZZER_OUTPUT_PORT,BUZZER_OUTPUT_PIN, PIN_OUTPUT);
	GPIO_writePin(BUZZER_OUTPUT_PORT,BUZZER_OUTPUT_PIN, LOGIC_LOW);

}
void BUZZER_start(void){

	GPIO_writePin(BUZZER_OUTPUT_PORT,BUZZER_OUTPUT_PIN, LOGIC_HIGH);

}
void BUZZER_stop(void){

	GPIO_writePin(BUZZER_OUTPUT_PORT,BUZZER_OUTPUT_PIN, LOGIC_LOW);

}
