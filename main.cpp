
#include <iostream>
#include "graph.cpp"
#include "multi_queue.cpp"
#define RAND_SEED 0
#define P 2

int main(int argc, const char * argv[]) {
    // insert code here...
    Graph g(argv[1]);
    g.print_graph();
	srand(RAND_SEED);
    return 0;
}

