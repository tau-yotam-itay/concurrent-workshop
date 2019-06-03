#pragma once
#include <iostream>
#include "graph.cpp"
#include "multi_queue.cpp"
#include "tests.cpp"
#define RAND_SEED 0
#define P 2

int main(int argc, const char * argv[]) {
    // insert code here...
    Graph g(argv[1]);
    g.print_graph();
	heap_sort_test(&g);
	srand(RAND_SEED);
    return 0;
}

