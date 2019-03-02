//二叉查找树
//#pragma once
#ifndef _BST_H_
#define _BST_H_

#include <iostream>
#include <cmath>
#include <cstdlib>

using namespace std;

typedef int DataType;
typedef struct node{
	bool ltag, rtag;
	DataType data;
	struct node *left,*right;
}BSTNode,*BSTree;
//查找
BSTNode *Search(BSTree root,DataType x,BSTNode *&father){
	BSTNode *p = root; father = nullptr;//father是查找结点的父节点
	while(p!=nullptr && p->data!=x){
		father = p;
		if(x < p->data) p = p->left;
		else p = p->right;
	}
	return p;
}
//插入
bool Insert(BSTree& root,DataType x){
	BSTNode *s,*p,*f;
	p = Search(root,x,f);		//查找插入位置
	if(p != nullptr) return false;	//查找成功则不插入
	s = new BSTNode;
	if(s == nullptr) return false;
	s->data = x; s->left = nullptr; s->right = nullptr;
	if(f == nullptr) root = s;		//空树，新节点为根
	else{
        if(x < f->data) f->left = s;//
        else f->right = s;
	}
	return true;
}
//删除
bool Remove(BSTree& root,DataType x){
	BSTNode *s,*p,*f;
	p = Search(root,x,f);		//查找删除节点
	if(p == nullptr) return false;	//查找失败则无法删除
	if(p->left!=nullptr && p->right!=nullptr){
		s = p->left;			//有双子女，找p的中序前驱s
		while(s->right != nullptr) {f = s; s = s->right;}
		p->data = s->data; p = s;//用s代替p再删除s
	}
	if(p->left!=nullptr)s = p->left;//左子树非空，记下左子女节点
	else s = p->right;			//记下右子女节点
	if(p == root) root = s;		//删除的是根
	else if(s->data < f->data) f->left = s;//父节点直接串接子女节点
		 else f->right = s;
	delete p; return true;		//删除
}
//判断是否为二叉查找树
void isBST(BTNode *t,BTNode *&pr,bool &bs){
	if(t!=nullptr && bs){
		isBST(t->left,pr,bs);
		if(pr==nullptr) {pr = t; bs = true;}//t为中序首节点
		else{
			if(t->data > pr->data) {pr = t; ba = true;}
			else bs = false;
		}
		if(bs) isBST(t->right,pr,bs);
	}
}
//二叉查找树对数组A[n]进行排序
void BSTInsert(BSTree& T,DataType A[],int n){
	BSTNode *p, *pr, *q; int i;
	for(i=0; i<n; ++i){
		q = new BSTNode;				//创建新节点
		q->data = A[i]; q->left = q->right = nullptr;
		if(T == nullptr) T = q;			//新节点成为根节点
		else{
			p = T; pr = nullptr;			//从根往下查找结点
			while(p!=nullptr && p->data != q->data){
				pr = p;
				if(p->data < q->data) p = p->right;
				else p = p->left;
			}
			if(p == nullptr){				//找不到q，插入
				if(q->data < pr->data) pr->left = q;
				else pr->right = q;
			}
		}
	}
}
void InOrderTraverse(BSTNode *t,DataType A[],int& n){
	if(t!=nullptr){
		InOrderTraverse(t->left,A,n);
		A[n++] = t->data;
		InOrderTraverse(t->right,A,n);
	}
}
bool RemoveMax(BSTree& T){
	if(T==nullptr) return false;//空树，删除失败
	BSTNode *p = T,*pr = nullptr;
	while(p->right!=nullptr) {pr = p; p = p->right;}//沿着右子女链查找最右下节点
	if(pr == nullptr) {delete p; T = nullptr;} //树中只有根节点，删除
	else {pr->right = p->left; delete p;} //重新链接再删除
	return true;
}
//删除二叉查找树中所有不大于x的节点
void del_ngt(BSTNode *t,DataType x){
	BSTNode *p, *pr = nullptr;
	while(t!=nullptr){
		while(t!=nullptr && t->data <= x){
			p = t; t = t->right;
			delSubTree(p->left);
			delete p;
			if(pr!=nullptr) pr->left = t;
		}
		while(t!=nullptr && t->data > x){
			pr = t; t = t->left;
		}
	}
}
void delSubTree(BSTNode *p){
	if(p!=nullptr){
		delSubTree(p->left);
		delSubTree(p->right);
		delete p;
	}
}
#endif
