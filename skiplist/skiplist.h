#ifndef CONCURRENT_WORKSHOP_SKIPLIST_H
#define CONCURRENT_WORKSHOP_SKIPLIST_H

#include "graph.h"


class Skiplist_node
{
private:
    Skiplist_node **next_arr;
    Vertex* v;
    int dist;
    int level;
    bool inserting;

public:
    Skiplist_node(int level_arg, Vertex* vertex);
    bool is_deleted();
    void set_inserting(bool insert);
    int get_dist();
    int get_level();
    bool is_inserting();
    Skiplist_node **get_next_arr();
    Vertex *get_vertex();
    Skiplist_node* get_marked_ptr();
    Skiplist_node* get_unmarked_ptr();
};



class Skiplist
{

private:
    //fields:
    int max_level;
    int bound_offset;
    float next_level_prob;
    int nlevels; //higest level chosen so far
    Skiplist_node *head;
    Skiplist_node *tail;

    //functions:
    int random_level();
    Skiplist_node* create_node(int level_arg, Vertex* vertex);
    void destroy_node(Skiplist_node* node);
    Skiplist_node* locate_preds(int dist, Skiplist_node** preds, Skiplist_node** succs);
    void restructure();

public:
    Skiplist(int max_lvl, float prob, int offset);
    void insert(Vertex* vertex);
    Vertex* extract_min();
    //void display_list();
};

#endif //CONCURRENT_WORKSHOP_SKIPLIST_H
