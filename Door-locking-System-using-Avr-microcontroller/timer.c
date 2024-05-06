 /******************************************************************************
 *
 * Module: timer
 *
 * File Name: timer.c
 *
 * Description: source file for the AVR timer driver
 *
 * Author: Kaream adel
 *
 *******************************************************************************/
/*
 * this timer module will always configure by interrupt method and non_pwm mode
 */

#include <avr/io.h>
#include"gpio.h"
#include"common_macros.h"
#include"timer.h"
#include <avr/interrupt.h> /* For ICU ISR */

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

/* Global variables to hold the address of the call back function in the application */
static volatile void (*g_callBackPtrT0)(void) = NULL_PTR;
static volatile void (*g_callBackPtrT1)(void) = NULL_PTR;
static volatile void (*g_callBackPtrT2)(void) = NULL_PTR;
/*******************************************************************************
 *                       Interrupt Service Routines                            *
 *******************************************************************************/

ISR(TIMER0_OVF_vect)
{
	if(g_callBackPtrT0 != NULL_PTR)
	{
		/* Call the Call Back function in the application after the interrupt is happened */
		(*g_callBackPtrT0)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

ISR(TIMER0_COMP_vect)
{
	if(g_callBackPtrT0 != NULL_PTR)
	{
		/* Call the Call Back function in the application after the interrupt is happened */
		(*g_callBackPtrT0)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}
ISR(TIMER1_OVF_vect)
{
	if(g_callBackPtrT1 != NULL_PTR)
	{
		/* Call the Call Back function in the application after the interrupt is happened */
		(*g_callBackPtrT1)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}
ISR(TIMER1_COMPA_vect)
{
	if(g_callBackPtrT1 != NULL_PTR)
	{
		/* Call the Call Back function in the application after the interrupt is happened */
		(*g_callBackPtrT1)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

ISR(TIMER2_OVF_vect)
{
	if(g_callBackPtrT2 != NULL_PTR)
	{
		/* Call the Call Back function in the application after the interrupt is happened */
		(*g_callBackPtrT2)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

ISR(TIMER2_COMP_vect)
{
	if(g_callBackPtrT2 != NULL_PTR)
	{
		/* Call the Call Back function in the application after the interrupt is happened */
		(*g_callBackPtrT2)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}


/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/
/*
 * Description :
 * initialize timer0 to start counting by choosing the required mode operation &
 * prescaler & initial value and the compare value if compare mode required
 */

void TIMER0_init(const CONFIG_TIMER* Config_timer0 ){

	TCNT0 = (uint8)(Config_timer0->initial_value); // Set Timer0 initial value to start counting from it
	/* Configure the timer control register
	 * 1.  FOC0=1             Non PWM mode
	 * 2.  WGM01 & WGM00      to choose the operation mode
	 * 3.  COM00 & COM01      to choose the operation mode
	 * 4.  CS00 & CS01 & CS02 to choose a specific prescaler to the timer frequency
	 */
	TCCR0 |= (1 << FOC0);

	TCCR0 = (TCCR0 & 0xF8) | (Config_timer0->clock);

    /*
     * COM01 & COM00 ( 00 -> normal) , (01-> compare )
     *  WGM00 = 0 beacause always non pwm_mode
     *  WGM01 = 1 ( Compare Mode)
     *  WGM01 = 0 ( Normal Mode)
     */
	TCCR0 = (TCCR0 & 0xF7) | ((Config_timer0->mode)<<3);

	if ((Config_timer0->mode) == NORMAL)
	{
		TIMSK |= (1<<TOIE0);// Overflow interrupt enable
	}
	else if ((Config_timer0->mode) == CTC_8BIT)
	{
		TCCR0 |=(1<<COM00);
		OCR0 = (uint8)(Config_timer0->Compare_value); // loading the compare value to OCR0 register
		TIMSK |= (1<<OCIE0);// Output compare match interrupt enable
	}


}

/*
 * Description :
 * initialize timer1 to start counting by choosing the required mode operation &
 * prescaler & initial value and the compare value if compare mode required
 */

void TIMER1_init(const CONFIG_TIMER* Config_timer1 ){
	TCNT1 = (uint8) (Config_timer1->initial_value); // Set Timer1 initial value to specific value

	/* Configure the timer control register TCCR1B
	 * 1.  WGM12                (0 -> NORMAL , 1 -> CTC)
	 * 2.  WGM10 & WGM11        always 0 (non-pwm mode)
	 * 3.  4.  CS00 & CS01 & CS02 to choose a specific prescaler to the timer frequency
	 */

	/* Configure the timer control register TCCR1A
	 * 1.  FOC0:1               always 1Non PWM mode
	 * 2.  WGM10 & WGM11        always 0 (non-pwm mode)
	 * 3.  COM1A0 & COM1B0      00
	 * 4.  COM1A1 & COM1B1      = 0
	 */
	 if (Config_timer1->mode == NORMAL) {

		TIMSK |= (1 << TOIE1); // overflow interrupt enable

	}
    else if(Config_timer1->mode == CTC_16BIT){
		OCR1A = Config_timer1->Compare_value; // loading the compare value to OCR1A register
		TIMSK |= (1 << OCIE1A);//// Output compare match interrupt enable
    	TCCR1B |= (1<<WGM12);

    }

	TCCR1A |= (1<<FOC1A);


	TCCR1B = (TCCR1B & 0xF8) | (Config_timer1->clock);

}

/*
 * Description :
 * initialize timer2 to start counting by choosing the required mode operation &
 * prescaler & initial value and the compare value if compare mode required
 */

void TIMER2_init(const CONFIG_TIMER* Config_timer2 ){

	TCNT2 = (uint8)(Config_timer2->initial_value); // Set Timer2 initial value to start counting from it
			/* Configure the timer control register
			 * 1.  FOC2=1             Non PWM mode
			 * 2.  WGM21 & WGM20      to choose the operation mode
			 * 3.  COM20 & COM21      to choose the operation mode
			 * 4.  CS20 & CS21 & CS22 to choose a specific prescaler to the timer frequency
			 */
			TCCR2 |= (1 << FOC2);

			TCCR2 = (TCCR2 & 0xF8) | (Config_timer2->clock);

		    /*
		     * COM21 & COM20 ( 00 -> normal) , (01-> compare )
		     *  WGM20 = 0 beacause always non pwm_mode
		     *  WGM21 = 1 ( Compare Mode)
		     *  WGM21 = 0 ( Normal Mode)
		     */
			TCCR2 = (TCCR2 & 0xF7) | ((Config_timer2->mode)<<3);

			if ((Config_timer2->mode) == NORMAL)
			{
				TIMSK |= (1<<TOIE2);// Overflow interrupt enable
			}
			else if ((Config_timer2->mode) == CTC_8BIT)
			{
				TCCR2 |=(1<<COM20);
				OCR2 = (uint8)(Config_timer2->Compare_value);// loading the compare value to OCR2 register
				TIMSK |= (1<<OCIE2);// Output compare match interrupt enable

			}

}

/*
 * Description: Function to set the Call Back function address.
 */
void TIMER0_setCallBack(void(*a_ptr)(void)){

	g_callBackPtrT0 = a_ptr;

}
/*
 * Description: Function to set the Call Back function address.
 */
void TIMER1_setCallBack(void(*c_ptr)(void)){

	g_callBackPtrT1 = c_ptr;

}
/*
 * Description: Function to set the Call Back function address.
 */
void TIMER2_setCallBack(void(*b_ptr)(void)){

	g_callBackPtrT2 = b_ptr;

}
/*
 * Description: Function to stop the timer counting by deactivate the clock source in Control register
 */

void TIMER0_deinit(void){

	TCCR0 = 0;
}
/*
 * Description: Function to stop the timer counting by deactivate the clock source in Control register
 */

void TIMER1_deinit(void){

	TCCR1A = 0;
	TCCR1B = 0;
}

/*
 * Description: Function to stop the timer counting by deactivate the clock source in Control register
 */

void TIMER2_deinit(void){

	TCCR2 = 0;
}
