
#ifndef graph_hpp
#define graph_hpp

#include <iostream>
#include <tuple>
#include <string>
#include <fstream>

class Vertex;
class Graph;

typedef struct neighbor{
    Vertex* v;
    int weight;
    struct neighbor* next;
}neighbor;


typedef struct vertex_list_node{
    Vertex* v;
    struct vertex_list_node* next;
}vertex_list_node;

#endif /* graph_hpp */
