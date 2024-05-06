 /******************************************************************************
 *
 * Module: TWI(I2C)
 *
 * File Name: twi.h
 *
 * Description: Header file for the TWI(I2C) AVR driver
 *
 * Author: Kaream adel
 *
 *******************************************************************************/ 

#ifndef TWI_H_
#define TWI_H_

#include "std_types.h"

/*******************************************************************************
 *                               Definition                                    *
 *******************************************************************************/

/* I2C Status Bits in the TWSR Register */
#define TWI_START         0x08 /* start has been sent */
#define TWI_REP_START     0x10 /* repeated start */
#define TWI_MT_SLA_W_ACK  0x18 /* Master transmit ( slave address + Write request ) to slave + ACK received from slave. */
#define TWI_MT_SLA_W_NACK 0x20 /* Master transmit ( slave address + Write request ) to slave + NACK received from slave. */
#define TWI_MT_SLA_R_ACK  0x40 /* Master transmit ( slave address + Read request ) to slave + ACK received from slave. */
#define TWI_MT_DATA_ACK   0x28 /* Master transmit data and ACK has been received from Slave. */
#define TWI_MR_DATA_ACK   0x50 /* Master received data and send ACK to slave. */
#define TWI_MR_DATA_NACK  0x58 /* Master received data but doesn't send ACK to slave. */
#define NORMAL_MODE_8MHZ  0x95 /*for operating the SCL frequency 100 Kbps*/
#define FAST_MODE_8MHZ    0x02 /*for operating the SCL frequency 400 Kbps*/

/*******************************************************************************
 *                               type definitions                              *
 *******************************************************************************/

typedef enum{

	NORMAL_=100000,FAST=400000

}SCL_frequency;
typedef struct{
	SCL_frequency freq;
	uint8 Slave_adress;
	/*
	* note that this adress will be shifted <<1 because the adress is 7_bits
	*/
}Config_TWI;




/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
void TWI_init(const Config_TWI* Config_ptr);
void TWI_start(void);
void TWI_stop(void);
void TWI_writeByte(uint8 data);
uint8 TWI_readByteWithACK(void);
uint8 TWI_readByteWithNACK(void);
uint8 TWI_getStatus(void);


#endif /* TWI_H_ */
