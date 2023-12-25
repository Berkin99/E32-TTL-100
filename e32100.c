/*
 *  E32100.c
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

#include <stdbool.h>
#include <stdint.h>

#include "E32100.h"
#include "serial.h"

static UART_HandleTypeDef *E32100_huart;
static E32100_Handle_t E32100_handle;

static uint8_t buffer [E32100_MAX_BUFFER_LEN];

void E32100_Command(E32100_Command_e cmd);

void E32100_Init(UART_HandleTypeDef *huart, GPIO_TypeDef * M0_t ,uint16_t M0, GPIO_TypeDef * M1_t, uint16_t M1, GPIO_TypeDef * AUX_t, uint16_t AUX){

	E32100_huart = huart;

	E32100_handle.M0_GPIOx = M0_t;
	E32100_handle.M0_Pin = M0;

	E32100_handle.M1_GPIOx = M1_t;
	E32100_handle.M1_Pin = M1;

	E32100_handle.AUX_GPIOx = AUX_t;
	E32100_handle.AUX_Pin = AUX;

	E32100_SetMode(E32100_MODE_NORMAL);
	E32100_handle.mode = E32100_MODE_NORMAL;
}

void E32100_SetMode(E32100_Mode_e mode){

	uint8_t _m0 = 0;
	uint8_t _m1 = 0;

	if(((uint8_t)mode & (1U<<0))>0) _m0 = 1;
	if(((uint8_t)mode & (1U<<1))>0) _m1 = 1;

	HAL_GPIO_WritePin(E32100_handle.M0_GPIOx, E32100_handle.M0_Pin, _m0);
	HAL_GPIO_WritePin(E32100_handle.M1_GPIOx, E32100_handle.M1_Pin, _m1);

	E32100_handle.mode = mode;

	HAL_Delay(E32100_MODE_CHANGE_INTERVAL);
}

void E32100_SetConfig(E32100_Config_t config, bool save){

	if(E32100_handle.mode!=E32100_MODE_SLEEP) E32100_SetMode(E32100_MODE_SLEEP);

	uint8_t param[6];

	if(save) param[0] = E32100_CMD_SAVE_ON;
	else param[0] = E32100_CMD_SAVE_OFF;

	param[1] = config.addh;
	param[2] = config.addl;
	param[3] = E32100_SpedByte(config.sped);
	param[4] = E32100_ChannelByte(config.channel);
	param[5] = E32100_OptionByte(config.option);

	HAL_UART_Transmit(E32100_huart, param, 6, 100);
}

void E32100_SetDefaultConfig(void){
	E32100_Config_t conf;

	conf.addh = E32100_DEFAULT_ADDH;
	conf.addh = E32100_DEFAULT_ADDL;
	conf.channel = E32100_DEFAULT_CHANNEL;

	conf.sped.airDataRate = E32100_ADR_2400;
	conf.sped.baudRate = E32100_BAUD_RATE_9600;
	conf.sped.parity = E32100_PARITY_8N1;

	conf.option.fec = E32100_FEC_ON;
	conf.option.ioMode = E32100_IOMODE_ON;
	conf.option.txMode = E32100_TXMODE_TRANSPARENT;
	conf.option.wakeUpTime = E32100_WAKEUP_250;
	conf.option.txPower = E32100_TXPOWER_20;

	E32100_SetConfig(conf, false);

	E32100_SetMode(E32100_MODE_NORMAL);
}

void E32100_Command(E32100_Command_e cmd){
	if(E32100_handle.mode!=E32100_MODE_SLEEP) E32100_SetMode(E32100_MODE_SLEEP);
	uint8_t param[3] = {cmd,cmd,cmd};
	HAL_UART_Transmit(E32100_huart, param, 3, 100);
	HAL_Delay(E32100_COMMAND_INTERVAL);
}

void E32100_Reset(void){
	E32100_Command(E32100_CMD_RESET);
	E32100_SetMode(E32100_MODE_NORMAL);
}

void E32100_GetConfig(uint8_t * buffer){
	E32100_Command(E32100_CMD_READ_CFG);
	HAL_UART_Receive(E32100_huart, buffer, 6, 100);
	E32100_SetMode(E32100_MODE_NORMAL);
}

void E32100_GetModuleVersion(uint8_t * buffer){
	E32100_Command(E32100_CMD_MODULE);
	HAL_UART_Receive(E32100_huart, buffer, 6, 100);
	E32100_SetMode(E32100_MODE_NORMAL);
}

void E32100_Transmit(const uint8_t* payload, uint8_t size){
	if(E32100_handle.mode!=E32100_MODE_NORMAL)E32100_SetMode(E32100_MODE_NORMAL);
	HAL_UART_Transmit(E32100_huart, payload, size, 100);
}

void E32100_Receive(void){
	HAL_UARTEx_ReceiveToIdle_DMA(E32100_huart, buffer, E32100_MAX_BUFFER_LEN);
}

uint8_t * E32100_GetBuffer(void){
	return buffer;
}

uint8_t E32100_SpedByte (E32100_Sped_t sped){
	uint8_t temp=((uint8_t)(sped.airDataRate)<<E32100_OFFSET_AIRDATARATE);
	temp|=((uint8_t)(sped.baudRate)<<E32100_OFFSET_BAUDRATE);
	temp|=((uint8_t)(sped.parity)<<E32100_OFFSET_PARITY);
	return temp;
}

uint8_t E32100_OptionByte (E32100_Option_t option){
	uint8_t temp=((uint8_t)(option.txPower)<<E32100_OFFSET_TXPOWER);
	temp|=((uint8_t)(option.fec)<<E32100_OFFSET_FEC);
	temp|=((uint8_t)(option.wakeUpTime)<<E32100_OFFSET_WAKEUP);
	temp|=((uint8_t)(option.ioMode)<<E32100_OFFSET_IOMODE);
	temp|=((uint8_t)(option.txMode)<<E32100_OFFSET_TXMODE);
	return temp;
}

uint8_t E32100_ChannelByte (uint16_t channel){
	return (uint8_t)(channel-410);
}

