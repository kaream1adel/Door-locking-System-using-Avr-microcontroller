 /******************************************************************************
 *
 * Module: pwm
 *
 * File Name: pwm.c
 *
 * Description: source file for the AVR pwm driver
 *
 * Author: Kaream adel
 *
 *******************************************************************************/


#include <avr/io.h>
#include"gpio.h"
#include"pwm.h"


 /*******************************************************************************
 *                              Functions definitions                           *
 *******************************************************************************/
/*
 * Description :
 * initialize timer0 to generate PWM signal in OC0 pin as output signal.
 * it takes the initial value & required clock & mode of operation and
 * the compare value which will store in OCR0 REG
  */

void PWM_timer0init(const CONFIG_PWM *Config_PWM0){
	TCNT0 = (uint8) (Config_PWM0->initial_value); /* loading the initial value to start counting from it */
	OCR0 = (uint8) (Config_PWM0->compare_value);/* loading the compare value in OCR0 register*/
	GPIO_setupPinDirection(PORTB_ID, PIN3_ID, PIN_OUTPUT); /* setup the direction of OC0 pin as output */

	/* Configure timer controller register
	 * FOC0 = 0         Fast PWM mode
	 * WGM00 , WGM01    (11) for fast PWM mode
	 * COM0:1           choose the PWM mode (inverting - non_inverting).
	 * CS00:02           choose the suitable prescaler for the frequency.
	 */

	TCCR0 |= (1<<WGM00)|(1<<WGM01);
	TCCR0 = (TCCR0 & 0xF8) | (Config_PWM0->prescaler);
	TCCR0 = (TCCR0 & 0xCF) | ((Config_PWM0->mode)<<4);
}

/*
 * Description :
 * initialize timer1 to start counting by choosing the required mode operation &
 * prescaler & initial value and the compare value if compare mode required
 * this function only operates at 10 bits fast pwm mode (mode 7) and
 * it is only deal with channel A
 */

void PWM_timer1init(const CONFIG_PWM *Config_PWM1) {

	TCNT1 = Config_PWM1->initial_value;; /* loading the initial value to start counting from it */
    OCR1A = Config_PWM1->compare_value;
    GPIO_setupPinDirection(PORTD_ID, PIN5_ID, PIN_OUTPUT); /* setup the direction of OC1A pin as output */
	/* Configure timer controller register TCCR1A
	 * FOC1A = 0                Fast PWM mode
	 * WGM11 , WGM10           (11) for fast PWM mode (10 bit)
	 * COM1A0, COM1A1          choose the PWM mode (inverting - non_inverting).
	 */
	TCCR1A |= (1 << WGM11) | (1 << WGM10);
	if ((Config_PWM1->mode) == NON_INVERTING) {
		TCCR1A |= (1 << COM1A1) ;
	} else if((Config_PWM1->mode) == INVERTING){
		TCCR1A |= (1 << COM1A1) |(1<<COM1A0);
	}

	/* Configure timer controller register TCCR1B
	 * WGM13 , WGM12           (01) for fast PWM mode (10 bit)
	 * CS10:12                  choose the suitable prescaler for the frequency.
	 */

	TCCR1B |=(1<<WGM12);
	TCCR1B = (TCCR1B & 0xF8) | (Config_PWM1->prescaler);

}


/*
 * Description :
 * initialize timer2 to generate PWM signal in OC2 pin as output signal.
 * it takes the initial value & required clock & mode of operation and
 * the compare value which will store in OCR2 REG
  */

void PWM_timer2init(const CONFIG_PWM* Config_PWM2 ){
	TCNT2 = (uint8) (Config_PWM2->initial_value); /* loading the initial value to start counting from it */
	OCR2 = (uint8) (Config_PWM2->compare_value);/* loading the compare value in OCR0 register*/
	GPIO_setupPinDirection(PORTD_ID, PIN7_ID, PIN_OUTPUT); /* setup the direction of OC2 pin as output */

	/* Configure timer controller register
	 * FOC2 = 0         Fast PWM mode
	 * WGM20 , WGM21    (11) for fast PWM mode
	 * COM20:21           choose the PWM mode (inverting - non_inverting).
	 * CS20:22           choose the suitable prescaler for the frequency.
	 */

	TCCR2 |= (1<<WGM20)|(1<<WGM21);
	TCCR2 = (TCCR0 & 0xF8) | (Config_PWM2->prescaler);
	TCCR2 = (TCCR0 & 0xCF) | ((Config_PWM2->mode)<<4);


}


/*
 * Description :Function to stop the timer counting by deactivate the clock source in Control register
 */
void PWM_timer0DeInit(void){

TCCR0 = 0;

}
/*
 * Description :Function to stop the timer counting by deactivate the clock source in Control register
 */
void PWM_timer1DeInit(void){

TCCR1A = 0;
TCCR1B = 0;

}

/*
 * Description :Function to stop the timer counting by deactivate the clock source in Control register
 */

void PWM_timer2DeInit(void){

TCCR2 =0;


}
