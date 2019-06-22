/* 
 * File:   Dijkstra.cpp
 * Author: elerson
 * 
 * Created on May 14, 2013, 6:32 PM
 */

#include "Dijkstra.h"
#include "cstdio"
#include "cstdlib"
#include "Heap.h"
#define INF 65000

Dijkstra::Dijkstra() {
}

Dijkstra::~Dijkstra() {
}

AdjList* Dijkstra::getMinDist(int root,AdjList* adj){
   
    AdjList* outdist = new AdjList(adj->size);
    //create a heap with nodes
    Heap heap(adj->size);
    HeapNode** nodeposition = new HeapNode*[adj->size];
    for(int i = 0; i < adj->size;i++){
        HeapNode* node = new HeapNode();
        node->pi = -1;
        node->node = i;
        node->key = INF;
        node->used = false;
        heap.insert(node);
        nodeposition[i] = node;
    }
    
    
    HeapNode* node = nodeposition[root];
    node->key = 0;    
    heap.decreaseKey(node);
    
    while(1)
    {
        HeapNode* aux = heap.removeHead();
        if(aux == NULL) break;
        aux->used = true;
        
        if(aux->pi >= 0){
            GraphNode* gnode = new GraphNode(aux->pi,aux->key);
            outdist->List[aux->node]->push_back(gnode);            
        }  
        
        
        //find the next node
        for(int i = 0; i < adj->List[aux->node]->size();i++){
            HeapNode* node = nodeposition[adj->List[aux->node]->at(i)->j];
            
            if(node->used) continue;
             
            if(adj->List[aux->node]->at(i)->w + aux->key < node->key){
                node->key = adj->List[aux->node]->at(i)->w + aux->key;
                node->pi = aux->node;
                heap.decreaseKey(node);
               
            }
        }        
    }
    return outdist;
}
