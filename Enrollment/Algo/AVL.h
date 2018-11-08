//平衡二叉查找树
//#pragma once
#ifndef _AVL_H_
#define _AVL_H_

#include <iostream>
#include <cmath>
#include <cstdlib>

using namespace std;

typedef int DataType;
typedef struct node{
	DataType data; int bf; //平衡因子
	struct node *left,*right;
}AVLNode,*AVLTree;

void RotateLL(AVLNode *&a){
	AVLNode *b = a->left; a->left = b->right;
	b->right = a; b->bf = a->bf = 0; a = b;//右单旋，a为新根
}
void RotateRR(AVLNode *&a){
	AVLNode *b = a->right; a->right = b->left;
	b->left = a; b->bf = a->bf = 0; a = b;//左单旋，a为新根
}
void RotateLR(AVLNode *&a){
	AVLNode *b = a->left,*c = b->right;
	b->right = c->left; c->left = b; //左单旋，c为局部新根
	if(c->bf <= 0) b->bf = 1;		 //插入后c->bf <= 0
	else b->bf = 0;
	a->left = c->right; c->right = a;//右单旋，c为全局新根
	if(c->bf == -1) a->bf = 0;		 //插入后c->bf = -1
	else a->bf = -1;
	c->bf = 0; a = c;
}
void RotateRL(AVLNode *&a){
	AVLNode *b = a->right,*c = b->left;
	b->left = c->right; c->right = b; //右单旋，c为局部新根
	if(c->bf >= 0) b->bf = -1;		  //插入后c->bf >= 0
	else b->bf = 0;
	a->right = c->left; c->left = a;  //左单旋，c为全局新根
	if(c->bf == 1) a->bf = 0;		  //插入后c->bf = 1
	else a->bf = 1;
	c->bf = 0; a = c;
}
bool isAVL(AVLNode *t,int& h){
	int hl, hr;
	if(t == NULL) {h = 0; return true;}
	if(t->left == NULL && t->right == NULL) {h = 1; return true;}
	if(!isAVL(t->left,hl)) return false;
	if(!isAVL(t->right,hr)) return false;
	h = (hl>hr)? hl+1:hr+1;
	return abs(hl-hr)<2;
}
#endif
