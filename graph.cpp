#include "graph.h"
#define PRINT_INTERVAL_GRAPH 50000

using namespace std;

Vertex::Vertex(int k)
{
  key = k;
  neighbors = NULL;
  dist = INT_MAX;
  lock = false;
}

Vertex::Vertex(int k, int new_dist)
{
  key = k;
  neighbors = NULL;
  dist = new_dist;
  lock = false;
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

volatile bool* Vertex::get_lock() { return &lock; }

void Vertex::set_lock(bool b)
{
  lock = b;
}

// void Graph::add_to_vertex_list(Vertex* v)
// {
//   vertex_list_node* new_vertex = new vertex_list_node;
//   new_vertex->v = v;
//   new_vertex->next = vertex_list;
//   vertex_list = new_vertex;
//   count++;
// }

void Graph::add_edge(int key1, int key2, int weight)
{
  // Vertex* v1 = get_vertex(key1);
  // Vertex* v2 = get_vertex(key2);
  Vertex* v1 = vertices[key1];
  Vertex* v2 = vertices[key2];
  if (!v1) {
    v1 = new Vertex(key1);
    vertices[key1] = v1;
  }
  if (!v2) {
    v2 = new Vertex(key2);
    vertices[key2] = v2;
  }
  v1->add_neighbor(v2, weight);
  v2->add_neighbor(v1, weight);
}

Graph::Graph(const char* filepath)
{
  int c = 0;
  //vertex_list = NULL;
  ifstream fp(filepath);
  fp >> number_of_vertices >> number_of_edges >> source_vertex_key;
  
  vertices = (Vertex**)calloc(number_of_vertices,sizeof(Vertex*));
  if(!vertices){exit(EXIT_FAILURE);}
  for(int i=0;i<number_of_vertices;i++){
    vertices[i]=NULL;
  }

  int key1, key2, weight;
  while (fp >> key1 >> key2 >> weight) {
    add_edge(key1, key2, weight);
    c++;
    if(c%PRINT_INTERVAL_GRAPH == 0){

      printf("added %d edges to the graph. \n",c);
    }
  }
}

// Vertex* Graph::get_vertex(int key)
// {
//   vertex_list_node* cur = vertex_list;
//   while (cur != NULL) {
//     if (cur->v->get_key() == key) {
//       return cur->v;
//     }
//     cur = cur->next;
//   }
//   return NULL;
// }

Vertex* Graph::get_source() { return vertices[source_vertex_key]; }

int Graph::get_verticies_num() { return number_of_vertices; }

void Graph::print_graph()
{
  printf("id, (key,weight)\n");
  int i = 0;
  while (i < number_of_vertices) {
    Vertex* v = vertices[i];
    v->print_neighbors();
    printf("\n");
    i++;
  }
}