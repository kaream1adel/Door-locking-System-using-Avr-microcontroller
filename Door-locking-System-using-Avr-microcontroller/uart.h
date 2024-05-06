 /******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.h
 *
 * Description: Header file for the UART AVR driver
 *
 * Author: Kaream adel
 *
 *******************************************************************************/

#ifndef UART_H_
#define UART_H_

#include "std_types.h"
/*******************************************************************************
 *                            definitions                    *
 *******************************************************************************/
#define DOUBLE_TRANSMISION_SPEED
#define UART_POLLING_METHOD



/*******************************************************************************
 *                            Types Declaration                    *
 *******************************************************************************/
typedef enum {
_5_BIT,_6_BIT,_7_BIT,_8_BIT
} character_size;


typedef enum {
DISABLE,RESERVED,EVEN_PARITY,ODD_PARITY
}parity_mode;


typedef enum {
ONE_BIT,TWO_BIT
}stop_bit_number;
typedef struct{
	character_size size;
	parity_mode  mode;
	stop_bit_number  no_bit;
    uint32 baud_rate;
}UART_ConfigType;


/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Functional responsible for Initialize the UART device by:
 * 1. Setup the Frame format like number of data bits, parity bit type and number of stop bits.
 * 2. Enable the UART.
 * 3. Setup the UART baud rate.
 */
void UART_init(const UART_ConfigType * Config_Ptr);

/*
 * Description :
 * Functional responsible for send byte to another UART device.
 */
void UART_sendByte(const uint8 data);

/*
 * Description :
 * Functional responsible for receive byte from another UART device.
 */
uint8 UART_recieveByte(void);

/*
 * Description :
 * Send the required string through UART to the other UART device.
 */
void UART_sendString(const uint8 *Str);

/*
 * Description :
 * Receive the required string until the '#' symbol through UART from the other UART device.
 */
void UART_receiveString(uint8 *Str); // Receive until #

#endif /* UART_H_ */
