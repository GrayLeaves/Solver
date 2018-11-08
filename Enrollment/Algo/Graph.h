#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <iostream>
#define maxVertices 64
#define maxWeight 999999 //infinity
using namespace std;

typedef char Type;
typedef float Weight;
//邻接矩阵表示法
typedef struct{
	int numVertices, numEdges;
	Type *VertexList;
	Weight **Edge;
}MGraph;
void InitialGraph(MGraph &G){
	G.VertexList = new Type[maxVertices];
	G.Edge = new Weight*[maxVertices];
	for(int i=0; i<maxVertices; ++i) G.Edge[i] = new Weight[maxVertices];
	for(int i=0; i<maxVertices; ++i){
		G.VertexList[i] = 0;
		for(int j=0; j<maxVertices; ++j){
			G.Edge[i][j] = (i==j)? 0:maxWeight;
		}
	}
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
		if(p!=NULL) return p->dest;
	}
	return -1;
}
//给出顶点v的某个邻接顶点w的下一个邻接顶点
int getNextNeighbor(MGraph &G,int v, int w){
	if(v!=-1){
		EdgeNode *p = G.VertexList[v].adj;
		while(p!=NULL && p->dest!=w) p = p->link;
		if(p!=NULL && p->link!=NULL) return p->link->dest;
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
	while(p!=NULL && p->dest!=w) p = p->link;
	if(p!=NULL) return p->cost;
	else return -1;
}




//邻接矩阵转换为邻接表
void MatToList(MGraph &G1,ALGraph &G2){
	G2.numVertices = G1.numVertices;
	G2.numEdges = G1.numEdges;
	G2.VertexList = new VertexNode[maxVertices];
	int i,j,k;
	EdgeNode *p, *rear;
	for(i=0; i<G1.numVertices; ++i){	//逐节点转换
		G2.VertexList[i].data = G1.VertexList[i];
		G2.VertexList[i].adj = NULL;
		for(j=0; j<G1.numVertices; ++j) if(G1.Edge[i][j]) break;//找到第一个邻接顶点
		if(j<G1.numVertices){
			p = new EdgeNode; p->dest = j;//插入新的边节点
			p->link = G2.VertexList[i].adj; G2.VertexList[i].adj = p;
			rear = p;
			for(k=j+1; k<G1.numVertices; ++k){//找到下一个邻接顶点
				if(G1.Edge[i][k]){
					p = EdgeNode; p->dest = k;
					p->link = rear->link; rear->link = p; rear = p;//链尾插入
				}
			}
		}
	}
}
void ListToMat(ALGraph &G1,MGraph &G2){
	G2.numVertices = G1.numVertices;
	G2.numEdges = G1.numEdges;
	int i,j; EdgeNode *p;
	G2.VertexList = new Type[maxVertices];
	G2.Edge = new Weight*[maxVertices]; //创建邻接矩阵
	for(i=0; i<maxVertices; ++i) G2.Edge[i] = new Weight[maxVertices];
	for(i=0; i<maxVertices; ++i)
		for(j=0; j<maxVertices; ++j)
			G2.Edge[i][j] = 0;
	for(i=0; i<maxVertices; ++i){ //逐个顶点复制
		G2.VertexList[i] = G1.VertexList[i].data;
		for(p=G1.VertexList[i].adj; p!=NULL; p=p->link)
			G2.Edge[i][p->dest] = 1;
	}
}
//有向图的邻接表G1转化为其逆邻接表
void ConverseList(ALGraph &G1,ALGraph &G2){
	G2.numVertices = G1.numVertices;
	G2.numEdges = G1.numEdges;
	int i; EdgeNode *p, *q;
	G2.VertexList = new VertexNode[maxVertices]; //创建逆邻接表顶点集
	for(i=0; i<G1.numVertices; ++i){	//逐节点转换
		G2.VertexList[i].data = G1.VertexList[i].data;
		G2.VertexList[i].adj = NULL;
	}
	for(i=0; i<maxVertices; ++i){ 					//逐个顶点转换
		for(p=G1.VertexList[i].adj; p!=NULL; p=p->link){
			q = new EdgeNode; q->dest = i;  		//创建入边表新节点
			q->link = G2.VertexList[p->dest].adj;	//插入入边表的前端
			G2.VertexList[p->dest].adj = q;
		}
}
//求有向图的邻接表各顶点的入度
void CalcInDegree(ALGraph &G,int inDegree[]){
	for(int i=0; i<maxVertices; ++i) inDegree[i] = 0;
	for(int i=0; i<maxVertices; ++i){
		w = getFirstNeighbor(G,i);
		while(w!=-1){
			inDegree[w] += 1;
			G.getNextNeighbor(G,i,w);
		}
	}
}
//求连通图中i到j的所有简单路径
void AllSimplePath(Graph &G,int i,int j){
	bool *visited = new bool[G.numVertices];
	int *path = new int[G.numOfVertices];
	for(int i=0; i<maxVertices; ++i) visited[i] = false;
	dfs(G,i,j,visited,path,G.numVertices,0);
	delete [] visited; delete [] path;
}
void dfs(Graph &G,int v,int j,int visited[],int path[],int n,int k){
	//用图的深度优先搜索在连通图中G找到从v到j的简单路径，数组visited记录
	//访问过的顶点，数组path记录路径上的顶点序列，k是path中当前可存放的位置
	visited[v] = true; path[k] = v;
	int w = getFirstNeighbor(G,v); //取顶点v的首个邻接顶点
	while(w!=-1){
		if(!visited[w])
			if(w==j){		//输出路径
				for(int i=0; i<=k; ++i) cout << path[i] << " ";
				cout << w << endl;
			}
			else dfs(G,w,j,visited,path,n,k+1);
		w = getNextNeighbor(G,v,w); //取v排在w后的下一个邻接顶点
	}
	visited[v] = false;				//重新标记再探寻
}
//判断无向图是否棵树，即判断是否存在回路或为n-1条边的连通图
bool IsTree(Graph &G){
	bool visited[G.numVertices];
	for(int i=0; i<maxVertices; ++i) visited[i] = false;
	int vv = 0, ee = 0;
	dfs(G,1,vv,ee,visited);
	delete [] visited;
	if(vv==G.numVertices && ee==2*(G.numVertices-1)) return true;
	else return false;
}
void dfs(Graph &G,int v,int& vv,int& ee,bool visited[]){
	//dfs统计访问过的顶点数和边数，由vv和ee返回
	visited[v] = true; vv++;					//做访问标记，顶点计数
	int w = getFirstNeighbor(G,v);				//取顶点v的首个邻接顶点
	while(w!=-1){
		ee++;									//边存在则计数
		if(!visited[w]) dfs(G,w,vv,ee,visited); //当该邻接顶点未被访问过
		w = getNextNeighbor(G,v,w); 			//取v排在w后的下一个邻接顶点
	}
}
//若顶点r到G中每个顶点皆可达则为根顶点，打印图中所有根顶点
void DFSTraverse(Graph &G){
	bool visited[G.numVertices];
	for(int i=0; i<maxVertices; ++i) visited[i] = false;
	int i, count;
	for(i=0; i<G.numVertices; ++i){
		count = 0;
		DFS(G,i,visited,count);
		if(count==n) cout << getVal(G,i) << endl;
	}
}
void DFS(Graph &G,int v,bool visited[],int &cnt){
	visited[v] = true; cnt++;
	for(w=getFirstNeighbor(G,v); w!=-1; w=getNextNeighbor(G,v,w)){
		if(!visited[w]) DFS(G,w,visited,cnt);
	}
}
//在连通图中找到经过v的简单回路
void CyclePath(Graph &G,int v,int d){
	bool visited[G.numVertices]; int S[maxVertices];
	for(int i=0; i<maxVertices; ++i) {visited[i] = false; S[i] = 0;}
	int d = 0;
	dfspath(G,v,v,visited,S,d);
}
void dfspath(Graph &G,int i,int j,bool visited[],int S[],int& d){
	visited[i] = true; S[++d] = i;
	if(i==j && d>=2){
		cout << "The path is ";
		for(int i=1; i<=d; ++i) cout << S[i] << " ";
		cout << endl;
	}
	int w = getFirstNeighbor(G,i);
	while(w!=-1){
		if(!visited[w] || w==j) dfspath(G,w,j,visited,S,d);
		w = getNextNeighbor(G,i,w);
	}
	visited[i] = false;
}
#endif
