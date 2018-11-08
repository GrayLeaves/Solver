//二叉树
//#pragma once
#ifndef _BT_H_
#define _BT_H_

#include <iostream>
#include <cmath>
#include <cstdlib>

using namespace std;

typedef int DataType;
typedef struct node{
	DataType data;
	struct node *left,*right;
}BTNode,*BT;

//递归遍历
void PreOrder(BTNode *t){
	if(t!=NULL){
		cout << t->data << " ";
		PreOrder(t->left);
		PreOrder(t->right);
	}
}
void InOrder(BTNode *t){
	if(t!=NULL){
		InOrder(t->left);
		cout << t->data << " ";
		InOrder(t->right);
	}
}
void PostOrder(BTNode *t){
	if(t!=NULL){
		PostOrder(t->left);
		PostOrder(t->right);
		cout << t->data << " ";
	}
}
//非递归遍历
#include "stack.h"
#include "queue.h"
void InOrder(BT T){
	stack S; initStack(S);
	BTNode *p = T;
	do{
		while(p!=NULL){
			Push(S,p); p = p->left;
		}
		if(!IsEmpty(S)){
			Pop(S,p);
			cout << p->data << " ";
			p = p->right;
		}
	}while(p!=NULL && !IsEmpty(S));
}
void LevelOrder(BT T){
	BTNode *p;
	Queue Q; initQueue();
	enQueue(Q,T);
	while(!IsEmpty(Q)){
		deQueue(Q,p);
		cout << p->data << " ";
		if(p->left  != NULL) enQueue(Q,p->left);
		if(p->right != NULL) enQueue(Q,p->right);
	}
}
//完全二叉树存放在数组T[n]中，起始位置为0，转换为二叉链表
//节点i的左子女为2i+1，右子女为2i+2
//ConstructTree(T,n,0,root);
void ConstructTree(DataType T[],int n,int i,BTNode* &p){
	if(i>n) p = NULL;
	else{
		p = new BTNode;
		p->data = T[i];
		ConstructTree(T,n,2*i+1,p->left);
		ConstructTree(T,n,2*i+2,p->right);
	}
}
void TreeToArray(BTNode *t,DataType A[],int n,int i){
	if(t == NULL) return;
	if(i >= n) {cerr << "空间不足" << endl; exit(1);}
	A[i] = t->data;
	TreeToArray(t->left,A,n,2*i+1);
	TreeToArray(t->right,A,n,2*i+2);
}
//二叉链树中指定节点的父节点
BTNode* Parent(BTNode *t,BTNode *p){
	if(t == NULL) return NULL;
	if(t->left == p || t->right == p) return p;
	BTNode *p = Parent(t->left,p);
	if(p!=NULL) return p;
	else return Parent(t->right,p);
}
//统计二叉链树中度为0,1,2的节点数
int Degree0(BTNode* t){
	if(t==NULL) return 0;
	if(t->left == NULL && t->right == NULL) return 1;
		return Degree0(t->left)+Degree0(t->right);
}
int Degree1(BTNode* t){
	if(t==NULL) return 0;
	if(t->left != NULL && t->right == NULL || t->right != NULL && t->left == NULL)
		return 1+Degree1(t->left)+Degree1(t->right);
	else return  Degree1(t->left)+Degree1(t->right);
}
int Degree2(BTNode* t){
	if(t==NULL) return 0;
	if(t->left != NULL && t->right != NULL)
		return 1+Degree2(t->left)+Degree2(t->right);
	else return  Degree2(t->left)+Degree2(t->right);
}
//二叉链树指定节点所在深度 depth = Level(root,p,1)
int Level(BTNode *t,BTNode *p,int d){
	if(t == NULL) return 0;
	if(t == p) return d;
	int sub = Level(t->left,p,d+1);
	if(sub > 0) return sub;
	else return Level(t->right,p,d+1);
}
//删除叶节点
void defolite(BTNode* &t){
	if(t == NULL) return;
	if(t->left == NULL && t->right == NULL) {delete t; t = NULL;}
	else {defolite(t->left); defolite(t->right);}
}
//求二叉链树最值
void maxVal(BTNode *t,DataType& max){
	if(t!=NULL){
		if(t->data > max) max = t->data;
		maxVal(t->left,max);
		maxVal(t->right,max);
	}
}
//前序序列转为后序序列
void PreToPost(BTNode* pre[],BTNode* post[],int s1,int t1,ints2,int t2){
	int n = t1-s1+1; m = (n-1)/2;
	if(n!=0){									//递归结束为n=0
		post[t2] = pre[s1];
		PreToPost(pre,post,s1+1,s1+m,s2,s2+m-1);//左子树递归
		PreToPost(pre,post,t1-m+1,t1,t2-m,t2-1);
	}
}
//已知二叉树的前序序列pre[s1,..,t1]和中序序列in[s2,..,t2],构造该二叉链树
void MakeTree(BTNode* &t, DataType pre[], DataType in[], int s1, int t1, int s2, int t2){
	int i;
	if(s1<=t1){
		t = new BTNode;
		t->data = pre[s1]; t->left = t->right = NULL;
		for(i=s2; i<=t2; ++i){
			if(in[i] == pre[s1]) break; //中序序列中查找根
		}
		MakeTree(t->left,pre,in,s1+1,s1+i-s2,s2,i-1);
		MakeTree(t->right,pre,in,s1+i-s2+1,t1,i+1,t2);
	}
}
//已知二叉树的后序序列post[s1,..,t1]和中序序列in[s2,..,t2],构造该二叉链树
void MakeTree(BTNode* &t, DataType post[], DataType in[], int s1, int t1, int s2, int t2){
	if(s1<=t1){
		t = new BTNode;
		t->data = post[t1]; t->left = t->right = NULL;
		for(i=s2; i<=t2; ++i){
			if(in[i] == post[t1]) break; //中序序列中查找根
		}
		MakeTree(t->left,post,in,s1+i-s2,t1-1,i+1,t2);
		MakeTree(t->right,post,in,s1,s1+i-s2-1,s2,i-1);
	}
}
//二叉链树中找到指定节点并记录过程
bool Find_Print(BTNode *t,DataType x,DataType path[],int level,int count){
	if(t!=NULL){
		path[level-1] = t->data;
		if(t->data == x) {count = level; return true;}
		if(Find_Print(t->left,x,path,level+1,count)) retuen true;
		return Find_Print(t->right,x,path,level+1,count);
	}
	return false;
}
//查找前序序列的第n个节点 PreSearchK(root,count,k)/*count = 0*/
BTNode* PreSearchK(BTNode *t,int &count,int k){
	if(t!=NULL){
		if(++count == k) retuen t;
		BTNode* p = PreSearchK(t->left,count,k);
		if(p!=NULL) return p;
		return PreSearchK(t->right,count,k);
	}
	return NULL;
}
//后序遍历求树高并判断树是否平衡
bool HeightBalance(BTNode *t,int& height){
	if(t!=NULL){
		int lh, rh;
		if(!HeightBalance(t->left,lh)) return false;
		if(!HeightBalance(t->right,rh)) return false;
		height = (lh>rh)? 1+lh : 1+rh;
		if(abs(lh-rh)<2) return true;
		else return false;
	}
	else {height = 0; return true;}
}
//求二叉链树中每层节点数并返回该层最大节点数（树宽）
int LevelNodes(BT& T,int count[],int& level){
	BTNode* p = T;
	if(p!=NULL){
		level = 0;
		int front=0, rear=0, last=1, num=0, max=0;//last 是每层最后节点号
		BTNode* Q[n]; rear++; Q[rear] = p;		  //根进队列
		while(front!=rear){
			front = (front+1)%n; p = Q[front]; num++; // 记录节点数
			if(p->left!=NULL) {rear = (rear+1)%n; Q[rear] = p->left;}
			if(p->right!=NULL) {rear = (rear+1)%n; Q[rear] = p->right;}
			if(front == last){
				last = rear; count[level++] = num; num = 0;
			}
		}
		for(num = 0; num<level; ++num)
			if(count[num]>max) max = count[num];
		return max;
	}
	return 0;
}
//判断是否为完全二叉树
bool isCompleted(BT& T){
	BTNode* p = T;
	if(p == NULL) return true;
	BTNode* Q[n], int front=0, int rear=0;
	bool flag = false;				//标志队列中未遇到空队列元素
	Q[rear] = p; rear = (rear+1)%n;
	while(front!=rear){
		p = Q[front]; front = (front+1)%n;
		if(p == NULL) flag = true;	//遍历中遇到空队列
		else{
            if(flag) return false; //前面夹杂着空队列元素，非完全二叉树
            else {					//不管子女是否为空都入队
                Q[rear] = p->left; rear = (rear+1)%n;
                Q[rear] = p->right; rear = (rear+1)%n;
            }
		}
	}
	return true;
}
#endif
