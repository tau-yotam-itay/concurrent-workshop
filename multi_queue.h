#ifndef CONCURRENT_WORKSHOP_MASTER_MULTI_QUEUE_H
#define CONCURRENT_WORKSHOP_MASTER_MULTI_QUEUE_H

#include "binary_heap.h"
#include "constants.h"
#include <pthread.h>
#include <semaphore.h>
//#include <sys/sem.h>
#include <unistd.h>
//#include <atomic>  // std::atomic
//#include <iostream>
//#include <mutex>
//#include <sstream>
//#include <string>
//#include <thread>  // std::thread
#include <tuple>

class Multi_Queue
{
 private:
  int C;
  int P;
  sem_t sem_mutex;
  // record_manager<reclaimer_debra<>,allocator_new<>,pool_none<>,BH_Node> * mgr;

  BH_Node* allocate_node(Vertex* v, int dist);
  void destroy_node(BH_Node* node);
 
 public:
  int finish;
  Binary_Heap** queues_array;
  sem_t* get_sem_mutex();
  Multi_Queue(int c, int p);
  void insert(Vertex* v);
  std::tuple<Vertex*, int> extract_min();
  // need to implement
  bool is_empty();
};

#endif  // CONCURRENT_WORKSHOP_MASTER_MULTI_QUEUE_H
