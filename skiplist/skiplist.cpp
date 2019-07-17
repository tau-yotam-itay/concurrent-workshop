#include "skiplist.h"
#include <cstring>

#define EMPTY_QUEUE NULL


Skiplist_node::Skiplist_node(int level_arg, Vertex* vertex)
{
    dist = v->get_dist();
    level = level_arg;
    next_arr = new Skiplist_node*[level+1];
    memset(next_arr, 0, sizeof(Skiplist_node*)*(level+1));
    v = vertex;
};


Skiplist_node* Skiplist::create_node(int level_arg, Vertex* vertex){
    return new Skiplist_node(level_arg, vertex);
    //switch with debra
}


bool Skiplist_node::is_deleted() {
    if( (uintptr_t)next_arr[0] & 1){
        return true;
    }
    return false;
}

Skiplist_node* Skiplist_node::get_marked_ptr() {
    return (Skiplist_node*) ( (uintptr_t)next_arr[0] | 1 );
}

Skiplist_node* Skiplist_node::get_unmarked_ptr() {
    return (Skiplist_node*) ( (uintptr_t)next_arr[0] & ~1 );
}

int Skiplist_node::get_dist() {
    return dist;
}

int Skiplist_node::get_level() {
    return level;
}

bool Skiplist_node::is_inserting() {
    return inserting;
}

Skiplist_node **Skiplist_node::get_next_arr() {
    return next_arr;
}

Vertex *Skiplist_node::get_vertex() {
    return v;
}

void Skiplist_node::set_inserting(bool insert) {
    inserting = insert;
}


Skiplist::Skiplist(int max_lvl, float prob, int offset)
{
    max_level = max_lvl;
    next_level_prob = prob;
    bound_offset = offset;
    nlevels = 0;

    head = new Skiplist_node(max_lvl, NULL);
};


void Skiplist::insert(Vertex* vertex){
    //enter quicent state
    int height = random_level(), i = 1;
    Skiplist_node* new_node = create_node(height, vertex);
    Skiplist_node* del_node = NULL;
    Skiplist_node *preds[max_level], *succs[max_level];

    new_node->set_inserting(true);
    do{
        del_node = locate_preds(vertex->get_dist(), preds, succs);
        new_node->get_next_arr()[0] = succs[0];
    }while(__sync_bool_compare_and_swap(&preds[0]->get_next_arr()[0], succs[0], new_node));

    while(i < height){
        new_node->get_next_arr()[i] = succs[i];
        if(new_node->is_deleted() || succs[i]->is_deleted() || succs[i] == del_node){
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
    //exit quicent state
}



Skiplist_node* Skiplist::locate_preds(int dist, Skiplist_node** preds, Skiplist_node** succs){
    int i = nlevels - 1;
    Skiplist_node *pred = head, *cur, *del = NULL;
    bool d;

    while(i >= 0){
        cur = pred->get_next_arr()[i];
        d = cur->is_deleted();
        cur = cur->get_unmarked_ptr();
        while(cur->get_dist() < dist || cur->is_deleted() ||
                (d && i == 0)){
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
    return NULL;
}

//credit: geeksforgeeks.com
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


void Skiplist::restructure(){
    int i = nlevels - 1;
    Skiplist_node *pred, *h, *cur;

    while(i>0){
        h = head->get_next_arr()[i];
        cur = pred->get_next_arr()[i];
        if(!h->is_deleted()){
            i--;
            continue;
        }
        while(cur->is_deleted()){
            pred = cur;
            cur = pred->get_next_arr()[i];
        }
        if(__sync_bool_compare_and_swap(&head->get_next_arr()[i], h, cur)){
            i--;
        }
    }
}

void Skiplist::destroy_node(Skiplist_node* node){

}

Vertex* Skiplist::extract_min(){
    Skiplist_node *x = head, *newhead = NULL, *obshead = x->get_next_arr()[0], *next, *cur;
    int offset = 0;
    bool d = true;
    Vertex* v;

    do{
        next = x->get_next_arr()[0];
        d = x->is_deleted();
        if(next == tail){
            return EMPTY_QUEUE;
        }
        if (x->is_inserting() && newhead == NULL){
            newhead = x;
        }
        next = __sync_fetch_and_or(&x->get_next_arr()[0], 1);
        d = x->is_deleted();
        offset++;
        x = next;
    }while(!d);

    v = x->get_vertex();
    if(offset < bound_offset){
        return v;
    }
    if(newhead == NULL){
        newhead = x;
    }
    if(__sync_bool_compare_and_swap(&head->get_next_arr()[0], obshead, newhead->get_marked_ptr())){
        restructure();
        cur = obshead;
        while(cur != newhead){
            next = cur->get_next_arr()[0];
            destroy_node(cur);
            cur = next;
        }
    }
    return v;
}
