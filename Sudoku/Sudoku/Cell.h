
//数独求解说明：
//规则一：使用显/隐性唯一候选数法，对付初级
//   显性：能填入此空格点中的数必定是候选数之一，若仅有一个候选数，那么就填它。（已实现）
//   隐性：在某行、列、宫中的某个空格点的某个候选数与该区域内的其他空格点的不同，那么也就该填它。(已实现)
//规则二：使用隐性N链数删减法，对付中级,为了简便，这里取N=3。
//   当N个数字仅在某行、列、宫的N个空格点的候选数出现时，
//   那么，这三个空格点仅能填入这N个数，其他候选数全部删除，再调用规则一
//规则三：使用隐性数对偶删减法
//	 在某行、列、宫中的候选数集中的两个数仅出现在该区域的两个空格点中，
//   这两数只能限制在这两个位置，则可将这两个格点的其他候选数删除，再调用规则一

#pragma once
#include <iostream>
#include <array>
#include <stack>
#include <vector>
#include <algorithm>
#include <iomanip>
using namespace std;

//#define DEBUG
//#define OUTPUT

const int RANK = 9;
const int LEN = RANK*RANK;
typedef int pos_t;
typedef int data_t;
typedef array<int, LEN> Mat;
typedef array<vector<int>, LEN> Zip;

class Cell {
public:
	Cell() {};
	~Cell() {};
	Cell(const Cell& mat);
	Cell& operator=(const Cell& mat);
	bool assign(pos_t row, pos_t col,data_t dat); //这里保证了正确填写
	inline data_t at(pos_t row, pos_t col)const; //返回所在位置的数值
	bool readMat(void);	//读取数独矩阵
	bool init(const Mat& mat);
	void print()const;
	bool solve();
	bool isCompleted();
private:
	Mat cell;				//数独的存储矩阵
	Zip zip;				//数独所有格点上解空间
	stack<data_t> bucket;	//保存每一步填入的坐标和数字，pos*10 + dat
	int step = 0;			//试探的步数
	vector<data_t> old_pair;//
	vector<data_t> pair;    //保存数偶
	vector<data_t> next_pair;//未被试探的数偶
	bool isInit = false;	//是否已初始化完成
	bool isInTest = false;  //是否处于试探解中

	void searchUniq(vector<int>& placeOnly);
	void updatePairSpace();
	bool placeUpdate(pos_t pos); //放置并更新
	bool placeOneNum();			 //填写一个数字
	bool placeOnePair();		 //试探着放置数偶，50%的成功率
	void backUpdate();			 //试探失败，回退到试探前，换回
	bool placeNextPair();		 //试探失败则交换位置
	inline bool inRange(data_t dat);  //是否在0-9内

	bool isAllFull();		//是否全部填好
	bool isOkRow(pos_t row, pos_t col, data_t dat); //若填入dat,看row行是否填对，唯一性检查
	bool isOkCol(pos_t row, pos_t col, data_t dat); //若填入dat,看col列是否填对，唯一性检查
	bool isOkGrid(pos_t row, pos_t col, data_t dat);//若填入dat,看(row,col)所在宫是否填对，唯一性检查
	bool check(pos_t row, pos_t col, data_t dat);   //是否符合数独的填写要求，唯一性检查
	bool checkAll();	//对每个数都调用check

	vector<int> gridSpace(pos_t row, pos_t col);    //在assign的前提下返回该位置可填的值空间
	void delVal(pos_t row, pos_t col, data_t dat);	//删除解空间的值
	bool updateSpace(pos_t row, pos_t col);			//更新在(row,col)所在行列宫的解空间
	void gridSpaceAll();							//生成全部空间的Zip
	void gridSpaceAll(pos_t row, pos_t col);		//仅对(row,col)所在行、列、宫更新解空间
};