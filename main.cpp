#include "sssp.h"
#include "tests.h"
//#include <iostream>

int main(int argc, const char* argv[])
{
  Graph g(argv[1]);
  printf("done building graph. \n");
  // g.print_graph();
  srand(RAND_SEED);
  // heap_sort_test(20);
  Vertex* s = g.get_source();
  dijkstra(s, &g);
  print_distances(&g);
  return 0;
}
