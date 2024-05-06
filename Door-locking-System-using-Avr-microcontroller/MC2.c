/*
 ================================================================================================
 Name        : MC2.c
 * Author: Kaream adel
 Description : MC2 application code
 ================================================================================================
 */



#include"twi.h"
#include"external_eeprom.h"
#include"uart.h"
#include"dc_motor.h"
#include"buzzer.h"
#include"util/delay.h"
#include"timer.h"
#include<avr/io.h>

/*******************************************************************************
 *                                definition                                   *
 *******************************************************************************/
#define SIZE_OF_PASSWORD              5
#define FIRST_PASSWORD_READY          0x10
#define ACTIVE_OPENING_DOOR_FEATURES  0x20
#define RECIEVE_OLD_PASSWORD          0x30
#define CHANGE_PASSWORD_READY         0x40
#define WRONG_PASSWORD                0x50
#define FIRST_PASSWORSD_CONFIRMATION  0x60
#define PASSWORD_NOT_MATCH            0x70
#define PASSWORD_MATCH                0x80
#define FIRST_PASSWORD_HAS_WRITTEN    0x90
#define MC1_READY                     0x91
/*******************************************************************************
 *                         private Functions Prototypes                        *
 *******************************************************************************/


static uint8 check_twoPasswords(uint8 *password ,uint8 *re_password,uint8 size);

/*******************************************************************************
 *                          global variables                                   *
 *******************************************************************************/
uint16 g_tick = 0;// indicate that the interrupt has been happened
uint8 seconds = 0;
/*******************************************************************************
 *                          call back functions                                *
 *******************************************************************************/
/*
 * these call back functions are executed when an interrupt happen in any timer register
 * the system enter any function then increases the number of ticks until it reaches
 * a specific value  (number of interrupts per second)
 */
/*
 * NOTE: you must aware of the number of interrupts per seconds in each function
 * by setting the configuration of the timer that will hold
 */
void Timer0_func(void) {
	g_tick++;
	if (g_tick == 4000) { // number of over flows per second
		seconds++;//to return counting again
		g_tick = 0;
	}
}
void Timer1_func(void) {
	g_tick++;
	if (g_tick == 500) {
		seconds++;
		g_tick = 0;
	}
}
void Timer2_func(void) {
	g_tick++;
	if (g_tick == 3921) {
		seconds++;
		g_tick = 0;
	}
}

int main(void) {
	/*******************************************************************************
	 *                             local variables                                 *
	 *******************************************************************************/
    uint8 i;
    uint8 result = 0;// to store the checking result between the two passwords
    uint8 operation ;//  store the value returned from MC1 by get_operation function
    uint8 Flag;//  this flag to check if the EEPROM has a starting password or not
    uint16 address = 0x0311; // the address of the first byte of the password that will be written in EEPROM
    uint8 password[SIZE_OF_PASSWORD];
	uint8 re_password[SIZE_OF_PASSWORD];
	/*******************************************************************************
	 *                 configuration of init functions for the drivers              *
	 *******************************************************************************/
	SREG |= (1<<7); // enable interrupt global bit
	Config_TWI Config_TWI = { FAST, 0x01 }; // {bit_rate, slave address}
	UART_ConfigType Config_UART = { _8_BIT, DISABLE, ONE_BIT, 9600 };//{send_size,parity_mode,no_of_Stop_bits,baud_rate}
	CONFIG_TIMER Config_TIMER0 = { 250, CTC_8BIT, F_CPU_8, 0 };//{compare_value,mode_of_operation,prescaler,initial_value}
	CONFIG_TIMER Config_TIMER1 = {2000, CTC_16BIT, F_CPU_8, 0};//{compare_value,mode_of_operation,prescaler,initial_value}
	CONFIG_TIMER Config_TIMER2 = {0, NORMAL, F_CPU_8, 0 };//{compare_value,mode_of_operation,prescaler,initial_value}
	TWI_init(&Config_TWI); // configure the I2C to be ready for writing and reading from EEPROM
	UART_init(&Config_UART); // configure the UART to be ready for recieve and send
	DC_MOTOR1_init();
	BUZZER_init();
	TIMER0_setCallBack(Timer0_func);
	TIMER1_setCallBack(Timer1_func);
	TIMER2_setCallBack(Timer2_func);
	/*******************************************************************************
	 *                 application instructions                                    *
	 *******************************************************************************/
    /* if you want to start system again from begining just build these two lines and
     * then remove them and build*/
	/*EEPROM_writeByte(0x0310,0x50);
    _delay_ms(50);*/


    EEPROM_readByte(0x0310,&Flag);//the flag will read the value of a certain address in the EEPROM and send it to MC1
    _delay_ms(50);
	UART_sendByte(MC1_READY); // send the handshaking to MC1 to start receiving the flag
	UART_sendByte(Flag); // send the flag to MC1

	/* if else condition description :
	 *
	 * case ( flag does not match the value written in 0X0310 address which indicates that the password has been written):
	 * 1_ the system will receive the password and re_password from MC1 by the UART
	 * 2_ checking that the two passwords are the same and send to MC1 the result to take an action
	 * 2.1 _ if matching : the system will store the password in the EEPROM by I2C
	 * 2.2 _if not matching : the system will repeat itself until entering two exact passwords
	 *
	 * case (( flag matches the value written in 0X0310 address which indicates that the password has been written)
	 * the system will read the password from EEPROM and store it in password[] array.
	 */


    if(Flag != FIRST_PASSWORD_HAS_WRITTEN){
	while (1) {
		while (UART_recieveByte() != FIRST_PASSWORD_READY); // waiting until the user get the password from MC1
		for (i = 0; i < SIZE_OF_PASSWORD; i++) {
			password[i] = UART_recieveByte(); // Receiving the array of password digit by digit by the UART in a for loop
		}
		while (UART_recieveByte() != FIRST_PASSWORSD_CONFIRMATION)// waiting until the user get the re_password from MC1
			;
		for (i = 0; i < SIZE_OF_PASSWORD; i++) {
			re_password[i] = UART_recieveByte();// Receiving the array of password digit by digit by the UART in a for loop
		}
		result = check_twoPasswords(password, re_password, SIZE_OF_PASSWORD); // checking the two passwords and return the result
		UART_sendByte(result); // send the result to MC1
		if (result == PASSWORD_MATCH) {
			   EEPROM_writeByte(0x0310, FIRST_PASSWORD_HAS_WRITTEN); // write this byte as a flag that the password written
				_delay_ms(50);
			for (i = 0; i < SIZE_OF_PASSWORD; i++) {
			  EEPROM_writeByte(address + i, password[i]);// storing the password in the EEPROM
				_delay_ms(50);
				}
				break; // terminates the while loop
			}
		}
	}
    else { // means that the password already has been written
		for (i = 0; i < SIZE_OF_PASSWORD; i++) {
			EEPROM_readByte(address + i, password + i);
			_delay_ms(50);
		}
	}
    /*
     *  if you want to check the password that stored in the EEPROM active these four lines with 159-165 lines in MC1
     *
    UART_sendByte(RECIEVE_OLD_PASSWORD);
	for (i = 0; i < SIZE_OF_PASSWORD; i++) {
		UART_sendByte(password[i]);
		_delay_ms(50);
	}
     */

    /* Infinite loop Description:
     * wait until the MC1 send the operation and store it in MC2
     * if else condition Description:
     * case '+':
     * the system will receive the password sent from MC1 and store it in re_password array
     * check the re_Password with the password stored in the EEPROM and send the result to MC1
     * if matching the timer will start counting and the motor will do its job
     * if not the door will not open and the system will wait again another operation
     *
     * case '-':
     * the system will receive the password sent from MC1 and store it in re_password array
     * check the re_Password with the password stored in the EEPROM and send the result to MC1
     * if matching the user will ask to enter a new password to store it in EEPROM
     * if the matching proccess not matches for three times the system will suspend for 60 seconds with buzzer
     */
	while (1) {
      operation = UART_recieveByte();
      if (operation == '+'){
			for (i = 0; i < SIZE_OF_PASSWORD; i++) {
				re_password[i] = UART_recieveByte(); // Receiving the array of password digit by digit by the UART in a for loop
			}
			result = check_twoPasswords(password, re_password,SIZE_OF_PASSWORD);// checking the two passwords and return the result
			UART_sendByte(result);// send the result to MC1
			if(result == PASSWORD_MATCH ){
				/*
				 * speed = 100 -> maximum speed
				 * speed = 75 -> 3/4 maximum speed
				 * speed = 50 -> half maximum speed
				 */
				TIMER0_init(&Config_TIMER0);// configure timer 1 to start counting
				DC_MOTOR1_setDirection(CLOCK_WISE, 50);
		         while(seconds != 15);// wait until the counter reaches 15 seconds
				DC_MOTOR1_setDirection(STOP, 0);
				while(seconds != 17);// wait until the counter reaches 17 seconds
				DC_MOTOR1_setDirection(ANTI_CLOCK_WISE, 50);
				while(seconds != 32);// wait until the counter reaches 32 seconds
				DC_MOTOR1_setDirection(STOP, 0);
				while (seconds != 33);// wait until the counter reaches 33 seconds
				TIMER0_deinit();// stop the timer by deactivate the clock and the control register
				DC_MOTOR1_deinit();
				seconds = 0;// return the seconds to 0 to re-use it
			}


      }
      else if(operation == '-'){
  		for (i = 0; i < SIZE_OF_PASSWORD; i++) {
  			re_password[i] = UART_recieveByte();// Receiving the array of password digit by digit by the UART in a for loop
  		}
  		result = check_twoPasswords(password, re_password,SIZE_OF_PASSWORD);// checking the two passwords and return the result
        UART_sendByte(result);// send the result to MC1
        if(result == PASSWORD_MATCH){
        	while( UART_recieveByte()!= CHANGE_PASSWORD_READY);
				for (i = 0; i < SIZE_OF_PASSWORD; i++) {
					password[i] = UART_recieveByte();// Receiving the array of password digit by digit by the UART in a for loop to write it in EEPROM
				}
				for (i = 0; i < SIZE_OF_PASSWORD; i++) {
					EEPROM_writeByte(address + i, password[i]); // write the new password in the EEPROM
					_delay_ms(500);
				}
			}

		} else if (operation == WRONG_PASSWORD) { // if the user enter the password 3 times incorrectly
			TIMER2_init(&Config_TIMER2);// configure timer 0 to start counting
			BUZZER_start(); // start_the buzzer
            while(seconds != 60)// wait until the counter reaches 15 seconds
			BUZZER_stop(); //stop the buzzer
			TIMER2_deinit(); // stop the timer by deactivate the clock and the control register
			seconds = 0; // return the seconds to 0 to re-use it
		}
}
}

/************************************************************************************
 * Function Name: check_twoPasswords
 * Parameters (in): *Password , *re_password, size of password
 * Parameters (out): result
 * Return value: uint8
 * Description: responsible to check the two passwords by comparing them element by element
 **********************************************************************************/

static uint8 check_twoPasswords(uint8 *password ,uint8 *re_password,uint8 size){

uint8 i = 0;
uint8 result;
for ( ; i<size ;i++){

		if (password[i] != re_password[i])
		{
           result = PASSWORD_NOT_MATCH;
		}
		else if (i==4) {
			result = PASSWORD_MATCH;
		}
	}
return result;
}
