#include "graph.h"

using namespace std;

Vertex::Vertex(int k)
{
  key = k;
  neighbors = NULL;
  dist = INT_MAX;
}

int Vertex::get_key() { return key; }

int Vertex::get_dist() { return dist; }

void Vertex::set_dist(int d) { dist = d; }

void Vertex::add_neighbor(Vertex* v, int weight)
{
  neighbor* new_neighbor = new neighbor;
  new_neighbor->v = v;
  new_neighbor->weight = weight;
  new_neighbor->next = get_neighbors();
  neighbors = new_neighbor;
}

neighbor* Vertex::get_neighbors() { return neighbors; }

neighbor* Vertex::get_neighbor(int key)
{
  neighbor* cur = neighbors;
  while (cur != NULL) {
    if (cur->v->get_key() == key) {
      return cur;
    }
    cur = cur->next;
  }
  return NULL;
}

void Vertex::print_neighbors()
{
  neighbor* cur = neighbors;
  printf("vertex %d: ", key);
  while (cur != NULL) {
    printf("(%d, %d), ", cur->v->get_key(), cur->weight);
    cur = cur->next;
  }
}

void Graph::add_to_vertex_list(Vertex* v)
{
  vertex_list_node* new_vertex = new vertex_list_node;
  new_vertex->v = v;
  new_vertex->next = vertex_list;
  vertex_list = new_vertex;
  count++;
}

void Graph::add_edge(int key1, int key2, int weight)
{
  Vertex* v1 = get_vertex(key1);
  Vertex* v2 = get_vertex(key2);
  if (!v1) {
    v1 = new Vertex(key1);
    add_to_vertex_list(v1);
  }
  if (!v2) {
    v2 = new Vertex(key2);
    add_to_vertex_list(v2);
  }
  v1->add_neighbor(v2, weight);
  v2->add_neighbor(v1, weight);
}

Graph::Graph(const char* filepath)
{
  vertex_list = NULL;
  ifstream fp(filepath);
  fp >> number_of_vertices >> number_of_edges >> source_vertex_key;
  int key1, key2, weight;
  while (fp >> key1 >> key2 >> weight) {
    add_edge(key1, key2, weight);
  }
}

Vertex* Graph::get_vertex(int key)
{
  vertex_list_node* cur = vertex_list;
  while (cur != NULL) {
    if (cur->v->get_key() == key) {
      return cur->v;
    }
    cur = cur->next;
  }
  return NULL;
}

int Graph::get_verticies_num() { return number_of_vertices; }

void Graph::print_graph()
{
  printf("id, (key,weight)\n");
  vertex_list_node* cur = vertex_list;
  while (cur != NULL) {
    cur->v->print_neighbors();
    printf("\n");
    cur = cur->next;
  }
}