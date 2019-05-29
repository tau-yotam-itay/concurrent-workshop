#include "multi_queue.hpp"

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
        bool successful_lock = false;
        int rand_queue_index;
        do {
            rand_queue_index = ( rand() % P ) + 1;
            
        } while (!successful_lock);
    };
    
    BH_Node* extract_min(){
        return NULL;
    }
    


};
