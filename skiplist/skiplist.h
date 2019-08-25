#ifndef CONCURRENT_WORKSHOP_SKIPLIST_H
#define CONCURRENT_WORKSHOP_SKIPLIST_H

#include "../priority_queue.h"
//#include "../recordmgr/record_manager.h"

#define MAX_LVL 25
#define LEVEL_PROB 0.5
#define BOUND_OFFSET 50
#define CACHE_PADDING_SKIPLIST 96 //128-sizeof(Skiplist_node)

class Skiplist_node
{
private:
    Skiplist_node **next_arr;
    Vertex* v;
    int dist;
    int level;
    bool inserting;
    char pad[CACHE_PADDING_SKIPLIST];

public:
    Skiplist_node(int level_arg, Vertex* vertex);
    bool is_deleted();
    void set_inserting(bool insert);
    int get_dist();
    bool is_inserting();
    Skiplist_node **get_next_arr();
    Vertex *get_vertex();
    Skiplist_node* get_marked_ptr();
    Skiplist_node* get_unmarked_ptr();
};



class Skiplist: public Priority_Queue
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
    Skiplist(int max_lvl, float prob, int offset, int p);

    void insert(Vertex* vertex) override;
    Vertex* extract_min() override;
    bool is_empty() override;
    void print_skiplist();
};

#endif //CONCURRENT_WORKSHOP_SKIPLIST_H
