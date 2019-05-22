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
    BH_Node* successor(){
        return NULL;
    }
    BH_Node* predecessor(){
        return NULL;
    }
    bool is_leaf(){
        return (!left && !right);
    }
};

class Binary_Heap {
private:
	BH_Node* min_node;
	BH_Node* last_node;
    
    void swap(BH_Node* child,BH_Node* parent){
        bool is_child_left = (parent->get_left() == child);
        bool is_parent_left = (parent->get_parent()->get_left() == parent);
        child->set_parent(parent->get_parent());
        BH_Node* temp;
    }
    
    void heapify_up(BH_Node* node){
        while(node->get_dist() < node->get_parent()->get_dist()){
            swap(node, node->get_parent());
        }
    }
//heapify_up,heapify_down
public:
	volatile bool lock;
//constructor, get funcs, extract_min, insert, decrease_key

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

private:


};



