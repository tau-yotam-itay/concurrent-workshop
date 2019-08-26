#include "sssp.h"
using namespace std;

typedef struct thread_args {
  Priority_Queue* Q;
  int* thread_status;
  int thread_idx;
} thread_args;

/**
 * Print all graph vertices distances from source vertex into output file.
 * @param g graph
 */
void export_distances(Graph* g)
{
  int dist;
  ofstream outputFile;
  outputFile.open("our_distances");
  for (int i = 0; i < g->get_vertices_num(); i++) {
    dist = g->vertices[i]->get_dist();
    outputFile << dist << endl;
  }
  outputFile.close();
}

/**
 * @param thread_idx id of thread to be checked
 * @param threads_status all threads status (working-1 ,sleeping-0) array
 * @param Q Priority Queue
 * @return true if thread_idx is the single thread currently working.
 */
bool is_only_worker(int thread_idx, int* threads_status, Priority_Queue* Q)
{
  bool locked = false;
  do {
    locked = __sync_bool_compare_and_swap(Q->get_all_sleep_lock(), false, true);
  } while (!locked);
  for (int i = 0; i < Q->get_P(); i++) {
    if (i != thread_idx && threads_status[i] == 1) {
      threads_status[thread_idx] = 0;
      Q->set_all_sleep_lock(false);
      return false;
    }
  }
  threads_status[thread_idx] = 0;
  Q->set_all_sleep_lock(false);
  return true;
}

/**
 * @param sem semaphore
 * @param P total number of threads
 */
void wake_all_threads(sem_t* sem, int P)
{
  for (int i = 0; i < P; i++) {
    sem_post(sem);
  }
}

/**
 * Preform dijkstra relax operation.
 * If dist has been updated - re insert vertex to the priority queue.
 * @param Q Priority Queue
 * @param dist current distance fron source vertex
 * @param n neighbors list
 */
void relax(Priority_Queue* Q, int dist, neighbor* n)
{
  bool locked = false;
  int new_dist = dist + n->weight;  // d' = d(n) + w(v,n)
  Vertex* v = n->v;
  do {
    locked = __sync_bool_compare_and_swap(v->get_lock(), false, true);
  } while (!locked);

  if (new_dist < n->v->get_dist()) {  // if distance is smaller - relax and add to Q
    n->v->set_dist(new_dist);
    Q->insert(n->v);
    sem_post(Q->get_sem_mutex());
  }

  v->set_lock(false);
}

/**
 * Independent "work unit" for a thread. Preforms single dijkstra iteration.
 * @param args thread arguments
 * @return NULL
 */
void* thread_worker(void* args)
{
  thread_args* t_args = (thread_args*)args;
  Priority_Queue* Q = t_args->Q;
  int* thread_status = t_args->thread_status;
  int thread_idx = t_args->thread_idx;
  thread_status[thread_idx] = 1;

  while (!Q->finish) {
    if ((is_only_worker(thread_idx, thread_status, Q) && Q->is_empty() && !Q->finish)) {  // check if time to terminate
      Q->finish = 1;
      wake_all_threads(Q->get_sem_mutex(), Q->get_P());
      break;
    }

    sem_wait(Q->get_sem_mutex());
    thread_status[thread_idx] = 1;
    if (Q->finish) {
      break;
    }
    Vertex* min = Q->extract_min();
    if (!min) {
      continue;
    }

    int dist = min->get_dist();

    neighbor* n = min->get_neighbors();
    while (n != NULL) {  // iterate all neighbors of min
      relax(Q, dist, n);
      n = n->next;
    }
  }
  thread_status[thread_idx] = 0;
  free(args);
  return NULL;
}

/**
 * Initialize and launch all threads
 * @param Q Priority Queue
 * @param threads_status_arr all threads status (working-1 ,sleeping-0) array
 * @param threads_arr array of all threads
 */
void init_threads(Priority_Queue* Q, int* threads_status_arr, pthread_t* threads_arr)
{
  for (int i = 0; i < Q->get_P(); i++) {
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

/**
 * A Dijkstra sssp version for relaxed priority queue.
 * When vertex distane is updated in relax operation - the vertex is
 * re inserted to the priority queue.
 * @param Q Priority Queue
 * @param s source vertex
 */
void dijkstra(Priority_Queue* Q, Vertex* s)
{
  int* threads_status_arr = (int*)calloc(Q->get_P(), sizeof(int));
  pthread_t* threads_arr = (pthread_t*)calloc(Q->get_P(), sizeof(pthread_t));
  if (threads_arr == NULL || threads_status_arr == NULL) {
    perror("malloc");
    exit(EXIT_FAILURE);
  }

  s->set_dist(0);
  Q->insert(s);
  if (Q->is_empty()) {
    printf("empty\n");
  }
  init_threads(Q, threads_status_arr, threads_arr);

  // pthread join - wait for everyone to finish
  for (int i = 0; i < Q->get_P(); i++) {
    pthread_join(threads_arr[i], NULL);
  }

  sem_destroy(Q->get_sem_mutex());
  free(threads_arr);
  free(threads_status_arr);
}