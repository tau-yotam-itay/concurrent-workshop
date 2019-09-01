#include "sssp.h"
#include "skiplist/skiplist.h"
#include <chrono>


int main(int argc, const char* argv[])
{
  int num_of_threads;
  if(argc < 2){
    printf("Usage: ./concurrent_skiplist [edges file] [num of threads]");
    return 0;
  }
  else if(argc < 3){
    printf("Number of threads not specified in the command line, using default: 80 threads\n");
    num_of_threads = NUM_OF_THREADS;
  }
  else{
    num_of_threads = atoi(argv[2]);
  }

  // Build graph from input
  Graph g(argv[1]);
  // printf("done building graph. \n");

  // Set random seed for consistent results
  srand(RAND_SEED);

  // Create priority queue
  record_manager<reclaimer_debra<>,allocator_new<>,pool_none<>,Skiplist_node> * mgr;
  Skiplist* Q = new Skiplist(MAX_LVL, LEVEL_PROB, BOUND_OFFSET, num_of_threads);

  // Start timer
  auto start = std::chrono::high_resolution_clock::now();

  // Execute SSSP
  dijkstra(Q, g.get_source());

  // Free allocated space
  Q->free_sentinels();
  g.free_graph();

  // Stop timer and print result
  auto finish = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed = finish - start;
  printf("%d\t%f\n", num_of_threads,elapsed.count());

  export_distances(&g);
  return 0;
}
