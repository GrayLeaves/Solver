//seqlist
//#pragma once
#ifndef _CIRCLIST_H_
#define _CIRCLIST_H_
#define DefaultSize 64
#include <iostream>
#include <cstdlib>
using namespace std;

//循环链表
typedef int DataType;
typedef struct node{
	DataType data;
	struct node *link;
}LinkNode,CircNode,*CircList;

//静态链表
typedef LinkNode staticList[DefaultSize];

//不带头节点，仅有表尾指针的循环链表中插入元素x
void InsertLink(CircNode* &rear,DataType x){
	CircNode *s = new CircNode;
	if(s == NULL) {cout << "Allocation Failure." << endl; exit(1);}
	s->data = x; s->link = rear->link; rear->link = s; rear = s;
}
//删除首节点，表尾指针
bool RemoveLink(CircNode* &rear,DataType& x){
	if(rear == NULL) return false;
	CircNode* s = rear->link;
	rear->link = s->link; x = s->data; delete s;
	return true;
}
//逆转循环链表
void ReverseLink(CircNode* L){
	CircNode *p, *q;
	p = L->link; L->link = L; //L形成短循环链表
	while(p!=L){
		q = p->link;          //p从原链表中摘下
		p->link = L->link; L->link = p; p = q;//p插入新链表头部
	}
}
void ReverseLink(CircList& L){ //三兄弟法
	CircNode *pr, *p, *q;
	pr = L; p = L->link; q = p->link; //循环链表遍历指针p
	while(p!=L){
		p->link = pr;				  //p链表指针方向逆转
		pr = p; p = q; q = q->link;   //三兄弟同步右移
	}
	p->link = pr;					  //头节点指针逆转
}
#endif
