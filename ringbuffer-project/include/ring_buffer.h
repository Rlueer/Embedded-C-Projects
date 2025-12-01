#include <stdint.h>   // uint8_t
#include <stddef.h>   // size_t, NULL
#include <stdbool.h>  // bool
#include <stdio.h>	  //printf

typedef struct {
	
	uint8_t* buffer;
	size_t capacity;
	size_t head;
	size_t tail;
	bool overwrite_flag;
	
} ring_buffer_t;

bool rb_init(ring_buffer_t* rb,uint8_t* buffer_mem,size_t capacity,bool overwrite);

bool rb_is_empty(const ring_buffer_t* rb);

bool rb_is_full(const ring_buffer_t* rb);

size_t rb_size(const ring_buffer_t* rb);

//write to rb from outside 
bool rb_write(ring_buffer_t* rb, uint8_t data);

//read from rb to outside 
bool rb_read(ring_buffer_t* rb, uint8_t* data_out);

//readden farkı tail ilerlemesi yok 
bool rb_peek(const ring_buffer_t* rb, uint8_t* data_out);

// ring bufferin içeriklerini görebilmemiz imkan sağlıyor debug fonksiyonu
void rb_dump(const ring_buffer_t* rb);