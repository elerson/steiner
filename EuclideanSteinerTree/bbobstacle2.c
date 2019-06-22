#include "p1io.h"
#include "steiner.h"


/*
 * Global Routines
 */

int			main (int, char **);
void saveSet(struct cinfo *info,int* usedPoints);
boolType verifiesTrees(struct cinfo *info,int* usedPoints);


/*
 * Local Variables
 */


static int32u		cpu_time_limit;
static char *		me;

/*
 * The main routine for the "bb" program.  It takes the output from
 * the "prep" program (phase 1 of our Steiner tree program) and uses
 * a branch-and-cut to find the Steiner minimal tree.
 */
  double menorcusto = 9999999.0;
  int *sets;
  int **matrix;
	int
main (

int		argc,
char **		argv
)
{
int			i;
int			j;
int			fpsave;
int* usedPoints;
struct cinfo		cinfo;
int * treeUsed;
struct pset *		pts;
char			buf1 [32];
char			buf2 [32];
char			buf3 [32];
char			buf4 [32];


	fpsave = set_floating_point_double_precision ();

	setbuf (stdout, NULL);
	
	init_tables ();

	read_phase_1_data (&cinfo);

	/* Enable CPU time limitation, if any. */
	//start_limiting_cpu_time (cpu_time_limit);

        treeUsed = malloc(sizeof(int)*cinfo.num_edges);
        
        usedPoints = malloc(sizeof(int)*cinfo.pts->n);
        sets = malloc(sizeof(int)*cinfo.num_edges);
        for(i=0;i<cinfo.num_edges;i++)
        {
            treeUsed[i] = 0;
            sets[i] = 0;
        }
        /*matrix = malloc(sizeof(int*)*cinfo.num_edges);
        for(i=0;i<cinfo.num_edges;i++)
            matrix[i] = malloc(sizeof(int)*cinfo.num_edges);
        for(i=0;i<cinfo.num_edges;i++)
            for(j=0;j<cinfo.num_edges;j++)
                matrix[i][j] = 0;
         */

        int flag = 0;
          for(i=0;i< cinfo.pts->n;i++)
        {
              if(flag == 0)
                  if(cinfo.pts->a[i].polygon == 1){
                      cinfo.intialPolygons = i;
                      flag =1;
                  }

           usedPoints[i] = 0;           
        }
 

        excludeBadTrees(&cinfo,treeUsed);
               
        int *n = malloc(sizeof(int));
        int *p = malloc(sizeof(int));
        double *sc = malloc(sizeof(double));
        *n =0;*p =0;*sc=0;
        findBestTree(&cinfo,treeUsed,usedPoints,p,n,sc,0,0);
//printf(" aqui \n");
         for(i=0;i<cinfo.num_edges;i++)
           // if(sets[i] == 1) printf("%d ",i);
                printf("%d ",sets[i]);
   //     print_phase_1_data (&cinfo, CURRENT_P1IO_VERSION);
        }



void findBestTree(struct cinfo *info,int *usedTrees,int* usedPoints,int *nedges,int *nvertices,double *custo,int l,int h)
{
   
    if(h > 5) return;
    int i,k,flag,edaux;
    for(i=0;i<info->num_edges;i++)
    {
        if(usedTrees[i] > 0) continue;
        usedTrees[i] = 1;
        *custo += info->full_trees[i]->tree_len;
        if(*custo > menorcusto)
        {
           *custo -= info->full_trees[i]->tree_len;
           usedTrees[i] = 0;
           continue;
        }
        k = setNewPoints(info->full_trees[i],usedPoints,&edaux,2);
        /*if(k < 0 )
        {
           *custo -= info->full_trees[i]->tree_len;
            usedTrees[i] = 0;
            continue;
        }*/


        *nedges+= info->full_trees[i]->nedges;
        *nvertices+= k;
        flag = 0;
       
       //if(*nedges == *nvertices -1)
       {
           //printf("(%d %d)\n",*nedges,k);
            if(verifiesTrees(info,usedPoints) == TRUE)
            {   flag =1;
             
                if(*custo < menorcusto)
                {
                    saveSet(info,usedTrees);
                    menorcusto = *custo;
                    
                }
            
            }
                

       }
        
        if(flag == 0 )
            findBestTree(info,usedTrees,usedPoints,nedges,nvertices,custo,i+1,h+1);
        *nvertices-= k;
        *nedges-= info->full_trees[i]->nedges;
        setNewPoints(info->full_trees[i],usedPoints,-2);
        *custo -= info->full_trees[i]->tree_len;
        usedTrees[i] = 0;
    }

}

int setNewPoints(struct full_set* set,int* usedPoints,int *edges,int val)
{
    int i,j,k = 0;

    if(val > 0)
    for(i = 0;i < set->terminals->n;i++)
    {
        for(j = i+1;j < set->terminals->n;j++){
            {
               if((matrix[set->terminals->a[i].pnum][set->terminals->a[j].pnum]) > 0)
                   return -1;
               if(matrix[set->terminals->a[j].pnum][set->terminals->a[i].pnum] > 0)
                   return -1;
            }
        }
    }

*edges = 0;
     printf("%d\n",val);
    for(i = 0;i < set->terminals->n;i++)
    {
          for(j = i+1;j < set->terminals->n;j++){

               matrix[set->terminals->a[i].pnum][set->terminals->a[j].pnum] += val;
               matrix[set->terminals->a[j].pnum][set->terminals->a[i].pnum] += val;
               *edges+=1;

            }

        if(usedPoints[set->terminals->a[i].pnum] == 0)
            k++;
       
            
           
        usedPoints[set->terminals->a[i].pnum]+= val;
    }
    k+=set->steiners->n;
    return k;
}
boolType verifiesTrees(struct cinfo *info,int* usedPoints)
{
    int i;
    for(i=0;i<info->intialPolygons;i++)
        if(usedPoints[i] == 0 ) return FALSE;
/*
   for(i=info->intialPolygons;i < info->pts->n;i++)
        if(usedPoints[i] != 0)
            if(usedPoints[i] < 2 || usedPoints[i] >3) return FALSE;
*/
   
    return TRUE;
}

void saveSet(struct cinfo *info,int* usedPoints)
{
      int i;
    for(i=0;i<info->num_edges;i++)
        sets[i] = usedPoints[i];
}

void excludeBadTrees(struct cinfo *info,int *usedTrees)
{
    int i,j,flag;
    for(i=0;i<info->num_edges;i++)
    {
        flag = 0;
       // printf("%d [%d]\n",info->intialPolygons,i);
        for(j=0;j<info->full_trees[i]->terminals->n;j++)
        {
           // printf("(%d)",info->full_trees[i]->terminals->a[j].pnum );
            if(info->full_trees[i]->terminals->a[j].pnum <= info->intialPolygons)
            {
                flag = 1;
                break;
            }
        }
        if(flag == 0){
            usedTrees[i] = 2;
            // printf("%d [%d]\n",info->intialPolygons,i);
                    }
        
    }

}
