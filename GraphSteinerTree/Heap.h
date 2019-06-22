/* 
 * File:   Heap.h
 * Author: elerson
 *
 * Created on May 14, 2013, 6:47 PM
 */

#ifndef HEAP_H
#define	HEAP_H

struct HeapNode{
    int position;
    double key;
    int node;
    int pi;
    bool used;
};

class Heap {
public:
    Heap(int size);
    void insert(HeapNode*,bool makeHeap = true);
    HeapNode* removeHead();
    void decreaseKey(const HeapNode* node);    
    virtual ~Heap();
private:
    
void makeHeap(int);
HeapNode** heap;
int currentSize;
int maxSize;

};

#endif	/* HEAP_H */

