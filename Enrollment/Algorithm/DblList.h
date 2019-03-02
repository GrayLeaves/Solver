//seqlist
//#pragma once
#ifndef _DBLLIST_H_
#define _DBLLIST_H_

#include <iostream>
#include <cmath>
#include <cstdlib>
using namespace std;
//双向链表
typedef int DataType;
typedef struct node{
	DataType data;
	struct node *next, *prev;
}DblNode,*DblList;

//一元多项式链
typedef struct Term{
	float coef;
	int exp;
	struct Term *link;
}*Polynomial;

//删除双向链表中元素值为x的节点
bool Remove(DblList L, DataType x){
	DblNode *p = L->link;
	while(p!=L && p->data!=x) p = p->link;
	if(p == L) return false;
	p->prev->next = p->next;
	p->next->prev = p->prev;
	delete p; return true;
}
//逆转带头节点的双向循环链表
void Reverse(DblList L){
	DblNode *p = L, *q;
	do{//交换p的前驱和后继
		q = p->next;
		p->next = p->prev;
		p->prev = q;
		p = q;
	}while(p!=L);
}
//排序,prev链域把所有节点元素值递增链接
void OrderedLink(DblList& DL){
	DblNode *pr, *p, *s = DL->next->next;
	DL->prev = DL->next; DL->prev->prev = nullptr;
	while(s!=nullptr){
		p = DL->prev; pr = DL;				//从链表启动检测
		while(p!=nullptr && p->data < s->data){
			pr = p; p = p->prev;			//找寻插入点
		}
		pr->prev = s; s->prev = p;			//链入prev
		s = s->next;						//处理下一个
	}
}
//生成一元多项式
void CreatPolynomial(Polynomial& L){
	Term *p, *pr, *s;
	float c; int e; char flag;
	cout << "Input a term (coef,exp):" << endl;
	cin >> c >> e;
	while(e>=0){
		p = L->link; pr = L;
		while(p!=nullptr && p->exp < e) {pr = p; p = p->link;}
		if(p!=nullptr && p->exp == e){
			cout << "Exist and whether to replace it [y/n]:" << endl;
			cin >> flag; if(flag == 'y') p->coef = c;
		}
		else{
			s = new Term;
			if(s==nullptr){cout << "Allocation Failure." << endl; exit(1);}
			s->coef = c; s->exp = e;
			pr->link = s; s->link = p;
		}
		cout << "Input a term (coef,exp):" << endl;
		cin >> c >> e;
	}
}
//一元多项式插入新项
void InsertPolynomial(Polynomial& L,float c,int e){
	Term *pre = L, *p = L->link;
	while(p!=nullptr && p->exp > e) {pre = p; p = p->link;}
	if(p->exp == e){
		if(p->coef + c != 0) p->coef = p->coef + c; //合并
		else {pre->link = p->link; delete p;}		//或删除
	}
	else {
		Term *pc = new Term;
		pc->exp = e; pc->coef = c;
		pre->link = pc; pc->link = p;
	}
}
//一元多项式的乘法
void MultiplePolynomial(Polynomial& A,Polynomial& B,Polynomial& C){
	Term *pa = A->link, *pb;
	C = new Term; C->link = nullptr;
	while(pa!=nullptr){
		pb = B->link
		while(pb!=nullptr){
			InsertPolynomial(C,pa->coef * pb->coef, pa->exp + pb->exp);
			pb = pb->link;
		}
		pa = pa->link;
	}
}
//分别求稠密和稀疏一元多项式在x处的值
//降幂链接的稠密一元多项式
float calcValue(Polynomial& L,float x){
	Term *p = L->link; float result = 0.0f;
	while(p!=nullptr){
		result = result*x + p->coef;
		p = p->link;
	}
	return result;
}
//升幂链接的稠密一元多项式
float calcValueR(Polynomial& L,float x){
	Term *pr = L, *p = L->link, *q; float result = 0.0f;
	while(p!=nullptr){
		q = p->link; p->link = pr; pr = p; p = q;	//逆转单链表
	}
	while(pr!=L){
		result = result*x + p->coef;
		q = pr->link; pr->link = p; p = pr; pr = q;
	}
	pr->link = p;
	return result;
}
//升幂链接的稀疏一元多项式
float calcValueS(Polynomial& L,float x){
	Term *p = L->link; float result = 0.0f;
	while(p!=nullptr){
		result += pow(x,p->exp) * p->coef;
		p = p->link;
	}
	return result;
}
//Josephus环，n个人围成圈，从第s位报数，数到m的出局
void Josephus(int s,int m,int n){
	int i,j, k = n;
	int *A = new int[n];
	for(i=0; i<n; ++i) A[i] = i+1; //编号
	i = (s-1+n)%n;  //预除名的号码
	while(k>0){
		for(j=1; j<m; j++) i = (i+1)%k;// i = (i+m-1)%k;
		cout << A[i] << " ";
		for(j=i+1; j<k; j++) A[j-1] = A[j];
		--k;
	}
	cout << endl;
	delete [] A;
}
#endif
