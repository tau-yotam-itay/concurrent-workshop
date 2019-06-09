//
// Created by itaylevi on 6/4/19.
//

#ifndef CONCURRENT_WORKSHOP_MASTER_SSSP_H
#define CONCURRENT_WORKSHOP_MASTER_SSSP_H

#include "graph.h"
#include "multi_queue.h"
#include <stdio.h>

void print_distances(Graph*g);
void dijkstra(Vertex* s, Graph* g);

#endif  // CONCURRENT_WORKSHOP_MASTER_SSSP_H
