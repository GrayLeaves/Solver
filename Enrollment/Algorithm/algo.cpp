//下面的程序可以从0....n-1中随机等概率的输出m个不重复的数。这里我们假设n远大于m
/*void knuth(int n, int m)
{
    srand((unsigned int)time(nullptrptr));
    for (int i = 0; i < n; i++) {
        if ( rand()%(n-i)<m ) {
            cout << i << endl;
            m--;
        }
     }
}
void quick_sort(vector<int>& lst,int left,int right){
    int i = left, j = right, pivot = lst[i];
    while(i<j){
        if(i<j && lst[j]>= pivot) --j;
        if(i<j) swap(lst[i],lst[j]);
        if(i<j && lst[i]<= pivot) ++i;
        if(i<j) swap(lst[i],lst[j]);
    }
    if(i!=left) quick_sort(lst,left,i-1);
    if(j!=right)quick_sort(lst,j+1,right);
}
void perm(vector<int> list, int k)
{
    if ( k==list.size()-1 )
    {
        copy(list.begin(), list.end(), ostream_iterator<int>(cout," "));
        cout << endl;
        return;
    }
    for (int i=k; i<list.size(); i++)
    {
        swap(list[k],list[i]);
        perm(list,k+1);
        swap(list[k],list[i]);
    }
}*/
#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
using namespace std;
//void test_sort(vector<int>& lst,(*fun)(vector<int>& lst));
void quick_sort(vector<int>& lst,int low,int high);
int partition(vector<int>& lst,int low,int high);
void heap_sort(vector<int>& lst);
void shell_sort(vector<int>& lst);
void filterDown(vector<int>& lst,int cur,int last);
int kth_elem(vector<int>& lst,int low,int high,int k);
void merge(vector<int>& A,int low,int high);
void merge_sort(vector<int>& A,int low,int high);
void bubble_sort(vector<int>& lst);
void insert_sort(vector<int>& lst);
void insert_sort_swap(vector<int>& lst);
int main()
{
    const int SIZE = 26;
    vector<int> lst(SIZE);
    for(int i=0; i<SIZE; i++){
        lst[i] = i+1;
    }
    random_shuffle(lst.begin(),lst.end());
    for(auto i : lst)
        cout << i << " ";
    cout << endl;
    /*for(int i=0; i<20; i++){
        cout << kth_elem(lst,0,lst.size()-1,i) << " ";
    }*/
    //insert_sort_swap(lst);
    shell_sort(lst);
    //bubble_sort(lst);
    //quick_sort(lst,0,lst.size()-1);
    //merge_sort(lst,0,lst.size()-1);
    copy(lst.begin(),lst.end(),ostream_iterator<int>(cout," "));
    cout << endl;
    return 0;
}

void bubble_sort(vector<int>& lst){
    int len = lst.size();
    bool exchange;
    for(int i=0; i<len; ++i){
        exchange = false;
        for(int j=0; j<len-i; ++j){
            if(lst[j]>lst[j+1]){
                swap(lst[j],lst[j+1]);
                exchange = true;
            }
        }
        if(!exchange) break;
    }
}
void insert_sort_swap(vector<int>& lst){
    int len = lst.size();
    for(int i=1; i<len; ++i){
        for(int j=i-1; j>=0; --j)
            if(lst[j]>lst[j+1]){ //交换腾出空位
                swap(lst[j],lst[j+1]);
            }
    }
}
//折半插入排序仅减少了关键字之间的比较次数,但移动次数不变。
//因此时间复杂度还是O(n^2).算法稳定性:稳定。
void insert_sort(vector<int>& lst)
{
    int len = lst.size();
    for(int i=1; i<len; ++i){
        int low = 0, high = i-1;
        while(high>=low){
            int mid = (low+high)/2;
            if(lst[mid]<lst[i]) low = mid+1;
            else high = mid-1;
        }
        int tmp = lst[i];
        for(int k=i-1; k>high; --k){
            lst[k+1] = lst[k];
        }
        lst[high+1] = tmp;
    }
}
void shell_sort(vector<int>& lst){
	int len = lst.size();
	int i, j, num, gap = len;//初始增量;
	while(gap != 1){
        gap = gap/3+1;
		for(i=gap; i<len; ++i){ //对gap间隔的子序列排序
			num = lst[i];       //寻找插入位置
			for(j=i; j>=gap && num<lst[j-gap]; j -= gap)
                lst[j] = lst[j-gap]; //腾出插入位置
			lst[j] = num;
		}
	}
}
void quick_sort(vector<int>& lst,int low,int high){
    if(low<high){
        int pos = partition(lst,low,high);
        quick_sort(lst,low,pos-1);
        quick_sort(lst,pos+1,high);
    }
}
int partition(vector<int>& lst,int low,int high){
    int pivot = lst[low];  //首个元素作为中轴值
    while(low<high){        //结束条件,填坑完又形成新坑
        while(low<high && lst[high]>=pivot) --high; //找到比中轴值小的元素
        //if(low<high){lst[low++] = lst[high];}  //当low!=high,移到左边,low往后
        lst[low] = lst[high]; //简写式
        while(low<high && lst[low]<=pivot) ++low;
        //if(low<high){lst[high--] = lst[low];} //当low!=high,移到右边,high回退
        lst[high] = lst[low]; //简写式
    }
    lst[low] = pivot; //中轴值移到最终位置
    return low; //返回该位置
}
int kth_elem(vector<int>& lst,int low,int high,int k){
    int pivot = lst[low];
    int left = low, right = high;
    while(low<high){
        while(low<high && lst[high]>=pivot) --high;
        lst[low] = lst[high];
        while(low<high && lst[low]<=pivot) ++low;
        lst[high] = lst[low];
    }
    lst[low] = pivot;
    if(low == k) return lst[low];
    else{
        if(low > k)
            return kth_elem(lst,left,low-1,k);
         else
            return kth_elem(lst,low+1,right,k);
    }
}
void heap_sort(vector<int>& lst){
    int len = lst.size();
    for(int i = len/2-1; i>=0; --i)
        filterDown(lst,i,len-1);
    for(int i = len-1; i>0; --i){
        swap(lst[0],lst[i]);
        filterDown(lst,0,i-1);
    }
}
void filterDown(vector<int>& lst,int cur,int last){
    int child = 2*cur+1, tmp = lst[cur]; //左儿子，暂存
    while(child <= last){
        if(child<last && lst[child]<lst[child+1]) ++child; //指向数值大的儿子
        if(tmp >= lst[child]) break; //不必交换
        else{
            lst[cur] = lst[child]; //调换到双亲节点上
            cur = child; child = 2*child+1; //指向儿子
        }
    }
    lst[cur] = tmp;
}
void merge(vector<int>& A,int low,int high){
    vector<int> B = A; //辅助内存拷贝
    int mid = (low+high)/2;
    int i = low, j = mid+1, k = low;
    while(i<=mid && j<=high){ //前后两半归并
        if(B[i] <= B[j]) A[k++] = B[i++];
        else A[k++] = B[j++];
    }
    while(i<=mid) A[k++] = B[i++]; //处理余项
    while(j<=high) A[k++] = B[j++];
}
void merge_sort(vector<int>& A,int low,int high){
    if(low<high){
        int mid = (low+high)/2;
        merge_sort(A,low,mid); //左半
        merge_sort(A,mid+1,high);//右半
        merge(A,low,high);//归并
    }
}
// 基数排序：一种多关键字的排序算法，可用桶排序实现。
int maxbit(vector<int>& data)
{   //辅助函数，求数据的最大位数
    int maxData = data[0];
    for (int i = 1; i < data.size(); ++i)
    {
        if (maxData < data[i])
            maxData = data[i];
    }
    int d = 1;
    while (maxData > 9)
    {
        maxData /= 10;
        ++d;
    }
    return d;
}
void radixsort(vector<int>& data) //基数排序
{
    int d = maxbit(data);
    int n = data.size();
    vector<int> tmp(n);
    int i, j, k;
    int radix = 1;
    for(i = 1; i <= d; i++) //进行d次排序
    {
        vector<int> count(10,0); //每次分配前清空计数器
        for(j = 0; j < n; j++)
        {
            k = (data[j] / radix) % 10; //统计每个桶中的记录数
            count[k]++;
        }
        for(j = 1; j < 10; j++)
            count[j] += count[j - 1]; //将tmp中的位置依次分配给每个桶
        for(j = n - 1; j >= 0; j--) //将所有桶中记录依次收集到tmp中
        {
            k = (data[j] / radix) % 10;
            tmp[count[k] - 1] = data[j];
            count[k]--;
        }
        for(j = 0; j < n; j++) //将临时数组的内容复制到data中
            data[j] = tmp[j];
        radix = radix * 10;
    }
}
//逆转单链表,h为链表头节点
void ReverseList(LinkNode* &h){
	if(h == nullptr) return;
	LinkNode *p = h->link, *pr = nullptr;
	while(p!=nullptr){
		h->link = pr;				//逆转h指针
		pr = h; h = p; p = p->link; //指针前移
	}
}
void ReverseList(LinkList& L){
    if(L == nullptr) return;
	LinkNode *p = L->link, *pr;
	L->link = nullptr;
	while(p!=nullptr){
		pr = p; p = p->link;  //摘下余链首节点
		pr->link = L->link;	  //作为首节点插入结果链
		L->link = pr;
	}
}
//两个带头节点非递减有序的单链表合并成非递增有序的单链表，仍用原来的空间存储,头插法
void ReverseMerge(LinkList& ha, LinkList& hb){
	LinkNode *pa, *pb, *last, *q;
	pa = ha->link; pb = hb->link;		//设置ha hb链表的检测指针
	ha->link = nullptr; delete hb;			//结果链表初始化
	while(pa!=nullptr && pb!=nullptr){
		if(pa->data <= pb->data)
            {q = pa; pa = pa->link;}
		else
            {q = pb; pb = pb->link;}
		q->link = ha->link; ha->link = q; //插入到链头中
	}
	if(pb!=nullptr) pa = pb;				//处理余链，选择长的那个
	while(pa!=nullptr){
		q = pa; pa = pa->link;
		q->link = ha->link; ha->link = q;
	}
}
////递归遍历
void PreOrder(BTNode *t){
	if(t!=nullptr){
		cout << t->data << " ";
		PreOrder(t->left);
		PreOrder(t->right);
	}
}
void InOrder(BTNode *t){
	if(t!=nullptr){
		InOrder(t->left);
		cout << t->data << " ";
		InOrder(t->right);
	}
}
void PostOrder(BTNode *t){
	if(t!=nullptr){
		PostOrder(t->left);
		PostOrder(t->right);
		cout << t->data << " ";
	}
}
//非递归遍历
//#include "stack.h"
//#include "queue.h"
void InOrder(BT T){
	stack S; initStack(S);
	BTNode *p = T;
	do{
		while(p!=nullptr){
			Push(S,p); p = p->left;
		}
		if(!IsEmpty(S)){
			Pop(S,p);
			cout << p->data << " ";
			p = p->right;
		}
	}while(p!=nullptr && !IsEmpty(S));
}
void LevelOrder(BT T){
	BTNode *p = T;
	Queue Q; initQueue(Q);
	enQueue(Q,p);
	while(!IsEmpty(Q)){
		deQueue(Q,p);
		cout << p->data << " ";
		if(p->left  != nullptr) enQueue(Q,p->left);
		if(p->right != nullptr) enQueue(Q,p->right);
	}
}
//二叉链树中指定节点的父节点
BTNode* Parent(BTNode *t,BTNode *p){
	if(t == nullptr) return nullptr;
	if(t->left == p || t->right == p) return p;
	BTNode *p = Parent(t->left,p);
	if(p!=nullptr) return p;
	else return Parent(t->right,p);
}
//二叉链树指定节点所在深度 depth = Level(root,p,1)
int Level(BTNode *t,BTNode *p,int d){
	if(t == nullptr) return 0;
	if(t == p) return d;
	int sub = Level(t->left,p,d+1);
	if(sub > 0) return sub;
	else return Level(t->right,p,d+1);
}
//二叉链树中找到指定节点并记录过程
bool Find_Print(BTNode *t,DataType x,DataType path[],int level,int count){
	if(t!=nullptr){
		path[level-1] = t->data;
		if(t->data == x) {count = level; return true;}
		if(Find_Print(t->left,x,path,level+1,count)) retuen true;
		return Find_Print(t->right,x,path,level+1,count);
	}
	return false;
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
//后序遍历求树高并判断树是否平衡
bool HeightBalance(BTNode *t,int& height){
	if(t!=nullptr){
		int lh, rh;
		if(!HeightBalance(t->left,lh)) return false;
		if(!HeightBalance(t->right,rh)) return false;
		height = (lh>rh)? 1+lh : 1+rh;
		if(abs(lh-rh)<2) return true;
		else return false;
	}
	else {height = 0; return true;}
}
//判断是否为完全二叉树
bool isCompleted(BT& T){
	BTNode* p = T;
	if(p == nullptr) return true;
	BTNode* Q[n], int front=0, int rear=0;
	bool flag = false;				//标志队列中未遇到空队列元素
	Q[rear] = p; rear = (rear+1)%n;
	while(front!=rear){
		p = Q[front]; front = (front+1)%n;
		if(p == nullptr) flag = true;	//遍历中遇到空队列
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
//邻接表表示法
typedef struct Enode{
	int dest;
	Weight cost;
	struct Enode *link;
}EdgeNode;
typedef struct Vnode{
	Type data;
	struct Enode *adj;
}VertexNode;
typedef struct{
	VertexNode * VertexList;
	int numVertices, numEdges;
}ALGraph;
//邻接矩阵
//取顶点v的首个邻接顶点
int getFirstNeighbor(MGraph &G,int v){
	if(v!=-1){
		for(int col=0; col<G.numVertices; ++col){
			if(G.Edge[v][col]>0 && G.Edge[v][col]<maxWeight) return col;
		}
	}
	return -1;
}
//给出顶点v的某个邻接顶点w的下一个邻接顶点
int getNextNeighbor(MGraph &G,int v, int w){
	if(v!=-1 && w!=-1){
		for(int col=w+1; col<G.numVertices; ++col)
			if(G.Edge[v][col]>0 && G.Edge[v][col]<maxWeight) return col;
	}
	return -1;
}
Type getVal(MGraph &G,int v){
	if(v!=-1) return G.VertexList[v];
	else return -1;
}
Weight getWeight(MGraph &G,int v,int w){
	if(v!=-1 && w!=-1) return G.Edge[v][w];
	else return -1;
}
//邻接表
//取顶点v的首个邻接顶点
int getFirstNeighbor(ALGraph &G,int v){
	if(v!=-1){
		EdgeNode *p = G.VertexList[v].adj;
		if(p!=nullptr) return p->dest;
	}
	return -1;
}
//给出顶点v的某个邻接顶点w的下一个邻接顶点
int getNextNeighbor(MGraph &G,int v, int w){
	if(v!=-1){
		EdgeNode *p = G.VertexList[v].adj;
		while(p!=nullptr && p->dest!=w) p = p->link;
		if(p!=nullptr && p->link!=nullptr) return p->link->dest;
	}
	return -1;
}
//图的遍历
void DFS(Graph &G,int v,bool visited[]){
	visit(getVal(G,v));					//访问顶点v
	visited[v] = true;					//作个已访问过的标记
	int w = getFirstNeighbor(G,v);      //找v的首个邻接点w
	while(w!=-1){						//若存在
		if(!visited[w]) DFS(G,w,visited);//w未被访问过则递归
		w = getNextNeighbor(G,v,w);		//取v排在w后的下一个邻接顶点
	}
}
//非递归算法
void DFS(Graph &G,int v){
	int i,j,k; Weight w; Stack S;
	bool inStack[G.numVertices];
	for(int i=0; i<maxVertices; ++i) inStack[i] = false;
	Push(S,v); inStack[v] = true;
	while(!IsEmpty(S)){
		Pop(S,k);				    //栈中退出一个顶点
		cout << getVal(G,k) << " "; //访问
		for(j=n-1; j>=0; --j){      //检查k的所有邻接顶点
			w = getWeight(G,k,j);   //取k-j边的权重
			if(w>0 && w<maxWeight && !inStack[j]){
				Push(S,j); inStack[j] = true;//未进过栈的顶点进栈
			}
		}
	}
}
void BFS(Graph &G,int v){
	int i,w,n = numOfVertices(G);		//取图中的顶点数
	bool *visited = new bool[n]; 		//访问印记
	for(i=0; i<n; ++i) visited[i] = false;
	visit(getVal(G,v)); visited[v] = true;
	Queue Q; initQueue(Q); EnQueue(Q,v);//顶点入队，实现分层访问
	while(Q.IsEmpty()){					//外层循环检测队是否为空
		DeQueue(Q,v); 					//取出顶点v
		w = getFirstNeighbor(G,v);		//找v的首个邻接点w
		while(w!=-1){
			if(!visited[w]){
				visit(getVal(G,v));
				visited[v] = true;
				EnQueue(Q,v); 			//顶点w入队
			}
			w = getNextNeighbor(G,v,w);	//取v排在w后的下一个邻接顶点
		}
	}
	delete [] visited;					//释放印记
}

Type getVal(MGraph &G,int v){
	if(v!=-1) return G.VertexList[v].data;
	else return -1;
}
Weight getWeight(MGraph &G,int v,int w){
	EdgeNode *p = G.VertexList[v].adj;
	while(p!=nullptr && p->dest!=w) p = p->link;
	if(p!=nullptr) return p->cost;
	else return -1;
}
