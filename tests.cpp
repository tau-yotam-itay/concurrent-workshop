#include "tests.h"
using namespace std;

struct Trunk {
  Trunk* prev;
  string str;

  Trunk(Trunk* prev, string str)
  {
    this->prev = prev;
    this->str = str;
  }
};

// Helper function to print branches of the binary tree
void showTrunks(Trunk* p)
{
  if (p == nullptr)
    return;

  showTrunks(p->prev);

  // cout << p->str;
  printf("%s", (p->str).c_str());
}

// Recursive function to print binary tree
// It uses inorder traversal
void printTree(BH_Node* root, Trunk* prev, bool isLeft)
{
  if (root == nullptr)
    return;

  string prev_str = "    ";
  Trunk* trunk = new Trunk(prev, prev_str);

  printTree(root->get_left(), trunk, true);

  if (!prev)
    trunk->str = "---";
  else if (isLeft) {
    trunk->str = ".---";
    prev_str = "   |";
  } else {
    trunk->str = "'---";
    prev->str = prev_str;
  }

  showTrunks(trunk);
  // cout << root->get_dist() << endl;
  printf("%d\n", root->get_dist());

  if (prev)
    prev->str = prev_str;
  trunk->str = "   |";

  printTree(root->get_right(), trunk, false);
}

void print_arr(int* arr, int n)
{
  for (int i = 0; i < n; i++) {
    printf("%d, ", arr[i]);
  }
  fflush(stdout);
}

void heap_sort_test(int num_of_verticies)
{

  // initialie_dist(g);

  Binary_Heap* heap = new Binary_Heap();
  BH_Node* curr_heap_node;
  int* heap_sorted_dist_arr = (int*)calloc(num_of_verticies, sizeof(int));
  int* true_sorted_dist_arr = (int*)calloc(num_of_verticies, sizeof(int));
  int i;

  // insert all graph verticies
  for (i = 0; i < num_of_verticies; i++) {
    Vertex* v = new Vertex(i, num_of_verticies - i);
    curr_heap_node = new BH_Node(v);
    true_sorted_dist_arr[i] = curr_heap_node->get_dist();
    heap->insert(curr_heap_node);
    printTree(heap->get_min(), NULL, false);
    // printf("%d ,", num_of_verticies - i);
  }
  /*for (int j = 0; j < i; j++) {
      printf("%d ,", true_sorted_dist_arr[j]);
  }*/
  sort(true_sorted_dist_arr, true_sorted_dist_arr + num_of_verticies);
  // printf("system sorted arr:\n");
  // print_arr(true_sorted_dist_arr, num_of_verticies);
  // extract min all graph verticies
  printTree(heap->get_min(), NULL, false);
  i = 0;
  printf("\nheap sorted arr:\n");
  while (heap->get_min() != NULL) {  // while heap is not empty
    curr_heap_node = heap->extract_min();
    heap_sorted_dist_arr[i++] = curr_heap_node->get_dist();
    printf("%d, ", heap_sorted_dist_arr[i - 1]);
    fflush(stdout);
    printTree(heap->get_min(), NULL, false);
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

void skiplist_sort_test()
{
  Vertex* v;
  Skiplist* q = new Skiplist(10, 0.5, 50, 2);
  for (int i = 20; i >= 0; i--) {
    v = new Vertex(i, i);
    q->insert(v);
    // q->print_skiplist();
  }
  while (!q->is_empty()) {
    v = q->extract_min();
    if (v)
      printf("%d\n", v->get_dist());
    // q->print_skiplist();
  }
  bool a = q->is_empty();
}
