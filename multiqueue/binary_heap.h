#ifndef CONCURRENT_WORKSHOP_MASTER_BINARY_HEAP_H
#define CONCURRENT_WORKSHOP_MASTER_BINARY_HEAP_H

#include "../graph.h"

#define CACHE_PADDING_BHEAP 88 //128-sizeof(BH_Node)

class BH_Node
{
 private:
  int dist;
  Vertex* v;
  BH_Node* parent;
  BH_Node* left;
  BH_Node* right;
  char pad[CACHE_PADDING_BHEAP];

 public:
  BH_Node(Vertex* vertex);
  BH_Node(Vertex* vertex, int distance); //delete?
  int get_dist();
  Vertex* get_vertex();
  BH_Node* get_parent();
  BH_Node* get_left();
  BH_Node* get_right();
  void set_dist(int d);
  void set_vertex(Vertex* vertex);
  void set_parent(BH_Node* node);
  void set_left(BH_Node* node);
  void set_right(BH_Node* node);
  bool is_leaf();
  bool is_root();
  bool is_left_child();
  bool is_right_child();
  bool has_left();
  bool has_right();
  BH_Node* go_leftmost(BH_Node* pNode);
  BH_Node* go_rightmost(BH_Node* pNode);
  BH_Node* successor();
  BH_Node* predecessor();
};

class Binary_Heap
{
 private:
  BH_Node* min_node;
  BH_Node* last_node;
  volatile bool lock;

  void swap(BH_Node* child, BH_Node* parent);
  void choose_swap(BH_Node* parent, BH_Node** chosen_node);
  void heapify_up(BH_Node* node);
  void heapify_down(BH_Node* node);

 public:
  Binary_Heap();
  bool is_empty();
  BH_Node* get_min();
  volatile bool* get_lock();
  void set_lock(bool b);
  void insert(BH_Node* node);
  void disconnect_node(BH_Node* node);
  BH_Node* extract_min();


};

#endif  // CONCURRENT_WORKSHOP_MASTER_BINARY_HEAP_H