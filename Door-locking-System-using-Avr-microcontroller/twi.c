/******************************************************************************
 *
 * Module: TWI(I2C)
 *
 * File Name: twi.h
 *
 * Description: Source file for the TWI(I2C) AVR driver
 *
 * Author: Kaream adel
 *
 *******************************************************************************/

#include "twi.h"
#include "common_macros.h"
#include <avr/io.h>
void TWI_init(const Config_TWI* Config_ptr) {
	/*
	 * this equation will be applied to determine the value of TWBR
	 * it depends on the CPU_frequency and the SCL frequency that is required
	 * SCL frequency = (CPU CLOCK frequency/(16 + 2*(TWBR) * POW(4,TWPS))
	 *  and the term POW(4,TWPS) = 1 to minimize the equation
	 */
	/* NOTE:
	 * before entering the structure argument we must check the values in the equation
	 * because negative numbers can not be accessed in this case to avoid any
	 * problems during run time
	 */
	TWBR = ((F_CPU / Config_ptr->freq) - 16) / 2;
	TWSR = 0x00;
	/* Two Wire Bus address my address if any master device want to call me &
	 General Call Recognition: Off */
	TWAR = (Config_ptr->Slave_adress) << 1; /* my address = slave address  and the shift operation to turn off General Call Recognition*/

	TWCR = (1 << TWEN); /* enable TWI */
}

void TWI_start(void) {
	/*
	 * Clear the TWINT flag before sending the start bit TWINT=1
	 * send the start bit by TWSTA=1
	 * Enable TWI Module TWEN=1 
	 */
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);

	/* Wait for TWINT flag set in TWCR Register (start bit is send successfully) */
	while (BIT_IS_CLEAR(TWCR, TWINT))
		;
}

void TWI_stop(void) {
	/*
	 * Clear the TWINT flag before sending the stop bit TWINT=1
	 * send the stop bit by TWSTO=1
	 * Enable TWI Module TWEN=1 
	 */
	TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
}

void TWI_writeByte(uint8 data) {
	/* Put data On TWI data Register */
	TWDR = data;
	/*
	 * Clear the TWINT flag before sending the data TWINT=1
	 * Enable TWI Module TWEN=1 
	 */
	TWCR = (1 << TWINT) | (1 << TWEN);
	/* Wait for TWINT flag set in TWCR Register(data is send successfully) */
	while (BIT_IS_CLEAR(TWCR, TWINT));

}

uint8 TWI_readByteWithACK(void) {
	/* 
	 * Clear the TWINT flag before reading the data TWINT=1
	 * Enable sending ACK after reading or receiving data TWEA=1
	 * Enable TWI Module TWEN=1 
	 */
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
	/* Wait for TWINT flag set in TWCR Register (data received successfully) */
	while (BIT_IS_CLEAR(TWCR, TWINT))
		;
	/* Read Data */
	return TWDR;
}

uint8 TWI_readByteWithNACK(void) {
	/* 
	 * Clear the TWINT flag before reading the data TWINT=1
	 * Enable TWI Module TWEN=1 
	 */
	TWCR = (1 << TWINT) | (1 << TWEN);
	/* Wait for TWINT flag set in TWCR Register (data received successfully) */
	while (BIT_IS_CLEAR(TWCR, TWINT))
		;
	/* Read Data */
	return TWDR;
}

uint8 TWI_getStatus(void) {
	uint8 status;
	/* masking to eliminate first 3 bits and get the last 5 bits (status bits) */
	status = TWSR & 0xF8;
	return status;
}
