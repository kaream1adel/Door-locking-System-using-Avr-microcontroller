 /******************************************************************************
 *
 * Module: timer
 *
 * File Name: timer.h
 *
 * Description: Header file for the AVR timer driver
 *
 * Author: Kaream adel
 *
 *******************************************************************************/

#ifndef TIMER_H_
#define TIMER_H_
#include "std_types.h"

/*******************************************************************************
 *                               Types Declaration                             *
 *******************************************************************************/
typedef enum{
	NORMAL,CTC_16BIT=1, CTC_8BIT=1


}MODE_OF_OPERATION;

typedef enum{

	NO_CLOCK , F_CPU_ ,F_CPU_8, F_CPU_16, F_CPU_64, F_CPU_256, F_CPU_1024


}CLOCK_SELECT;


typedef struct{
	uint16 Compare_value;
	MODE_OF_OPERATION mode;
	CLOCK_SELECT clock ;
    uint16 initial_value ;
}CONFIG_TIMER;

/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/
/*
 * Description :
 * initialize timer0 to start counting by choosing the required mode operation &
 * prescaler & initial value and the compare value if compare mode required
 */

void TIMER0_init(const CONFIG_TIMER* Config_timer0 );

/*
 * Description :
 * initialize timer1 to start counting by choosing the required mode operation &
 * prescaler & initial value and the compare value if compare mode required
 */

void TIMER1_init(const CONFIG_TIMER* Config_timer1 );

/*
 * Description :
 * initialize timer2 to start counting by choosing the required mode operation &
 * prescaler & initial value and the compare value if compare mode required
 */

void TIMER2_init(const CONFIG_TIMER* Config_timer2 );

/*
 * Description: Function to set the Call Back function address.
 */

void TIMER0_setCallBack(void(*a_ptr)(void));


/*
 * Description: Function to set the Call Back function address.
 */
void TIMER1_setCallBack(void(*c_ptr)(void));


/*
 * Description: Function to set the Call Back function address.
 */

void TIMER2_setCallBack(void(*b_ptr)(void));

/*
 * Description :Function to stop the timer counting.
 */
void TIMER0_deinit(void);
/*
 * Description :Function to stop the timer counting.
 */
void TIMER1_deinit(void);
/*
 * Description :Function to stop the timer counting.
 */
void TIMER2_deinit(void);

#endif /* TIMER_H_ */
