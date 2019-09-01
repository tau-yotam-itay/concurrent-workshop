#ifndef CONCURRENT_WORKSHOP_SKIPLIST_H
#define CONCURRENT_WORKSHOP_SKIPLIST_H

#include "../priority_queue.h"

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
    Skiplist_node();
    void set_properties(int level_arg, Vertex* vertex);
    bool is_deleted();
    bool is_inserting();
    void set_inserting(bool insert);
    Skiplist_node* get_marked_ptr();
    Skiplist_node* get_unmarked_ptr();
    Skiplist_node **get_next_arr();
    Vertex *get_vertex();
    int get_dist();
};


class Skiplist: public Priority_Queue
{

private:
    //fields:
    int max_level;
    int bound_offset;
    float next_level_prob;
    Skiplist_node *head;
    Skiplist_node *tail;
    record_manager<reclaimer_debra<>,allocator_new<>,pool_none<>,Skiplist_node> * mgr;

    //functions:
    int random_level();
    Skiplist_node* create_node(int level_arg, Vertex* vertex, int tid);
    void destroy_node(Skiplist_node* node, int tid);
    Skiplist_node* locate_preds(int dist, Skiplist_node** preds, Skiplist_node** succs);
    void restructure();

public:
    Skiplist(int max_lvl, float prob, int offset, int p);
    ~Skiplist();
    void free_sentinels();
    bool is_empty() override;
    void insert(Vertex* vertex, int tid) override;
    Vertex* extract_min(int tid) override;
};

#endif //CONCURRENT_WORKSHOP_SKIPLIST_H