#pragma once
#include "binary_heap.hpp"
#include <limits>

using namespace std;

class BH_Node {
private:
	int dist;
	Vertex* v;
	BH_Node* parent;
	BH_Node* left;
	BH_Node* right;

public:

	BH_Node(Vertex* vertex) {
		dist = INT_MAX;
		v = vertex;
		parent = NULL;
		left = NULL;
		right = NULL;
	}

	BH_Node(Vertex* vertex, int distance) {
		//maybe call the default constructor instead
		dist = distance;
		v = vertex;
		parent = NULL;
		left = NULL;
		right = NULL;
	}
	int get_dist() {
		return dist;
	}
	Vertex* get_vertex() {
		return v;
	}
	BH_Node* get_parent() {
		return parent;
	}
	BH_Node* get_left() {
		return left;
	}
	BH_Node* get_right() {
		return right;
	}
	void set_parent(BH_Node* node) {
		parent = node;
	}
	void set_left(BH_Node* node) {
		left = node;
	}
	void set_right(BH_Node* node) {
		right = node;
	}
    void set_vertex(Vertex* vertex) {
        v = vertex;
    }
    void set_dist(int d) {
        dist = d;
    }
    BH_Node* successor(){
		BH_Node* parent = this->get_parent();
		BH_Node* curr_node = this;

		while (curr_node->parent != NULL && parent->get_left() != curr_node) {	//go up until  last edge used was a left edge
			curr_node = parent;
			parent = parent->get_parent();
		}

		curr_node = curr_node->get_right();	//go right

		while (true) {	//go left until reach a leaf
			if (curr_node->get_left() != NULL) {
				curr_node = curr_node->get_left();
			}
			else if (curr_node->get_right() != NULL) {	//impossible with full tree
				curr_node = curr_node->get_right();
			}
			else {
				return curr_node;
			}
		}
    }

    BH_Node* predecessor(){
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
			else if (curr_node->get_left() != NULL) {	//impossible with full tree
				curr_node = curr_node->get_left();
			}
			else {
				return curr_node;
			}
		}
    }
    bool is_leaf(){
        return (!left && !right);
    }
};

class Binary_Heap {
private:
	BH_Node* min_node;
	BH_Node* last_node;
    
    // value swap - make sure it doesnt cause external pointers problems
    void swap(BH_Node* child,BH_Node* parent){
        int c_dist = child->get_dist();
        Vertex* c_vertex = child->get_vertex();
        child->set_dist(parent->get_dist());
        child->set_vertex(parent->get_vertex());
        parent->set_dist(c_dist);
        parent->set_vertex(c_vertex);
    }
    
    void heapify_up(BH_Node* node){
        while(node != min_node && node->get_dist() < node->get_parent()->get_dist()){
            swap(node, node->get_parent());
            node = node->get_parent();
        }
    }
    
    void heapify_down(BH_Node* node){
        if(node->get_dist() < node->get_left()->get_dist()){
            swap(node, node->get_left());
            heapify_down(node->get_left());
        }
        else if(node->get_dist() < node->get_right()->get_dist()){
            swap(node, node->get_right());
            heapify_down(node->get_right());
        }
    }
public:
	volatile bool lock;
//get funcs, extract_min, insert

	Binary_Heap() {
		min_node = NULL;
		last_node = NULL;
	}
/*
	BH_Node* get_min() {
		return min_node;
	}
	BH_Node* get_last() {
		return last_node;
	}
*/

	void insert(BH_Node* node) {
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
    
    BH_Node* extract_min(){
        swap(min_node, last_node);
        BH_Node* min = last_node;
        last_node = min->predecessor();
        // verify how predecessor works here
        // retire min
        return min;
    }

private:


};



