#include "binary_heap.h"

using namespace std;

BH_Node::BH_Node(){}

/**
 * BH_Node constructor
 *
 * @param vertex
 */
void BH_Node::set_properties(Vertex* vertex)
{
  dist = vertex->get_dist();
  v = vertex;
  parent = NULL;
  left = NULL;
  right = NULL;
}

/**
 * BH_Node constructor (with distance initialization)
 *
 * @param vertex
 * @param distance new distance from source vertex
 */
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
void BH_Node::set_dist(int d) { dist = d; }
void BH_Node::set_vertex(Vertex* vertex) { v = vertex; }
void BH_Node::set_parent(BH_Node* node) { parent = node; }
void BH_Node::set_left(BH_Node* node) { left = node; }
void BH_Node::set_right(BH_Node* node) { right = node; }
bool BH_Node::is_leaf() { return (!left && !right); }
bool BH_Node::is_root() { return this->parent == NULL; }
bool BH_Node::is_left_child() { return !this->is_root() && this->parent->left == this; }
bool BH_Node::is_right_child() { return !this->is_root() && this->parent->right == this; }
bool BH_Node::has_left() { return this->left != NULL; }
bool BH_Node::has_right() { return this->right != NULL; }

/**
 * @param curr_node
 * @return pointer to the leftmost successor of curr_node
 */
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

/**
 * @param curr_node
 * @return pointer to the rightmost successor of curr_node
 */
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

/**
 * @return pointer to the postorder closest successor of binary heap node
 */
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

/**
 * @return pointer to the postorder closest predecessor of binary heap node
 */
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

/**
 * Binary_Heap constructor
 */
Binary_Heap::Binary_Heap()
{
  min_node = NULL;
  last_node = NULL;
  volatile bool lock = false;
}

bool Binary_Heap::is_empty() { return min_node == NULL; }
BH_Node* Binary_Heap::get_min() { return min_node; }
volatile bool* Binary_Heap::get_lock() { return &lock; }
void Binary_Heap::set_lock(bool b){ lock = b;}

/**
 * Swap places between two nodes in the heap (Nodes data is being swapped and not the actual nodes).
 * @param child
 * @param parent
 */
void Binary_Heap::swap(BH_Node* child, BH_Node* parent)
{
    Vertex* v = child->get_vertex();
    int dist = child->get_dist();

    child->set_dist(parent->get_dist());
    child->set_vertex(parent->get_vertex());

    parent->set_dist(dist);
    parent->set_vertex(v);
}

/**
 * Choose next node to swap places within the binary heap, according to heap rules
 * @param parent node needed to be swapped
 * @param chosen_node right or left child of the parent to be swapped, chosen according to heap rules
 */
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

/**
 * Correct the position of the newly inserted node.
 * @param node inserted node
 */
void Binary_Heap::heapify_up(BH_Node* node)
{
    BH_Node* parent_ptr;

    while ( !node->is_root() && (node->get_dist() < node->get_parent()->get_dist() )) {
        parent_ptr = node->get_parent();
        swap(node, node->get_parent());
        node = parent_ptr;
    }
    if(node->is_root()){ min_node = node;}
}

/**
 * Correct the position of the root after executing extract min operation
 * @param node node to be heapified down.
 */
void Binary_Heap::heapify_down(BH_Node* node)
{
    BH_Node* chosen_node = NULL;
    choose_swap(node,&chosen_node);
    if(chosen_node){
        swap(chosen_node, node);
        heapify_down(chosen_node);
    }
}

/**
 * Insert a new node to the binary heap.
 * @param node new node to be inserted to the heap
 */
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

/**
 * Disconnect node from heap structure.
 * @param node to be removed from the heap structure.
 */
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

/**
 * Extract node with minimum value, and apply methods to preserve heap constrains.
 * @return pointer to BH_Node with minimum value.
 */
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
  //call debra deletion here?

  heapify_down(min_node);

  return min;
}