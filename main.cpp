#include "sssp.h"
#include "tests.h"
#include <iostream>

int main(int argc, const char* argv[])
{
  // insert code here...
  Graph g(argv[1]);
  g.print_graph();
  srand(RAND_SEED);
  heap_sort_test(&g);
  return 0;
}
