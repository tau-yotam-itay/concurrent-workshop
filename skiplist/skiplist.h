#ifndef CONCURRENT_WORKSHOP_SKIPLIST_H
#define CONCURRENT_WORKSHOP_SKIPLIST_H

#include "graph.h"


class Skiplist_node
{
private:
    int dist;
    int level;
    Skiplist_node **next_arr;
    Vertex* v;

public:
    Skiplist_node(int level_arg, Vertex* vertex);
};



class Skiplist
{

private:
    int max_level;
    float next_level_prob;
    int level; //higest level chosen so far
    Skiplist_node *head;
    std::tuple<Skiplist_node*, Skiplist_node*, Skiplist_node*> locate_preds(int dist);

public:
    Skiplist(int MAX_LVL, float P);
    int random_level();
    Skiplist_node* create_node(int level_arg, Vertex* vertex);
    void destroy_node(Skiplist_node* node);
    void insert(Vertex* vertex);
    std::tuple<Vertex*, int> extract_min();
    //void display_list();
};

#endif //CONCURRENT_WORKSHOP_SKIPLIST_H
