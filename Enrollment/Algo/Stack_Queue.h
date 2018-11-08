//#pragma once
#ifndef _STACK_QUEUE_H_
#define _STACK_QUEUE_H_

#include <iostream>
#define maxSize 64
using namespace std;

//动态数组栈
typedef int DataType;
typedef struct{
	DataType *elem;
	int maxSize, top;
}SeqStack;

//链式栈
typedef struct node{
	DataType data;
	struct node *link;
}LinkNode,*LinkStack;

//双栈
typedef struct{
	DataType *elem;
	int top[2],bot[2];
	int size; //栈的容量
}DblStack;

//进栈
void Push(SeqStack& S,DataType x){
	if(S.top = maxSize-1) StackFull(S);
	S.elem[++S.top] = x;
}
//入栈
bool Pop(SeqStack& S,DataType& x){
	if(S.top = 0) {cout << "Empty." << endl; return false;}
	x = S.elem[S.top--];
	return true;
}
//栈满扩张
void StackFull(SeqStack& S){
	DataType* temp = new DataType[2*S.maxSize];
	if(temp == NULL) {cout << "Allocation Failure." << endl; exit(1);}
	for(int i=0; i<=S.top; ++i) temp[i] = S.elem[i];
	delete [] S.elem;
	S.maxSize *= 2;
	S.elem = temp;
}
//借助栈实现链表逆序
void Reverse(LinkList& L){
	LinkStack S; LinkNode *p = L->link, *q;
	if(p!=NULL){
		L->link = p->link; Push(S,p);	//摘下节点，进栈
		p = L->link;
	}
	p = L;
	while(!StackEmpty(S)){
		Pop(S,q);						//q指向退栈节点
		p->link = q; p = q;				//插入链尾中
	}
	p->link = NULL;
}

void initStack(DblStack& S,int sz){
	S.elem = new DataType[sz];
	if(S.elem == NULL) {cout << "Allocation Failure." << endl; exit(1);}
	S.size = sz; S.top[0] = S.bot[0] = -1; S.top[1] = S.top[1] = sz;
}
bool StackEmpty(DblStack& S, int i) {return top[i] == bot[i];}
bool StackFull(DblStack& S) {return top[0]+1 == top[1];}
bool Push(DblStack& S,DataType x, int i){
	if(StackFull(S)) return false;
	if(i==0) S.elem[++S.top[0]] = x;   //stack 0
	else S.elem[--S.top[1]] = x;	   //stack 1
	return true;
}
bool Pop(DblStack& S,DataType& x,int i){
	if(StackEmpty(S,i)) return false;
	if(i==0) x = S.elem[S.top[0]--];   //stack 0
	else x = S.elem[S.top[1]++];	   //stack 1
	return true;
}
bool getTop(DblStack& S,DataType& x,int i){
	if(StackEmpty(S,i)) return false;
	if(i==0) x = S.elem[S.top[i]];   //stack
	return true;
}
void makeEmpty(DblStack& S,int i){
	if(i==0) S.top[0] = S.bot[0] = -1;
	else S.top[1] = S.bot[1] = S.size;
}
//以数组Q.elem[maxSize]存放循环队列的元素，
//以Q.rear和Q.length分别表示队尾和队长
//求写出插入和删除操作
typedef struct{
	DataType elem[maxSize];
	int rear, length;
}CircQueue;

bool enQueue(CircQueue& Q, DataType x){
	if(Q.length == maxSize) return false;
	Q.rear = (Q.rear+1)%maxSize;
	Q.elem[Q.rear] = x;
	Q.length++;
	retuen true;
}
bool deQueue(CircQueue& Q, DataType x){
	if(Q.length == 0) return false;
	x = Q.elem[(Q.rear-Q.length+1+maxSize)%maxSize];
	Q.length--;
	retuen true;
}
//以数组Q.elem[maxSize]存放循环队列的元素，
//以Q.rear和Q.front分别表示队尾和队首
//Q.tag来识别队空还是满，求写出插入和删除操作
typedef struct{
	DataType elem[maxSize];
	int rear, front;
	bool tag;		//是否为不空的标志
}CircQueue;

bool enQueue(CircQueue& Q, DataType x){
	if(Q.front == Q.rear && Q.tag) return false;	//队满判错
	Q.rear = (Q.rear+1)%maxSize;
	Q.elem[Q.rear] = x;
	Q.tag = true;									//不空
	retuen true;
}
bool deQueue(CircQueue& Q, DataType x){
	if(Q.front == Q.rear && !Q.tag) return false;	//队空判错
	Q.front = (Q.front+1)%maxSize;
	x = Q.elem[Q.front];
	retuen true;
}
#endif
