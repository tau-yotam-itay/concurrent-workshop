#include "sssp.h"
#include "multiqueue/multi_queue.h"
#include "skiplist/skiplist.h"
#include "tests.h"
#include <chrono>
//#include "tests.h"
//#include <iostream>

int main(int argc, const char* argv[])
{
  //skiplist_sort_test();

  //Build graph from input
  Graph g(argv[1]);
  printf("done building graph. \n");

  //Set random seed for consistent results
  srand(RAND_SEED);

  // Get the source vertex
  Vertex* s = g.get_source();

  // Create priority queue
  Priority_Queue* Q = new Multi_Queue(C_CONSTANT, NUM_OF_THREADS);
  //Priority_Queue* Q = new Skiplist(MAX_LVL, LEVEL_PROB, BOUND_OFFSET, NUM_OF_THREADS);

  // Start timer
  auto start = std::chrono::high_resolution_clock::now();
  
  // Execute SSSP
  dijkstra(Q, s, &g);

  // Stop timer and print result
  auto finish = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed = finish - start;
  printf("Elapsed time: %f seconds\n",elapsed.count());

  export_distances(&g);
  return 0;
}
