#include "sssp.h"
#include "multiqueue/multi_queue.h"
#include <chrono>

int main(int argc, const char* argv[])
{
  int num_of_threads;
  if(argc < 2){
    printf("Usage: ./concurrent_multiqueue [edges file] [num of threads]");
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

  // Set random seed for consistent results
  srand(RAND_SEED);

  // Create priority queue
  Multi_Queue* Q = new Multi_Queue(C_CONSTANT, num_of_threads);
  // Start timer
  auto start = std::chrono::high_resolution_clock::now();

  // Execute SSSP
  dijkstra(Q, g.get_source());

  // Stop timer and print result
  auto finish = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed = finish - start;
  printf("%d\t%f\n", num_of_threads,elapsed.count());

  export_distances(&g);

  //Free allocated space
  delete Q;

  return 0;
}
