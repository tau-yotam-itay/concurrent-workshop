#include "skiplist.h"
#include <cstring>

#define EMPTY_QUEUE NULL


Skiplist_node::Skiplist_node(int level_arg, Vertex* vertex)
{
    dist = vertex->get_dist();
    level = level_arg;
    next_arr = new Skiplist_node*[level+1];
    memset(next_arr, 0, sizeof(Skiplist_node*)*(level+1));
    v = vertex;
    inserting = false;
};


Skiplist_node* Skiplist::create_node(int level_arg, Vertex* vertex){
    return new Skiplist_node(level_arg, vertex);
    //switch with debra
}


bool Skiplist_node::is_deleted() {
    if(((uintptr_t)(this)) & 1){
        return true;
    }
    return false;
}

Skiplist_node* Skiplist_node::get_marked_ptr() {
    return (Skiplist_node *)(((uintptr_t)(this)) | 1);
}

Skiplist_node* Skiplist_node::get_unmarked_ptr() {
    return (Skiplist_node *)(((uintptr_t)(this)) & ~1);
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


Skiplist::Skiplist(int max_lvl, float prob, int offset, int p) : Priority_Queue(p)
{
    max_level = max_lvl;
    next_level_prob = prob;
    bound_offset = offset;
    nlevels = 1;

    Vertex* head_v = new Vertex(INT_MIN, INT_MIN);
    Vertex* tail_v = new Vertex(INT_MAX, INT_MAX);
    head = new Skiplist_node(max_lvl, head_v);
    tail = new Skiplist_node(max_lvl, tail_v);
    for(int i = 0; i < max_lvl; i++){
        head->get_next_arr()[i] = tail;
    }
};

bool Skiplist::is_empty(){
    Skiplist_node* node = head->get_next_arr()[0];
    while(node->is_deleted()){
        node = node->get_unmarked_ptr()->get_next_arr()[0];
    }
    return node == tail;
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


void Skiplist::destroy_node(Skiplist_node* node){

}

Skiplist_node* Skiplist::locate_preds(int dist, Skiplist_node** preds, Skiplist_node** succs){
    int m = max_level;
    int i = m - 1;
    Skiplist_node *pred = head, *cur, *del = NULL;
    bool d;
    // pred = x;    cur = x_next;
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

void Skiplist::insert(Vertex* vertex){
    //enter quicent state
    int height = random_level(), i = 1;
    printf("insert, height= %d\n",height);
    Skiplist_node* new_node = create_node(height, vertex);
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
    //exit quicent state
}

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

Vertex* Skiplist::extract_min(){
    printf("extract min\n");
    Skiplist_node *x = head, *newhead = NULL, *obshead = x->get_next_arr()[0], *next, *cur;
    int offset = 0;
    bool d = true;
    Vertex* v;

    do{
        next = x->get_next_arr()[0];
        //d = next->is_deleted(); //need this line?
        if(next->get_unmarked_ptr() == tail){
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
    if(offset < bound_offset){
        return v;
    }
    if(newhead == NULL){
        newhead = x;
    }
    if(__sync_bool_compare_and_swap(&head->get_next_arr()[0], obshead, newhead->get_marked_ptr())){ //maybe need to get marked also obshead?
        restructure();
        cur = obshead->get_unmarked_ptr();
        while(cur != newhead){
            next = cur->get_unmarked_ptr()->get_next_arr()[0];
            destroy_node(cur);
            cur = next->get_unmarked_ptr();
        }
    }
    return v;
}

void Skiplist::print_skiplist() {
    printf("\n*****Skip List*****\n"); 
    for (int i=0;i<=max_level;i++) 
    { 
        Skiplist_node *node = head->get_next_arr()[i]; 
        printf("Level %d: ",i); 
        // fflush(stdout);
        while (node != NULL) 
        { 
            if(!node->is_deleted()){
                printf("%d ",node->get_dist());
                // fflush(stdout); 
            }
            node = node->get_unmarked_ptr();
            node = node->get_next_arr()[i]; 
        } 
        printf("\n");
        fflush(stdout);
    }
    printf("********\n");
}