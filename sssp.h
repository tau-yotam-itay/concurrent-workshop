#ifndef CONCURRENT_WORKSHOP_MASTER_SSSP_H
#define CONCURRENT_WORKSHOP_MASTER_SSSP_H

#include "priority_queue.h"

void export_distances(Graph* g);
void dijkstra(Priority_Queue* Q, Vertex* s);

#endif  // CONCURRENT_WORKSHOP_MASTER_SSSP_H