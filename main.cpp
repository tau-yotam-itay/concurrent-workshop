#include <iostream>
#include "graph.h"
#include "multi_queue.h"
#include "tests.h"
#include "constants.h"


int main(int argc, const char * argv[]) {
    // insert code here...
    Graph g(argv[1]);
    g.print_graph();
    srand(RAND_SEED);
    heap_sort_test(&g);
    return 0;
}

