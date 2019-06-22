/* 
 * File:   Graph.h
 * Author: elerson
 *
 * Created on June 1, 2013, 10:56 AM
 */

#ifndef GRAPH_H
#define	GRAPH_H

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream> 
#include <cstdlib>


struct GraphNode{    
    int j;
    double w;
    GraphNode(int j,double w){
        this->j = j;
        this->w = w;
    }    
};

struct AdjList{
    std::vector<GraphNode*>** List;
    int size;
    bool undirected;
    double mstSize;
    AdjList(){}
    AdjList(int size, bool undirected = false){
        List = (std::vector<GraphNode*>**) new std::vector<GraphNode*>*[size];
        for(int i = 0; i < size;i++)
            List[i] = new std::vector<GraphNode*>;
        
        this->size = size;
        this->undirected = undirected;
        this->mstSize = 0;        
    };
    
    void add(int i,GraphNode* node){
        List[i]->push_back(node);
        if(undirected){
            GraphNode* node2 = new GraphNode(i,node->w);
            List[node->j]->push_back(node2);
        }
    }
    
    void print(){        
        for(int i = 0 ; i < size; i++)
        {
             std::cout << i <<" <- ";
            for(int j = 0; j < List[i]->size();j++)
            {
                std::cout << "("<<List[i]->at(j)->j <<","<< List[i]->at(j)->w << ") ";
            }   
             std::cout << std::endl;
        }          
        std::cout << "mstSize " <<mstSize<<std::endl;        
    }
    
    ~AdjList(){
        for(int i = 0; i < size; i++){
            for(int j = 0 ; j < List[i]->size(); j++)
                delete List[i]->at(j);
            List[i]->clear();
            delete List[i];
        }            
       delete[] List;
    }
    
};



#endif	/* GRAPH_H */

