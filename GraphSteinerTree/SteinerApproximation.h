/* 
 * File:   SteinerApproximation.h
 * Author: Elerson Rubens da S. Santos
 * 
 * This file implements the approximantion proposed in 
 * Tighter Bounds for Graph Steiner Tree Approximation
 * from Gabriel Robins and Alex Zelikovsky
 * 
 * Created on May 25, 2013, 3:38 PM
 */

#ifndef STEINERAPPROXIMATION_H
#define	STEINERAPPROXIMATION_H

#include "Dijkstra.h"
#include "Prim.h"
#include "Graph.h"

#include<vector>
#include<list>
#include<algorithm>

class SteinerApproximation {
public:
    SteinerApproximation();
    AdjList* calculateSteinerTree(AdjList*,std::vector<int>* terminals,int);    
    virtual ~SteinerApproximation();
private:
    struct CostLost{
        int terminalsSet;
        int steinerSet;
        int cost;
        int loss;
        CostLost();
        CostLost(int terminal, int steiner,int cost, int loss){
            terminalsSet = terminal;
            steinerSet = steiner;
            this->cost = cost;
            this->loss = loss;
        }
        
        
    };
    
    //bool subset( std::vector<bool> bit_mask, std::size_t req_size,std::vector<int>& outvec );
    //bool nextbitmask( std::vector<bool>& bit_mask );
    AdjList* generateSteinerCost(AdjList**,std::vector<int>* terminals,std::vector<int>* steiners);
    AdjList* generateLoss(AdjList**,std::vector<int>* terminals,std::vector<int>* steiners);
    AdjList* generateCost(AdjList**,std::vector<int>* terminals,std::vector<int>* steiners);
    AdjList* generateCostT(AdjList*,std::vector<int>* terminals);
    AdjList* generateMst(AdjList*,AdjList*,AdjList**,std::vector<int>* terminals,std::vector<int>* steiners);
    void addVertices(int i, int j,AdjList*,AdjList*);
    bool removeSteiners(AdjList*,std::vector<int>* steiners);
    
    //generate subsets
    void create( std::list<int> l,std::vector< std::vector<int>* >& set);
    void subset(std::vector<int>& arr, int size, int left, int index, std::list<int> &l,std::vector< std::vector<int>* >& set);
    
    
    
};

#endif	/* STEINERAPPROXIMATION_H */

