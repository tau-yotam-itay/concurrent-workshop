#ifndef CONCURRENT_WORKSHOP_MASTER_SSSP_H
#define CONCURRENT_WORKSHOP_MASTER_SSSP_H

#include "priority_queue.h"
//#include "graph.h"
//#include <stdio.h>

void export_distances(Graph* g);
void dijkstra(Priority_Queue *Q, Vertex* s, Graph* g);

#endif  // CONCURRENT_WORKSHOP_MASTER_SSSP_H
