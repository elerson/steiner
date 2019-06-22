/* 
 * File:   Dijkstra.h
 * Author: elerson
 *
 * Created on May 14, 2013, 6:32 PM
 */

#ifndef DIJKSTRA_H
#define	DIJKSTRA_H
#include "Graph.h"

class Dijkstra {
public:
    Dijkstra();    
    AdjList* getMinDist(int,AdjList* adj);
    virtual ~Dijkstra();
private:

};

#endif	/* DIJKSTRA_H */

