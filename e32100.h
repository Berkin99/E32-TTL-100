/*
 *  E32100.h
 *
 *  Created on: Dec 22, 2023
 *      Author: BerkN
 *
 *  E-Byte E32-TTL-100 Transceiver Module Driver
 *  E32-TTL-100 Module is based on Semtech SX1278 chip.
 *  UART communication.
 *  DMA Receive.
 *  Changeable all possible settings.
 *
 *  22.12.2023 : Created E32-TTL-100 module driver.
 *
 *  References:
 *  [0] e32-ttl-100-datasheet-en-v1-0.pdf
 *  [1] semtech-sx127x-series-datasheet.pdf
 *
 */

#ifndef INC_E32100_H_
#define INC_E32100_H_

#include <stdbool.h>
#include <stdint.h>

#include "serial.h"

#define E32100_MAX_BUFFER_LEN 			58
#define E32100_MODE_CHANGE_INTERVAL 	2
#define E32100_COMMAND_INTERVAL			5

#define E32100_DEFAULT_CHANNEL		 	433
#define E32100_DEFAULT_ADDH				0x00
#define E32100_DEFAULT_ADDL				0x00

#define E32100_DEFAULT_AIRDATARATE		0x05
#define E32100_DEFAULT_BAUDRATE			0x03
#define E32100_DEFAULT_PARITY			0x00

#define E32100_DEFAULT_TXPOWER			0x00
#define E32100_DEFAULT_FEC				0x01
#define E32100_DEFAULT_WAKEUP			0x00
#define E32100_DEFAULT_IOMODE			0x01
#define E32100_DEFAULT_TXMODE			0x00

#define E32100_OFFSET_AIRDATARATE		0
#define E32100_OFFSET_BAUDRATE			3
#define E32100_OFFSET_PARITY			6

#define E32100_OFFSET_TXPOWER			0
#define E32100_OFFSET_FEC				2
#define E32100_OFFSET_WAKEUP			3
#define E32100_OFFSET_IOMODE			6
#define E32100_OFFSET_TXMODE			7

typedef enum {
	E32100_MODE_NORMAL = 0,
	E32100_MODE_WAKEUP,
	E32100_MODE_POWERSAVE,
	E32100_MODE_SLEEP
}E32100_Mode_e;

/*  In sleep mode（mode 3：M1=1, M0=1）, it supports below instructions on list.
 *	Only support 9600 and 8N1 format when set
 */
typedef enum {
	E32100_CMD_SAVE_ON  	= 0xC0,
	E32100_CMD_READ_CFG 	= 0xC1,
	E32100_CMD_SAVE_OFF		= 0xC2,
	E32100_CMD_MODULE   	= 0xC3,
	E32100_CMD_RESET     	= 0xC4
}E32100_Command_e;

typedef enum {
	E32100_ADR_300 = 0,
	E32100_ADR_1200,
	E32100_ADR_2400,
	E32100_ADR_4800,
	E32100_ADR_9600,
	E32100_ADR_19200
}E32100_AirDataRate_e;

typedef enum {
	E32100_BAUD_RATE_1200 = 0,
	E32100_BAUD_RATE_2400,
	E32100_BAUD_RATE_4800,
	E32100_BAUD_RATE_9600,
	E32100_BAUD_RATE_19200,
	E32100_BAUD_RATE_38400,
	E32100_BAUD_RATE_57600,
	E32100_BAUD_RATE_115200
}E32100_BaudRate_e;

typedef enum {
	E32100_PARITY_8N1 = 0,
	E32100_PARITY_8O1,
	E32100_PARITY_8E1
}E32100_UartParity_e;

typedef enum {
	E32100_TXPOWER_20 = 0,
	E32100_TXPOWER_17,
	E32100_TXPOWER_14,
	E32100_TXPOWER_10
}E32100_TxPower_e;

typedef enum {
	E32100_FEC_OFF = 0,
	E32100_FEC_ON
}E32100_Fec_e;

typedef enum {
	E32100_WAKEUP_250 = 0,
	E32100_WAKEUP_500,
	E32100_WAKEUP_750,
	E32100_WAKEUP_1000,
	E32100_WAKEUP_1250,
	E32100_WAKEUP_1500,
	E32100_WAKEUP_1750,
	E32100_WAKEUP_2000
}E32100_WakeUpTime_e;

typedef enum {
	E32100_IOMODE_OFF = 0,
	E32100_IOMODE_ON
}E32100_IOMode_e;

typedef enum {
	E32100_TXMODE_TRANSPARENT = 0,
	E32100_TXMODE_FIXED
}E32100_TXMode_e;

typedef struct E32100_Sped_s{
	E32100_AirDataRate_e airDataRate;
	E32100_BaudRate_e baudRate;
	E32100_UartParity_e parity;
}E32100_Sped_t;

typedef struct E32100_Option_s{
	E32100_TxPower_e txPower;
	E32100_Fec_e fec;
	E32100_WakeUpTime_e wakeUpTime;
	E32100_IOMode_e ioMode;
	E32100_TXMode_e txMode;
}E32100_Option_t;

typedef struct E32100_Handle_s{
	GPIO_TypeDef * M0_GPIOx;
	uint16_t M0_Pin;

	GPIO_TypeDef * M1_GPIOx;
	uint16_t M1_Pin;

	GPIO_TypeDef * AUX_GPIOx;
	uint16_t AUX_Pin;

	E32100_Mode_e mode;

}E32100_Handle_t;

typedef struct E32100_Config_s{
	uint8_t 		addh;
	uint8_t 		addl;
	E32100_Sped_t  	sped;
	uint16_t  		channel;
	E32100_Option_t option;
}E32100_Config_t;

void E32100_Init(UART_HandleTypeDef *huart, GPIO_TypeDef * M0_t ,uint16_t M0, GPIO_TypeDef * M1_t, uint16_t M1, GPIO_TypeDef * AUX_t, uint16_t AUX);

void E32100_SetMode (E32100_Mode_e mode);
void E32100_SetConfig(E32100_Config_t config, bool save);
void E32100_SetDefaultConfig(void);

void E32100_Reset(void);
void E32100_GetConfig(uint8_t * buffer);
void E32100_GetModuleVersion(uint8_t * buffer);

void E32100_Transmit(const uint8_t* payload, uint8_t size);
void E32100_Receive(void);

uint8_t * E32100_GetBuffer(void);

uint8_t E32100_SpedByte 	(E32100_Sped_t sped);
uint8_t E32100_OptionByte 	(E32100_Option_t option);
uint8_t E32100_ChannelByte 	(uint16_t channel);

#endif /* INC_E32100_H_ */
