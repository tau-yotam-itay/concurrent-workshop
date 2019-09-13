#include "skiplist/skiplist.h"
#include "sssp.h"
#include <chrono>

int main(int argc, const char* argv[])
{
  int num_of_threads;
  if (argc < 2) {
    printf("Usage: ./concurrent_skiplist [edges file] [num of threads]");
    return 0;
  } else if (argc < 3) {
    printf("Number of threads not specified in the command line, using default: 80 threads\n");
    num_of_threads = NUM_OF_THREADS;
  } else {
    num_of_threads = atoi(argv[2]);
  }
  // Build graph from input
  Graph g(argv[1]);
  // Set random seed for consistent results
  srand(RAND_SEED);
  // Create priority queue
  Skiplist* Q = new Skiplist(MAX_LVL, LEVEL_PROB, BOUND_OFFSET, num_of_threads);
  // Start timer
  auto start = std::chrono::high_resolution_clock::now();
  // Execute SSSP
  dijkstra(Q, g.get_source());
  // Stop timer and print result
  auto finish = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed = finish - start;
  printf("Number of threads: %d, elapsed time: %f.\n", num_of_threads, elapsed.count());
  // Export distances to output file
  export_distances(&g);
  delete Q;

  return 0;
}