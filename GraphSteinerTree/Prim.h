/* 
 * File:   Prim.h
 * Author: elerson
 *
 * Created on May 14, 2013, 6:32 PM
 */

#ifndef PRIM_H
#define	PRIM_H
#include "Graph.h"

/*
    input -> adjacence list  vector<Node>
 *  output -> adjacence list
 */

class Prim {
public:
    Prim();
    AdjList* getMST(AdjList*);
    virtual ~Prim();
private:

};

#endif	/* PRIM_H */

