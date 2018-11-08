#pragma once
//稀疏矩阵
#define maxTerms 64
typedef int DataType;
typedef struct{
	int row, col;
	DataType val;
}Triple;

typedef struct{
	int rows, cols, terms;
	Triple elem[maxTerms];
}SparseMatrix;

//拉丁方阵
void LatinMat(int A[][n],int n){
	int i, j;
	for(i=0; i<n; ++i)
		for(j=0; j<n; ++j){
			if(i+j+1<n) A[i][j] = i+j+1;
			else A[i][j] = i+j+1-n;
		}
}
//蛇形矩阵
void SnakeMat(int A[][n], int n){
	int i, j, k, row, col, m=1;  //m为蛇形矩阵的值
	for(k=1; k<=2*n-1; ++k){
		if(k <= n) j = k;
		else j = 2*n-k;
		for(i=0; i<j; ++i){
			if(k<=n){
				if(k%2) {row = k-i-1; col = i;}
				else {row = i; col = k-i-1;}
			}
			else {row = k+i-n; col = k-row-1;}
			A[row][col] = m++;
		}
	}
}
//螺旋矩阵
void SpiralMat(int A[][n], int n){
	int i, j, flag = 0;
	for(i=0; i<n; ++i)
		for(j=0; j<n; ++j)
			A[i][j] = 0;
	i = j = 0;
	for(int m=1; m<n*n; ++m){
		A[i][j] = m;
		if(flag == 0){
			if(i+1<n && A[i+1][j] == 0) i++;
			else {j++; flag = 1;}
		}
		if(flag == 1){
			if(j+1<n && A[i][j+1] == 0) j++;
			else {i--; flag = 2;}
		}
		if(flag == 2){
			if(i>0 && A[i-1][j] == 0) i--;
			else {j--; flag = 3;}
		}
		if(flag == 3){
			if(j>0 && A[i][j-1] == 0) j--;
			else {i++; flag = 0;}
		}
	}
}
