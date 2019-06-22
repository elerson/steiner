/* 
 * File:   SteinerGenetic.h
 * Author: elerson
 *
 * Created on June 28, 2013, 9:51 PM
 */

#ifndef STEINERGENETIC_H
#define	STEINERGENETIC_H
#include <time.h>

#include "Graph.h"
#include "Prim.h"
#include "Dijkstra.h"
#include <vector>

struct Subject{
    bool* vertices;
    bool* terminals;
    double fitness;
    int N; 
    ~Subject(){
        delete[] vertices;
        delete[] terminals;
    }
    Subject* clone(){
        Subject* sub = new Subject(N);
        sub->N = this->N;
        sub->fitness = this->fitness;
        for(int i = 0 ; i < N; i++)
        {
            sub->vertices[i] = this->vertices[i];
            sub->terminals[i] = this->terminals[i];
        } 
        return sub;
    }
    Subject(int N){   
        fitness = -1;
        this->N = N;
        this->terminals = new bool[N];
        this->vertices = new bool[N];
    }
    
    Subject(int N,bool* terminals,int n){
        this->N = N;
        this->terminals = new bool[N];
        this->vertices = new bool[N];
        
        //std::cout << (unsigned)time(NULL) << std::endl;
        for(int i = 0; i < N ; i++){
            this->terminals[i] = terminals[i];
            if(this->terminals[i]){
                this->vertices[i] = true;
                continue;
            }            
            int v = rand() % 100;            
            if(v < n){
                vertices[i] = true;
            }else{
                vertices[i] = false;
            }
        }
        fitness = -1;
    }
    
    void Mutate(int n){
        //srand (time(NULL));
        int v;
        for(int i = 0; i < N ; i++){
            if(terminals[i]) continue;
            v = rand() % 100;
            if(v < n){
                vertices[i] = vertices[i]^true;
            }
        }
        fitness = -1;
    }
    
    static Subject** reproduct(Subject* s1,Subject* s2){
        
        Subject* ns1 = new Subject(s1->N);
        Subject* ns2 = new Subject(s1->N);
        //srand (time(NULL));
        int v = rand() % s1->N;
        for(int i = 0; i < s1->N; i++){
            if(i < v){
               ns1->terminals[i] = s1->terminals[i];
               ns1->vertices[i] = s1->vertices[i];
            }else{
               ns1->terminals[i] = s2->terminals[i]; 
               ns1->vertices[i] =  s2->vertices[i];
            }
            
            if(i < v){
               ns2->terminals[i] = s2->terminals[i];
               ns2->vertices[i] = s2->vertices[i];
            }else{
               ns2->terminals[i] = s1->terminals[i]; 
               ns2->vertices[i] =  s1->vertices[i];
            }            
        }
        
        Subject** nss = new Subject*[2]; 
        nss[0] = ns1;
        nss[1] = ns2;
        return nss;        
    }
    
    
    double generateFitness(AdjList** minDistances){
        if(fitness > 0) return fitness;
        
        std::vector<int> usedVertices;
        for(int i=0;i<N;i++)
            if(this->vertices[i])
                usedVertices.push_back(i);
        
        AdjList* graph = new AdjList(N,true);
        GraphNode* node;
        for(int i = 0;i < usedVertices.size();i++)
            for(int j = i+1 ; j < usedVertices.size(); j++){
                double dist =  minDistances[usedVertices.at(i)]->List[usedVertices.at(j)]->at(0)->w;
                node = new GraphNode(usedVertices.at(i),dist);
                graph->add(usedVertices.at(j),node);
            }
        //std::cout << N;
        //graph->print();
        Prim prim;
        AdjList* mst = prim.getMST(graph);
        fitness = mst->mstSize;
        delete mst;
        delete graph;
        
        return fitness;
    }
    
};

class SteinerGenetic {
public:
    SteinerGenetic();
    SteinerGenetic(const SteinerGenetic& orig);
    double calculateSteinerTree(AdjList* inGraph,std::vector<int>* terminals,int k);
    int selectionTournament(int N,std::vector<Subject*>* population,AdjList** minDistances);
    int selectionProportional(std::vector<double> sum);

    virtual ~SteinerGenetic();
private:

};

#endif	/* STEINERGENETIC_H */

