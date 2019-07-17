#include "priority_queue.h"


sem_t* Priority_Queue::get_sem_mutex() { return &sem_mutex; }



volatile bool* Priority_Queue::get_all_sleep_lock() { return &all_sleep_lock; }

void Priority_Queue::set_all_sleep_lock(bool b)
{
    all_sleep_lock = b;
}

int Priority_Queue::get_P(){
    return P;
}
