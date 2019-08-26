#ifndef CONCURRENT_WORKSHOP_PRIORITY_QUEUE_H
#define CONCURRENT_WORKSHOP_PRIORITY_QUEUE_H

#include "graph.h"
#include <semaphore.h>

#define THREAD_SHARED 0
#define SEMAPHORE_INIT_VALUE 1
#define RAND_SEED 0
#define NUM_OF_THREADS 10

class Priority_Queue
{

 protected:
  int P;
  sem_t sem_mutex;
  volatile bool all_sleep_lock;

 public:
  bool finish;

  Priority_Queue(int p);
  virtual void insert(Vertex* vertex) = 0;
  virtual Vertex* extract_min() = 0;
  virtual bool is_empty() = 0;
  sem_t* get_sem_mutex();
  volatile bool* get_all_sleep_lock();
  void set_all_sleep_lock(bool b);
  int get_P();
};

#endif  // CONCURRENT_WORKSHOP_PRIORITY_QUEUE_H