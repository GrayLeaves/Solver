#ifndef _SORT_H_
#define _SORT_H_

#include <iostream>
#include <limits.h>
#define maxSize 128
#define maxVal INT_MAX

using namespace std;

//推荐风格的排序
#include <vector>
#include <iostream>
using namespace std;
class InsertSort{
public:
	InsertSort(vector<int> _list, int _len);
	void print();//输出排序完的数组
private:
	int len;
	vector<int> lst;
	insert_sort();
}
InsertSort::InsertSort(vector<int> _list, int _len){
	for(int i=0; i<_len; ++i) lst.push_back(_list[i]);
	this->len = _len;
}
//直接插入排序
void InsertSort::insert_sort(){
	int num; //插入值
	for(int i=1; i<len; ++i){
		num = lst[i]; //寻找并腾出插入的位置
		for(int j=i; j>0 && num<lst[j-1]; --j){lst[j] = lst[j-1];}
		lst[j] = num; //插入
	}
}
//二分插入排序
void BinaInsertSort::binary_insert_sort(){
	int mid, num, left, right;
	for(int i=0; i<len; ++i){
		num = lst[i];
		left = 0, right = i-1;
		while(left<=right){
			mid = (left+right)/2;
			if(num > lst[mid]) left = mid+1; //往右找
			else right = mid-1; //往左找
		}
		for(int j=i; j>left; --j) lst[j] = lst[j-1];
		lst[left] = num;
	}
}
//归并排序
void merge0(int A[],int low,int mid,int high){
    for(int k=low, k<=high; k++) B[k] = A[k];
    for(i=low,j=mid+1,k=i; i<=mid&&j<=high; ++k){
        if(B[i]<=B[j]) A[k] = B[i++];
        else A[k] = B[j++];
    }
    while(i<=mid) A[k++] = B[i++];
    while(j<=high) A[k++] = B[j++];
}
void merge_sort(int A[],int low,int high){
    if(low<high){
        int mid = (low+high)/2;
        merge_sort(A,low,mid);
        merge_sort(A,mid+1,high);
        merge0(A,low,mid,high);
    }
}
//希尔排序
void ShellSort::shell_sort(){
	int num; int gap = len/2;//初始增量
	while(gap != 0){
		for(int i=gap; i<len; ++i){ //对gap间隔的子序列排序
			num = lst[i];           //寻找插入位置
			for(int j=i; j>=gap && num<lst[j-gap]; j -= gap)
                {lst[j] = lst[j-gap];}
			lst[j] = num;
		}
		gap /= 2;
	}
}
//直接选择排序
void SelectSort::select_sort(){
	int k;
	for(int i=0; i<len; ++i){
		k = i;
		for(int j=i+1; j<len; ++j)
			if(lst[j] < lst[k]) k = j;
		swap(lst[i],lst[k]);
	}
}
//堆排序
void HeapSort::heap_sort(){
	for(int i=(len-2)/2; i>=0; --i) filterDown(i,len-1);//建立堆
	for(i=len-1; i>0; --i){
		swap(lst[0],lst[i]);
		filterDown(0,i-1);//不断调整为最大堆
	}
}
void HeapSort::filterDown(int cur, int last){
	int child = 2*cur+1;//child为子女位置
	int temp = lst[cur];//暂存子树根节点
	while(child <= last){
		if(child<last && lst[child]<lst[child+1]) child++;//child指向两子女中大者
		if(temp >= lst[child]) break;//temp大则不做调整
		else{//否则子女大者上浮
			lst[cur] = lst[child];
			cur = child; child = 2*child+1;//child下降到子女位置
		}
	}
	lst[cur] = temp;
}
//snaker排序
void SnakerSort::snaker_sort(){
	int i,left = 0,right = len-1, shift = 0;
	while(left<right){
		for(i=left; i<right; ++i){//向右冒泡排序
			if(lst[i]>lst[i+1]){
				swap(lst[i],lst[i+1]);
				shift = i;//记录最后一次所处状态
			}
		}
		right = shift;
		for(i=right; i>left; --i){
			if(lst[i]<lst[i-1]){
				swap(lst[i],lst[i-1]);
				shift = i;
			}
		}
		left = shift;
	}
}
//快速排序
void QuickSort::quick_sort(int left,int right){
	int i = left,j = right, pivot = lst[left];
	while(i<j){
		while(i<j && lst[j]>=pivot) --j;//找到比基数小的元素
		if(i<j) swap(i,j);
		while(i<j && lst[j]<=pivot) ++i;
		if(i<j) swap(i,j);
	}
	if(i!=left) quick_sort(left,i-1);//对lst[left,...,i-1]排序
	if(j!=right) quick_sort(j+1,right);
}
//归并排序
int MergeSort::list_merge(int st1,int st2){
	int k = 0, i = st1, j = st2;
	while(i&&j){//两序列未检测完
		if(lst[i]<=lst[j]){//将merge_list[i/j]中小的链接到merge_link[i]后面
			link[k] = i;
			k = i;
			i = link[i];
		}
		else{
			link[k] = j;
			k = j;
			j = link[i];
		}
		if(!i) link[k] = j;//余链处理
		else link[k] = i;
		return link[0];
	}
}
int MergeSort::merge_sort(int left,int right){
	if(left>=right) return left;
	int mid = (left+right)/2;
	return list_merge(merge_sort(left,mid),merge_sort(mid+1,right));//对左右两子序列归并
}
//计数排序
void CountingSort::counting_sort(){
	int *mark = new int[max+1]; //max为数组中的最大值
	memset(mark,0,max*sizeof(int));//标记无序表中元素
	for(int i=0; i<len; ++i) mark[lst[i]]++;//统计元素出现次数
	for(int i=1; i<=max; ++i) mark[i] += mark[i-1];//计算元素在有序表中的位置
	for(int i=len-1; i>=0; --i){
		result[mark[lst[i]]-1] = lst[i];
		mark[lst[i]]--;//通过mark将list中直接存入result中
	}
	delete [] mark;
}
//考研书上的排序解答
typedef struct DataList{
	Datatype elem[maxSize];
	int size;
}
//静态链表
typedef struct SLNode{
	Datatype data;
	int link;
}
typedef struct staticLinkedList{
	SLNode elem[maxSize];
	int size;
}
void InsertSort(DataList &L){
	Datatype temp; int i,j;
	for(i=1; i<=L.size-1; ++i)
		if(L.elem[i].key < L.elem[i-1].key){ //逆序查找插入位置，否则留着原位
			temp = L.elem[i];
			for(j=i-1; j>=0&&temp.key<L.elem[i].key; --j)
				L.elem[j+1] = L.elem[j];	//逆序查找temp插入位置
			L.elem[j+1] = temp;
		}
}
void BinaInsertSort(DataList &L){
	Datatype temp; int i,j,low,mid,high;
	for(i=1; i<L.size; ++i)
		if(L.elem[i].key < L.elem[i-1].key){
			temp = L.elem[i];
			low = 0; high = i-1;
			while(low<=high){		//利用折半查找寻找插入位置
				mid = (low+high)/2;
				if(temp.key < L.elem[mid].key) high = mid-1;
				else low = mid+1;
			}
			for(j=i-1; j>=low; --j) L.elem[j+1] = L.elem[j];//成块移动
			L.elem[low] = temp;
		}
}
void insert_sort(DataList &L,int start,int gap){
	Datatype temp; int i,j;
	for(i=start+gap; i<L.size; i+=gap)
		if(L.elem[i-gap].key > L.elem[i].key){ //发现逆序
			temp = L.elem[i]; j = i;		   //寻找插入点
			do{                                //腾出位置
				L.elem[j] = L.elem[j-gap];     //间隔gap做比较
				j -= gap;
			}while(j>0 && L.elem[j-gap].key>temp.key);
			L.elem[j] = temp;
		}
}
void ShellSort(DataList &L,int d[],int m){//m是增量数组d的大小
	int i, start, gap;
	for(i=0; i<m&&d[i]<L.size; ++i);      //增量初值在d[i-1]中
	while(i<m && i>0){
		for(start=0; start<d[i-1]; ++start)//调用d[i-1]次直接插入排序
			insert_sort(L,start,d[i-1]);
		--i;
	}		//直到d[0] = 1停止迭代
}
#endif
