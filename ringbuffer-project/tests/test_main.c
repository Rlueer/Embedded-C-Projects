#include "ring_buffer.h"
#include <stdio.h>


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