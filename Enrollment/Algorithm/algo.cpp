//����ĳ�����Դ�0....n-1������ȸ��ʵ����m�����ظ��������������Ǽ���nԶ����m
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
            if(lst[j]>lst[j+1]){ //�����ڳ���λ
                swap(lst[j],lst[j+1]);
            }
    }
}
//�۰��������������˹ؼ���֮��ıȽϴ���,���ƶ��������䡣
//���ʱ�临�ӶȻ���O(n^2).�㷨�ȶ���:�ȶ���
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
	int i, j, num, gap = len;//��ʼ����;
	while(gap != 1){
        gap = gap/3+1;
		for(i=gap; i<len; ++i){ //��gap���������������
			num = lst[i];       //Ѱ�Ҳ���λ��
			for(j=i; j>=gap && num<lst[j-gap]; j -= gap)
                lst[j] = lst[j-gap]; //�ڳ�����λ��
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
    int pivot = lst[low];  //�׸�Ԫ����Ϊ����ֵ
    while(low<high){        //��������,��������γ��¿�
        while(low<high && lst[high]>=pivot) --high; //�ҵ�������ֵС��Ԫ��
        //if(low<high){lst[low++] = lst[high];}  //��low!=high,�Ƶ����,low����
        lst[low] = lst[high]; //��дʽ
        while(low<high && lst[low]<=pivot) ++low;
        //if(low<high){lst[high--] = lst[low];} //��low!=high,�Ƶ��ұ�,high����
        lst[high] = lst[low]; //��дʽ
    }
    lst[low] = pivot; //����ֵ�Ƶ�����λ��
    return low; //���ظ�λ��
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
    int child = 2*cur+1, tmp = lst[cur]; //����ӣ��ݴ�
    while(child <= last){
        if(child<last && lst[child]<lst[child+1]) ++child; //ָ����ֵ��Ķ���
        if(tmp >= lst[child]) break; //���ؽ���
        else{
            lst[cur] = lst[child]; //������˫�׽ڵ���
            cur = child; child = 2*child+1; //ָ�����
        }
    }
    lst[cur] = tmp;
}
void merge(vector<int>& A,int low,int high){
    vector<int> B = A; //�����ڴ濽��
    int mid = (low+high)/2;
    int i = low, j = mid+1, k = low;
    while(i<=mid && j<=high){ //ǰ������鲢
        if(B[i] <= B[j]) A[k++] = B[i++];
        else A[k++] = B[j++];
    }
    while(i<=mid) A[k++] = B[i++]; //��������
    while(j<=high) A[k++] = B[j++];
}
void merge_sort(vector<int>& A,int low,int high){
    if(low<high){
        int mid = (low+high)/2;
        merge_sort(A,low,mid); //���
        merge_sort(A,mid+1,high);//�Ұ�
        merge(A,low,high);//�鲢
    }
}
// ��������һ�ֶ�ؼ��ֵ������㷨������Ͱ����ʵ�֡�
int maxbit(vector<int>& data)
{   //���������������ݵ����λ��
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
void radixsort(vector<int>& data) //��������
{
    int d = maxbit(data);
    int n = data.size();
    vector<int> tmp(n);
    int i, j, k;
    int radix = 1;
    for(i = 1; i <= d; i++) //����d������
    {
        vector<int> count(10,0); //ÿ�η���ǰ��ռ�����
        for(j = 0; j < n; j++)
        {
            k = (data[j] / radix) % 10; //ͳ��ÿ��Ͱ�еļ�¼��
            count[k]++;
        }
        for(j = 1; j < 10; j++)
            count[j] += count[j - 1]; //��tmp�е�λ�����η����ÿ��Ͱ
        for(j = n - 1; j >= 0; j--) //������Ͱ�м�¼�����ռ���tmp��
        {
            k = (data[j] / radix) % 10;
            tmp[count[k] - 1] = data[j];
            count[k]--;
        }
        for(j = 0; j < n; j++) //����ʱ��������ݸ��Ƶ�data��
            data[j] = tmp[j];
        radix = radix * 10;
    }
}
//��ת������,hΪ����ͷ�ڵ�
void ReverseList(LinkNode* &h){
	if(h == nullptr) return;
	LinkNode *p = h->link, *pr = nullptr;
	while(p!=nullptr){
		h->link = pr;				//��תhָ��
		pr = h; h = p; p = p->link; //ָ��ǰ��
	}
}
void ReverseList(LinkList& L){
    if(L == nullptr) return;
	LinkNode *p = L->link, *pr;
	L->link = nullptr;
	while(p!=nullptr){
		pr = p; p = p->link;  //ժ�������׽ڵ�
		pr->link = L->link;	  //��Ϊ�׽ڵ��������
		L->link = pr;
	}
}
//������ͷ�ڵ�ǵݼ�����ĵ�����ϲ��ɷǵ�������ĵ���������ԭ���Ŀռ�洢,ͷ�巨
void ReverseMerge(LinkList& ha, LinkList& hb){
	LinkNode *pa, *pb, *last, *q;
	pa = ha->link; pb = hb->link;		//����ha hb����ļ��ָ��
	ha->link = nullptr; delete hb;			//��������ʼ��
	while(pa!=nullptr && pb!=nullptr){
		if(pa->data <= pb->data)
            {q = pa; pa = pa->link;}
		else
            {q = pb; pb = pb->link;}
		q->link = ha->link; ha->link = q; //���뵽��ͷ��
	}
	if(pb!=nullptr) pa = pb;				//����������ѡ�񳤵��Ǹ�
	while(pa!=nullptr){
		q = pa; pa = pa->link;
		q->link = ha->link; ha->link = q;
	}
}
////�ݹ����
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
//�ǵݹ����
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
//����������ָ���ڵ�ĸ��ڵ�
BTNode* Parent(BTNode *t,BTNode *p){
	if(t == nullptr) return nullptr;
	if(t->left == p || t->right == p) return p;
	BTNode *p = Parent(t->left,p);
	if(p!=nullptr) return p;
	else return Parent(t->right,p);
}
//��������ָ���ڵ�������� depth = Level(root,p,1)
int Level(BTNode *t,BTNode *p,int d){
	if(t == nullptr) return 0;
	if(t == p) return d;
	int sub = Level(t->left,p,d+1);
	if(sub > 0) return sub;
	else return Level(t->right,p,d+1);
}
//�����������ҵ�ָ���ڵ㲢��¼����
bool Find_Print(BTNode *t,DataType x,DataType path[],int level,int count){
	if(t!=nullptr){
		path[level-1] = t->data;
		if(t->data == x) {count = level; return true;}
		if(Find_Print(t->left,x,path,level+1,count)) retuen true;
		return Find_Print(t->right,x,path,level+1,count);
	}
	return false;
}
//ɾ����������������в�����x�Ľڵ�
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
//������������߲��ж����Ƿ�ƽ��
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
//�ж��Ƿ�Ϊ��ȫ������
bool isCompleted(BT& T){
	BTNode* p = T;
	if(p == nullptr) return true;
	BTNode* Q[n], int front=0, int rear=0;
	bool flag = false;				//��־������δ�����ն���Ԫ��
	Q[rear] = p; rear = (rear+1)%n;
	while(front!=rear){
		p = Q[front]; front = (front+1)%n;
		if(p == nullptr) flag = true;	//�����������ն���
		else{
            if(flag) return false; //ǰ������ſն���Ԫ�أ�����ȫ������
            else {					//������Ů�Ƿ�Ϊ�ն����
                Q[rear] = p->left; rear = (rear+1)%n;
                Q[rear] = p->right; rear = (rear+1)%n;
            }
		}
	}
	return true;
}
//�ڽӱ��ʾ��
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
//�ڽӾ���
//ȡ����v���׸��ڽӶ���
int getFirstNeighbor(MGraph &G,int v){
	if(v!=-1){
		for(int col=0; col<G.numVertices; ++col){
			if(G.Edge[v][col]>0 && G.Edge[v][col]<maxWeight) return col;
		}
	}
	return -1;
}
//��������v��ĳ���ڽӶ���w����һ���ڽӶ���
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
//�ڽӱ�
//ȡ����v���׸��ڽӶ���
int getFirstNeighbor(ALGraph &G,int v){
	if(v!=-1){
		EdgeNode *p = G.VertexList[v].adj;
		if(p!=nullptr) return p->dest;
	}
	return -1;
}
//��������v��ĳ���ڽӶ���w����һ���ڽӶ���
int getNextNeighbor(MGraph &G,int v, int w){
	if(v!=-1){
		EdgeNode *p = G.VertexList[v].adj;
		while(p!=nullptr && p->dest!=w) p = p->link;
		if(p!=nullptr && p->link!=nullptr) return p->link->dest;
	}
	return -1;
}
//ͼ�ı���
void DFS(Graph &G,int v,bool visited[]){
	visit(getVal(G,v));					//���ʶ���v
	visited[v] = true;					//�����ѷ��ʹ��ı��
	int w = getFirstNeighbor(G,v);      //��v���׸��ڽӵ�w
	while(w!=-1){						//������
		if(!visited[w]) DFS(G,w,visited);//wδ�����ʹ���ݹ�
		w = getNextNeighbor(G,v,w);		//ȡv����w�����һ���ڽӶ���
	}
}
//�ǵݹ��㷨
void DFS(Graph &G,int v){
	int i,j,k; Weight w; Stack S;
	bool inStack[G.numVertices];
	for(int i=0; i<maxVertices; ++i) inStack[i] = false;
	Push(S,v); inStack[v] = true;
	while(!IsEmpty(S)){
		Pop(S,k);				    //ջ���˳�һ������
		cout << getVal(G,k) << " "; //����
		for(j=n-1; j>=0; --j){      //���k�������ڽӶ���
			w = getWeight(G,k,j);   //ȡk-j�ߵ�Ȩ��
			if(w>0 && w<maxWeight && !inStack[j]){
				Push(S,j); inStack[j] = true;//δ����ջ�Ķ����ջ
			}
		}
	}
}
void BFS(Graph &G,int v){
	int i,w,n = numOfVertices(G);		//ȡͼ�еĶ�����
	bool *visited = new bool[n]; 		//����ӡ��
	for(i=0; i<n; ++i) visited[i] = false;
	visit(getVal(G,v)); visited[v] = true;
	Queue Q; initQueue(Q); EnQueue(Q,v);//������ӣ�ʵ�ֲַ����
	while(Q.IsEmpty()){					//���ѭ�������Ƿ�Ϊ��
		DeQueue(Q,v); 					//ȡ������v
		w = getFirstNeighbor(G,v);		//��v���׸��ڽӵ�w
		while(w!=-1){
			if(!visited[w]){
				visit(getVal(G,v));
				visited[v] = true;
				EnQueue(Q,v); 			//����w���
			}
			w = getNextNeighbor(G,v,w);	//ȡv����w�����һ���ڽӶ���
		}
	}
	delete [] visited;					//�ͷ�ӡ��
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
