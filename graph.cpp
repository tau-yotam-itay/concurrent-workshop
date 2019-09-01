#include "graph.h"
#define PRINT_INTERVAL_GRAPH 50000
#define PRINT_PROGRESS 0

using namespace std;

/**
 * Vertex constructor
 * @param k key(id) of vertex
 */
Vertex::Vertex(int k)
{
  neighbors = NULL;
  dist = INT_MAX;
  lock = false;
}

/**
 * Vertex constructor (with initialization to dist)
 * @param k key(id) of vertex
 * @param new_dist distance from source
 */
Vertex::Vertex(int k, int new_dist)
{
  key = k;
  neighbors = NULL;
  dist = new_dist;
  lock = false;
}

/**
 * @return distance of current vertex from source vertex
 */
int Vertex::get_dist() { return dist; }

/**
 * Set new distance from source
 * @param d new distance from source vertex
 */
void Vertex::set_dist(int d) { dist = d; }

/**
 * Add new vertex to neigbor list.
 * @param v vertex to be added
 * @param weight edge (curr,v) weight
 */
void Vertex::add_neighbor(Vertex* v, int weight)
{
  neighbor* new_neighbor = new neighbor;
  new_neighbor->v = v;
  new_neighbor->weight = weight;
  new_neighbor->next = get_neighbors();
  neighbors = new_neighbor;
}

/**
 * @return neighbors list
 */
neighbor* Vertex::get_neighbors() { return neighbors; }

/**
 * @return pointer to current vertex lock
 */
volatile bool* Vertex::get_lock() { return &lock; }

/**
 * Set vertex lock to value b
 * @param b new lock value
 */
void Vertex::set_lock(bool b) { lock = b; }

/**
 * Graph constructor
 * @param filepath path to graph edges txt file
 */
Graph::Graph(const char* filepath)
{
  int c = 0;

  ifstream fp(filepath);
  fp >> number_of_vertices >> number_of_edges >> source_vertex_key;

  vertices = (Vertex**)calloc(number_of_vertices, sizeof(Vertex*));
  if (!vertices) {
    exit(EXIT_FAILURE);
  }
  for (int i = 0; i < number_of_vertices; i++) {
    vertices[i] = NULL;
  }

  int key1, key2, weight;
  while (fp >> key1 >> key2 >> weight) {
    add_edge(key1, key2, weight);
    c++;
    if (PRINT_PROGRESS && c % PRINT_INTERVAL_GRAPH == 0) {

      printf("added %d edges to the graph. \n", c);
    }
  }
}

/**
 * Free Graph allocated memory
 */
/*void Graph::free_graph(){
    int i;
    neighbor *n, *temp;

    for(i = 0; i < number_of_vertices; i++){
        n = vertices[i]->get_neighbors();
        while (n != NULL) {  // iterate all neighbors of vertices[i]
            temp = n;
            n = n->next;
            delete temp;
        }
        delete vertices[i];
    }
    free(vertices);
}*/

/**
 * Add new edge to graph
 * @param key1 vertex 1 key
 * @param key2 vertex 1 key
 * @param weight weight of edge (key1,key1)
 */
void Graph::add_edge(int key1, int key2, int weight)
{
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

/**
 * @return pointer to source vertex
 */
Vertex* Graph::get_source() { return vertices[source_vertex_key]; }

/**
 * @return total number of vertices in graph
 */
int Graph::get_vertices_num() { return number_of_vertices; }