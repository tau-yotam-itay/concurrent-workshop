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
        // enter debra quiscent state
        volatile bool safe;
		bool thrnd_won;
        int rand_queue_index;
        do {
            rand_queue_index = rand() % P; // +1 needed?
			thrnd_won = __sync_bool_compare_and_swap(&safe, true, false); //Compare and swap
        } while (!thrnd_won);
		BH_Node* to_insert_node = new BH_Node(v);
		queues_array[rand_queue_index]->insert(to_insert_node);
		safe = true;
        // exit debra quiscent state
    }
    
    tuple<Vertex*,int> extract_min(){
        // enter debra quiscent state
		volatile bool safe;
		bool thrnd_won;
		int rand_queue_index_1, rand_queue_index_2;
		do {
            do{
			rand_queue_index_1 = rand() % P; // +1 needed?
			rand_queue_index_2 = rand() % P; // +1 needed?
            // make sure they are not the same index
            }while(rand_queue_index_1 == rand_queue_index_2);
			if (queues_array[rand_queue_index_1]->get_min()->get_dist() >
				queues_array[rand_queue_index_2]->get_min()->get_dist() ) {
				swap(rand_queue_index_1, rand_queue_index_2);
			}
			thrnd_won = __sync_bool_compare_and_swap(&safe, true, false); //Compare and swap
		} while (!thrnd_won);
		BH_Node* extracted_node = queues_array[rand_queue_index_1]->extract_min();
		tuple<Vertex*, int> ret = make_tuple(extracted_node->get_vertex(), extracted_node->get_dist());
        destroy_node(extracted_node);
		safe = true;
        // exit debra quiscent state
		return ret;
    }
    
    // need to implement
    bool is_empty(){
        return false;
    }


};
