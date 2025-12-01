#include "ring_buffer.h"
#include "uart_parser.h"
#include <stdio.h>

size_t incoming_byte_count;
size_t noise_byte_count;
size_t valid_packet_count;
size_t checksum_fail_count;
size_t length_fail_count;
size_t drift_reset_count; //(payload_index >= length resetlerini say)


bool test_stream(){
	/*random data + valid packet + random data + valid packet oluşturup
	bunları simulate_uart_rx() ile ring buffer’a gönder
	sonra process_main_loop()’u çok defa çalıştır (ör. döngü içinde)*
	
	test_stream() fonksiyonunda şöyle bir akış kur:
	1) 50 byte noise gönder
	sim_uart_rx(noise[i])
	2) Bir valid paket gönder
	sim_uart_rx(packet_bytes[j])
	3) 30 byte noise gönder
	4) 100 byte noise gönder
	5) Aynı işlemi 50 kez tekrarla
	Her döngüde process_main_loop() da çalışacak: */
	return false;
}

bool build_packet(uint8_t payload_bytes[], uint8_t len, uint8_t out_buffer[]){
	
	/* Yapması gereken:
	AA
	55
	length
	payload byte’ları
	checksum = XOR(payload[])hepsini sırayla out_buffer’a yaz
	Bu fonksiyon hiçbir zaman ring buffer’a yazmaz, sadece array oluşturur.
	Amacımız:
	test_stream() içinde kolayca “valid paket” oluşturabilmek.*/
	return false;
}

bool generate_noise(uint8_t* buffer, int count){
	/* count adet random byte yaz
	fakat şunları özellikle üret:
	%10 ihtimal 0xAA
	%10 ihtimal 0x55
	%5 ihtimal 0x00
	%5 ihtimal 0xFF
	Çünkü gerçek hayatta “start sequencelerine benzeyen çöp” parser’ı en çok zorlayan şeydir.*/
	return false;
}

bool simulate_uart_rx(ring_buffer_t* rb, uint8_t byte){
	/*Ring buffer’a rb_write() çağıracak
	UART ISR’in birebir simülasyonu olacak
	Tek byte alır, başka iş yapmaz*/
	if(rb_write(rb,byte)){
		printf("rbwritesimulate\n");
		incoming_byte_count++;
		return true;
	}
	else{
		printf("DROP %d byte\n",byte);
	}
	return false;
	
}


bool process_main_loop(ring_buffer_t* rb, uart_parser_t* up ,uint8_t* data_out){
	/*ring buffer boş değilse 1 byte okur
	bu byte’ı parser_process() içine atar
	parser true dönerse “valid_packet_count++”
	bu fonksiyon main döngüsü gibi sık sık çağrılacak*/
	
	if(rb_is_empty(rb) == true){
		return false;
	}
	
	rb_read(rb,data_out);
	uart_parser_process(up,*data_out);
	
	if(up->packet_ready==true){

		printf("Packet received! Length=%d\n", up->packet_len);

		for (int i = 0; i < up->packet_len; i++)
			printf("Packet_payload[%d] is %d\n",i,up->packet_payload[i]);

		up->packet_ready = false;   // tüketildi

		valid_packet_count++;
		return true;
	}
	return false;
	
}


int main(){
		
	ring_buffer_t rb;
	uart_parser_t up;
	uint8_t data_out;
	uint8_t storage[32];
	
	
	rb_init(&rb,storage,32,false);
	uart_parser_init(&up);
	
	simulate_uart_rx(&rb, 0xAA);
	simulate_uart_rx(&rb, 0x55);
	simulate_uart_rx(&rb, 0x03);
	simulate_uart_rx(&rb, 0x10);
	simulate_uart_rx(&rb, 0x20);
	simulate_uart_rx(&rb, 0x30);
	simulate_uart_rx(&rb, 0x00);
	
	process_main_loop(&rb,&up,&data_out);
	
	simulate_uart_rx(&rb, 0x00);
	simulate_uart_rx(&rb, 0xAA);
	simulate_uart_rx(&rb, 0x55);
	simulate_uart_rx(&rb, 0x03);
	simulate_uart_rx(&rb, 0x11);
	simulate_uart_rx(&rb, 0x21);
	simulate_uart_rx(&rb, 0x31);
	simulate_uart_rx(&rb, 0x00);
	
	while(1){
		process_main_loop(&rb,&up,&data_out);
	}
	return 1;	
}


/*
int main(){
	
	ring_buffer_t rb_test;
	uint8_t a=10,b=20,c=30;
	uint8_t storage[4];
	uint8_t x;
	
	rb_init(&rb_test,storage,4,false);
	rb_write(&rb_test,a);
	rb_write(&rb_test,b);
	rb_write(&rb_test,c);
	
	for(int i =0;i<rb_test.capacity;i++){
		rb_read(&rb_test,&x);
		printf("x is %d\n",x);
	}
	printf("-----------------------------\n");
	
	ring_buffer_t rb_test2;
	uint8_t a1=10,b1=20,c1=30,d1=40;
	uint8_t storage2[4];
	
	rb_init(&rb_test2,storage2,4,true);
	rb_write(&rb_test2,a1);
	rb_write(&rb_test2,b1);
	rb_write(&rb_test2,c1);
	rb_write(&rb_test2,d1);
	if (!rb_write(&rb_test2, d1))
		printf("11111WRITE BLOCKED OK\n");
	else
		printf("overwrite is true \n");
	if (!rb_write(&rb_test2, 100))
		printf("2222WRITE BLOCKED OK\n");
	else
		printf("overwrite is true \n");
	
	if (rb_is_full(&rb_test2))  // pointer!!! & işareti!!!
		printf("FULL OK\n");
	else
		printf("FULL FAIL\n");
	
	for(int i =0;i<rb_test2.capacity-1;i++){
		if(true==rb_read(&rb_test2,&x)){
			printf("x is %d\n",x);
		}
		else{
			printf("couldnt readt value\n");
		}
	}
	rb_write(&rb_test2,31);
	if(true==rb_read(&rb_test2,&x)){
		printf("x is %d\n",x);
	}
	else{
		printf("couldnt readt value\n");
	}

	printf("-----------------------------\n");
	
	ring_buffer_t rb_test3;
	uint8_t a2=10,b2=20,c2=30,d2=40;
	uint8_t storage3[4];
	uint8_t x_out;
	
	rb_init(&rb_test3,storage3,4,true);
	printf("after init --------\n");
	if(rb_peek(&rb_test3,&x_out)==true){
		printf("rb_peek result %d \n",x_out);
	}
	else{
		printf("rb_peek couldnt work \n");
	}
	
	rb_dump(&rb_test3);
	printf("after first dump ----------------\n");
	
	rb_write(&rb_test3,a2);
	rb_write(&rb_test3,b2);
	rb_write(&rb_test3,c2);
	
	printf("after write ----------------\n");
	
	if(rb_peek(&rb_test3,&x_out)==true){
		printf("rb_peek result %d \n",x_out);
	}
	else{
		printf("rb_peek couldnt work \n");
	}
	
	rb_dump(&rb_test3);
	
	printf("after second dump  ----------------\n");
	
	if(rb_read(&rb_test3,&x_out)==true){
		printf("x is %d\n",x_out);
	}
	else{
		printf("couldnt readt value\n");
	}
	
	printf("after read   ----------------\n");
	
	rb_dump(&rb_test3);
	
	
	return 1;

}

*/

