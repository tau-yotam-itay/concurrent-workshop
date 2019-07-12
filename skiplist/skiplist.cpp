#include "skiplist.h"
#include <cstring>


Skiplist_node::Skiplist_node(int level_arg, Vertex* vertex)
{
    dist = v->get_dist();
    level = level_arg;
    next_arr = new Skiplist_node*[level+1];
    memset(next_arr, 0, sizeof(Skiplist_node*)*(level+1));
    v = vertex;
};


Skiplist::Skiplist(int MAX_LVL, float P)
{
    max_level = MAX_LVL;
    next_level_prob = P;
    level = 0;

    header = new Skiplist_node(-1, MAX_LVL, NULL);
};


void Skiplist::insert(Vertex* vertex){
    int height = random_level();
    Skiplist_node* node = create_node(height, vertex);

    do{
        std::tuple<Skiplist_node*, Skiplist_node*, Skiplist_node*> located_preds = locate_preds(vertex->get_dist());
        Skiplist_node* preds = get<0>(located_preds);
        Skiplist_node* succs = get<1>(located_preds);
        Skiplist_node* del = get<2>(located_preds);
    }while(CAS);
}



std::tuple<Skiplist_node*, Skiplist_node*, Skiplist_node*>t::locate_preds(){
    return NULL;
}


std::tuple<Vertex*, int> Skiplist::extract_min(){

}