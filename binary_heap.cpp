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
	BH_Node* get_parent(BH_Node* node) {
		return parent;
	}
	BH_Node* get_left(BH_Node* node) {
		return left;
	}
	BH_Node* get_right(BH_Node* node) {
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
};

class Binary_Heap {
private:
	BH_Node* min_node;
	BH_Node* last_node;

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
	void insert(BH_Node) {

	}

private:


};



