#ifndef BBOBSTACLE_H
#define BBOBSTACLE_H
#include "efuncs.h"
#define BPW2 32
#define	SETBIT2(bm, n)	((bm) [(n) / BPW2] |= (1ul << ((n) % BPW2)))
#define	CLRBIT2(bm, n)	((bm) [(n) / BPW2] &= ~(1ul << ((n) % BPW2)))
#define	BITON2(bm, n)	(((bm) [(n) / BPW2] & (1ul << ((n) % BPW2))) NE 0)
#define	BITON3(bm, n)	(bm & (1ul << n) NE 0)

void saveSet(struct cinfo *info,struct mTree *);
boolType verifiesTrees(struct cinfo *info,struct mTree *mintree);
//void verifiesEdges(struct cinfo *info,int i,int n,int *nmax);
void verifica(struct cinfo *info,int i,int *count);
void findCycle(struct cinfo *info,int i,struct mTree *mintree);
int count(struct cinfo *info);
int preenche(struct cinfo *info,struct mTree *mintree );
void inicializaPile(int N);
int pop();
void push(int p);
void savePile();

boolType verifiesPointsUseds_b(int* pointsUseds);
void findBestTree_b(struct cinfo *info,int *loopMask,int *incomMask,int*,int*,float val,int l);
boolType verifiesCycle_b();
boolType  stopCondition(int* div);
void subtraction(int* div);
void sum(int* mul);
#ifdef __cplusplus
extern "C" {
#endif 
int* buidBestTree(struct cinfo *cinfo,double max);
int* buidBestTree_b(struct cinfo* cinfo,double max);
#ifdef __cplusplus
}
#endif 


#endif
