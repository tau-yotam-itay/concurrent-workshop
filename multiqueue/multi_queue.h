#ifndef CONCURRENT_WORKSHOP_MASTER_MULTI_QUEUE_H
#define CONCURRENT_WORKSHOP_MASTER_MULTI_QUEUE_H

#include "binary_heap.h"
#include "../constants.h"
#include <semaphore.h>
//#include <pthread.h>
//#include <sys/sem.h>
//#include <unistd.h>
//#include <atomic>  // std::atomic
//#include <iostream>
//#include <mutex>
//#include <sstream>
//#include <string>
//#include <thread>  // std::thread
//#include <tuple>

class Multi_Queue
{
 private:
  int C;
  int P;
  sem_t sem_mutex;
  volatile bool all_sleep_lock;
  // record_manager<reclaimer_debra<>,allocator_new<>,pool_none<>,BH_Node> * mgr;

  BH_Node* allocate_node(Vertex* v, int dist);
  void destroy_node(BH_Node* node);
  void choose_random_heap(Binary_Heap** bh1, Binary_Heap** bh2);
  bool try_lock_heaps(Binary_Heap* bh1, Binary_Heap* bh2);
  void choose_one_heap(Binary_Heap* bh1, Binary_Heap* bh2, Binary_Heap** chosen_heap);

 public:
  int finish;
  Binary_Heap** queues_array;
  sem_t* get_sem_mutex();
  Multi_Queue(int c, int p);
  void insert(Vertex* v);
  Vertex* extract_min();
  volatile bool* get_all_sleep_lock();
  void set_all_sleep_lock(bool b);
  bool is_empty();
};

#endif  // CONCURRENT_WORKSHOP_MASTER_MULTI_QUEUE_H
