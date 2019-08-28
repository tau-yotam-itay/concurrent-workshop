#include "multi_queue.h"
//#include "../recordmgr/record_manager.h"

/**
 * Multi Queue constructor
 * @param c some constant to be multiplied by total number of threads (p) to define the total number of heaps (cp)
 * @param p total number of threads
 */
Multi_Queue::Multi_Queue(int c, int p) : Priority_Queue(p)
{
    C = c;
    mgr = new record_manager<reclaimer_debra<>,allocator_new<>,pool_none<>,BH_Node>(p,SIGQUIT);
    queues_array = new Binary_Heap*[C * P];
    // mgr = new record_manager<reclaimer_debra<>,allocator_new<>,pool_none<>,BH_Node>(P_CONSTANT,SIGQUIT);
    for (int i = 0; i < C * P; i++) {
        queues_array[i] = new Binary_Heap();
    }
}

/**
 * @param v node's vertex
 * @return pointer to new BH_Node
 */
BH_Node* Multi_Queue::create_node(Vertex* v, int tid)
{
    //debra enters here
    BH_Node* node = mgr->template allocate<BH_Node>(tid);
    node->set_properties(v);
    return node;
}

/**
 * Free node's used memory using debra
 * @param node node to be destroyed
 */
void Multi_Queue::destroy_node(BH_Node* node, int tid) {
    mgr->retire(tid,node);
}

/**
 * Insert new node to the Multi_Queue priority queue
 * @param vertex
 */
void Multi_Queue::insert(Vertex* vertex, int tid)
{
    // enter debra quiscent state
    mgr->enterQuiescentState(tid);
    volatile bool thrnd_won = false;
    int rand_queue_index;
    do {
        rand_queue_index = rand() % (C * P);
        thrnd_won = __sync_bool_compare_and_swap(queues_array[rand_queue_index]->get_lock(), false, true);  // Compare and swap
    } while (!thrnd_won);
    BH_Node* to_insert_node = create_node(vertex, tid);
    queues_array[rand_queue_index]->insert(to_insert_node);
    queues_array[rand_queue_index]->set_lock(false);
    mgr->leaveQuiescentState(tid);
    // exit debra quiscent state
}

/**
 * @return true iff all binary heaps are empty
 */
bool Multi_Queue::is_empty()
{
    for (int i = 0; i < C * P; i++) {
        if (queues_array[i]->get_min() != NULL) {
            return false;
        }
    }
    return true;
}

/**
 * Choose one of two heaps by the heap which has node with minimal value of both.
 * @param bh1 first heap
 * @param bh2 second heap
 * @param chosen_heap pointer to the selected heap's pointer
 */
void Multi_Queue::choose_one_heap(Binary_Heap* bh1, Binary_Heap* bh2, Binary_Heap** chosen_heap){
    bool both_empty;

    both_empty = (bh1->is_empty() && bh2->is_empty());
    if(both_empty){
        // skip all other statments
    }
    else if(bh1->is_empty()){
        *chosen_heap = bh2;
    }
    else if(bh2->is_empty()){
        *chosen_heap = bh1;
    }
    else if(bh1->get_min()->get_dist() > bh2->get_min()->get_dist()){
        *chosen_heap = bh2;
    }
    else{
        *chosen_heap = bh1;
    }

    //release unchosen heap locks
    if(*chosen_heap != bh1){
        bh1->set_lock(false);
    }
    if(*chosen_heap != bh2){
        bh2->set_lock(false);
    }
}

/**
 * Try to lock previously randomized heaps using __sync_bool_compare_and_swap
 * If lock is unsuccessful for one of the heaps - release the other heap's lock
 * @param bh1 first heap
 * @param bh2 second heap
 * @return true if both heaps are successfully lockes, otherwise - false
 */
bool Multi_Queue::try_lock_heaps(Binary_Heap* bh1, Binary_Heap* bh2){
    bool is_locked_queue1 = false, is_locked_queue2 = false;

    is_locked_queue1 = __sync_bool_compare_and_swap(bh1->get_lock(), false, true);
    is_locked_queue2 = __sync_bool_compare_and_swap(bh2->get_lock(), false, true);
    if(!is_locked_queue1 && is_locked_queue2) {
        bh2->set_lock(false);
        return false;
    }
    if(!is_locked_queue2 && is_locked_queue1) {
        bh1->set_lock(false);
        return false;
    }
    if(!is_locked_queue1 && !is_locked_queue2){
        return false;
    }
    return true;
}

/**
 * Randomize two valid different heap indexes
 * @param bh1 pointer to first randomized heap
 * @param bh2 pointer to seconed randomized heap
 */
void Multi_Queue::choose_random_heap(Binary_Heap **bh1, Binary_Heap **bh2) {
    int rand_queue_index_1, rand_queue_index_2;

    do{
        rand_queue_index_1 = rand() % (C * P);
        rand_queue_index_2 = rand() % (C * P);

    }while(rand_queue_index_1 == rand_queue_index_2);

    *bh1 = queues_array[rand_queue_index_1];
    *bh2 = queues_array[rand_queue_index_2];
}

/**
 * Extract node with minimal value (minimum distance of vertex from source) from Multi_Queue
 */
Vertex* Multi_Queue::extract_min(int tid)
{
    mgr->enterQuiescentState(tid);
    // enter debra quiscent state
    Binary_Heap *bh1 = NULL, *bh2 = NULL, *chosen_heap = NULL;

    do {
        choose_random_heap(&bh1, &bh2);
        if(!try_lock_heaps(bh1, bh2)){
            continue;
        }
        choose_one_heap(bh1,bh2,&chosen_heap);
    } while (!chosen_heap);

    BH_Node* extracted_node = chosen_heap->extract_min();
    Vertex* ret = extracted_node->get_vertex();
    destroy_node(extracted_node,tid);
    chosen_heap->set_lock(false);
    mgr->leaveQuiescentState(tid);
    // exit debra quiscent state
    return ret;
}