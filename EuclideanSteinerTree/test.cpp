#include "./visibilityGraph/generator.hpp"
#include "./efst.h"
#include "./bbobstacle.h"

#include<cstdio>

int main()
{

 Gerador g;
 g.generateVisibility("inGerador","outGerador");
 struct cinfo *cinfo = buildSteinerTrees("outGerador");
 int *a ;//= buidBestTree(cinfo,-100);

//bbSteiner (1,NULL,cinfo);
// a = buidBestTree_b(cinfo,-1);
 
 //for( int i=0;i<cinfo->num_edges;i++)
             //if(a[i] == 1 )
            //    printf("%d ",a[i]);

}
