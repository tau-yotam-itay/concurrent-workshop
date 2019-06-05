#ifndef CONCURRENT_WORKSHOP_MASTER_BINARY_HEAP_H
#define CONCURRENT_WORKSHOP_MASTER_BINARY_HEAP_H

#include <iosfwd>
#include "graph.h"
#include <mutex>

class BH_Node {
private:
  int dist;
  Vertex* v;
  BH_Node* parent;
  BH_Node* left;
  BH_Node* right;

public:
  BH_Node(Vertex* vertex);
  BH_Node(Vertex* vertex, int distance);
  int get_dist();
  Vertex* get_vertex();
  BH_Node* get_parent();
  BH_Node* get_left();
  BH_Node* get_right();
  void set_parent(BH_Node* node);
  void set_left(BH_Node* node);
  void set_right(BH_Node* node);
  void set_vertex(Vertex* vertex);
  void set_dist(int d);
  BH_Node* successor();
  BH_Node* predecessor();
  bool is_leaf();
  bool is_root();
  bool is_right_child();
  bool is_left_child();
  bool has_right();
  bool has_left();
  BH_Node* go_leftmost(BH_Node * pNode);
  BH_Node* go_rightmost(BH_Node * pNode);
};


class Binary_Heap{
private:
  BH_Node* min_node;
  BH_Node* last_node;
  // value swap - make sure it doesnt cause external pointers problems
  void swap(BH_Node* child,BH_Node* parent);
  void heapify_up(BH_Node* node);
  void heapify_down(BH_Node* node);
public:
  volatile bool lock;
  Binary_Heap();
  void insert(BH_Node* node);
  void disconnect_node(BH_Node* node);
  BH_Node* extract_min();
  BH_Node* get_min();
};

#endif // CONCURRENT_WORKSHOP_MASTER_BINARY_HEAP_H
