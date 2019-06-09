#include "sssp.h"
#include "tests.h"
//#include <iostream>

int main(int argc, const char* argv[])
{
  Graph g(argv[1]);
  g.print_graph();
  srand(RAND_SEED);
  //heap_sort_test(&g);
  Vertex* v = g.get_vertex(0);
  dijkstra(v,&g);
  print_distances(&g);
  return 0;
}
