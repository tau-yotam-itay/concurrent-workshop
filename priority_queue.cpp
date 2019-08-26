#include "priority_queue.h"

/**
 * Priority_Queue constructor
 * @param p total number of threads
 */
Priority_Queue::Priority_Queue(int p)
{
  P = p;
  finish = false;
  sem_init(&sem_mutex, THREAD_SHARED, SEMAPHORE_INIT_VALUE);
  all_sleep_lock = false;
}

/**
 * @return pointer to semaphore mutex
 */
sem_t* Priority_Queue::get_sem_mutex() { return &sem_mutex; }

/**
 * @return pointer to lock indicator (true iff all threads are in sleeping state)
 */
volatile bool* Priority_Queue::get_all_sleep_lock() { return &all_sleep_lock; }

/**
 * Set lock indicator (true iff all threads are in sleeping state)
 * @param b boolean value to be set
 */
void Priority_Queue::set_all_sleep_lock(bool b) { all_sleep_lock = b; }

/**
 * @return P - total number of threads
 */
int Priority_Queue::get_P() { return P; }