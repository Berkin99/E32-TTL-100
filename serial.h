/*
 * serial.h
 *
 *  Created on: Dec 9, 2023
 *      Author: User
 */

#ifndef INC_SERIAL_H_
#define INC_SERIAL_H_

#include "stm32f4xx_hal.h"

#ifdef USE_UART1
    SERIAL_PORT_USART1,
#endif
#ifdef USE_UART2
    SERIAL_PORT_USART2,
#endif
#ifdef USE_UART3
    SERIAL_PORT_USART3,
#endif
#ifdef USE_UART4
    SERIAL_PORT_UART4,
#endif
#ifdef USE_UART5
    SERIAL_PORT_UART5,
#endif
#ifdef USE_UART6
    SERIAL_PORT_USART6,
#endif
#ifdef USE_UART7
    SERIAL_PORT_USART7,
#endif
#ifdef USE_UART8
    SERIAL_PORT_USART8,
#endif

typedef enum {
	BAUD_9600,
	BAUD_19200,
	BAUD_38400,
	BAUD_57600,
	BAUD_115200,
	BAUD_230400,
	BAUD_250000,
	BAUD_400000,
	BAUD_460800,
	BAUD_500000,
	BAUD_921600,
	BAUD_1000000,
	BAUD_1500000,
	BAUD_2000000,
	BAUD_2470000,
	BAUD_COUNT
} baudRate_e;

static const uint32_t baudRates[] = {9600, 19200, 38400, 57600, 115200, 230400, 250000,
		400000, 460800, 500000, 921600, 1000000, 1500000, 2000000, 2470000};

#define BAUD_RATE_COUNT 14


//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);

//void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size);


//port idnentifier
//general serialWrite for port
//general serialRead for port


#endif /* INC_SERIAL_H_ */
