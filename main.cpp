#include "sssp.h"
#include "multiqueue/multi_queue.h"
#include "skiplist/skiplist.h"
//#include "tests.h"
//#include <iostream>

int main(int argc, const char* argv[])
{
  Graph g(argv[1]);
  printf("done building graph. \n");
  // g.print_graph();
  srand(RAND_SEED);
  // heap_sort_test(20);
  Vertex* s = g.get_source();
  //Priority_Queue* Q = new Multi_Queue(C_CONSTANT, NUM_OF_THREADS);
  Priority_Queue* Q = new Skiplist(MAX_LVL, LEVEL_PROB, BOUND_OFFSET, NUM_OF_THREADS);
  dijkstra(Q, s, &g);
  print_distances(&g);
  return 0;
}
