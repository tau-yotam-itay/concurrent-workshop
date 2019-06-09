#include "sssp.h"
using namespace std;

typedef struct thread_args{
  Multi_Queue* Q;
  int* thread_status;
  int thread_idx;
}thread_args;


void init_threads(Multi_Queue*Q,int*threads_status_arr, pthread_t*threads_arr){
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
}

bool is_only_worker(int thread_idx, int*threads_status){
  for(int i=0; i<P_CONSTANT; i++){
    if(i != thread_idx && threads_status[i] == 1){
      return false;
    }
  }
  return true;
}

void wake_all_threads(sem_t* sem){
  for(int i=0; i<P_CONSTANT; i++){
    sem_post(sem);
  }
}

void relax(Multi_Queue*Q, int dist, neighbor*n){
  int new_dist = dist + n->weight;    // d' = d(n) + w(v,n)
  if (new_dist < n->v->get_dist()) {  // if distance is smaller - relax and add to Q
    n->v->set_dist(new_dist);
    Q->insert(n->v);
    sem_post(Q->get_sem_mutex());
  }
  // maybe if finished node gets better distance then he is re inserted to queue? check this
}

void* thread_worker(void* args)
{

  thread_args* t_args = (thread_args*) args;
  Multi_Queue* Q = t_args->Q;
  int* thread_status = t_args->thread_status;
  int thread_idx = t_args->thread_idx;

  while(!Q->finish){
    sem_wait(Q->get_sem_mutex());

    std::tuple<Vertex*, int> min = Q->extract_min();
    Vertex* v = get<0>(min);
    int dist = get<1>(min);

    neighbor* n = v->get_neighbors();
    while (n != NULL) { // iterate all neighbors of v
      relax(Q, dist, n);
      n = n->next;
    }
    if(is_only_worker(thread_idx,thread_status) && Q->is_empty() && !Q->finish){ // check if time to terminate
      Q->finish = 1;
      wake_all_threads(Q->get_sem_mutex());
      break;
    }
  }
  thread_status[thread_idx] = 0;
  return NULL;
}

void dijkstra(Vertex* s, Graph* g)
{

  int* threads_status_arr =       (int*)calloc(P_CONSTANT, sizeof(int));
  pthread_t* threads_arr  = (pthread_t*)calloc(P_CONSTANT, sizeof(pthread_t));
  if(threads_arr == NULL || threads_status_arr == NULL){
    perror("malloc");
    exit(EXIT_FAILURE);
  }

  Multi_Queue* Q = new Multi_Queue(C_CONSTANT, P_CONSTANT);
  Q->insert(s);

  init_threads(Q, threads_status_arr, threads_arr);

  //pthread join - wait for everyone to finish
  for(int i=0; i<P_CONSTANT; i++){
    pthread_join(threads_arr[i],NULL);
  }

  sem_destroy(Q->get_sem_mutex());
}
