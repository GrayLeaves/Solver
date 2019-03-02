//seqlist
//#pragma once
#ifndef _SEQLIST_H_
#define _SEQLIST_H_

#include <iostream>
#define MAXSIZE 64
#define SIZE 16
using namespace std;

typedef int DataType;
struct SeqList{
	DataType data[SIZE];
	int size;
	SeqList():size(SIZE){
		for(int i=0; i<size; ++i) data[i] = 0;
	}
};
//交换
inline Swap(DataType& left, DataType& right){
	DataType tmp = left;
	left = right;
	right = tmp;
}
//逆序
void Reverse(SeqList& L)
{
	if(L.size == 0) return;
	for(int i=0; i<=L.size/2; ++i){
		Swap(L.data[i],L.data[L.size-i-1]);
	}
}
//删除给定位置i的元素并将该值返回为x——顺序表
bool Del_pos(SeqList& L,int i,DataType x){
	if(L.size == 0) {cout << "Empty." << endl; return false;}
	if(i<1||i>L.size-1) {cout << "Out of range." << endl; return false;}
	x = L.data[i];
	for(int j=i; j<L.size; ++j) L.data[j-1] = L.data[j];
	L.size--;
	return true;
}
//删除给定值x的所有元素——无序表
void Del_val(SeqList& L,DataType x){
	int k = 0; //记忆指针
	for(int i=0; i<L.size; ++i){
		if(L.data[i]!=x){
			if(i!=k) L.data[k++] = L.data[i];
		}
	}
	L.size = k;
}
//删除其值在给值在(s,t)内的所有元素——无序表
bool Del_val_urange(SeqList& L,DataType s,DataType t){
	if(L.size == 0) {cout << "Empty." << endl; return false;}
	if(s>t) Swap(s,t);
	int k = 0;
	for(int i=0; i<L.size; ++i){
		if(L.data[i]<s||L.data[i]>t){
			if(i!=k) L.data[k++] = L.data[i];
		}
	}
	L.size = k;
	return true;
}
//删除其值在给值在(s,t)内的所有元素——有序表
bool Del_val_range(SeqList& L,DataType s,DataType t){
	if(L.size == 0) {cout << "Empty." << endl; return false;}
	if(s>t) Swap(s,t);
	int i,j,u,v;
	for(i=0; i<L.size&&L.data[i]<s; ++i); //定位到左端
	if(i==L.size) {cout << "All elements < s" << endl; return false;}
	for(j=i; j<L.size&&L.data[i]<t; ++j); //定位到右端
	for(u=i,v=j; v<L.size; ++u,++v) L.data[u] = L.data[v];
	L.size = L.size-j+i;
	return true;
}
//合并两顺序表A、B到C中
bool Merge(SeqList& A,SeqList& B,SeqList& C){
	if(A.size+B.size>C.size) return false;
	int i=0, j=0, k=0;
	while(i<A.size&&j<B.size){ //归并
		if(A.data[i]<=B.data[j]) C.data[k++] = A.data[i++];
		else C.data[k++] = B.data[j++];
	}   //处理余下的部分
	while(i<A.size) C.data[k++] = A.data[i++];
	while(j<B.size) C.data[k++] = A.data[j++];
	C.size = k;
	return true;
}
#endif
