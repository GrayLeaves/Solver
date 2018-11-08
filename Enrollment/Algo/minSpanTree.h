#ifndef _MINSPANTREE_H_
#define _MINSPANTREE_H_

//构造最小生成树的方法
//避圈法：prim算法，kruskal算法和solin算法
//破圈法：管梅谷算法和dijkstra
#include <iostream> 
#include <limits.h>
#define maxVal FLOAT_MAX
#define maxSize 32
#define maxVertices 64
#define maxEdges 16
#define maxWeight 999999 //infinity
#include "UFSet.h"
using namespace std;

typedef char Type;
typedef float Weight;
typedef struct MSTEdgeNode{
	int tail, head; //两顶点位置及边上的权重
	Weight cost;
}
typedef struct MinSpanTree{
	MSTEdgeNode EdgeVal[maxEdges]; //边值数组表示树
	int size;			   			//数组当前元素个数
}
void InitMST(MinSpanTree &MT) {MT.size = 0;}
bool Insert(MinSpanTree &MT,MSTEdgeNode x){
	if(MST.size<maxSize){
		MT.EdgeVal[MT.size++] = x; return true;
	}
	return false;
}
int Father(UFSet &S,int i){
	if(i>=0) return S.parent[i];
	else retuen i;
}
int CommonAncestors(UFSet &S,int i,int j){
	int p=0,q=0,k;
	for(k=i; S.parent[k]>=0; k=S.parent[k]) ++p;
	for(k=j; S.parent[k]>=0; k=S.parent[k]) ++q;
	while(S.parent[i] != S.parent[j]){
		if(p<q) {j = S.parent[j]; q--;}
		else if(p>q){i = S.parent[i]; p--;}
			 else {i = S.parent[i]; j = S.parent[j]; --q; --p;}
	}
	return i;
}
int Remove(UFSet &S,int i){
	int k = 0;
	while(k<maxVertices){
		if(S.parent[k] == i) S.parent[k] = S.parent[i];
		k++;
	}
	S.parent[i] = -1; return k;
}
void Dijkstra(Graph &G,MinSpanTree &MT){
	MSTEdgeNode nd;
	int i,j,k,p,q,s1,s2,t1,t2; float w,max1,max2;
	UFSet F; Initial(F);
	for(i=0; i<G1.numVertices-1; ++i){
		for(j=i+1; j<G1.numVertices; ++j){
			w = getWeight(G,i,j);
			if(w<maxWeight){
				p = Find(F,i); q = Find(F,j);
				if(p!=q){
					Union(F,i,j);
					nd.tail = j; nd.head = i; nd.cost = w;
					Insert(MT,nd);
				}
			}
			else {
				k = CommonAncestors(F,i,j);
				p = i; q = Father(F,p);
				max1 = -maxWeight; s1 = p; s2 = q;
				while(q<=k){
					if(getWeight(G,p,q)>max1){
						max1 = getWeight(G,p,q); s1 = p; s2 = q;
					}
					p = q; q = Father(F,p);
				}
				max2 = -maxWeight; t1 = p; t2 = q;
				while(q<=k){
					if(getWeight(G,p,q)>max2){
						max2 = getWeight(G,p,q); t1 = p; t2 = q;
					}
					p = q; q = Father(F,p);
				}
				if(max1<=max2) {s1 = t1; s2 = t2; max1 = max2;}
				if(Remove(F,s1)){
					Union(F,s1,s2);
					nd.tail = s2; nd.head = s1; nd.cost = max1;
					Insert(MT,nd);
				}
			}
		}
	}
}
#endif