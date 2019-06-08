#include "sssp.h"
using namespace std;

void thread_worker(Multi_Queue* Q)
{
  std::tuple<Vertex*, int> min = Q->extract_min();
  Vertex* v = get<0>(min);
  int dist = get<1>(min);

  neighbor* n = v->get_neighbors();
  while (n != NULL) {                   // iterate all neighbors of v
    int new_dist = dist + n->weight;    // d' = d(n) + w(v,n)
    if (new_dist < n->v->get_dist()) {  // if distance is smaller - relax and add to Q
      n->v->set_dist(new_dist);
      Q->insert(n->v);
    }
    // maybe if finished node gets better distance then he is re inserted to queue? check this
    n = n->next;
  }
}

void dijkstra(Vertex* s, Graph* g)
{

/*
  void* thread(void* arg)
  {
    //wait
    sem_wait(&mutex);
    printf("\nEntered..\n");

    //critical section
    sleep(4);

    //signal

    sem_post(&mutex);
  }
  */


  Multi_Queue* Q = new Multi_Queue(C_CONSTANT, P_CONSTANT);
  Q->insert(s);
  int* threads_status_arr= (int*)calloc(P_CONSTANT, sizeof(int)); //add check failures
  pthread_t* threads_arr = (pthread_t*)calloc(P_CONSTANT, sizeof(pthread_t)); //add check failures
  // init threads
  while (!Q->is_empty()) {
  }
  // while q not empty - stop main thread from quiting
  sem_destroy(Q->get_sem_mutex());
}
