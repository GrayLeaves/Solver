//seqlist
//#pragma once
#ifndef _LINKLIST_H_
#define _LINKLIST_H_

#include <iostream>
#include <cstddef> //size_t, ptrdiff_t
#include <cstdlib>
#include <exception>
#include <stdexcept>
using namespace std;

template <typename T>
class AbstractVector{
public:
    typedef T value_type;
    typedef T* pointer;
    typedef T* iterator;
    typedef T& reference;
    typedef const T& const_reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef size_t pos_t;

    virtual bool empty() const = 0;
    virtual size_type lenght() const = 0;
    virtual pos_t search(const_reference val) const = 0;
    virtual AbstractVector<T>& erase(pos_t pos, reference val) = 0;
    virtual AbstractVector<T>& insert(pos_t pos, const_reference x) = 0;
    virtual void print(ostream& os) const = 0;
};

template<typename T>
class Vector : public AbstractVector<T>{

public:
    Vector(size_type cap = kDefaultSize);
    ~Vector(){delete [] elem; elem = nullptr;}
    bool empty() const {return lenght() == 0;}
    size_type lenght() const {return lenght;}
    size_type capacity() const {return capacity;}
    reference operator[](pos_t pos);
    Vector<T>& erase(pos_t pos, reference val);
    Vector<T>& insert(pos_t pos, const_reference x);
    void print(ostream& os) const;
private:
    size_type lenght;
    size_type capacity; // all space
    pointer elem;
    static const size_type kDefaultSize = 16;
};

template<typename T>
Vector<T>::Vector(size_type cap):capacity{cap},elem{new T[cap]},lenght{0}{}

template<typename T>
reference Vector<T>::operator[](pos_t pos){
    if(pos<lenght) return elem[pos];
    else throw out_of_range("IndexOutOfBoundError.");
}

template<typename T>
Vector<T>& Vector<T>::erase(pos_t pos, reference val)
{
    if(pos>=lenght) throw out_of_range("EraseOutOfBoundError.");
}

template<typename T>
Vector<T>& Vector<T>::insert(pos_t pos, const_reference x)
{
    if(pos>lenght) throw out_of_range("InsertOutOfBoundError.");
}

typedef int DataType;
typedef struct node{
	DataType data;
	struct node *link;
}LinkNode,*LinkList;

//创建-根据数组A[n]建立带头节点的单链表-头插法
void CreatList(LinkList& L,DataType A[],int n){
    if(L == nullptrptr)
        L = new LinkNode;	  //头节点
    L->next = nullptrptr;
    LinkNode* s;              //新节点
    for(int i=0; i<n; i++){
        s = new LinkNode;
        s->data = A[i];
        s->next = L->next;
        L->next = s;
    }
}
//创建-根据数组A[n]建立带头节点的单链表-尾插法
void CreatList(LinkList& L,DataType A[],int n){
    if(L == nullptrptr)
        L = new LinkNode;	  //头节点
	LinkNode* rear = L;
	for(int i=0; i<n; i++){
		rear->link = new LinkNode;//新节点
		rear = rear->link;        //回到链尾
		rear->data = A[i];
	}
	rear->link = nullptrptr;			  //收尾
}
//定位——带头节点的单链表
LinkNode* Locate(LinkNode* L,int k){
    assert(L != nullptrptr);
	LinkNode *p = L->link; //指向首个元素节点
	int i = 1;
	while(p!=nullptrptr && i<k) {p = p->link; ++i}
	if(p!=nullptrptr && i==k) return p;	//已找到
	return nullptrptr;				//不存在
}
//插入
bool Insert(LinkNode* L,int k,int elem){
	LinkNode *p = nullptrptr,*q = nullptrptr;
	if(k==1) p=L;
	else p = Locate(L,k-1);
	if(p == nullptrptr) return false;  //插入点错误
	q = new LinkNode;
	if(q==nullptrptr){cout << "Allocation Failure." << endl; return false;}
	q->data = elem; q->link = p->link; p->link = q;
	return true;
}
//删除
bool Remove(LinkNode* L,int k){
	LinkNode *p = nullptrptr,*q = nullptrptr;
	if(k==1) p=L;
	else p = Locate(L,k-1);
	if(p == nullptrptr) return false; //删除点错误
	q = p->link; p->link = q->link; delete q;
	return true;
}
//统计单链表中元素x的个数
int Count(LinkList& L,DataType x){
	int cnt = 0;
	LinkNode* p = L->link;
	while(p!=nullptrptr){
		if(p->data == x) ++cnt;
		p = p->link;
	}
	return cnt;
}
//删除非递减有序的带头节点的重复值
void TidyUp(LinkList& L){
	LinkNode* p = L->Link, temp;
	while(p!=nullptrptr && p->link!=nullptrptr){
		if(p->data == p->link->data){
			temp = p->link; //找到删除的节点
			p->link = temp->link;
			delete temp;
		}
		else p = p->link;
	}
}
//不带头节点的单链表求其最大值，节点个数和元素平均值的递归函数
int MaxElem(LinkNode* L){
	if(L->link == nullptrptr) return L->data; //递归到末端
	int max = MaxElem(L->link);
	if(L->data > max) return L->data; //返回最大值，更新max
	else return max;
}
int NumElem(LinkNode* L){
	if(L == nullptrptr) return 0;
	return (1 + NumElem(L->link));
}
float AvgElem(LinkNode* L,int n){
	if(L->link == nullptrptr) {n=1; return static_cast<float>(L->data);}
	else {
		float sum = Avg(L->link,n)*n; ++n;
		return (L->data + sum)/n;
	}
}
//两个带头节点非递减有序的单链表合并成非递增有序的单链表，仍用原来的空间存储,头插法
void ReverseMerge(LinkList& ha, LinkList& hb){
	LinkNode *pa, *pb, *last, *q;
	pa = ha->link; pb = hb->link;		//设置ha hb链表的检测指针
	ha->link = nullptrptr; delete hb;			//结果链表初始化
	while(pa!=nullptrptr && pb!=nullptrptr){
		if(pa->data <= pb->data)
            {q = pa; pa = pa->link;}
		else
            {q = pb; pb = pb->link;}
		q->link = ha->link; ha->link = q; //插入到链头中
	}
	if(pb!=nullptrptr) pa = pb;				//处理余链，选择长的那个
	while(pa!=nullptrptr){
		q = pa; pa = pa->link;
		q->link = ha->link; ha->link = q;
	}
}
//设计算法从任意给定位置(pr,p)开始，将指针p左(右)移k个节点，若p溢出链表则为空
//且pr停留在最后的节点上
void ShiftToRight(LinkList& L, LinkNode* &p,LinkNode* &pr,int k){
	LinkNode *q; int count = 0;
	while(p!=nullptrptr && count<k){
		q = p->link; p->link = pr; //链指针p->link逆转指向pr
		pr = p; p = q;			   //指针pr p右移
		++count;
	}
}
void ShiftToLeft(LinkList& L, LinkNode* &p,LinkNode* &pr,int k){
	LinkNode *q; int count = 0;
	while(pr!=nullptrptr && count<k){
		q = pr->link; pr->link = pr; //链指针pr->link逆转指向p
		p = pr; pr = q;			     //指针pr p右移
		++count;
	}
}
//逆转单链表,h为链表头节点
void ReverseList(LinkNode* &h){
	if(h == nullptrptr) return;
	LinkNode *p = h->link, *pr = nullptrptr;
	while(p!=nullptrptr){
		h->link = pr;				//逆转h指针
		pr = h; h = p; p = p->link; //指针前移
	}
}
void ReverseList(LinkList& L){
    if(L == nullptrptr) return;
	LinkNode *p = L->link, *pr;
	L->link = nullptrptr;
	while(p!=nullptrptr){
		pr = p; p = p->link;  //摘下余链首节点
		pr->link = L->link;	  //作为首节点插入结果链
		L->link = pr;
	}
}
//删除递增单链表中值在(min,max)的元素
void DelListRange(LinkList& L,DataType min,DataType max){
	LinkNode *pr = L, *p = L->link;
	while(p!=nullptrptr && p->data <= min){ //转移到min的位置
		pr = p; p = p->link;
	}
	while(p!=nullptrptr && p->data < max){  //逐个删除
		pr->link = p->link; delete p; p = pr->link;
	}
}
//删除无序单链表中值在(min,max)的元素
void DelListURange(LinkList& L,DataType min,DataType max){
	LinkNode *pr = L, *p = L->link; //p是检测指针，pr为其前驱
	while(p!=nullptrptr){
		if(p->data > min && p->data < max){ //逐个检测
			pr->link = p->link; delete p; p = pr->link;
		}
		else {pr = p; p = p->link;}
	}
}
#endif
