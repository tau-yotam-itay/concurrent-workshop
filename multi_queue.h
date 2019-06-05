#ifndef CONCURRENT_WORKSHOP_MASTER_MULTI_QUEUE_H
#define CONCURRENT_WORKSHOP_MASTER_MULTI_QUEUE_H

#include <sstream>
#include <atomic>         // std::atomic
#include <thread>         // std::thread
#include <mutex>
#include <tuple>
#include <iostream>
#include <string>
#include "binary_heap.h"
#include "constants.h"
#include "recordmgr/record_manager.h"



class Multi_Queue{
private:
  int C;
  int P;
  BH_Node* allocate_node(Vertex* v, int dist);
  void destroy_node(BH_Node* node);
  // record_manager<reclaimer_debra<>,allocator_new<>,pool_none<>,BH_Node> * mgr;
public:
  Binary_Heap** queues_array;
  Multi_Queue(int c, int p);
  void insert(Vertex* v);
  std::tuple<Vertex*,int> extract_min();
  // need to implement
  bool is_empty();
};

#endif // CONCURRENT_WORKSHOP_MASTER_MULTI_QUEUE_H
