#ifndef CONCURRENT_WORKSHOP_MASTER_MULTI_QUEUE_H
#define CONCURRENT_WORKSHOP_MASTER_MULTI_QUEUE_H

#define C_CONSTANT 2

#include "binary_heap.h"
#include "../priority_queue.h"

class Multi_Queue: public Priority_Queue
{
 private:
  int C;
  record_manager<reclaimer_debra<>,allocator_new<>,pool_none<>,BH_Node> * mgr;
  BH_Node* create_node(Vertex* v, int tid);
  void destroy_node(BH_Node* node, int tid);
  void choose_one_heap(Binary_Heap* bh1, Binary_Heap* bh2, Binary_Heap** chosen_heap);
  bool try_lock_heaps(Binary_Heap* bh1, Binary_Heap* bh2);
  void choose_random_heap(Binary_Heap** bh1, Binary_Heap** bh2);

 public:
    Binary_Heap** queues_array;
    Multi_Queue(int c, int p);
    //void free_heaps();
    void insert(Vertex* vertex, int tid) override;
    bool is_empty() override;
    Vertex* extract_min(int tid) override;
/*
    static void operator delete(void* ptr, std::size_t sz)
    {
        //cout << "custom delete for size " << sz <<endl;
        delete (ptr); // ::operator delete(ptr) can also be used
    }*/
};

#endif  // CONCURRENT_WORKSHOP_MASTER_MULTI_QUEUE_H