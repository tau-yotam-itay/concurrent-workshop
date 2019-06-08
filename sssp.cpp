#include "sssp.h"
using namespace std;

typedef struct thread_args{
  Multi_Queue* Q;
  int* thread_status;
  int thread_idx;
}thread_args;

void* thread_worker(void* args)
{

  thread_args* t_args = (thread_args*) args;
  Multi_Queue* Q = t_args->Q;
  int* thread_status = t_args->thread_status;
  int thread_idx = t_args->thread_idx;

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
  return NULL;
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

  int* threads_status_arr =       (int*)calloc(P_CONSTANT, sizeof(int));
  pthread_t* threads_arr  = (pthread_t*)calloc(P_CONSTANT, sizeof(pthread_t));
  if(threads_arr == NULL || threads_status_arr == NULL){
    perror("malloc");
    exit(EXIT_FAILURE);
  }

  Multi_Queue* Q = new Multi_Queue(C_CONSTANT, P_CONSTANT);
  Q->insert(s);

  // init threads (maybe move to function)
  for(int i=0;i<P_CONSTANT;i++){
    thread_args* args = new thread_args;
    args->Q = Q;
    args->thread_idx = i;
    args->thread_status = threads_status_arr;
    int ret = pthread_create(&threads_arr[i],NULL,&thread_worker,&args);
    if(ret){
      perror("pthread_create");
      exit(EXIT_FAILURE);
    }
  }

  while (!Q->is_empty()) {
  }
  // while q not empty - stop main thread from quiting
  sem_destroy(Q->get_sem_mutex());
}
