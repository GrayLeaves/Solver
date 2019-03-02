#ifndef _HEAP_DISSET_H_
#define _HEAP_DISSET_H_

#include <iostream> 
#define maxSize 64
using namespace std;

//最小堆
typedef int DataType;
typedef struct MinHeap{
	DataType heap[maxSize];
	int size;
};

//建堆
void MakeHeap(MinHeap &H,DataType arr[],int n){
	for(int i=0; i<n; ++i) H.heap[i] = arr[i];
	H.size = n;
	int cur_pos = (H.size-2)/2;		//最初调整位置在最后分支结点
	while(cur_pos>=0){
		SiftDown(H,cur_pos,H.size-1);//局部自上往下筛选
		--cur_pos;
	}
}
void SiftDown(MinHeap &H,int i,int m){
	//自i到m，自上往下比较，小则上浮，局部调整为最小堆
	DataType temp = H.heap[i];
	for(int j=2*i+1; j<=m; j=2*j+1){//j是i的left，检查到最后位置
		if(j<m && H.heap[j].key > H.heap[j+1].key) ++j;//j指向子女中最小者
		if(temp.key <= H.heap[j].key) break;//小则不做调整
		else {
			H.heap[i] = H.heap[j]; i = j;//小则上浮，i下降
		}
	}
	H.heap[i] = temp;					//回收temp元素
}

bool InsertHeap(MinHeap &H,DataType x){
	if(H.size == maxSize) return false;
	H.heap[H.size] = x;
	SiftUp(H,H.size);
	++H.size;
	return true;
}
void SiftDown(MinHeap &H,int start){
	//自start到0，自上往下比较，小则上浮，局部调整为最小堆
	DataType temp = H.heap[start];
	int j = start, i = (j-1)/2;
	while(j>0){
		if(temp.key >= H.heap[i].key) break;//小则不做调整
		else {
			H.heap[j] = H.heap[i]; j = i; i = (i-1)/2;//小则上浮，i下降
		}
	}
	H.heap[i] = temp;					//回收temp元素
}

bool RemoveHeap(MinHeap &H,DataType &x){
	if(!H.size) return false;
	x = H.heap(0); H.heap[0] = H.heap[H.size-1];//最后的元素填补到根节点
	--H.size;
	SiftDown(H,0,H.size-1);//自上往下调整为堆
	return true;
}

//最大最小堆
typedef struct{
	DataType elem[maxSize];
	int size;
}MinMaxHeap;

void MinMaxHeapInsert(MinMaxHeap &H, DataType x){
	int m = H.size+1, h = 0;	//m为结点个数，h为高度
	int i, j = H.size;			//j是插入位置
	while(m>0) {m /= 2; ++h;}	//计算高度
	if(h%2==0){					//插入结点在偶数层（最大层）
		i = (j-1)/2;			//父节点
		if(x<H.elem[i]){		//插入元素小于父节点则交换
			do{
				H.elem[j] = H.elem[i]; j = i; i = (j-3)/4;
			}while(i>=0 && x<H.elem[i]);
			H.elem[j] = x;
		}
		else {
			i = (j-3)/4;
			while(i>0 && x>H.elem[i]){
				H.elem[j] = H.elem[i]; j = i; i = (j-3)/4; 
			}
			H.elem[j] = x;
		}
	}
	else {
		i = (j-1)/2;
		if(x>H.elem[i]){
			do{
				H.elem[j] = H.elem[i]; j = i; i = (j-3)/4;
			}while(i>0 && x>H.elem[i]);
			H.elem[j] = x;
		}
		else {
			i = (j-3)/4;
			while(i>=0 && x<H.elem[i]){
				H.elem[j] = H.elem[i]; j = i; i = (j-3)/4; 
			}
			H.elem[j] = x;
		}
	}
}
//并查集
int UFSet[maxSize];
//初始化
void Initial(int S[]){
	for(int i=0; i<maxSize; ++i) S[i] = -1;
}
//寻根
int Find(int S[],int x){
	while(S[x]>=0) x = S[x];
	return x;
}
//合并
void Merge(int S[],int l,int r){
	S[r] = l;
}
//加权合并的优化策略
void WeightedMerge(UFSet &S,int l,int r){
	int i = Find(S,l), j = Find(S,r), temp;
	if(i!=j){
		temp = S.parent[i] + S.parent[j];
		if(S.parent[j] < S.parent[i]){			//以r为根的树节点多
			S.parent[i] = j; S.parent[j] = temp;//l接r下面，r为新根
		}
		else{
			S.parent[j] = i; S.parent[i] = temp;
		}
	}
}
//查找中路径压缩
int CollapsingFind(UFSet &S,int i){
	int r,temp;
	for(r=i; S.parent[r]>=0; r=S.parent[r]);//查找i所在树的根，用r指示
	while(i!=r){
		temp = S.parent[i]; S.parent[i] = r; i = temp;//让i沿着根的路径上移，沿途结点父指针指向根r
	}
	return r;//返回根
}
#endif