#include "multi_queue.hpp"
#include "binary_heap.cpp" //remove this and rearrange includes!

class Multi_Queue {
private:
	int C;
	int P;
    // add debra manager field
    
    // allocate new node using debra
    BH_Node* allocate_node(Vertex* v, int dist){
        return NULL;
    }
    
    // free node using debra
    void destroy_node(BH_Node* node){
        
    }
    // need to discuss where is the correct place in the code to free/allocate memory
    
public:
	Binary_Heap** queues_array;

	Multi_Queue(int c, int p) {
		P = p;
		C = c;
		queues_array = new Binary_Heap*[C*P];
        // debra manager = new debra manager...
	}
    
    void insert(Vertex* v) {
        volatile bool safe;
        int rand_queue_index;
        do {
            rand_queue_index = rand() % P; // +1 needed?
			bool thrnd_won = __sync_bool_compare_and_swap(&safe, true, false); //Compare and swap
        } while (!safe);
		BH_Node* to_insert_node = new BH_Node(v);
		queues_array[rand_queue_index]->insert(to_insert_node);
		safe = true;
    }
    
    BH_Node* extract_min(){
		volatile bool safe;
		int rand_queue_index_1, rand_queue_index_2;
		do {
			rand_queue_index_1 = rand() % P; // +1 needed?
			rand_queue_index_2 = rand() % P; // +1 needed?
			if (queues_array[rand_queue_index_1]->get_min()->get_dist() >
				queues_array[rand_queue_index_2]->get_min()->get_dist() ) {
				swap(rand_queue_index_1, rand_queue_index_2);
			}
			bool thrnd_won = __sync_bool_compare_and_swap(&safe, true, false); //Compare and swap
		} while (!safe);
		BH_Node* extracted_node = queues_array[rand_queue_index_1]->extract_min();
		safe = true;
		return extracted_node;
    }
    


};
