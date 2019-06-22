#include "p1io.h"
#include "steiner.h"
#include "bbobstacle3.h"
#include<map>
/*
 * Global Routines
 */

/*
 * Local Variables
 */

int numEqualponits(int a,int b);
boolType verifiesCyclePrune_b(int mm);
void findBestTree(struct cinfo *info,struct mTree *minTree,int l,int h);
boolType imcomp(struct full_set* set1,struct full_set* set2);
bool incomp2(struct cinfo *info,int i,int j);
void sum2(int *B,int *C,int* E);
boolType verifiesCyclePruneLast_b(struct cinfo *info,int *loopMask,int *incompMask,int mm);
int setNewPoints(struct cinfo *info,struct full_set* set,struct mTree *mintree,int *edges,int val);

static int32u		cpu_time_limit;
static char *		me;

/*
 * The main routine for the "bb" program.  It takes the output from
 * the "prep" program (phase 1 of our Steiner tree program) and uses
 * a branch-and-cut to find the Steiner minimal tree.
 */



  int fila[1000];
  int fila2[1000];
  int initial = 0;

int* buidBestTree(struct cinfo* cinfo,double max)
{
int			i;
int			j;
int			fpsave;
//struct cinfo		cinfo;
struct pset *		pts;
char			buf1 [32];
char			buf2 [32];
char			buf3 [32];
char			buf4 [32];
struct mTree           mintree;

mintree.nmm = 0;

	//fpsave = set_floating_point_double_precision ();

	//init_tables ();
	// incialize used data structures
        mintree.treeUsed =(int*) malloc(sizeof(int)*cinfo->num_edges);
        mintree.vertices = (int*)malloc(sizeof(int)*cinfo->pts->n);
        mintree.usedPoints = (int*)malloc(sizeof(int)*cinfo->pts->n);
        mintree.sets =(int*) malloc(sizeof(int)*cinfo->num_edges);
        for(i=0;i<cinfo->num_edges;i++)
        {
            mintree.treeUsed[i] = 0;
            mintree.sets[i] = 0;
        }

        int flag = 0;
          for(i=0;i<cinfo->pts->n;i++)
        {
              if(flag == 0)
                  if(cinfo->pts->a[i].polygon == 1){
                  
                      flag =1;
                  }
           mintree.vertices[i] = 0;
           mintree.usedPoints[i] = 0;
        }
         if (max < 0)
         	mintree.menorcusto = 99999999.0;
       	 else
	        mintree.menorcusto = max;

        mintree.nedges = 0;
        mintree.nvertices =0;
        mintree.custo = 0;
      
        mintree.matrix =(int**) malloc(sizeof(int*)*cinfo->num_verts);
        for(i=0;i<cinfo->num_verts;i++)
            mintree.matrix[i] =(int*) malloc(sizeof(int)*cinfo->num_verts);

        for(i=0;i<cinfo->num_verts;i++)
            for(j=0;j<cinfo->num_verts;j++)
                mintree.matrix[i][j] = 0;

	
        mintree.matrix2 = (int**) malloc(sizeof(int*)*cinfo->num_verts);
        for(i=0;i<cinfo->num_verts;i++)
            mintree.matrix2[i] =(int*) malloc(sizeof(int)*cinfo->num_verts);

        for(i=0;i<cinfo->num_verts;i++)
            for(j=0;j<cinfo->num_verts;j++)
                mintree.matrix2[i][j] = 0;

        findBestTree(cinfo,&mintree,0,0);
        
        
       for( i=0;i<cinfo->num_edges;i++)
             if(mintree.sets[i] != 0 )
                printf("%d ",i);
        
 	return mintree.sets;
    }



void findBestTree(struct cinfo *info,struct mTree *minTree,int l,int h)
{
    //if(h > 8) return;
   
    int i,k,flag,ed;
    for(i=l;i<info->num_edges;i++)
    {
        if(minTree->treeUsed[i] > 0) continue;
        minTree->treeUsed[i] = 1;
        minTree->custo += info->full_trees[i]->tree_len;

        if( minTree->custo > minTree->menorcusto)
        {
           minTree->custo -= info->full_trees[i]->tree_len;
            minTree->treeUsed[i] = 0;
            continue;
        }
        

      

        ed = 0;
        k = setNewPoints(info,info->full_trees[i], minTree,&ed,1);


        if( k < 0)
        {
              minTree->custo -= info->full_trees[i]->tree_len;
              minTree->treeUsed[i] = 0;
             // setNewPoints(info,info->full_trees[i],usedTrees,usedPoints,&ed,-1);
              continue;
        }


        minTree->nedges+= info->full_trees[i]->nedges;
        minTree->nvertices+= k;
        flag = 0;

        //
        if( minTree->nedges == minTree->nvertices -1)
	{

            if(verifiesTrees(info,minTree) == TRUE)
            {   flag =1;


                
                minTree->nmax = 0;
                preenche(info,minTree);
                findCycle(info,0,minTree);
               if( minTree->nmax == minTree->nmm )
                    if(minTree->custo < minTree->menorcusto)
                    {

                        saveSet(info,minTree);
                        minTree->menorcusto = minTree->custo; //9 16
                        printf("(%f)\n",minTree->custo);

                    }

            }
	}
      

        if(flag == 0)
          findBestTree(info,minTree,i+1,h+1);
        minTree->nvertices-= k;
        minTree->nedges-= info->full_trees[i]->nedges;
        setNewPoints(info,info->full_trees[i],minTree,&ed,-1);
        minTree->custo -= info->full_trees[i]->tree_len;
        minTree->treeUsed[i] = 0;

    }

}

int setNewPoints(struct cinfo *info,struct full_set* set,struct mTree *mintree,int *edges,int val)
{
    int i,j,n,k = 0;

    if(val > 0)
    for(i = 0;i < set->terminals->n;i++)
    {
         for(j = i+1;j < set->terminals->n;j++){
             int p1,p2;
          
             p2 = set->terminals->a[i].pnum;
             p1 = set->terminals->a[j].pnum;
             if(mintree->matrix[p1][p2] != 0 ) return -1;

         }
    }

    int flag = 0;
    initial = 0;
    int pos = 0;
    for(i = 0;i < set->terminals->n;i++)
    {

          if(flag == 1) break;
         for(j = i+1;j < set->terminals->n;j++){
             int p1,p2;
          
             p2 = set->terminals->a[i].pnum;
             p1 = set->terminals->a[j].pnum;

              if(val >0)
             {
                 //initial+=2;
                  if( mintree->matrix[p1][p2] != 0 || mintree->matrix[p2][p1] != 0 ){
                      flag = 1;
                      break;
                  }
                 fila[initial] = p1;
                 fila[initial+1] = p2;
                 initial+=2;
             }
              if(flag == 1) break;

            // printf("(%d %d),%d",p1,p2,initial);
              mintree->matrix[p1][p2] += val;
              mintree->matrix[p2][p1] += val;
              
//              nmm+=val;
            }
        if(flag == 1) break;

        if(mintree->usedPoints[set->terminals->a[i].pnum] == 0)
            k++;
        mintree->usedPoints[set->terminals->a[i].pnum]+= val;
        fila2[pos++] = set->terminals->a[i].pnum;
    }

    if(flag == 1){
        for(i=0;i< initial;i+=2)
        {
           // printf("[%d %d]",fila[i+1],fila[i]);
            mintree->matrix[fila[i]][fila[i+1]] -= val;
            mintree->matrix[fila[i+1]][fila[i]] -= val;
            if(i < pos)
            {
                mintree->usedPoints[fila2[i]] -= val;
            }

        }
    return -1;
    }
   // printf("%d ",initial);
    if(set->steiners != NULL)
   	 k+=set->steiners->n;
    return k;
}




boolType verifiesTrees(struct cinfo *info,struct mTree *mintree)
{
    int i;

    for(i=0;i<info->intialPolygons;i++)
        if(mintree->usedPoints[i] == 0) return FALSE;


    for(i=info->intialPolygons ;i < info->pts->n;i++)
        if(mintree->usedPoints[i] != 0)
            if(mintree->usedPoints[i] < 2 || mintree->usedPoints[i] > 3) return FALSE;

    return TRUE;
}

void saveSet(struct cinfo *info,struct mTree *mintree)
{
      int i;
    for(i=0;i<info->num_edges;i++)
        mintree->sets[i] = mintree->treeUsed[i];
}
int preenche(struct cinfo *info,struct mTree *mintree )
{
    int i,j,k=0;
    mintree->nmm = 0;
    for(i=0;i<info->num_verts;i++)
    {
        for(j=0;j<info->num_verts;j++)
        {
            mintree->matrix2[i][j] = 0;
            
            if(mintree->matrix[i][j] != 0)
            {
               mintree->matrix2[i][j] = 1;
               mintree->nmm++;
            }
             else
               mintree->matrix2[i][j] = 0;
        }
        
    }
    mintree->nmm/=2;
}


void findCycle(struct cinfo *info,int i,struct mTree *mintree)
{
    int j,aux,l,k;

   for(j = 0; j < info->pts->n;j++ )
   {

       if(mintree->matrix2[i][j] == 0) continue;
       mintree->nmax+=  1;
     
       mintree->matrix2[i][j] = 0;
       mintree->matrix2[j][i] = 0;
       findCycle(info,j,mintree);
       
   }

}
/*

                BBObstacle second version

 */
int *endMask; //*
int sizeEndMak; //*
int **pointMask; //*
int **inconpatibilityMask; //*
int bpsize; //*
int incompSize; //*
//
int *stack; //*
int *beststack; //*
int *stackaux; //*
int stackTop; //*
int beststackTop; //*
int bestval = 99999999;
std::map<int,int> newbound;
//
int *mask0,*mask1;

void inicializestack(int N)
{
    stack = (int*) malloc(sizeof(int)* N+1);
    stackTop = 0;

    beststack = (int*) malloc(sizeof(int)* N+1);
    beststackTop = 0;

    stackaux = (int*) malloc(sizeof(int)* N+1);

}

int pop()
{
   stackTop--;
   return stack[stackTop];
}

void push(int p)
{

   stack[stackTop] = p;
   stackTop++;
}

void savestack()
{
    int i;
    for(i = 0;i < stackTop;i++){
        beststack[i] = stack[i];	
	}  


    beststackTop = stackTop;
}


int* buidBestTree_b(struct cinfo* cinfo,double max)
{
    int i,j,k,aux;

        // take the number of trees
        int nTrees = cinfo->num_edges;
        //take the number of points
        int nPoints = cinfo->pts->n;
        // binary vector size
        bpsize = floor(nPoints/BPW2)+1;
        //inicialize mask for each tree
	incompSize = floor(nTrees/BPW2)+1;
        newbound.clear();
        inicializestack(nTrees);
        pointMask = (int**) malloc(sizeof(int*)*nTrees);
	inconpatibilityMask = (int**) malloc(sizeof(int*)*nTrees);
        for(i = 0 ; i < nTrees;i++){
           pointMask[i] =  (int*) malloc(sizeof(int)* bpsize);
	}

	for(i = 0 ; i < nTrees;i++){
           inconpatibilityMask[i] =  (int*) malloc(sizeof(int)* incompSize);
	}
        // puts zero on incomp mask
        for(i = 0 ; i < nTrees;i++)
             for(j = 0 ; j < incompSize;j++)
                inconpatibilityMask[i][j] = 0;

        // puts zero on mask
        for(i = 0 ; i < nTrees;i++)
             for(j = 0 ; j < bpsize;j++)
                pointMask[i][j] = 0;

	if (max < 0)
         	bestval = 99999999.0;
       	else
	        bestval = max;



        //represents the points used by each tree
        for(i = 0; i < nTrees;i++)
          for(j = 0 ; j < cinfo->full_trees[i]->terminals->n;j++)
            {
               aux = cinfo->full_trees[i]->terminals->a[j].pnum;
               SETBIT2(pointMask[i],aux);
            }
       for(i = 0 ; i < nTrees;i++)
           printf("%d \n",pointMask[i][0]);
        printf("-- %d --\n",nTrees);
	int ncount;
	//incialize imcompatiblility var
	for(i = 0 ; i < nTrees;i++){

               // printf("{{%d %d}}", i ,cinfo->full_trees[i]->nedges);
		for(j = 0 ; j < nTrees;j++){
			ncount = 0;
			if(i == j) continue;
			for(k = 0 ; k < bpsize ; k++)
			{
				ncount += numEqualponits(pointMask[i][k], pointMask[j][k]);
			}
			// trees incompatible
			if(ncount <= 1) 
				if(imcomp(cinfo->full_trees[i],cinfo->full_trees[j]) == TRUE) ncount = 3;
			//if(ncount <= 1) 
			//	if(incomp2(cinfo,i,j) == TRUE) { printf("PODA"); ncount = 3;}
			//if(ncount == 2)
			//	printf("{{%d %d %d}}", i ,j,ncount);
			if(ncount > 1 )
			{        //printf("{{%d %d %d}}", i ,j,ncount);
				 SETBIT2(inconpatibilityMask[i],j);

			}
				
		}
	}
	
	
	// inicialize incompatible mask for loop
	int *incompMaskloop =  (int*) malloc(sizeof(int)* incompSize);
	for(i=0;i<incompSize;i++)
        	 incompMaskloop[i] = 0;


        // inicialize endmask
       sizeEndMak = cinfo->intialPolygons ;
       int sizeaux = sizeEndMak;       
       endMask =  (int*) malloc(sizeof(int)* bpsize);

       for(i=0;i<bpsize;i++)
            endMask[i] = 0;

       for(i=0;i<sizeaux;i++)
           SETBIT2(endMask,i);

        
       //inicialize looppointMask
       int *loopMask =  (int*) malloc(sizeof(int)* bpsize);
       for(j = 0 ; j < bpsize;j++)
            loopMask[j] = 0;


       int* B = (int*) malloc(sizeof(int)* bpsize);
       int* C = (int*) malloc(sizeof(int)* bpsize);
       for(i=0;i<bpsize;i++){
            B[i] = 0;
	    C[i] = 0;
	}

       
       findBestTree_b(cinfo,loopMask,incompMaskloop,B,C,0,0);

       int *result = (int*) malloc(sizeof(int)* nTrees);
       for( i = 0 ; i < nTrees;i++)
            result[i] = 0;

       for( i = 0 ; i < beststackTop;i++)
          result[beststack[i]] = 1;

       for( i=0;i<cinfo->num_edges;i++)
             if(result[i] != 0 )
                printf("%d ",i);

       return result;
 }

boolType verifiesPointsUseds_b(int* pointsUseds)
{
    int i;
    //all points are useds
    for(i = 0 ; i < bpsize;i++)
    {
        
        if((pointsUseds[i]&endMask[i]) != endMask[i])
            return FALSE;
    }
 

    return TRUE;
}


void findBestTree_b(struct cinfo *info,int *loopMask,int *incompMaskloop,int *B,int *C,float val,int l)
{

    int i,j,flag,ll,kk;
    int *nB,*nC;
    int num; 
    int *newLoopMask = (int*) malloc(sizeof(int)*bpsize);
    int *newIncompMaskloop = (int*) malloc(sizeof(int)*incompSize);
    nB = (int*) malloc(sizeof(int)*bpsize);
    nC = (int*) malloc(sizeof(int)*bpsize);

	
    for(i=0;i<info->num_edges;i++)
    {

	flag = 0;
	if(l != 0 )
		for(j=0;j<bpsize;j++)
			if((loopMask[j]&pointMask[i][j]) != 0 ) flag = 1;

	if(l != 0 )
		if(flag == 0) continue;
	
	//look for incompatiblility
	flag = 0;
	   
	    ll = i/BPW2;
	    kk = i%BPW2;
	    int u =incompMaskloop[ll] & (1ul << (kk)) ; 
            if( u != 0)
	    { 
		flag = 1;
		
	     }
	

	if(flag == 1) continue;

	for(j=0;j<bpsize;j++)
		if((B[j]&C[j]&pointMask[i][j]) != 0) flag = 1;
	
	if(flag == 1) continue;

	memcpy(nB, B, bpsize*4);
	memcpy(nC, C, bpsize*4);

	sum2(nB,nC,pointMask[i]);

	
	num = 0;
	for(j=0;j<bpsize;j++)
		num += numEqualponits(loopMask[j], pointMask[i][j]);

	if(num > 1)
		if(verifiesCyclePrune_b(i) == FALSE){continue;}

        push(i);
        memcpy(newLoopMask, loopMask, bpsize*4);
     
       //apply point mask
        for(j=0;j<bpsize;j++)
            newLoopMask[j] |= pointMask[i][j];

        val += info->full_trees[i]->tree_len;
        flag = 0;

	if(newbound.find(newLoopMask[0]) != newbound.end())
		if(newbound.find(newLoopMask[0])->second <= val) {flag = 1; }
	
	if(flag == 0)	
	        newbound[newLoopMask[0]] = val;
       
	if(flag == 0)
           if(val < bestval){

                        if(verifiesPointsUseds_b(newLoopMask) == TRUE)
                        {

                            if(verifiesCycle_b() == FALSE)
                            {
                                flag = 1;
                                savestack();
                                bestval = val;
                                printf("(%f)",bestval);
                            }
                        }
		
                if(flag == 0)
		{
			//new imcopatibility mask is created
			
        	    	for(j=0;j<incompSize;j++)
			{ 	newIncompMaskloop[j] = 0;
            	  		newIncompMaskloop[j] = incompMaskloop[j] | inconpatibilityMask[i][j];
			}
                       
			if(verifiesCyclePruneLast_b(info,newLoopMask,newIncompMaskloop,0) == TRUE)				
                   		findBestTree_b(info,newLoopMask,newIncompMaskloop,nB,nC,val,l+1);
		}

        }
        val -= info->full_trees[i]->tree_len;
        pop();
    }
	
    
    free(newLoopMask);
    free(newIncompMaskloop);
    free(nB);
    free(nC);


}


boolType verifiesCycle_b()
{
    int j,i,flag = 1,ormask,numAv;
    int *Mask = (int*) malloc(sizeof(int)*bpsize);

    for(j=0;j<bpsize;j++){
          Mask[j] = pointMask[stack[0]][j];
    }
    numAv = 0;
    for(j=1;j<stackTop;j++)
    {
        stackaux[j] = 0;
        numAv++;
    }
    stackaux[0] = 1;
    while(flag == 1)
    {
        flag = 0;
        for(j=1;j<stackTop;j++)
        {
            if( stackaux[j] == 0)
            {
                ormask = 0;
                for(i=0;i<bpsize;i++)
                   if ((Mask[i]&pointMask[stack[j]][i]) != 0)
                        ormask = 1;

                if( ormask != 0)
                {   
                    for(i=0;i<bpsize;i++){
                       Mask[i] |= pointMask[stack[j]][i];                         
                    }
                     stackaux[j] = 1;
                     flag = 1;
                     numAv--;
                     break;
                }
            }
        }
    }
    free(Mask);
    if(numAv == 0) return FALSE;
    return TRUE;
}

int numEqualponits(int a,int b)
{
	int c = (a)&(b);
	if(c == 0) return 0;
	int n = 0;
	int i;
	for(i = 0;i < BPW2; i++)
		if(((1ul << i) & c) != 0) n++;

	if(n == 0) printf("eerrrrrro");
	return n;
	
}


boolType verifiesCyclePrune_b(int mm)
{
    int j,i,flag = 1,ormask,numAv;
    int *Mask = (int*) malloc(sizeof(int)*bpsize);

    for(j=0;j<bpsize;j++){
          Mask[j] = pointMask[mm][j];
    }
    numAv = 0;
    for(j=0;j<stackTop;j++)
    {
        stackaux[j] = 0;
        numAv++;
    }
    
    while(flag == 1)
    {
        flag = 0;
        for(j=0;j<stackTop;j++)
        {
            if( stackaux[j] == 0)
            {
                ormask = 0;
	        for(i=0;i<bpsize;i++)
			ormask += numEqualponits(Mask[i], pointMask[stack[j]][i]);
		
		if(ormask > 1) {free(Mask);return FALSE;}
		
                if( ormask != 0)
                {   
                    for(i=0;i<bpsize;i++){
                       Mask[i] |= pointMask[stack[j]][i];                         
                    }
                     stackaux[j] = 1;
                     flag = 1;
                     numAv--;
                     break;
                }
            }
        }
    }
    free(Mask);
    return TRUE;
}



void sum2(int *C,int *B,int* E)
{
int i,j;
int *Cout = (int*) malloc(sizeof(int)* bpsize);
for(i=0;i<bpsize;i++)
	Cout[i] = 0;

//printf("e %d %d %d %d",C[0],B[0],E[0],bpsize);
for(i=0;i<bpsize;i++)
{
    Cout[i] = C[i] & E[i];
   // C[i] = (C[i] & !E[i]) | (!C[i] & E[i]);
    //B[i] = (Cout[i] & !B[i]) | (!Cout[i] & B[i]);
    C[i] ^= E[i];
    B[i] ^= Cout[i];

}
//printf("s %d %d %d %d",C[0],B[0],E[0]);
free(Cout);
}

boolType imcomp(struct full_set* set1,struct full_set* set2)
{
int i,j;
int set1p1,set1p2;
int set2p1,set2p2;
struct point 		p;	/* IN - first endpoint of first segment */
struct point 		q;	/* IN - second endpoint of first segment */
struct point 		r;	/* IN - first endpoint of second segment */
struct point 		s;	/* IN - second endpoint of second segment */
struct point 		is;
int pn,qn,rn,sn;
double dx,dy,norm;

 for(i = 0;i < set1->nedges;i++)
    {
	set1p1 = set1->edges[i].p1;
        set1p2 = set1->edges[i].p2;
        pn = -1;
        qn = -1;
	//p
	if(set1p1 < set1->terminals->n)
	{
		p.x = set1->terminals->a[set1p1].x;
		p.y = set1->terminals->a[set1p1].y;
		pn = set1->terminals->a[set1p1].pnum;	
	}else
	{
		p.x = set1->steiners->a[set1p1 - set1->terminals->n].x;
		p.y = set1->steiners->a[set1p1 - set1->terminals->n].y;
	}

	//q	
	if(set1p2 < set1->terminals->n)
	{
		q.x = set1->terminals->a[set1p2].x;
		q.y = set1->terminals->a[set1p2].y;
		qn = set1->terminals->a[set1p2].pnum;	
	}else
	{
		q.x = set1->steiners->a[set1p2 - set1->terminals->n].x;
		q.y = set1->steiners->a[set1p2 - set1->terminals->n].y;
	}

	
         for(j = 0;j < set2->nedges;j++){
	  	set2p1 = set2->edges[j].p1;
        	set2p2 = set2->edges[j].p2;
		rn = -1;sn=-1;
			//r
			if(set2p1 < set2->terminals->n)
			{
				r.x = set2->terminals->a[set2p1].x;
				r.y = set2->terminals->a[set2p1].y;
				rn = set2->terminals->a[set2p1].pnum;	
			}else
			{
				r.x = set2->steiners->a[set2p1 - set2->terminals->n].x;
				r.y = set2->steiners->a[set2p1 - set2->terminals->n].y;
			}

			//s	
			if(set2p2 < set2->terminals->n)
			{
				s.x = set2->terminals->a[set2p2].x;
				s.y = set2->terminals->a[set2p2].y;
				sn = set2->terminals->a[set2p2].pnum;	
			}else
			{
				s.x = set2->steiners->a[set2p2 - set2->terminals->n].x;
				s.y = set2->steiners->a[set2p2 - set2->terminals->n].y;
			}
/*
		//if(print == 1)
		//	printf("P %f %f Q %f %f R %f %f S %f %f",p.x,p.y,q.x,q.y,r.x,r.y,s.x,s.y);
		
		//p && r
		//if((pn >= 0) && (rn >=0))
			//if(pn == rn)
			if(EDIST(&p,&r) < 10.0)
			{
				get_angle_vector(&q,&p,&s,&dx,&dy);
				norm = sqrt(dx*dx +dy*dy);
				//dx = dx/norm;
				//dy = dy/norm;
				//if(print == 1)
				//	printf("PR %f %f",dx,dy);
				if((dx > 0) && (fabs(dy) < 0.8)) return TRUE;
			}

		//p && s
		//if((pn >= 0) && (sn >=0))
			//if(pn == sn)
			if(EDIST(&p,&s) < 10.0)
			{
				get_angle_vector(&q,&p,&r,&dx,&dy);
				norm = sqrt(dx*dx +dy*dy);
				//dx = dx/norm;
				//dy = dy/norm;
				if((dx > 0) && (fabs(dy) < 0.8)) return TRUE;
			}

		//q && r
		//if((qn >= 0) && (rn >=0))
			//if(qn == rn)
			if(EDIST(&q,&r) < 10.0)
			{
				get_angle_vector(&p,&q,&s,&dx,&dy);
				norm = sqrt(dx*dx +dy*dy);
				//dx = dx/norm;
				//dy = dy/norm;
				if((dx > 0) && (fabs(dy) < 0.8)) return TRUE;
			}

		//q && s
		//if((qn >= 0) && (sn >=0))
			//if(qn == sn)
			if(EDIST(&q,&s) < 10.0)
			{
				get_angle_vector(&p,&q,&r,&dx,&dy);
				norm = sqrt(dx*dx +dy*dy);
				//dx = dx/norm;
				//dy = dy/norm;
				if((dx > 0) && (fabs(dy) < 0.8)) return TRUE;
			}*/


		if( segment_intersection2(&p,&q,&r,&s,&is) == TRUE) return TRUE;

	}


}
return FALSE;
}




boolType verifiesCyclePruneLast_b(struct cinfo *info,int *loopMask,int *incompMask,int mm)
{
	
  
    int j,i,flag = 1,ormask,numAv,ll,kk,u,incomp;
    int *Mask = (int*) malloc(sizeof(int)*bpsize);
    int *Maskaux = (int*) malloc(sizeof(int)*bpsize);

    for(j=0;j<bpsize;j++){
          Mask[j] = pointMask[stack[0]][j];
	  Maskaux[j] = 0;
    }
    numAv = 0;
    for(j=1;j<stackTop;j++)
    {
        stackaux[j] = 0;
	
       
    }
    stackaux[0] = 1;
    while(flag == 1)
    {
        flag = 0;
        for(j=1;j<stackTop;j++)
        {
            if( stackaux[j] == 0)
            {
                ormask = 0;
                for(i=0;i<bpsize;i++)
                   if ((Mask[i]&pointMask[stack[j]][i]) != 0)
                        ormask = 1;

                if( ormask != 0)
                {   
                    for(i=0;i<bpsize;i++){
                       Mask[i] |= pointMask[stack[j]][i];                         
                    }
                     stackaux[j] = 1;
                     flag = 1;                    
                     break;
                }
            }
        }
    }
  
    for(j=1;j<stackTop;j++)
	if(stackaux[j]==0)
		for(i=0;i<bpsize;i++)
	   	     Maskaux[i] |= pointMask[stack[j]][i];
   

    flag = 1;
 
    int *useds = (int*) malloc(sizeof(int)*info->num_edges);
    for(j=mm;j<info->num_edges;j++)
	useds[j] = 0;

    while(flag == 1)
    {
        flag = 0;
        for(j=mm;j<info->num_edges;j++)
        {
	       
	    if(useds[j] == 0)
	    {
	        incomp = 0;
           
		ll = j/BPW2;
	        kk = j%BPW2;
		u = incompMask[ll] & (1ul << (kk)) ; 
                if( u != 0)
	    	{ 
		   incomp = 1;
		   useds[j] = 1;
	     	}
		if(incomp == 0){
			ormask = 0;
		        for(i=0;i<bpsize;i++)
		           if ((Mask[i]&pointMask[j][i]) != 0)
		                ormask = 1;

		        if( ormask != 0)
		        {   
		            for(i=0;i<bpsize;i++){
		               Mask[i] |= pointMask[j][i];
   
		            }
 		             if((Mask[0]&endMask[0]) == endMask[0]){    
   			        free(Maskaux);
			        free(useds);
			         free(Mask);
				return TRUE;}
			    
                             for(i=0;i<bpsize;i++)
				  if(Mask[i]&Maskaux[i] != 0)
				{
                                 free(Maskaux);
			         free(useds);
			         free(Mask);
				return TRUE;}

		             flag = 1;
			      useds[j] = 1;
		              break;
		        }
		}
            }
        }
    }
   // printf("Corte ");
    free(Maskaux);
    free(useds);
    free(Mask);
    return FALSE;
}


bool incomp2(struct cinfo *info,int i,int j)
{

int k,y,num;
if(info->full_trees[j]->nedges != 1) return false;
if(info->full_trees[i]->nedges != 1) return false;
int flag = 0;
for(y = 0 ; y < bpsize ; y++)
	if((pointMask[i][y]&pointMask[j][y]) !=0 ) flag =1;

if(flag == 0) return false;

for(k=0;k<info->num_edges;k++)
{
	if((info->full_trees[k]->nedges == 3))
	{       num = 0;
		for(y = 0 ; y < bpsize ; y++){
			int aux = pointMask[i][y]|pointMask[j][y];
			num += numEqualponits(pointMask[k][y],aux);
		}

		//printf("%d,i%d,j%d,k%d", num,i,j,k);
		if(num > 1) return true;
	}
}
return false;
}

