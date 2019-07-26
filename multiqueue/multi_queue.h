#ifndef CONCURRENT_WORKSHOP_MASTER_MULTI_QUEUE_H
#define CONCURRENT_WORKSHOP_MASTER_MULTI_QUEUE_H

#define C_CONSTANT 2

#include "binary_heap.h"
#include "../priority_queue.h"

class Multi_Queue: public Priority_Queue
{
 private:
  int C;
  // record_manager<reclaimer_debra<>,allocator_new<>,pool_none<>,BH_Node> * mgr;

  BH_Node* create_node(Vertex* v);
  void destroy_node(BH_Node* node);
  void choose_random_heap(Binary_Heap** bh1, Binary_Heap** bh2);
  bool try_lock_heaps(Binary_Heap* bh1, Binary_Heap* bh2);
  void choose_one_heap(Binary_Heap* bh1, Binary_Heap* bh2, Binary_Heap** chosen_heap);

 public:
    Multi_Queue(int c, int p);
    Binary_Heap** queues_array;

    void insert(Vertex* vertex) override;
    Vertex* extract_min() override;
    bool is_empty() override;
};

#endif  // CONCURRENT_WORKSHOP_MASTER_MULTI_QUEUE_H
