#ifndef CONCURRENT_WORKSHOP_MASTER_GRAPH_H
#define CONCURRENT_WORKSHOP_MASTER_GRAPH_H

#include <climits>
#include <fstream>
#include <map>

class Vertex;

typedef struct neighbor {
  Vertex* v;
  int weight;
  struct neighbor* next;
} neighbor;

class Vertex
{
 private:
  int key;
  int dist;
  neighbor* neighbors;
  volatile bool lock;

 public:
  Vertex(int k);
  Vertex(int k, int dist);
  int get_dist();
  void set_dist(int d);
  void add_neighbor(Vertex* v, int weight);
  neighbor* get_neighbors();
  volatile bool* get_lock();
  void set_lock(bool b);
};

class Graph
{
 private:
  int number_of_vertices;
  int number_of_edges;
  int source_vertex_key;

  void add_edge(int key1, int key2, int weight);

 public:
  Vertex** vertices;

  Graph(const char* filepath);
  void free_graph();
  Vertex* get_source();
  int get_vertices_num();
};

#endif  // CONCURRENT_WORKSHOP_MASTER_GRAPH_H