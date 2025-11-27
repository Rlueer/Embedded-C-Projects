#include <stdint.h>   // uint8_t
#include <stddef.h>   // size_t, NULL
#include <stdbool.h>  // bool
#include <stdio.h>	  //printf

typedef enum{
	STATE_WAIT_START1,
	STATE_WAIT_START2,
	STATE_WAIT_LEN,
	STATE_WAIT_PAYLOAD,
	STATE_WAIT_CHECKSUM
} uart_state_t;

typedef struct{
	uart_state_t state;
	uint8_t length;
	uint8_t payload[256];
	uint8_t payload_index;
} uart_parser_t;

void uart_parser_init(uart_parser_t* p);

bool uart_parser_process(uart_parser_t* p, uint8_t byte);

