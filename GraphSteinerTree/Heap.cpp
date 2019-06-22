/* 
 * File:   Heap.cpp
 * Author: elerson
 * 
 * Created on May 14, 2013, 6:47 PM
 */

#include "Heap.h"
#include "cstdio"
#include "cstdlib"
#include <iostream>

Heap::Heap(int size){
    size = size +1; // don't use index 0
    heap = (HeapNode**) new HeapNode*[size];
   
    for(int i = 0; i < size; i++)
        heap[i] = NULL; 
    
    currentSize = 1;
    maxSize = size;
}

void Heap::insert(HeapNode* node,bool makeHeap){
    heap[currentSize] = node;
    if(makeHeap==false){
        currentSize++;
        return;
    }
    
    int i = currentSize;
    node->position = currentSize;
    HeapNode* aux;
    int pos;
    while(i > 1)
    {      
       
       if(heap[i/2]->key > heap[i]->key){
          
           
          aux = heap[i/2];
          heap[i/2] = heap[i];
          heap[i] = aux;
          
          //save node position
          pos = heap[i]->position;
          heap[i]->position = heap[i/2]->position;
          heap[i/2]->position = pos;          
       }
       i/=2;
       
    }
    
    currentSize++;
}

HeapNode* Heap::removeHead(){
    
    if(currentSize == 1) return NULL;
    
    HeapNode* ret = heap[1];
    heap[1] = heap[currentSize-1];
    heap[1]->position = 1;
    heap[currentSize-1] = NULL;
    makeHeap(1);
    
    currentSize--;
    return ret;
        
}

void Heap::decreaseKey(const HeapNode* node){
    
    int i = node->position;   

    HeapNode* aux;
    int pos;
    while(i > 1)
    {      
       if(heap[i/2]->key > heap[i]->key){
          
          aux = heap[i/2];
          heap[i/2] = heap[i];
          heap[i] = aux;
          
          //save node position
          pos = heap[i]->position;
          heap[i]->position = heap[i/2]->position;
          heap[i/2]->position = pos;          
       }
       i/=2;
    }
    
}   

Heap::~Heap(){
    
    for(int i = 0; i < maxSize; i++)
       delete heap[i]; 
    
     delete[] heap;
}

void Heap::makeHeap(int i){    
    
    int l = 2*i;
    int r = 2*i + 1;
    int min = i;
    if((l < maxSize) && (heap[l] != NULL))
        if(heap[l]->key < heap[min]->key)
            min = l;
            
    if((r < maxSize) && (heap[r] != NULL))
        if(heap[r]->key < heap[min]->key)
            min = r;
    
    
    if(min == i) return;
    int pos;
    HeapNode* aux = heap[i]; 
    heap[i] = heap[min];
    heap[min] = aux;
    
    //save node position
    pos = heap[i]->position;
    heap[i]->position = heap[min]->position;
    heap[min]->position = pos; 
    
    makeHeap(min);
}