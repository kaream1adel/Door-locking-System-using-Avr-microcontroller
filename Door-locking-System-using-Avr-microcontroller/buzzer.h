 /******************************************************************************
 *
 * Module: BUZZER
 *
 * File Name: buzzer.h
 *
 * Description: header file for the AVR buzzer driver
 *
 * Author: Kaream adel
 *
 *******************************************************************************/


#ifndef BUZZER_H_
#define BUZZER_H_
#include "std_types.h"

#define BUZZER_OUTPUT_PORT  PORTC_ID
#define BUZZER_OUTPUT_PIN   PIN3_ID


void BUZZER_init (void);
void BUZZER_start(void);
void BUZZER_stop(void);


#endif /* BUZZER_H_ */
