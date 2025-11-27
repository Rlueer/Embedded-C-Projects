#include "ring_buffer.h"

bool rb_init(ring_buffer_t* rb, uint8_t* buffer_mem, size_t capacity, bool overwrite) {

    // 1. Null check
	if(rb==NULL || buffer_mem==NULL){
		return false;
	}

    // 2. Assign buffer pointer
	rb->buffer=buffer_mem;
	
    // 3. Assign capacity
	rb->capacity=capacity;
	
    // 4. Reset head and tail
	rb->head=0;
	rb->tail=0;
	
    // 5. Assign overwrite flag
	rb->overwrite_flag=overwrite;
	
    // 6. Return success
	return true;

}

bool rb_is_empty(const ring_buffer_t* rb){
	
	// 1. Null check
	if(rb==NULL){
		return false;
	}
	// 2. Empty check
	if(rb->head==rb->tail){
		return true;
	}
	
	return false;
}

bool rb_is_full(const ring_buffer_t* rb){
	
	// 1. Null check
	if(rb==NULL){
		return false;
	}
	
	// 2. Full check
	if((rb->head +1)%rb->capacity == rb->tail){
		return true;
	}
	
	return false;
	
}

size_t rb_size(const ring_buffer_t* rb){
		
	// 1. Null check
	if(rb==NULL){
		return 0;
	}
	
	// (head - tail + capacity) % capacity
	return (rb->head - rb->tail + rb->capacity)%rb->capacity  ;
	
}

bool rb_write(ring_buffer_t* rb, uint8_t data) {

    // 1. null kontrolü
	if(rb == NULL){
		return false;
	}
    // 2. full ise:
    //    - overwrite false → return false
    //    - overwrite true → tail'i ilerlet
	if(rb_is_full(rb)==true){
		if(rb->overwrite_flag==false){
			return false;
		}
		else if(rb->overwrite_flag==true){
			rb->tail = (rb->tail+1)%rb->capacity;
		}
	}
	
    // 3. buffer[head] = data
	rb->buffer[rb->head] = data;
	
    // 4. head'i modulo ile ilerlet
	rb->head = (rb->head+1)%rb->capacity;
	
    // 5. return true
	return true;
}

bool rb_read(ring_buffer_t* rb, uint8_t* data_out) {

    // 1. null pointer check (rb ve data_out)
	if(rb==NULL || data_out==NULL){
		return false;
	}

    // 2. empty check → empty ise false döndür
	if(rb_is_empty(rb)==true){
		return false;
	}
	
    // 3. tail’in gösterdiği veriyi data_out’a koy
	*data_out=rb->buffer[rb->tail];
	
    // 4. tail’i modulo ile ilerlet
	rb->tail= (rb->tail+1)%rb->capacity;

    // 5. true döndür
	return true;

}

// readden farkı tail ilerlemesi yok 
bool rb_peek(const ring_buffer_t* rb, uint8_t* data_out){
	
	// NULL and empty check
	if(rb == NULL || rb_is_empty(rb) == true){
		return false;
	}
	
	*data_out=rb->buffer[rb->tail];
	
	return true;
}
// debug fonksiyonu ring buffer içerigini print eder
void rb_dump(const ring_buffer_t* rb){
	
	if(rb==NULL){
		printf("rb is null\n");
		return ;
	}
	printf("capacity is %d \n", rb->capacity);
	printf("head at %d \n", rb->head);
	printf("tail at %d \n", rb->tail);
	printf("size is %d \n", rb_size(rb));

	printf("FIFO: ");
	size_t index = rb->tail;
	while (index != rb->head){
		printf("%d ", rb->buffer[index]);
		index = (index + 1) % rb->capacity;
	}
	printf("\n");

}