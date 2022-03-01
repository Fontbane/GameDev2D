#include "queue.h"

Queue* newq()
{
	Queue queue;
	queue.q = gfc_allocate_array(sizeof(Point8), 12);
	queue.front = 0;
	queue.end = 0;
	queue.count = 0;
	return &queue;
}

Point8 peek(Queue* qu) {
	return qu->q[qu->front];
}

u8 empty(Queue* qu) {
	return qu->count == 0;
}

u8 full(Queue* qu) {
	return qu->count == 12;
}

u8 enque(Queue* qu, Point8 data) {
	if (!full(qu)) {
		if (qu->end == 11) {
			qu->end = 0;
		}
		qu->q[qu->end++] = data;
		qu->count++;
		return 1;
	}
	return 0;
}

Point8 pop(Queue* qu) {
	Point8 data = qu->q[qu->front++];
	if (qu->front == 12) {
		qu->front = 0;
	}
	qu->count--;
	return data;
}