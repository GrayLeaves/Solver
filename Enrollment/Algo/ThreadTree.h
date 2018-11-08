//线索二叉树
//#pragma once
#ifndef _THREADTREE_H_
#define _THREADTREE_H_

#include <iostream>
#include <cmath>
#include <cstdlib>

using namespace std;

typedef int DataType;
typedef struct node{
	DataType data;
	struct node *left,*right;
}ThreadNode,*PreTree,*PostTree;
//ltag:false/left为左子女,true/left为前驱线索
//rtag:false/right为右子女,true/right为后继线索
//建立中序搜索树
void MakeInThread(ThreadNode *root){
	ThreadNode *pre = NULL;	//前驱结点指针
	if(root!=NULL){			//非空二叉树线索化
		InThread(root,pre); //中序遍历
		pre->right = NULL; pre->rtag = true;//处理中序遍历最后一个节点
	}
}
void InThread(ThreadNode *p,ThreadNode *&p){
	if(p!=NULL){
		InThread(p->left,pre);//递归，左子树线索化
		if(p->left==NULL){	  //建立当前节点的前驱线索
			p->left = pre; p->ltag = true;
		}
		if(pre!=NULL && p->right==NULL){//建立前驱结点的后继线索
			pre->right = p; pre->rtag = true;
		}
		pre = p;						//前驱跟上，当前指针向前遍历
		InThread(p->right,pre);   //递归，右子树线索化
	}
}
//中序线索二叉树的遍历
ThreadNode *First(ThreadNode *p){
	while(!(p->ltag)) p = p->left;
	return p;
}
ThreadNode *Next(ThreadNode *p){
	if(!(p->rtag)) return First(p->right);
	else return p->right;
}
void InOrder(ThreadNode *root){
	for(ThreadNode *p = First(root); p!=NULL; p = Next(p))
		cout << p->data << " ";
}
//前序线索二叉树
void MakePreThread(ThreadNode *t,ThreadNode *&pr){
	if(t!=NULL){
		if(t->left==NULL) {t->left = pr; t->ltag = true;}
		if(pr!=NULL && pr->right==NULL) {p->right = t; pr->rtag = true;}
		pr = t;
		MakePreThread(t->left,pr);
		MakePreThread(t->right,pr);
	}
}
void CreatPreThTree(PreTree& PT){
	ThreadNode *pre = NULL:
	MakePreThread(PT,pre);
	pre->rtag = true; pre->right = NULL;
}
//前序线索二叉树中查找指定节点的父节点
ThreadNode *Parent(PreTree& T,ThreadNode *p){
	if(p==NULL) return NULL;
	if(p->ltag) return p->left;
	else return Parent_Recur(T,p);
}
ThreadNode *Parent_Recur(ThreadNode *t,ThreadNode *p){
	if(t==NULL) return NULL;
	if(t->left==p||t->right==p) return t;
	if((ThreadNode *q = Parent_Recur(t->left,p))!=NULL) return q;
	else return Parent_Recur(p->right,p);
}
//前序线索二叉树
ThreadNode *preFirst(ThreadNode *p){
	return t;
}
ThreadNode *preLast(ThreadNode *p){
	if(t!=NULL && t->rtag) return preLast(t->right);//右子树非空
	else if(t!=NULL && !(t->ltag)) return pre(t->left);//右子树空而左子树非空
	      else return t;
}
ThreadNode *preNext(PreTree& T,ThreadNode *p){
	if(p!=NULL){
		if(!p->ltag) return p->left;
		else return p->right;
	}
}
ThreadNode *prePrior(PreTree& T,ThreadNode *p){
	if(p!=NULL)
		if(p->ltag) return p->left;
		else {
			ThreadNode *q = Parent(T,p);
			if(q->left==p) return q;
			else return preLast(q->left);
		}
}
void preTraversal(PreTree& T){
	for(ThreadNode *p = preFirst(T); p!=NULL; p = preNext(T,p))
		cout << p->data;
}
//后序线索二叉树
void MakePostThread(ThreadNode *t,ThreadNode *p){
	if(t!=NULL){
		MakePostThread(t->left,pr);
		MakePostThread(t->right,pr);
		if(t->left==NULL) {t->left = pr; t->ltag = true;}
		if(pr!=NULL && pr->right==NULL) {p->right = t; pr->rtag = true;}
		pr = t;
	}
}
void CreatPostThTree(PreTree& PT){
	ThreadNode *pre = NULL:
	MakePostThread(PT,pre);
	pre->rtag = true; pre->right = NULL;
}
ThreadNode *postLast(ThreadNode *t){
	return t;
}
ThreadNode *postFirst(ThreadNode *t){
	if(t!=NULL){
		if(!t->ltag) return postFirst(t->left);
		else if(!t->rtag) return postFirst(t->right);
			 else return t;
	}
}
ThreadNode *postPrior(PostTree& T,ThreadNode *p){
	if(p!=NULL){
		if(p->ltag) return p->left;
		else if(!p->rtag) return p->right;
			return p->left;
	}
}
ThreadNode *postNext(PreTree& T,ThreadNode *p){
	if(p!=NULL)
		if(p->rtag) return p->right;
		else {
			ThreadNode *q = Parent(T,p);
			if(q->right==p) return q;//t是父节点的右子女
			else if(q->rtag) return q;//t是父节点的左子女
			     else return postFirst(q->right);
		}
}
void preTraversal(PreTree& T){
	for(ThreadNode *p = postFirst(T); p!=NULL; p = postNext(T,p))
		cout << p->data;
}
#endif
