#pragma once
#include "binary_heap.hpp"
#include <limits>

using namespace std;

class BH_Node {
private:
	int dist;
	Vertex* v;
public:

	BH_Node(Vertex* vertex) {
		dist = INT_MAX;
		v = vertex;
	}

	BH_Node(Vertex* v, int distance) {
		dist = INT_MAX;
		dist = distance;
	}

	int get_dist() {
		return dist;
	}

	Vertex* get_vertex() {
		return v;
	}
};

class Binary_Heap {
private:
	mutex lock;
	BH_Node* min_node;
//heapify_up,heapify_down
public:
//constructor, get funcs, extract_min, insert

	void insert(BH_Node) {

	}

};



