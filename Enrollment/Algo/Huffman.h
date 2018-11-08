//哈夫曼树
//#pragma once
#ifndef _HUFFMAN_H_
#define _HUFFMAN_H_

#include <iostream> 
#include <cmath>
#include <cstdlib>
#define maxSize 32
using namespace std;

typedef char DataType;
typedef int WeightType;
typedef struct HTNode{
	DataType data;//值
	WeightType weight;//权
	int parent, left, right;//父左右
}
typedef struct HFTree{
	HTNode elem[maxSize];//哈夫曼树存储数组
	int num,root;  //num是外节点数，root是根节点
}
#define maxWeight Inf	//比所有权重都大的值
void MakeHRTree(HFTree& HF,DataType value[],WeightType fr[],int n){
	int i,j,k,s1,s2;
	WeightType min1,min2;
	for(i=0; i<n; ++i){
		HT.elem[i].data = value[i]; 
		HT.elem[i].weight = fr[i];
	}
	for(i=0; i<2*n-1; ++i){
		HT.elem[i].parent = HT.elem[i].left = HT.elem[i].right = -1;
	}
	for(i=n; i<2*n-1; ++i){
		min1 = min2 = maxWeight;  //min1是最小权值，min2是次小权重
		s1 = s2 = 0;			  //s1是最小权重点，s2是次小权重点
		for(k=0; k<i; ++k){
			if(HT.elem[k].parent == -1){
				if(HF.elem[k] <= min1){
					min2 = min1; min1 = HT.elem[k].weight;
					s2 = s1; s1 = k;
				}
			}
			else if(HT.elem[k].weight<min2){
					 min2 = HT.elem[k].weight; s2 = k;
			     }
		}
		HT.elem[s1].parent = HT.elem[s2].parent = i;
		HT.elem[i].left = s1; HT.elem[i].right = s2;
		HT.elem[i].weight = HT.elem[s1].weight + HT.elem[s2].weight;	
	}
	HT.num = n; HT.root = 2*n-2;
}
#define Len 32
typedef struct HFCode{//哈夫曼编码结构体
	char hcd[Len];		//节点哈夫曼编码存放数组
	int start;
}
//将哈夫曼树转换为编码
void MakeHFCode(HFTree& HF,HFCode HFcd[]){
	int i,f,c; HFCode hc;
	for(i=0; i<HF.num; ++i){
		HFcd.start = Len-1;
		c = i; f = HF.elem[c].parent;
		while(f!=-1){	//循环直到树的根节点
			if(HF.elem[f].left == c)//当前节点是其父节点的左子女
				HFcd.hcd[HFcd.start--] = '0';
			else HFcd.hcd[HFcd.start--] = '1'; //是其节点的右子女
			c = f; f = HF.elem[c].parent; //再对其父节点执行同种操作
		}
		HFcd.start++;//start指向哈夫曼编码的首个字符
	}
} 
#endif