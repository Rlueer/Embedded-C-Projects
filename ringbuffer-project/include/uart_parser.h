#include <stdint.h>   // uint8_t
#include <stddef.h>   // size_t, NULL
#include <stdbool.h>  // bool
#include <stdio.h>	  //printf

#define MAX_PAYLOAD 256

typedef enum{
	STATE_WAIT_START1,
	STATE_WAIT_START2,
	STATE_WAIT_LEN,
	STATE_WAIT_PAYLOAD,
	STATE_WAIT_CHECKSUM
} uart_state_t;

typedef struct{
	uart_state_t state;
	
	bool packet_ready;                   // Paket hazır mı?
	uint8_t packet_len;                  // Hazır paketin uzunluğu
	uint8_t packet_payload[MAX_PAYLOAD]; // Hazır paketin payload kopyası
	
	uint8_t length;
	uint8_t payload[MAX_PAYLOAD];
	uint8_t payload_index;
} uart_parser_t;

void uart_parser_init(uart_parser_t* p);

bool uart_parser_process(uart_parser_t* p, uint8_t byte);

