#include "uart_parser.h"


void uart_parser_init(uart_parser_t* p){
	p->state = STATE_WAIT_START1;
	p->payload_index=0;
	p->length=0;
	
}

//AA 55 03   10 20 30 00
//S1 S2 Size Payloadd checksum
 
bool uart_parser_process(uart_parser_t* p, uint8_t byte){
	
	switch (p->state){
		case STATE_WAIT_START1:
			if(byte == 0XAA)
				p->state=STATE_WAIT_START2;
			else 
				p->state=STATE_WAIT_START1;
			return false;
		break;
		case STATE_WAIT_START2:
			if(byte == 0X55)
				p->state=STATE_WAIT_LEN;
			else 
				p->state=STATE_WAIT_START1;
			return false;
		break;
		case STATE_WAIT_LEN:
			p->length=byte;
			p->state=STATE_WAIT_PAYLOAD;
			return false;
		break;
		case STATE_WAIT_PAYLOAD:
			p->payload[p->payload_index] = byte;
			p->payload_index=p->payload_index+1;
			if(p->payload_index == p->length)
				p->state=STATE_WAIT_CHECKSUM;
			else
				p->state=STATE_WAIT_PAYLOAD;
			return false;
		break;
		case STATE_WAIT_CHECKSUM:
			uint8_t check=0;
			for(int i=0; i < p->length;i++)
				check ^= p->payload[i];
			
			p->state=STATE_WAIT_START1;
			p->payload_index=0;
			p->length=0;
			if(check==byte)
				return true;
			return false;
		break;
		default :
			printf("error somehow\n");
			return false;
		
	}
	

}
