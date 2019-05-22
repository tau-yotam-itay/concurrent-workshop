#pragma once
#include "binary_heap.hpp"
#include <limits>

using namespace std;

class BH_Node {
private:
	int dist;
	Vertex* v;
	BH_Node* left;
	BH_Node* right;

public:

	BH_Node(Vertex* vertex) {
		dist = INT_MAX;
		v = vertex;
		left = NULL;
		right = NULL;
	}

	BH_Node(Vertex* vertex, int distance) {
		//maybe call the default constructor instead
		dist = distance;
		v = vertex;
		left = NULL;
		right = NULL;
	}

	int get_dist() {
		return dist;
	}

	Vertex* get_vertex() {
		return v;
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
//heapify_up,heapify_down
public:
	volatile bool lock;
//constructor, get funcs, extract_min, insert, decrease_key

	void insert(BH_Node) {

	}

};



