#pragma once
#include "tests.hpp"
#include "graph.hpp"//remove later and correct includes!!
#include "graph.cpp" //remove later and correct includes!!
#include "binary_heap.cpp"//remove later and correct includes!!
#include <algorithm>
using namespace std;

void heap_sort_test(Graph* g) {

	Binary_Heap* heap = new Binary_Heap();
	BH_Node* curr_heap_node;
	int num_of_verticies = g->get_verticies_num();
	int* heap_sorted_dist_arr = (int*) calloc(num_of_verticies, sizeof(int));
	int* true_sorted_dist_arr = (int*)calloc(num_of_verticies, sizeof(int));
	int i;

	//insert all graph verticies
	for (i = 0; i < num_of_verticies; i++) {
		curr_heap_node = new BH_Node(g->get_vertex(i));
		heap->insert(curr_heap_node);
		true_sorted_dist_arr[i] = curr_heap_node->get_dist();
	}
	sort(true_sorted_dist_arr, true_sorted_dist_arr + num_of_verticies);

	//extract min all graph verticies
	while (true) {
		i = 0;
		curr_heap_node = heap->extract_min();
		if (curr_heap_node == NULL) {
			break;
		}
		heap_sorted_dist_arr[i++] = curr_heap_node->get_dist();
	}
	//printf("sorted distance array is:\n");
	if (i != num_of_verticies + 1) {
		printf("Error: different num of verticies! \n");
		return;
	}
	for (int j = 0; j < i; j++) {
		if (heap_sorted_dist_arr[j] != true_sorted_dist_arr[i]) {
			printf("Error: difference in vertex number: %d \n", j);
		}
		//printf(heap_sorted_dist_arr[j] + ",");

	}
}