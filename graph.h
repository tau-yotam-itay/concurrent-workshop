#ifndef CONCURRENT_WORKSHOP_MASTER_GRAPH_H
#define CONCURRENT_WORKSHOP_MASTER_GRAPH_H

#include <climits>
#include <fstream>
#include <iosfwd>
#include <iostream>
#include <string>
#include <tuple>

class Vertex;

typedef struct neighbor {
  Vertex* v;
  int weight;
  struct neighbor* next;
} neighbor;

typedef struct vertex_list_node {
  Vertex* v;
  struct vertex_list_node* next;
} vertex_list_node;

class Vertex
{
 private:
  int key;
  unsigned int dist;
  neighbor* neighbors;

 public:
  Vertex(int k);
  int get_key();
  int get_dist();
  void set_dist(int d);
  void add_neighbor(Vertex* v, int weight);
  neighbor* get_neighbors();
  neighbor* get_neighbor(int key);
  void print_neighbors();
};

class Graph
{
 private:
  int number_of_vertices;
  int number_of_edges;
  int source_vertex_key;
  vertex_list_node* vertex_list;

  void add_to_vertex_list(Vertex* v);

  void add_edge(int key1, int key2, int weight);

 public:
  int count = 0;

  Graph(const char* filepath);
  Vertex* get_vertex(int key);
  int get_verticies_num();
  void print_graph();
};

#endif  // CONCURRENT_WORKSHOP_MASTER_GRAPH_H
