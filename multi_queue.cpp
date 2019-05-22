#include "graph.cpp"

#define P = 80
#define C = 2

Queue Multi_Queue[C*P];

void insert(Vertex* v) {
	bool successful_lock = false;
	int rand_queue_index;
	do {
		rand_queue_index = ( rand() % P ) + 1;

	} while (!successful_lock);
}