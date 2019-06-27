#include "multi_queue.h"
#include "recordmgr/record_manager.h"
#define THREAD_SHARED 0
#define SEMAPHORE_INIT_VALUE 1

Multi_Queue::Multi_Queue(int c, int p)
{
  P = p;
  C = c;
  finish = 0;
  queues_array = new Binary_Heap*[C * P];
  sem_init(&sem_mutex, THREAD_SHARED, SEMAPHORE_INIT_VALUE);
  // mgr = new record_manager<reclaimer_debra<>,allocator_new<>,pool_none<>,BH_Node>(P_CONSTANT,SIGQUIT);
  for (int i = 0; i < C * P; i++) {
    queues_array[i] = new Binary_Heap();
  }
}

BH_Node* allocate_node(Vertex* v, int dist)
{
  BH_Node* node = new BH_Node(v);
  return node;
}

// free node using debra
void Multi_Queue::destroy_node(BH_Node* node) { 
  //delete node; 
  }

void Multi_Queue::insert(Vertex* v)
{
  // enter debra quiscent state
  volatile bool thrnd_won = false;
  int rand_queue_index;
  do {
    rand_queue_index = rand() % (C * P);                                                             // +1 needed?
    thrnd_won = __sync_bool_compare_and_swap(queues_array[rand_queue_index]->get_lock(), false, true);  // Compare and swap
  } while (!thrnd_won);
  BH_Node* to_insert_node = new BH_Node(v);
  queues_array[rand_queue_index]->insert(to_insert_node);
  queues_array[rand_queue_index]->set_lock(false);
  // exit debra quiscent state
}

std::tuple<Vertex*, int> Multi_Queue::extract_min()  // dead lock here - stuck in loop while other thread terminates
{
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
  std::tuple<Vertex*, int> ret = std::make_tuple(extracted_node->get_vertex(), extracted_node->get_dist());
  destroy_node(extracted_node);
  chosen_heap->set_lock(false);
  // exit debra quiscent state
  return ret;
}

sem_t* Multi_Queue::get_sem_mutex() { return &sem_mutex; }

// returns true if all binary heaps are empty
bool Multi_Queue::is_empty()
{
  for (int i = 0; i < C * P; i++) {
    if (queues_array[i]->get_min() != NULL) {
      return false;
    }
  }
  return true;
}

volatile bool* Multi_Queue::get_all_sleep_lock() { return &all_sleep_lock; }

void Multi_Queue::set_all_sleep_lock(bool b)
{
  all_sleep_lock = b;
}

void Multi_Queue::choose_random_heap(Binary_Heap **bh1, Binary_Heap **bh2) {
  int rand_queue_index_1, rand_queue_index_2;

  do{
    rand_queue_index_1 = rand() % (C * P);  // +1 needed?
    rand_queue_index_2 = rand() % (C * P);  // +1 needed?

  }while(rand_queue_index_1 == rand_queue_index_2);

  *bh1 = queues_array[rand_queue_index_1];
  *bh2 = queues_array[rand_queue_index_2];
}
bool Multi_Queue::try_lock_heaps(Binary_Heap* bh1, Binary_Heap* bh2) {
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
