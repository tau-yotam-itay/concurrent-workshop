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
void Multi_Queue::destroy_node(BH_Node* node) { delete node; }

void Multi_Queue::insert(Vertex* v)
{
  // enter debra quiscent state
  volatile bool thrnd_won;
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
  volatile bool safe;
  bool thrnd_won, both_empty = false;
  int rand_queue_index_1, rand_queue_index_2;
  do {
    do {
      rand_queue_index_1 = rand() % (C * P);  // +1 needed?
      rand_queue_index_2 = rand() % (C * P);  // +1 needed?
      // make sure they are not the same index
      both_empty = (queues_array[rand_queue_index_1]->is_empty() && queues_array[rand_queue_index_2]->is_empty());
    } while (rand_queue_index_1 == rand_queue_index_2 || both_empty);

    bool one_empty = queues_array[rand_queue_index_1]->is_empty() || queues_array[rand_queue_index_2]->is_empty();
    if (!one_empty && queues_array[rand_queue_index_1]->get_min()->get_dist() >
                          queues_array[rand_queue_index_2]->get_min()->get_dist()) {
      std::swap(rand_queue_index_1, rand_queue_index_2);
    } else if (one_empty) {
      rand_queue_index_1 = (queues_array[rand_queue_index_1]->is_empty()) ? rand_queue_index_2 : rand_queue_index_1;
    }

    thrnd_won =
        __sync_bool_compare_and_swap(queues_array[rand_queue_index_1]->get_lock(), false, true);
  } while (!thrnd_won);
  Binary_Heap* b = queues_array[rand_queue_index_1];  // line for debugging/ delete after
  BH_Node* extracted_node = queues_array[rand_queue_index_1]->extract_min();
  std::tuple<Vertex*, int> ret = std::make_tuple(extracted_node->get_vertex(), extracted_node->get_dist());
  destroy_node(extracted_node);
  queues_array[rand_queue_index_1]->set_lock(false);
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