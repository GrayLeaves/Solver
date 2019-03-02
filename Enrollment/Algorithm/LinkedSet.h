//seqlist
//#pragma once
#ifndef _LINDEDSET_H_
#define _LINDEDSET_H_

#include <iostream> 
#include <cstdlib>
using namespace std;
//有序链表
typedef int ElemType;
typedef struct SetNode{
	ElemType data;
	struct SetNode *link;
}*LinkedSet;
//并
void UnionSet(LinkedSet& A,LinkedSet& B,LinkedSet& C){
	SetNode *pa = A->link, *pb = B->link, *p, *pc = C;
	while(pa!=nullptr && pb!=nullptr){
		if(pa->data==pb->data){
			pc->link = new SetNode; pc->data = pa->data;
			pa = pa->link; pb = pb->link;
		}
		else if(pa->data < pb->data){
				pc->link = new SetNode; pc->data = pa->data;
				pa = pa->link;
		     }
			 else {
				pc->link = new SetNode; pc->data = pb->data;
				pb = pb->link;
			 }
		pc = pc->link;
	}
	if(pa!=nullptr) p = pa;			//处理余链
	else p = pb;
	while(p!=nullptr){
		pc->link = new SetNode; pc->data = p->data;
		p = p->link; pc = pc->link;
	}
	pc->link = nullptr;				//收尾
}
//交
void IntersectionSet(LinkedSet& A,LinkedSet& B,LinkedSet& C){
	SetNode *pa = A->link, *pb = B->link, *pc = C;
	while(pa!=nullptr && pb!=nullptr){
		if(pa->data==pb->data){
			pc->link = new SetNode; pc->data = pa->data;
			pa = pa->link; pb = pb->link;
		}
		else if(pa->data < pb->data) pa = pa->link;
			 else pb = pb->link;
	}
	pc->link = nullptr;				//收尾
}
void DifferenceSet(LinkedSet& A,LinkedSet& B,LinkedSet& C){
	SetNode *pa = A->link, *pb = B->link, *pc = C;
	while(pa!=nullptr && pb!=nullptr){
		if(pa->data==pb->data){
			pa = pa->link; pb = pb->link;
		}
		else if(pa->data < pb->data){
				pc->link = new SetNode; pc->data = pa->data;
				pa = pa->link; pb = pb->link;
		     }
			 else pb = pb->link;
	}
	while(pa!=nullptr){
		pc->link = new SetNode; pc->data = pa->data;
		pa = pa->link; pc = pc->link;
	}
	pc->link = nullptr;				//收尾
}
#endif