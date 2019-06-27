#include "binary_heap.h"

using namespace std;
BH_Node::BH_Node(Vertex* vertex)
{
  dist = vertex->get_dist();
  v = vertex;
  parent = NULL;
  left = NULL;
  right = NULL;
}

BH_Node::BH_Node(Vertex* vertex, int distance)
{
  // maybe call the default constructor instead
  dist = distance;
  v = vertex;
  parent = NULL;
  left = NULL;
  right = NULL;
}
int BH_Node::get_dist() { return dist; }
Vertex* BH_Node::get_vertex() { return v; }
BH_Node* BH_Node::get_parent() { return parent; }
BH_Node* BH_Node::get_left() { return left; }
BH_Node* BH_Node::get_right() { return right; }
void BH_Node::set_parent(BH_Node* node) { parent = node; }
void BH_Node::set_left(BH_Node* node) { left = node; }
void BH_Node::set_right(BH_Node* node) { right = node; }
void BH_Node::set_vertex(Vertex* vertex) { v = vertex; }
void BH_Node::set_dist(int d) { dist = d; }
bool BH_Node::is_root() { return this->parent == NULL; }
bool BH_Node::is_right_child() { return !this->is_root() && this->parent->right == this; }
bool BH_Node::is_left_child() { return !this->is_root() && this->parent->left == this; }
bool BH_Node::has_right() { return this->right != NULL; }
bool BH_Node::has_left() { return this->left != NULL; }
BH_Node* BH_Node::successor()
{
  BH_Node* parent = this->get_parent();
  BH_Node* curr_node = this;

  if (!parent) {  // single element in heap
    return curr_node;
  }

  while (!curr_node->is_root() && curr_node->is_right_child()) {
    curr_node = parent;
    parent = parent->get_parent();
  }
  if (curr_node->is_root()) {  // curr_node is root and we went up from right(!) subtree
    return go_leftmost(curr_node);
  }

  curr_node = parent;
  if (!curr_node->has_right()) {
    return curr_node;
  }
  return go_leftmost(curr_node->get_right());
}

BH_Node* BH_Node::predecessor()
{
  BH_Node* parent = this->get_parent();
  BH_Node* curr_node = this;

  while (!curr_node->is_root() && curr_node->is_left_child()) {
    curr_node = parent;
    parent = parent->get_parent();
  }
  if (curr_node->is_root()) {  // curr_node is root and we went up from left(!) subtree
    return go_rightmost(curr_node);
  }

  curr_node = parent;
  if (!curr_node->has_left()) {
    return curr_node;
  }
  return go_rightmost(curr_node->get_left());
}
bool BH_Node::is_leaf() { return (!left && !right); }
BH_Node* BH_Node::go_leftmost(BH_Node* curr_node)
{
  while (true) {
    if (curr_node->has_left()) {
      curr_node = curr_node->get_left();
    } else {
      return curr_node;
    }
  }
}
BH_Node* BH_Node::go_rightmost(BH_Node* curr_node)
{
  while (true) {
    if (curr_node->has_right()) {
      curr_node = curr_node->get_right();
    } else {
      return curr_node;
    }
  }
}

void Binary_Heap::swap_left(BH_Node* child, BH_Node* parent){
  BH_Node *s, *t;

  s = parent->get_right();
  t = child->get_right();
  if(t){  t->set_parent(parent);}
  parent->set_right(t);
  if(s){  s->set_parent(child);}
  child->set_right(s);

  s = child->get_left();
  if(s){  s->set_parent(parent);}
  parent->set_left(s);

  t = parent->get_parent();
    if(t && t->get_left() == parent){
    t->set_left(child);
  }
  else if(t && t->get_right() == parent){
    t->set_right(child);
  }
  child->set_parent(t);

  child->set_left(parent);
  parent->set_parent(child);
}

void Binary_Heap::swap_right(BH_Node* child, BH_Node* parent){
  BH_Node *s, *t;

  s = parent->get_left();
  t = child->get_left();
  if(t){  t->set_parent(parent);}
  parent->set_left(t);
  if(s){  s->set_parent(child);}
  child->set_left(s);

  s = child->get_right();
  if(s){  s->set_parent(parent);}
  parent->set_right(s);

  t = parent->get_parent();
  if(t && t->get_right() == parent){
    t->set_right(child);
  }
  else if(t && t->get_left() == parent){
    t->set_left(child);
  }
  child->set_parent(t);

  child->set_right(parent);
  parent->set_parent(child);
}

void Binary_Heap::swap_relatives(BH_Node* child, BH_Node* parent){
  BH_Node* c_left = child->get_left();
  BH_Node* c_right = child->get_right();
  BH_Node* c_parent = child->get_parent();
  BH_Node* p_left = parent->get_left();
  BH_Node* p_right = parent->get_right();

  if(c_parent->get_left() == child){
    c_parent->set_left(parent);
  }
  if(c_parent->get_right() == child){
    c_parent->set_right(parent);
  }

  child->set_left(parent->get_left());
  child->set_right(parent->get_right());
  child->set_parent(parent->get_parent());

  if(p_left){ p_left->set_parent(child);}
  if(p_right){  p_right->set_parent(child);}

  parent->set_left(c_left);
  parent->set_right(c_right);
  parent->set_parent(c_parent);
}

// value swap - make sure it doesnt cause external pointers problems
void Binary_Heap::swap(BH_Node* child, BH_Node* parent)
{
  // if(last_node == child){
  //   last_node = parent;
  // }
  // if(parent->get_left() == child){  //child is left child of parent
  //   swap_left(child,parent);
  // }
  // else if(parent->get_right() == child){  //child is right child of parent
  //   swap_right(child,parent);
  // }
  // else{
  //   swap_relatives(child,parent); //parent is not a direct parent of child (for swapping root and leaf)
  // }
  // if(child->is_root()){
  //   min_node = child;
  // }
  Vertex* v = child->get_vertex();
  int dist = child->get_dist();

  child->set_dist(parent->get_dist());
  child->set_vertex(parent->get_vertex());

  parent->set_dist(dist);
  parent->set_vertex(v);
}

void Binary_Heap::heapify_up(BH_Node* node)
{
  BH_Node* parent_ptr;
  //while (node != min_node && (node->get_dist() < node->get_parent()->get_dist() )) {
  while ( !node->is_root() && (node->get_dist() < node->get_parent()->get_dist() )) {
    parent_ptr = node->get_parent();
    swap(node, node->get_parent());
    node = parent_ptr;
  }
  if(node->is_root()){ min_node = node;}
}

volatile bool* Binary_Heap::get_lock() { return &lock; }

void Binary_Heap::set_lock(bool b)
{
  lock = b;
}

void Binary_Heap::choose_swap(BH_Node* parent, BH_Node** chosen_node){
  if (parent->has_left() && parent->has_right()){ 
    BH_Node* left = parent->get_left();
    BH_Node* right = parent->get_right();
    if( (left->get_dist() < right->get_dist() ) && (parent->get_dist() > left->get_dist() )){
      *chosen_node = left;
    }
    else if( (right->get_dist() < left->get_dist() ) && ( parent->get_dist() > right->get_dist() )){
      *chosen_node = right;
    }
  }
  else if (parent->has_left() && (parent->get_dist() > parent->get_left()->get_dist() )) {
    *chosen_node = parent->get_left();
  }
  else if (parent->has_right() && (parent->get_dist() > parent->get_right()->get_dist() )) {
    *chosen_node = parent->get_right();
  }
}

void Binary_Heap::heapify_down(BH_Node* node)
{
  BH_Node* chosen_node = NULL;
  choose_swap(node,&chosen_node);
  if(chosen_node){
    swap(chosen_node, node);
    heapify_down(chosen_node);
    // if(node->is_root()){
    //   min_node = chosen_node;
    // }
  }
}

Binary_Heap::Binary_Heap()
{
  min_node = NULL;
  last_node = NULL;
  volatile bool lock = false;
}

void Binary_Heap::insert(BH_Node* node)
{
  if (!min_node) {  // empty heap
    min_node = node;
    last_node = node;
    return;
  }
  BH_Node* last_successor = last_node->successor();
  if (last_successor->is_leaf()) {
    last_successor->set_left(node);
  } else {
    last_successor->set_right(node);
  }
  node->set_parent(last_successor);
  last_node = node;
  this->heapify_up(node);
}

void Binary_Heap::disconnect_node(BH_Node* node)
{
  if(node->is_root()){
    return;
  }

  if (!node->is_right_child()) {
    node->get_parent()->set_left(NULL);
  } else if (node->is_right_child()) {
    node->get_parent()->set_right(NULL);
  }
  node->set_parent(NULL);
}

BH_Node* Binary_Heap::extract_min()
{
  BH_Node* min;
  if (min_node->is_leaf()) {  // if only root left in heap
    min = min_node;
    min_node = NULL;
    last_node = NULL;
    return min;
  }
  swap(last_node,min_node);
  min = last_node;
  if(min->get_parent()==NULL && min->is_leaf()){
    printf("s\n");
  }
  if(min->get_dist()<0){
    printf("d\n");
  }
  last_node = min->predecessor();

  // disconnect the node to be removed
  disconnect_node(min);

  heapify_down(min_node);

  return min;
}

BH_Node* Binary_Heap::get_min() { return min_node; }

bool Binary_Heap::is_empty() { return min_node == NULL; }
