
//�������˵����
//����һ��ʹ����/����Ψһ��ѡ�������Ը�����
//   ���ԣ�������˿ո���е����ض��Ǻ�ѡ��֮һ��������һ����ѡ������ô������������ʵ�֣�
//   ���ԣ���ĳ�С��С����е�ĳ���ո���ĳ����ѡ����������ڵ������ո��Ĳ�ͬ����ôҲ�͸�������(��ʵ��)
//�������ʹ������N����ɾ�������Ը��м�,Ϊ�˼�㣬����ȡN=3��
//   ��N�����ֽ���ĳ�С��С�����N���ո��ĺ�ѡ������ʱ��
//   ��ô���������ո�����������N������������ѡ��ȫ��ɾ�����ٵ��ù���һ
//��������ʹ����������żɾ����
//	 ��ĳ�С��С����еĺ�ѡ�����е��������������ڸ�����������ո���У�
//   ������ֻ��������������λ�ã���ɽ�����������������ѡ��ɾ�����ٵ��ù���һ

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
	bool assign(pos_t row, pos_t col,data_t dat); //���ﱣ֤����ȷ��д
	inline data_t at(pos_t row, pos_t col)const; //��������λ�õ���ֵ
	bool readMat(void);	//��ȡ��������
	bool init(const Mat& mat);
	void print()const;
	bool solve();
	bool isCompleted();
private:
	Mat cell;				//�����Ĵ洢����
	Zip zip;				//�������и���Ͻ�ռ�
	stack<data_t> bucket;	//����ÿһ���������������֣�pos*10 + dat
	int step = 0;			//��̽�Ĳ���
	vector<data_t> old_pair;//
	vector<data_t> pair;    //������ż
	vector<data_t> next_pair;//δ����̽����ż
	bool isInit = false;	//�Ƿ��ѳ�ʼ�����
	bool isInTest = false;  //�Ƿ�����̽����

	void searchUniq(vector<int>& placeOnly);
	void updatePairSpace();
	bool placeUpdate(pos_t pos); //���ò�����
	bool placeOneNum();			 //��дһ������
	bool placeOnePair();		 //��̽�ŷ�����ż��50%�ĳɹ���
	void backUpdate();			 //��̽ʧ�ܣ����˵���̽ǰ������
	bool placeNextPair();		 //��̽ʧ���򽻻�λ��
	inline bool inRange(data_t dat);  //�Ƿ���0-9��

	bool isAllFull();		//�Ƿ�ȫ�����
	bool isOkRow(pos_t row, pos_t col, data_t dat); //������dat,��row���Ƿ���ԣ�Ψһ�Լ��
	bool isOkCol(pos_t row, pos_t col, data_t dat); //������dat,��col���Ƿ���ԣ�Ψһ�Լ��
	bool isOkGrid(pos_t row, pos_t col, data_t dat);//������dat,��(row,col)���ڹ��Ƿ���ԣ�Ψһ�Լ��
	bool check(pos_t row, pos_t col, data_t dat);   //�Ƿ������������дҪ��Ψһ�Լ��
	bool checkAll();	//��ÿ����������check

	vector<int> gridSpace(pos_t row, pos_t col);    //��assign��ǰ���·��ظ�λ�ÿ����ֵ�ռ�
	void delVal(pos_t row, pos_t col, data_t dat);	//ɾ����ռ��ֵ
	bool updateSpace(pos_t row, pos_t col);			//������(row,col)�������й��Ľ�ռ�
	void gridSpaceAll();							//����ȫ���ռ��Zip
	void gridSpaceAll(pos_t row, pos_t col);		//����(row,col)�����С��С������½�ռ�
};