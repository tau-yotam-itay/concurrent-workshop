//
//  graph.cpp
//  workshop
//
//  Created by Yotam Manne on 18/05/2019.
//  Copyright © 2019 Yotam Manne. All rights reserved.
//

#include "graph.hpp"

using namespace std;


class Vertex{
private:
    int key;
    unsigned int dist;
    neighbor* neighbors;
public:
    Vertex(int k){
        key = k;
        neighbors = NULL;
        dist = INT_MAX;
    }
    
    int get_key(){
        return key;
    };
    
    void add_neighbor(Vertex* v, int weight){
        neighbor* new_neighbor = new neighbor;
        new_neighbor->v = v;
        new_neighbor->weight = weight;
        new_neighbor->next = get_neighbors();
        neighbors = new_neighbor;
    };
    
    neighbor* get_neighbors(){
        return neighbors;
    };
    
    neighbor* get_neighbor(int key){
        neighbor* cur = neighbors;
        while(cur != NULL){
            if(cur->v->get_key() == key){
                return cur;
            }
            cur = cur->next;
        }
        return NULL;
    };
    
    void print_neighbors(){
        neighbor* cur = neighbors;
        printf("vertex %d: ",key);
        while(cur!=NULL){
            printf("(%d, %d), ",cur->v->get_key(),cur->weight);
            cur = cur->next;
        }
    };
};


//class Edge{
//private:
//    Vertex* v1;
//    Vertex* v2;
//    int weight;
//
//public:
//    Edge(Vertex* a, Vertex* b, int w){
//        v1 = a;
//        v2 = b;
//        weight = w;
//    }
//
//    int get_weight(){
//        return weight;
//    }
//
//
//    Vertex* get_vertex(){
//        return v1;
//    }
//
//    /*
//     returns the second vertex (the one that not given as argument)
//     */
//    Vertex* get_vertex(Vertex* v){
//        if(v != v1 && v != v2){
//            return NULL;
//        }
//        if(v == v1){
//            return v2;
//        }
//        return v1;
//    }
//};



class Graph{
private:
    int number_of_vertices;
    int number_of_edges;
    int source_vertex_key;
    vertex_list_node* vertex_list;
    
    void add_to_vertex_list(Vertex* v){
        vertex_list_node* new_vertex = new vertex_list_node;
        new_vertex->v = v;
        new_vertex->next = vertex_list;
        vertex_list = new_vertex;
        count++;
    };
    
    void add_edge(int key1, int key2, int weight){
        Vertex* v1 = get_vertex(key1);
        Vertex* v2 = get_vertex(key2);
        if(!v1){
            v1 = new Vertex(key1);
            add_to_vertex_list(v1);
        }
        if(!v2){
            v2 = new Vertex(key2);
            add_to_vertex_list(v2);
        }
        v1->add_neighbor(v2, weight);
        v2->add_neighbor(v1, weight);
    };
    
public:
    int count = 0;
    
    Graph(const char* filepath){
        vertex_list = NULL;
        ifstream fp(filepath);
        fp >> number_of_vertices >> number_of_edges >> source_vertex_key;
        int key1,key2,weight;
        while(fp >> key1 >> key2 >> weight){
            add_edge(key1, key2, weight);
        }
    };
    
    Vertex* get_vertex(int key){
        vertex_list_node* cur = vertex_list;
        while(cur != NULL){
            if(cur->v->get_key() == key){
                return cur->v;
            }
            cur = cur->next;
        }
        return NULL;
    };
    
    
    void print_graph(){
        printf("id, (key,weight)\n");
        vertex_list_node* cur = vertex_list;
        while(cur!=NULL){
            cur->v->print_neighbors();
            printf("\n");
            cur = cur->next;
        }
    };
};
