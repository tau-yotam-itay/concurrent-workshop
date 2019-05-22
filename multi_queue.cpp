#include "multi_queue.hpp"

class Multi_Queue {
private:
	int C;
	int P;
public:
	Binary_Heap** queues_array;

	Multi_Queue(int c, int p) {
		P = p;
		C = c;
		queues_array = new Binary_Heap*[C*P];
	}
	//insert, extract_min

};

void insert(Vertex* v) {
	bool successful_lock = false;
	int rand_queue_index;
	do {
		rand_queue_index = ( rand() % P ) + 1;

	} while (!successful_lock);
}