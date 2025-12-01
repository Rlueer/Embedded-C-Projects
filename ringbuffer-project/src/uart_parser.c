#include "uart_parser.h"


void uart_parser_init(uart_parser_t* p){
	p->state = STATE_WAIT_START1;
	p->payload_index=0;
	p->length=0;
	p->packet_ready=false;
	p->packet_len=0;
	
}
/*reset_parser sadece parser'ın state_machine alanlarını sıfırlar.*/
void reset_parser(uart_parser_t* p){
	p->state = STATE_WAIT_START1;
	p->payload_index=0;
	p->length=0;
	
}
//AA 55 03   10 20 30 00
//S1 S2 Size Payloadd checksum
 
bool uart_parser_process(uart_parser_t* p, uint8_t byte){
	uint8_t check;
	bool check_ok;
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
			if(byte > MAX_PAYLOAD){
				reset_parser(p);
				return false;
			}
			p->payload_index = 0;
			p->length=byte;
			p->state=STATE_WAIT_PAYLOAD;
			return false;
		break;
		
		case STATE_WAIT_PAYLOAD:
			if (p->payload_index >= p->length){
				reset_parser(p);
				return false;
			}
			p->payload[p->payload_index] = byte;
			p->payload_index++;
			
			if(p->payload_index == p->length)
				p->state=STATE_WAIT_CHECKSUM;
			else
				p->state=STATE_WAIT_PAYLOAD;
			return false;
		break;
		
		case STATE_WAIT_CHECKSUM:
			check=0;
			check_ok=false;
			p->packet_ready=false;
			for(int i=0; i < p->length;i++)
				check ^= p->payload[i];
			
			if(check==byte){
				p->packet_ready=true;			//length ve arrayi kopyalıyoruz
				p->packet_len=p->length;		//çünkü resette kaybolmasın
				for(int i = 0; i < p->length; i++)
					p->packet_payload[i] = p->payload[i];	// payload kopyası
				check_ok=true;
			}
			reset_parser(p);	

			return check_ok;
		break;
		default :
			printf("error somehow\n");
			return false;
		
	}
	

}
