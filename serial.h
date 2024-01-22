#ifndef SERIAL_H_
#define SERIAL_H_

#include "main.h"

#define USART_PORT_SIZE 	8

#define USART1_ID  			0b00000001
#define USART2_ID  			0b00000010
#define USART3_ID  			0b00000100
#define USART4_ID  			0b00001000
#define USART5_ID  			0b00010000
#define USART6_ID  			0b00100000
#define USART7_ID  			0b01000000
#define USART8_ID  			0b10000000

typedef enum {
    USART1_INDEX,
	USART2_INDEX,
	USART3_INDEX,
	USART4_INDEX,
	USART5_INDEX,
	USART6_INDEX,
	USART7_INDEX,
	USART8_INDEX
} usartIndex_e;


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

static const uint32_t baudRates[] = {
	9600,
	19200,
	38400,
	57600,
	115200,
	230400,
	250000,
	400000,
	460800,
	500000,
	921600,
	1000000,
	1500000,
	2000000,
	2470000
};

#define BAUD_RATE_COUNT 14

usartIndex_e serialPortID(UART_HandleTypeDef* huart);
uint8_t serialPortIndex(UART_HandleTypeDef* huart);
uint8_t	serialPortSize(void);

void setPortBaudRate(UART_HandleTypeDef* huart, baudRate_e index);

#endif /* SERIAL_H_ */
