/* 
 * File:   Prim.cpp
 * Author: elerson
 * 
 * Created on May 14, 2013, 6:32 PM
 */

#include "Prim.h"
#include "cstdio"
#include "cstdlib"
#include "Heap.h"
#define INF 65000
Prim::Prim() {
}

AdjList* Prim::getMST(AdjList* adj){
 
    AdjList* mst = new AdjList(adj->size);
    
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
    
    
    HeapNode* node = nodeposition[0];
    node->pi = -1;
    node->key = 0;
    heap.decreaseKey(node);
   
    while(1)
    {
        HeapNode* aux = heap.removeHead();
        if(aux == NULL) break;
        
        
        aux->used = true;
        //create mst graph node
        if(aux->pi >= 0){
            GraphNode* gnode = new GraphNode(aux->pi,aux->key);
            mst->mstSize+= aux->key;
            mst->List[aux->node]->push_back(gnode);
        }       
       
         
        //find the next node
        for(int i = 0; i < adj->List[aux->node]->size();i++){
            HeapNode* node = nodeposition[adj->List[aux->node]->at(i)->j];
            if(node->used) continue;
            if(adj->List[aux->node]->at(i)->w < node->key){
                node->key = adj->List[aux->node]->at(i)->w;
                node->pi = aux->node;                 
                heap.decreaseKey(node);
            }
            
        }
       
        
    }
    
    for(int i = 0; i < adj->size;i++)
        delete nodeposition[i];
    delete[] nodeposition;
    
    return mst;
    
}

Prim::~Prim() {
}


