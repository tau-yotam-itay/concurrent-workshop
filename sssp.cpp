
#include "sssp.hpp"

void thread_worker(Multi_Queue* Q){
    tuple<Vertex*,int> min = Q->extract_min();
    Vertex* v = get<0>(min);
    int dist = get<1>(min);
    
    neighbor* n = v->get_neighbors();
    while(n != NULL){                    // iterate all neighbors of v
        int new_dist = dist + n->weight; // d' = d(n) + w(v,n)
        if(new_dist < n->v->get_dist()){ // if distance is smaller - relax and add to Q
            n->v->set_dist(new_dist);
            Q->insert(n->v);
        }
		//maybe if finished node gets better distance then he is re inserted to queue? check this
        n = n->next;
    }
}

void dijkstra(Vertex*s,Graph*g){
    Multi_Queue* Q = new Multi_Queue(C_CONST,P);
    Q->insert(s);
    //init threads
    while(!Q->is_empty()){
        
    }
    // while q not empty - stop main thread from quiting
}
