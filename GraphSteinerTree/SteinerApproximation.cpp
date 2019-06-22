/* 
 * File:   SteinerApproximation.cpp
 * Author: Elerson Rubens da S. Santos
 * 
 * This file implements the approximantion proposed in 
 * Tighter Bounds for Graph Steiner Tree Approximation
 * from Gabriel Robins and Alex Zelikovsky
 * 
 * Created on May 25, 2013, 3:38 PM
 */

#include "SteinerApproximation.h"

SteinerApproximation::SteinerApproximation() {
}

AdjList* SteinerApproximation::calculateSteinerTree(AdjList* inGraph,std::vector<int>* terminals,int k){
    
    AdjList* outGraph = new AdjList(inGraph->size);
    Prim prim;
    Dijkstra dijkstra;
    
    //calculate the min path bettwen all vertices
    
    AdjList** minDistances = new AdjList*[inGraph->size];
    for(int i =0; i < inGraph->size;i++){
        minDistances[i] = dijkstra.getMinDist(i,inGraph);
    }
   
    //create a complete graph only with the terminal in the distance network
    AdjList* terminalGraph = new AdjList(inGraph->size,true);
    int terminal1,terminal2;
    GraphNode* node;
    
    for(int i =0; i < terminals->size();i++){        
        for(int j = i+1; j < terminals->size();j++){
               terminal1 =  terminals->at(i);
               terminal2 =  terminals->at(j);
               if(terminal1 == terminal2) continue;
               //std::cout << "teste" <<inGraph->size << " "<< minDistances[terminal1]->List[terminal2]->size() <<std::endl;
               double dist =  minDistances[terminal1]->List[terminal2]->at(0)->w;
               
               node = new GraphNode(terminal1,dist);               
               terminalGraph->add(terminal2,node);        
        }
    }

    //return NULL;
    //calculate the mst with the previous graph -> this is a 2-approximantion for the steiner problem
    AdjList* mst2apGraph = prim.getMST(terminalGraph);
    //mst2apGraph->print();
    
    //find steiner points
    bool steinerArray[inGraph->size];
    std::vector<int> steiners;
    for(int i =0; i < inGraph->size; i++)
        steinerArray[i] = true;
    
    for(int i=0; i< terminals->size() ; i++)
        steinerArray[terminals->at(i)] = false;
    
    for(int i =0; i < inGraph->size; i++)
        if(steinerArray[i])
                steiners.push_back(i);
    
    //generate all sets of 3 to k terminals  
    //std::cout << "Generating terminals set" << std::endl;
    std::vector< std::vector<int>* > terminalSet;
    std::vector< int > terminalSetInitP;
    terminalSetInitP.push_back(0);
    
    for(int i = 3; i <= k ; i++){
        /*std::vector<bool> bit_mask(terminals->size());       
        do 
        {  
            std::vector<int> vec;
            bool genetated = this->subset( bit_mask, i, vec);
            if(genetated) {
                
                std::vector<int>* termvec = new std::vector<int>;
                for(int h = 0 ; h < vec.size() ; h++)
                   termvec->push_back(terminals->at(vec.at(h)));

                terminalSet.push_back(termvec);
            
            }           
                
        } while( nextbitmask(bit_mask) ) ;*/
        
        std::list<int> lt; 
        subset(*terminals,terminals->size(),k,0,lt,terminalSet);
        terminalSetInitP.push_back(terminalSet.size());
    }
    
    
    
    //generate all set of 1 to k-2 steiner points
    std::vector< std::vector<int>* > steinerSet;
    std::vector<int> steinerSetInitP;
    steinerSetInitP.push_back(0);
    for(int i = 0 ; i < steiners.size(); i++)
    {
        std::vector<int>* steinvec = new std::vector<int>;
        steinvec->push_back(steiners.at(i));
        steinerSet.push_back(steinvec);
    }    
    steinerSetInitP.push_back(steinerSet.size());
    //std::cout << "Generating steiner set " << std::endl;
    for(int i = 2; i <= k-2 ; i++){
        /*std::vector<bool> bit_mask(steiners.size());
        do 
        {
            std::vector<int> vec;
            bool genetated = subset( bit_mask, i, vec);
            if(genetated){
                
                std::vector<int>* steinvec = new std::vector<int>;
                for(int h = 0 ; h < vec.size() ; h++)
                   steinvec->push_back(steiners.at(vec.at(h)));                
                
                steinerSet.push_back(steinvec);
            }
           
                
        } while( nextbitmask(bit_mask) ) ;*/
        std::list<int> lt;       
        subset(steiners,steiners.size(),k,0,lt,steinerSet);
        steinerSetInitP.push_back(steinerSet.size());
    }
    
    // generate the cost lost for each possible pair of terminalSet and SteinerSet    
   // std::cout << "generation loos and cost - " <<steinerSet.size()*terminals->size() << std::endl;
    std::vector<CostLost* > costlostVector;    
    CostLost* costlost;
    for(int i = 1; i <= k-2; i++){
        for(int t = terminalSetInitP.at(i-1); t < terminalSetInitP.at(i);t++){
            for(int s = steinerSetInitP.at(i-1); s < steinerSetInitP.at(i);s++){
               
                AdjList* adjcost = generateCost(minDistances,terminalSet.at(t),steinerSet.at(s));                
                AdjList* adjloss = generateLoss(minDistances,terminalSet.at(t),steinerSet.at(s));
                AdjList* adjstcost = generateSteinerCost(minDistances,terminalSet.at(t),steinerSet.at(s));
                
                
                int cost = adjcost->mstSize;
                int loss = adjloss->mstSize;
                 //std::cout << cost << " " << adjstcost->mstSize << std::endl;
                if(cost < adjstcost->mstSize){
                    costlost = new CostLost(t,s,cost,loss);
                    costlostVector.push_back(costlost);
                    //std::cout << cost << " " << loss << std::endl;                    
                }
                
                delete adjcost;
                delete adjloss;
                delete adjstcost;
                //std::cout << costlostVector.size() << std::endl; 
            }
        }
    }
    
    
        
    
    //finally compute uses the algorithm from the paper
    //mst2apGraph
    //std::cout << "finding solution" << std::endl;
    std::vector<int> steinersToSolution;
    
    while(1){
        double r;
        int bestcostlossSteinerSet = 0;
        
        for(int i = 0; i < costlostVector.size(); i++){
            costlost = costlostVector.at(i);
            AdjList* newmst = generateCostT(mst2apGraph,terminalSet.at(costlost->terminalsSet));
            int newmstcontractedK = newmst->mstSize;
            
            double newr = (mst2apGraph->mstSize - newmstcontractedK - costlost->cost)/costlost->loss;
            
            if(i == 0) r = newr;
            if(newr > r){
                r = newr;
                bestcostlossSteinerSet = costlost->steinerSet;
                
            }
            
            //std::cout << newr << " "  << std::endl;
            
            delete newmst;
        }
        
        if(r <= 0) break; 
        //std::cout << r << std::endl;
        delete mst2apGraph;     
        for(int i = 0 ; i <steinerSet.at(bestcostlossSteinerSet)->size();i++)
                steinersToSolution.push_back(steinerSet.at(bestcostlossSteinerSet)->at(i));                
                
        mst2apGraph = generateCost(minDistances,terminals,&steinersToSolution); 
       // std::cout << mst2apGraph->mstSize << std::endl;
    }

    //the end -> return the graph to its own space
    //std::cout << "getting final solution" << std::endl;
    AdjList* out = generateMst(inGraph,mst2apGraph,minDistances,terminals,&steiners);
    //out->print();
    return out;
}


AdjList* SteinerApproximation::generateMst(AdjList* originalGraph,AdjList* steinerI,AdjList** minDistances,std::vector<int>* terminals,std::vector<int>* steiners){

    AdjList* outmst = new AdjList(originalGraph->size,true); 
    //for each edge take the min path ffrom minDistances
    for(int i = 0; i < steinerI->size; i++){
        
        if(steinerI->List[i]->size() > 0){
            int j = steinerI->List[i]->at(0)->j;
            //std::cout << i << " " << j << std::endl;
            addVertices(i,j,minDistances[i],outmst);
        }
    }
    Prim prim;
    AdjList* mst =  prim.getMST(outmst);
    while(removeSteiners(mst,steiners)); // remove the degree onde steiner vertices
    return mst;
}

void SteinerApproximation::addVertices(int i, int j,AdjList* distGraph,AdjList* outgraph){
    
    while(true){
        //if(distGraph->List[j]->size() == 0) break;
        int nj = distGraph->List[j]->at(0)->j;
        double w;
        if(distGraph->List[nj]->size() == 0)
        {
             w = distGraph->List[j]->at(0)->w; 
             GraphNode *node= new GraphNode(nj,w);
             outgraph->add(j,node);
        }else{
             w = distGraph->List[j]->at(0)->w - distGraph->List[nj]->at(0)->w;
             GraphNode *node= new GraphNode(nj,w);
             outgraph->add(j,node);
        }
        
        //std::cout << j <<" " << nj << std::endl;
        j = nj;
        
        if(i == j) break;
    }
    
    
}

AdjList* SteinerApproximation::generateSteinerCost(AdjList** minDistances,std::vector<int>* terminals,std::vector<int>* steiners){
   
   
    AdjList* verticeGraph = new AdjList(minDistances[0]->size,true);
    int vertice1,vertice2;
    GraphNode* node;
    //steiners terminals
    for(int i =0; i < terminals->size();i++){        
        for(int j = 0; j < steiners->size();j++){
               vertice1 =  terminals->at(i);
               vertice2 =  steiners->at(j);
              
               if(vertice1 == vertice2) continue;
               
               double dist =  minDistances[vertice1]->List[vertice2]->at(0)->w;
                
               //node = new GraphNode(vertice1,dist);               
               //verticeGraph->add(vertice2,node);        
        }
    }

    //steiners steiners
    
    for(int i =0; i < steiners->size();i++){        
        for(int j = i+1; j < steiners->size();j++){
               vertice1 =  steiners->at(i);
               vertice2 =  steiners->at(j);
               
               if(vertice1 == vertice2) continue;
               
               double dist =  minDistances[vertice1]->List[vertice2]->at(0)->w;
               
               //node = new GraphNode(vertice1,dist);               
               //verticeGraph->add(vertice2,node);        
        }
    }
    //terminals terminals
    for(int i =0; i < terminals->size();i++){        
        for(int j = i+1; j < terminals->size();j++){
               vertice1 =  terminals->at(i);
               vertice2 =  terminals->at(j);
               if(vertice1 == vertice2) continue;
               
               double dist =  minDistances[vertice1]->List[vertice2]->at(0)->w;
               
               node = new GraphNode(vertice1,dist);               
               verticeGraph->add(vertice2,node);        
        }
    }
    
    Prim prim;
    AdjList* mst = prim.getMST(verticeGraph);

    delete verticeGraph;
    return mst;
}

AdjList* SteinerApproximation::generateLoss(AdjList** minDistances,std::vector<int>* terminals,std::vector<int>* steiners){
   
   
    AdjList* verticeGraph = new AdjList(minDistances[0]->size,true);
    int vertice1,vertice2;
    GraphNode* node;
    //steiners terminals
    for(int i =0; i < terminals->size();i++){        
        for(int j = 0; j < steiners->size();j++){
               vertice1 =  terminals->at(i);
               vertice2 =  steiners->at(j);
               if(vertice1 == vertice2) continue;
               
               double dist =  minDistances[vertice1]->List[vertice2]->at(0)->w;
               
               node = new GraphNode(vertice1,dist);               
               verticeGraph->add(vertice2,node);        
        }
    }

    //steiners steiners
    for(int i =0; i < steiners->size();i++){        
        for(int j = i+1; j < steiners->size();j++){
               vertice1 =  steiners->at(i);
               vertice2 =  steiners->at(j);
               if(vertice1 == vertice2) continue;
               
               double dist =  minDistances[vertice1]->List[vertice2]->at(0)->w;
               
               node = new GraphNode(vertice1,dist);               
               verticeGraph->add(vertice2,node);        
        }
    }
    //terminals terminals
    for(int i =0; i < terminals->size();i++){        
        for(int j = i+1; j < terminals->size();j++){
               vertice1 =  terminals->at(i);
               vertice2 =  terminals->at(j);
               if(vertice1 == vertice2) continue;
               
               node = new GraphNode(vertice1,0);               
               verticeGraph->add(vertice2,node);        
        }
    }
    
    Prim prim;
    AdjList* mst = prim.getMST(verticeGraph);

    delete verticeGraph;
    return mst;
}

AdjList* SteinerApproximation::generateCost(AdjList** minDistances,std::vector<int>* terminals,std::vector<int>* steiners){
    
    
    std::vector<int> vertices;
    for(int i=0;i < terminals->size();i++)
        vertices.push_back(terminals->at(i));
    
    for(int i=0;i < steiners->size();i++)
        vertices.push_back(steiners->at(i));
    
    AdjList* verticeGraph = new AdjList(minDistances[0]->size,true);
    int vertice1,vertice2;
    GraphNode* node;
    for(int i =0; i < vertices.size();i++){        
        for(int j = i+1; j < vertices.size();j++){
               vertice1 =  vertices.at(i);
               vertice2 =  vertices.at(j);
               if(vertice1 == vertice2) continue;
               
               double dist =  minDistances[vertice1]->List[vertice2]->at(0)->w;
               
               node = new GraphNode(vertice1,dist);               
               verticeGraph->add(vertice2,node);        
        }
    }
    
    Prim prim;
    AdjList* mst = prim.getMST(verticeGraph);
    delete verticeGraph;
    return mst;

}

AdjList* SteinerApproximation::generateCostT(AdjList* adj,std::vector<int>* terminals){
    
    AdjList* verticeGraph = new AdjList(adj->size,true);
    bool teminalE[adj->size];
    for(int i = 0; i < adj->size; i++)
        teminalE[i] = false;
    
    for(int i = 0; i < terminals->size(); i++)
        teminalE[terminals->at(i)] = true;
    
    GraphNode* node;
    for(int i = 0; i < adj->size; i++){
        for(int j = 0 ; j < adj->List[i]->size(); j++)
        {
            if(teminalE[i]&&teminalE[adj->List[i]->at(j)->j]){            
                node = new GraphNode(adj->List[i]->at(j)->j,0);
                verticeGraph->add(i,node);
            }else{
                node = new GraphNode(adj->List[i]->at(j)->j,adj->List[i]->at(j)->w);
                verticeGraph->add(i,node);
            }          
        }
    }
    
    Prim prim;
    AdjList* mst = prim.getMST(verticeGraph);
    delete verticeGraph;
    return mst;

}

bool SteinerApproximation::removeSteiners(AdjList* adj,std::vector<int>* steiners){
    bool removed = false;
    
    bool steinersL[adj->size];
    for(int i= 0; i < adj->size ; i++)
        steinersL[i] = false;
    
    for(int i= 0; i < steiners->size(); i++)
        steinersL[steiners->at(i)] = true;
    
    for(int i = 0 ; i < adj->size; i++){
        
        if(adj->List[i]->size() > 0){
                int j = adj->List[i]->at(0)->j;
                steinersL[j] = false;
                 
        }
    }
    
    for(int i= 0; i < steiners->size(); i++)
        if(steinersL[steiners->at(i)]){
            if(adj->List[steiners->at(i)]->size() > 0){
                GraphNode* node = adj->List[i]->at(0);
                adj->mstSize -= node->w;
                removed = true;
                adj->List[steiners->at(i)]->pop_back();
            }
        }
    
    return removed;
}

/*bool SteinerApproximation::subset( std::vector<bool> bit_mask, std::size_t req_size,std::vector<int>& outvec )
{
    if( std::count( bit_mask.begin(), bit_mask.end(), true ) == req_size )
    {              
        for( std::size_t i = 0 ; i < bit_mask.size() ; ++i )
            if( bit_mask[i] ) outvec.push_back(i) ;
    }else return false;
    return true;
}

// generate the next Gray code (in reverse)
bool SteinerApproximation::nextbitmask( std::vector<bool>& bit_mask )
{
    std::size_t i = 0 ;
    for( ; ( i < bit_mask.size() ) && bit_mask[i] ; ++i )
        bit_mask[i] = false ;

    if( i < bit_mask.size() ) { bit_mask[i] = true ; return true ; }
    else return false ;
}*/

void SteinerApproximation::create( std::list<int> l,std::vector< std::vector<int>* >& set){
    std::vector<int>* newset = new std::vector<int>;
    for(std::list<int>::iterator it=l.begin(); it!=l.end() ; ++it)
            newset->push_back(*it);
    set.push_back(newset);
}

void SteinerApproximation::subset(std::vector<int>& arr, int size, int left, int index, std::list<int> &l,std::vector< std::vector<int>* >& set){
    if(left==0){
        create(l,set);
        return;
    }
    for(int i=index; i<size;i++){
        l.push_back(arr.at(i));
        subset(arr,size,left-1,i+1,l,set);
        l.pop_back();
    }

}   


SteinerApproximation::~SteinerApproximation() {
}
