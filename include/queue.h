#ifndef __QUEUE_H__
#define __QUEUE_H__

#include "k_global.h"

typedef struct kq {
	Point8* q;
	u8 front;
	u8 end;
	u8 count;
}Queue;

Queue* newq();

Point8 peek(Queue* qu);
u8 empty(Queue* qu);
u8 full(Queue* qu);
u8 enque(Queue* qu, u8 data);
Point8 pop(Queue* qu);


#endif