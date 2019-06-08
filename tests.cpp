#include "tests.h"
using namespace std;

void initialie_dist(Graph* g)
{

  int num_of_verticies = g->get_verticies_num();
  int i;

  // insert all graph verticies
  for (i = 0; i < num_of_verticies; i++) {
    g->get_vertex(i)->set_dist(num_of_verticies - i);  // set distances in reversed order
  }
}

void heap_sort_test(Graph* g)
{

  // initialie_dist(g);

  Binary_Heap* heap = new Binary_Heap();
  BH_Node* curr_heap_node;
  int num_of_verticies = g->get_verticies_num();
  int* heap_sorted_dist_arr = (int*)calloc(num_of_verticies, sizeof(int));
  int* true_sorted_dist_arr = (int*)calloc(num_of_verticies, sizeof(int));
  int i;

  // insert all graph verticies
  for (i = 0; i < num_of_verticies; i++) {
    curr_heap_node = new BH_Node(g->get_vertex(i), num_of_verticies - i);
    heap->insert(curr_heap_node);
    //printf("%d ,", num_of_verticies - i);
    true_sorted_dist_arr[i] = curr_heap_node->get_dist();
  }
  /*for (int j = 0; j < i; j++) {
      printf("%d ,", true_sorted_dist_arr[j]);
  }*/
  sort(true_sorted_dist_arr, true_sorted_dist_arr + num_of_verticies, greater<int>());

  // extract min all graph verticies
  i = 0;
  while (heap->get_min() != NULL) {  // while heap is not empty
    curr_heap_node = heap->extract_min();
    heap_sorted_dist_arr[i++] = curr_heap_node->get_dist();
    printf("%d ,", heap_sorted_dist_arr[i - 1]);
    fflush(stdout);
  }
  printf("\n");
  // printf("sorted distance array is:\n");
  if (i != num_of_verticies) {
    printf("Error: different num of verticies! \n");
    return;
  }
  for (int j = 0; j < num_of_verticies; j++) {
    if (heap_sorted_dist_arr[j] != true_sorted_dist_arr[j]) {
      printf("%d ,", true_sorted_dist_arr[j]);
      printf("Error: difference in vertex number: %d \n", j);
    }
    // printf(heap_sorted_dist_arr[j] + ",");
  }
}
