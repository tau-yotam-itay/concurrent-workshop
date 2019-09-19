#include "skiplist.h"
#include <cstring>

#define EMPTY_QUEUE NULL

/**
 * Skiplist_node constructor (empty - for debra usage)
 */
Skiplist_node::Skiplist_node(){}

/**
 * Sets (initialization) fields of a Skiplist_node
 * @param level_arg the node's level in the Skiplist
 * @param vertex
 */
void Skiplist_node::set_properties(int level_arg, Vertex* vertex)
{
    dist = vertex->get_dist();
    level = level_arg;
    next_arr = new Skiplist_node*[level+1];
    memset(next_arr, 0, sizeof(Skiplist_node*)*(level+1));
    v = vertex;
    inserting = false;
}

/**
 * Create new Skiplist_node using debra.
 * @param level_arg the node's level in the skiplist
 * @param vertex
 * @return pointer to the newly created node
 */
Skiplist_node* Skiplist::create_node(int level_arg, Vertex* vertex, int tid){
    Skiplist_node* new_node = mgr->template allocate<Skiplist_node>(tid);
    new_node->set_properties(level_arg,vertex);
    return new_node;
}

/**
 * Free node's used memory space using debra
 * @param node destroyed node
 */
void Skiplist::destroy_node(Skiplist_node* node, int tid){
    mgr->retire(tid,node);
}

/**
 * @return true iff node is logically deleted
 */
bool Skiplist_node::is_deleted() {
    if(((uintptr_t)(this)) & 1){
        return true;
    }
    return false;
}

/**
 * @return true iff node is being in an insertion process to the skiplist data structure
 */
bool Skiplist_node::is_inserting() {
    return inserting;
}

/**
 * Set 'inserting' field according to the param.
 * @param insert true iff node is being in an insertion process to the skiplist data structure
 */
void Skiplist_node::set_inserting(bool insert) {
    inserting = insert;
}

/**
 * mark node as logically deleted
 * @return pointer to the 'deleted' node
 */
Skiplist_node* Skiplist_node::get_marked_ptr() {
    return (Skiplist_node *)(((uintptr_t)(this)) | 1);
}

/**
 * unmark node from being logically deleted
 * @return pointer to the 'undeleted' node
 */
Skiplist_node* Skiplist_node::get_unmarked_ptr() {
    return (Skiplist_node *)(((uintptr_t)(this)) & ~1);
}

/**
 * @return pointer to next pointed node's array (levels array)
 */
Skiplist_node **Skiplist_node::get_next_arr() {
    return next_arr;
}

/**
 * @return node's vertex field
 */
Vertex *Skiplist_node::get_vertex() {
    return v;
}

/**
 * @return node's vertex's distance from source vertex
 */
int Skiplist_node::get_dist() {
    return dist;
}

/**
 * Skiplist constructor
 * @param max_lvl maximum level - maximum size of skiplist node array
 * @param prob probability of a node to be at next level given it's current maximal level
 * @param offset - how many logically deleted nodes are allowed in the head of the skiplist
 * @param p total number of threads
 */
Skiplist::Skiplist(int max_lvl, float prob, int offset, int p) : Priority_Queue(p)
{
    max_level = max_lvl;
    next_level_prob = prob;
    bound_offset = offset;
    mgr = new record_manager<reclaimer_debra<>,allocator_new<>,pool_none<>,Skiplist_node>(p,SIGQUIT);

    Vertex* head_v = new Vertex(INT_MIN, INT_MIN);
    Vertex* tail_v = new Vertex(INT_MAX, INT_MAX);
    head = new Skiplist_node();
    head->set_properties(max_lvl, head_v);
    tail = new Skiplist_node();
    tail->set_properties(max_lvl, tail_v);
    for(int i = 0; i < max_lvl; i++){
        head->get_next_arr()[i] = tail;
    }
}

/**
 * Free Skiplist's dynamically allocated memory space
 */
Skiplist::~Skiplist(){
    delete mgr;
    delete head;
    delete tail;
}

/**
 * Randomize maximum level for a specific node in Skiplist
 * @return randomized level
 * credit: geeksforgeeks.com
 */
int Skiplist::random_level() {
    float r = (float)rand()/RAND_MAX;
    int lvl = 0;
    while (r < next_level_prob && lvl < max_level)
    {
        lvl++;
        r = (float)rand()/RAND_MAX;
    }
    return lvl;
}

/**
 * @return true iff Skiplist data structure is empty
 */
bool Skiplist::is_empty(){
    Skiplist_node* node = head->get_next_arr()[0];
    while(node->is_deleted()){
        node = node->get_unmarked_ptr()->get_next_arr()[0];
    }
    return node == tail;
}

/**
 * Locates the predecessors and successors of a new node that is to be inserted
 * @param dist node's distance from source node
 * @param preds node's predecessors list
 * @param succs node's successors list
 * @return pointer to the last (at the moment of traversal) deleted node of the deleted prefix
 */
Skiplist_node* Skiplist::locate_preds(int dist, Skiplist_node** preds, Skiplist_node** succs){
    int m = max_level;
    int i = m - 1;
    Skiplist_node *pred = head, *cur, *del = NULL;
    bool d;

    while(i >= 0){
        cur = pred->get_next_arr()[i];
        d = cur->is_deleted();
        cur = cur->get_unmarked_ptr();
        while(cur->get_dist() < dist || cur->get_next_arr()[0]->is_deleted() ||
              ( (i == 0) && d ) ){
            if(d && i == 0){
                del = cur;
            }
            pred = cur;
            cur = pred->get_next_arr()[i];
            d = cur->is_deleted();
            cur = cur->get_unmarked_ptr();
        }
        preds[i] = pred;
        succs[i] = cur;
        i--;
    }
    return del;
}

/**
 * Insert new node to Skiplist data structure
 * @param vertex node's vertex field
 */
void Skiplist::insert(Vertex* vertex, int tid){
    //enter quiescent state
    mgr->enterQuiescentState(tid);
    int height = random_level(), i = 1;
    Skiplist_node* new_node = create_node(height, vertex, tid);
    Skiplist_node* del_node = NULL;
    Skiplist_node **preds = (Skiplist_node**)calloc(max_level,sizeof(Skiplist_node*));
    Skiplist_node **succs = (Skiplist_node**)calloc(max_level,sizeof(Skiplist_node*));
    if(!preds || !succs){
        perror("calloc");
        exit(EXIT_FAILURE);
    }

    new_node->set_inserting(true);
    do{
        del_node = locate_preds(vertex->get_dist(), preds, succs);
        // return if key already exists
        if (succs[0]->get_dist() == vertex->get_dist() && succs[0]->get_vertex() == vertex &&
        !(preds[0]->get_next_arr()[0]->is_deleted()) && preds[0]->get_next_arr()[0] == succs[0]) {
            new_node->set_inserting(false);
            destroy_node(new_node, tid);
            mgr->leaveQuiescentState(tid);
            return;
        }
        new_node->get_next_arr()[0] = succs[0];

    }while(!__sync_bool_compare_and_swap(&preds[0]->get_next_arr()[0], succs[0], new_node));

    while(i <= height){
        new_node->get_next_arr()[i] = succs[i];
        if(succs[i] == del_node || new_node->get_next_arr()[0]->is_deleted() || succs[i]->get_next_arr()[0]->is_deleted()){
            break;
        }
        if(__sync_bool_compare_and_swap(&preds[i]->get_next_arr()[i], succs[i], new_node)){
            i++;
        }
        else{
            del_node = locate_preds(vertex->get_dist(), preds, succs);
            if(succs[0] != new_node){
                break;
            }
        }
    }
    new_node->set_inserting(false);
    free(preds);
    free(succs);
    mgr->leaveQuiescentState(tid);
    //exit quiescent state
}

/**
 * Updates the pointers of q.head, except for level 0, starting from the highest level.
 */
void Skiplist::restructure(){
    int i = max_level - 1;
    Skiplist_node *pred = head, *h, *cur;

    while(i>0){
        h = head->get_next_arr()[i];
        cur = pred->get_next_arr()[i];
        if(!h->get_next_arr()[0]->is_deleted()){
            i--;
            continue;
        }
        while(cur->get_next_arr()[0]->is_deleted()){
            pred = cur;
            cur = pred->get_next_arr()[i];
        }
        if(__sync_bool_compare_and_swap(&head->get_next_arr()[i], h, cur)){
            i--;
        }
    }
}

/**
 * Extract the minimal node from Skiplist data structure
 * (minimal distance of a node to the source vertex)
 * @return pointer to the Vertex field of the minimal node
 */
Vertex* Skiplist::extract_min(int tid){
    mgr->enterQuiescentState(tid);
    Skiplist_node *x = head, *newhead = NULL, *obshead = x->get_next_arr()[0], *next, *cur;
    int offset = 0;
    bool d = true;
    Vertex* v;

    do{
        next = x->get_next_arr()[0];
        if(next->get_unmarked_ptr() == tail){
            mgr->leaveQuiescentState(tid);
            return EMPTY_QUEUE;
        }
        if (x->is_inserting() && newhead == NULL){
            newhead = x;
        }
        next = __sync_fetch_and_or(&x->get_next_arr()[0], 1);
        d = next->is_deleted();
        offset++;
        x = next->get_unmarked_ptr();
    }while(d);

    v = x->get_vertex();

    if(newhead == NULL){
        newhead = x;
    }
    if(offset < bound_offset){
        mgr->leaveQuiescentState(tid);
        return v;
    }

    if(__sync_bool_compare_and_swap(&head->get_next_arr()[0], obshead, newhead->get_marked_ptr())){
        restructure();
        cur = obshead->get_unmarked_ptr();
        while(cur != newhead){
            next = cur->get_unmarked_ptr()->get_next_arr()[0];
            destroy_node(cur, tid);
            cur = next->get_unmarked_ptr();
        }
    }
    mgr->leaveQuiescentState(tid);
    return v;
}
