#include "sssp.h"
#define PRINT_INTERVAL 1000
using namespace std;

typedef struct thread_args {
  Multi_Queue* Q;
  int* thread_status;
  int thread_idx;
} thread_args;

void print_distances(Graph* g)
{
  int dist;
  ofstream outputFile;
  outputFile.open("our_distances");
  for (int i = 0; i < g->get_verticies_num(); i++) {
    dist = g->vertices[i]->get_dist();
    outputFile << dist << endl;
    //printf("%d\n", g->get_vertex(i)->get_dist());
  }
  outputFile.close();
}

bool is_only_worker(int thread_idx, int* threads_status, Multi_Queue* Q)
{
  bool locked = false;
  locked = __sync_bool_compare_and_swap(Q->get_all_sleep_lock(), false, true);
  if(!locked){return false;}
  for (int i = 0; i < P_CONSTANT; i++) {
    if (i != thread_idx && threads_status[i] == 1) {
      Q->set_all_sleep_lock(false);
      return false;
    }
  }
  Q->set_all_sleep_lock(false);
  return true;
}

void wake_all_threads(sem_t* sem)
{
  for (int i = 0; i < P_CONSTANT; i++) {
    sem_post(sem);
  }
}

void relax(Multi_Queue* Q, int dist, neighbor* n, int*relax_count, int tidx)
{
  int new_dist = dist + n->weight;    // d' = d(n) + w(v,n)
  if (new_dist < n->v->get_dist()) {  // if distance is smaller - relax and add to Q
    n->v->set_dist(new_dist);
    // print progress of thread
    *relax_count = *relax_count + 1;
    if (*relax_count%PRINT_INTERVAL == 0){
      printf("thread %d: relax operations done: %d\n",tidx,*relax_count);
    }
    Q->insert(n->v);
    sem_post(Q->get_sem_mutex());
  }
  // maybe if finished node gets better distance then he is re inserted to queue? check this
}

void* thread_worker(void* args)
{

  thread_args* t_args = (thread_args*)args;
  Multi_Queue* Q = t_args->Q;
  int* thread_status = t_args->thread_status;
  int thread_idx = t_args->thread_idx;
  int relax_count = 0;

  while (!Q->finish) {
    if (is_only_worker(thread_idx, thread_status, Q) && Q->is_empty() && !Q->finish) {  // check if time to terminate
      Q->finish = 1;
      wake_all_threads(Q->get_sem_mutex());
      break;
    }
    thread_status[thread_idx] = 0;
    sem_wait(Q->get_sem_mutex());
    thread_status[thread_idx] = 1;
    if (Q->finish) {
      break;
    }
    std::tuple<Vertex*, int> min = Q->extract_min();
    Vertex* v = get<0>(min);
    int dist = get<1>(min);

    neighbor* n = v->get_neighbors();
    while (n != NULL) {  // iterate all neighbors of v
      relax(Q, dist, n, &relax_count, thread_idx);
      n = n->next;
    }
  }
  thread_status[thread_idx] = 0;
  free(args);
  return NULL;
}

void init_threads(Multi_Queue* Q, int* threads_status_arr, pthread_t* threads_arr)
{
  // init threads (maybe move to function)
  for (int i = 0; i < P_CONSTANT; i++) {
    thread_args* args = (thread_args*)calloc(1, sizeof(thread_args));
    if (!args) {
      perror("calloc");
      exit(EXIT_FAILURE);
    }
    args->Q = Q;
    args->thread_idx = i;
    args->thread_status = threads_status_arr;
    int ret = pthread_create(&threads_arr[i], NULL, &thread_worker, args);
    if (ret) {
      perror("pthread_create");
      exit(EXIT_FAILURE);
    }
  }
}

void dijkstra(Vertex* s, Graph* g)
{

  int* threads_status_arr = (int*)calloc(P_CONSTANT, sizeof(int));
  pthread_t* threads_arr = (pthread_t*)calloc(P_CONSTANT, sizeof(pthread_t));
  if (threads_arr == NULL || threads_status_arr == NULL) {
    perror("malloc");
    exit(EXIT_FAILURE);
  }

  Multi_Queue* Q = new Multi_Queue(C_CONSTANT, P_CONSTANT);
  s->set_dist(0);
  Q->insert(s);

  init_threads(Q, threads_status_arr, threads_arr);

  // pthread join - wait for everyone to finish
  for (int i = 0; i < P_CONSTANT; i++) {
    pthread_join(threads_arr[i], NULL);
  }

  sem_destroy(Q->get_sem_mutex());
  free(threads_arr);
  free(threads_status_arr);
}
