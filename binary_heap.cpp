#include "binary_heap.h"
#include <climits>
#include <limits>

using namespace std;
BH_Node::BH_Node(Vertex* vertex) {
    dist = INT_MAX;
    v = vertex;
    parent = NULL;
    left = NULL;
    right = NULL;
}

BH_Node::BH_Node(Vertex* vertex, int distance) {
    //maybe call the default constructor instead
    dist = distance;
    v = vertex;
    parent = NULL;
    left = NULL;
    right = NULL;
}
int BH_Node::get_dist() {
  return dist;
}
Vertex* BH_Node::get_vertex() {
  return v;
}
BH_Node* BH_Node::get_parent() {
  return parent;
}
BH_Node* BH_Node::get_left() {
  return left;
}
BH_Node* BH_Node::get_right() {
  return right;
}
void BH_Node::set_parent(BH_Node* node) {
  parent = node;
}
void BH_Node::set_left(BH_Node* node) {
  left = node;
}
void BH_Node::set_right(BH_Node* node) {
  right = node;
}
void BH_Node::set_vertex(Vertex* vertex) {
  v = vertex;
}
void BH_Node::set_dist(int d) {
  dist = d;
}
BH_Node* BH_Node::successor(){
  BH_Node* parent = this->get_parent();
  BH_Node* curr_node = this;

  if(!parent){
    return curr_node;
  }

  while (curr_node->get_parent() != NULL && parent->get_left() != curr_node) {	//go up until  last edge used was a left edge
          curr_node = parent;
          parent = parent->get_parent();
  }
  if(curr_node->get_parent() == NULL){ // parent is root and we went up from right subtree
    return go_leftmost(curr_node);
  }

  curr_node = parent;
  parent = curr_node;
  curr_node = curr_node->get_right();	//go right

  if(curr_node == NULL){
    return parent;
  }
  return go_leftmost(curr_node);
}

BH_Node* BH_Node::predecessor(){
  BH_Node* parent = this->get_parent();
  BH_Node* curr_node = this;

  while (curr_node->parent != NULL && parent->get_right() != curr_node) {	//go up until last edge used was a right edge
          curr_node = parent;
          parent = parent->get_parent();
  }

  curr_node = curr_node->get_left();	//go left

  while (true) {	//go right until reach leaf
          if (curr_node->get_right() != NULL) {
                  curr_node = curr_node->get_right();
          }
          else {
                  return curr_node;
          }
  }
}
bool BH_Node::is_leaf(){
    return (!left && !right);
}
BH_Node* BH_Node::go_leftmost(BH_Node *curr_node) {
  while (true) {
    if (curr_node->get_left() != NULL) {
      curr_node = curr_node->get_left();
    }
    else {
      return curr_node;
    }
  }
}

BH_Node* min_node;
BH_Node* last_node;

// value swap - make sure it doesnt cause external pointers problems
void Binary_Heap::swap(BH_Node* child,BH_Node* parent){
  int c_dist = child->get_dist();
  Vertex* c_vertex = child->get_vertex();
  child->set_dist(parent->get_dist());
  child->set_vertex(parent->get_vertex());
  parent->set_dist(c_dist);
  parent->set_vertex(c_vertex);
}

void Binary_Heap::heapify_up(BH_Node* node){
  while(node != min_node && node->get_dist() < node->get_parent()->get_dist()){
      swap(node, node->get_parent());
      node = node->get_parent();
  }
}

void Binary_Heap::heapify_down(BH_Node* node){
  if(node->get_dist() < node->get_left()->get_dist()){
      swap(node, node->get_left());
      heapify_down(node->get_left());
  }
  else if(node->get_dist() < node->get_right()->get_dist()){
      swap(node, node->get_right());
      heapify_down(node->get_right());
  }
}

volatile bool lock;

Binary_Heap::Binary_Heap() {
  min_node = NULL;
  last_node = NULL;
}

void Binary_Heap::insert(BH_Node* node) {
  if(!min_node){ //empty heap
    min_node = node;
    last_node = node;
    return;
  }
  BH_Node* last_successor = last_node->successor();
  if(last_successor->is_leaf()){
    last_successor->set_left(node);
  }
  else{
    last_successor->set_right(node);
  }
  node->set_parent(last_successor);
  last_node = node;
  this->heapify_up(node);
}

BH_Node* Binary_Heap::extract_min(){
  swap(min_node, last_node);
  BH_Node* min = last_node;
  last_node = min->predecessor();
  // verify how predecessor works here
  // retire min
  return min;
}

BH_Node* Binary_Heap::get_min() { return min_node; }
