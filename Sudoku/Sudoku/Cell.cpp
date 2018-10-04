#include "Cell.h"

Cell::Cell(const Cell& mat) {
	for (pos_t r = 1; r <= RANK; ++r)
		for (pos_t c = 1; c <= RANK; ++c)
			cell[(r - 1)*RANK + (c - 1)] = mat.at(r, c);
	isInit = true;
}

Cell& Cell::operator=(const Cell& mat) {
	for (pos_t r = 1; r <= RANK; ++r)
		for (pos_t c = 1; c <= RANK; ++c)
			cell[(r - 1)*RANK + (c - 1)] = mat.at(r, c);
	isInit = true;
	return *this;
}

bool Cell::readMat(void) {
	data_t dat;
	for (int i = 0; i < LEN; ++i) {
		cin >> dat;
		if (inRange(dat)) cell[i] = dat;
		else {
			for (int i = 0; i < LEN; ++i) cell[i] = 0;
			return false;
		};
	}
	bool flag = checkAll();
	if (flag) isInit = true;
	return flag;
}

bool Cell::init(const Mat& mat) {
	for (int i = 0; i < LEN; ++i) {
		if (inRange(mat[i])) cell[i] = mat[i];
		else {
#ifdef OUTPUT
			cout << "Init(" << i << "): out of range." << endl;
#endif
			for (i = 0; i < LEN; ++i) cell[i] = 0;
			return false;
		};
	};
	bool flag = checkAll();
	if (flag) isInit = true;
	return flag;
}

bool Cell::placeUpdate(pos_t pos) {
	vector<int> &v = zip[pos];
	if (v.size() == 1) {
		data_t dat = v.back();
		pos_t r = pos / RANK + 1, c = pos % RANK + 1;
		return (assign(r, c, dat) && updateSpace(r, c));
	}
	else return false;
}

void Cell::searchUniq(vector<int>& placeOnly) {
	int index = 0, len = 0;
	for (auto it = zip.begin(); it != zip.end(); ++it) {//����
		len = it->size();
		if (len == 1) placeOnly.push_back(it - zip.begin()); //���泤��Ϊ1��λ��
	}

	vector<int> tmp; //����Ψһ�ԵĽ�����Ա������С���ɨ���ͳһ���������ụ��Ӱ��
	struct {
		array<int, RANK + 1> count;			 //ͳ��ÿ������Ľ�ռ伯����Ƶ��
		array<vector<int>, RANK + 1> record; //��ֵ��Ƶ������Ӧ�ĸ������
	}nums;

	pos_t pos; vector<int> Two; int cnt = 0;
	for (int r = 1; r <= RANK; ++r) {	  //������

		for (int i = 0; i <= RANK; ++i) {//��ʼ��
			nums.count[i] = 0; nums.record[i].clear();
		}

		for (int c = 1; c <= RANK; ++c) { //������
			pos = (r - 1)*RANK + (c - 1);
			for (auto &e : zip[pos]) {
				++nums.count[e]; //ͳ�ƶ�Ӧֵ��Ƶ��
				nums.record[e].push_back(pos);//��Ӧλ���������
			}
		}//�б�������

		cnt = 0;
		for (int i = 1; i <= RANK; ++i) {
			if (nums.count[i] == 1) {//���Ψһ��ѡ��,����һ
				pos = nums.record[i].back();
				tmp.push_back(pos * 10 + i); //pos*10 + i
				placeOnly.push_back(pos);
			}
			if (nums.count[i] == 2) {
				++cnt; 
				for (auto &x : nums.record[i]) //pos*10+i
					Two.push_back(x * 10 + i);
			}
		}

		if (cnt == 2) {
			if (Two[0] / 10 == Two[2] / 10 && Two[1] / 10 == Two[3] / 10) { //pos*10+i
				auto end = pair.end();
				auto res = find(pair.begin(), end, Two[0]);
				if (res == end) {
#ifdef DEBUG
					cout << "������ż��" << Two[0] << "\t" << Two[1] << endl;
#endif
					pair.push_back(Two[0]); pair.push_back(Two[1]);
				}
			}
		}
		Two.clear(); //������μ�¼
	} //�б�������

	for (int r = 0; r < 3; r++) {//����9��������
		for (int c = 0; c < 3; c++) {//���ڱ���

			for (int i = 0; i <= RANK; ++i) {//����ǰ��ʼ��
				nums.count[i] = 0; nums.record[i].clear();
			}

			for (int i = 3 * r + 1; i <= 3 * (r + 1); ++i) {
				for (int j = 3 * c + 1; j <= 3 * (c + 1); ++j) {
					pos = (i - 1)*RANK + (j - 1);
					for (auto &e : zip[pos]) {
						++nums.count[e];//ͳ�ƶ�Ӧֵ��Ƶ��
						nums.record[e].push_back(pos);//��Ӧλ���������
					}
				}
			}
			cnt = 0;
			for (int i = 1; i <= RANK; ++i) {
				if (nums.count[i] == 1) { //���Ψһ��ѡ��
					pos = nums.record[i].back();
					tmp.push_back(pos * 10 + i); //pos*10 + i
					placeOnly.push_back(pos);
				}
				if (nums.count[i] == 2) {
					++cnt; 
					for(auto &x : nums.record[i])
						Two.push_back(x * 10 + i);
				}
			}

			if (cnt == 2) {
				if (Two[0] / 10 == Two[2] / 10 && Two[1] / 10 == Two[3] / 10) {
					auto end = pair.end();
					auto tail = find(pair.begin(), end, Two[0]);
					if (tail == end) {
#ifdef DEBUG
						cout << "������ż��" << Two[0] << "\t" << Two[1] << endl;
#endif
						pair.push_back(Two[0]); pair.push_back(Two[1]);
					}
				}
			}
			Two.clear();
		}//���ڱ�������
	}

	//��������Ψһ�ĺ�ѡ��Ҫ����������
	for (int& k : tmp) { //k = pos*10+i
		zip[k / 10].clear(); //������к�ѡ��
		zip[k / 10].push_back(k % 10);//��������㳤���Ƿ�Ϊ1
	}//������ż��ռ�
	updatePairSpace();
}

void Cell::updatePairSpace() {
	for (auto it = pair.begin(); it != pair.end(); it += 2) {
		vector<int> &v0 = zip[*it / 10], &v1 = zip[*(it + 1) / 10];
		if (v0.size() > 2) {
			v0.clear(); v0.push_back(*it % 10); v0.push_back(*(it + 1) % 10);
		}
		if (v1.size() > 2) {
			v1.clear(); v1.push_back(*it % 10); v1.push_back(*(it + 1) % 10);
		}
	}
}

bool Cell::placeOnePair() {
	if (!pair.empty()) {
		int index = pair.back(); pair.pop_back();
		pos_t pos = index / 10; data_t dat = index % 10;
		pos_t r = pos / RANK + 1, c = pos % RANK + 1;
		if (!pair.empty()) {
			next_pair.push_back(pair.back()); pair.pop_back();
			old_pair = pair; //���ڻ�ԭ
			if (assign(r, c, dat) && updateSpace(r, c)) {
				isInTest = true; ++step; //���µ�ǰ��һ��
				return true;
			}
		}
	}
	return false;
}

void Cell::backUpdate() {
	int index;
	while (step > 0) {
		index = bucket.top();
		cell[index / 10] = 0;
		--step;
	}
	isInTest = false;
	pair = old_pair;
	gridSpaceAll();			//�������пռ�
}

bool Cell::placeNextPair() {
	if (!next_pair.empty()) {
		int index = next_pair.back(); next_pair.pop_back();
		pos_t pos = index / 10; data_t dat = index % 10;
		pos_t r = pos / RANK + 1, c = pos % RANK + 1;
		if (assign(r, c, dat) && updateSpace(r, c)) {
			return true;
		}
	}
	else return false;
}
bool Cell::placeOneNum() {
	if (!isInit) return false; //��û��ʼ�����
	vector<int> place;
	searchUniq(place);	   //���ú���������
	if (place.empty()) return false;
	while (!place.empty()) {
		auto p = place.back();
		place.pop_back();
		if (!placeUpdate(p)) {;
			return false;
		}
	}
	return true;
}

bool Cell::solve() {
	bool flag = true;
	while (!isAllFull()) {
		gridSpaceAll();            //�����ռ�
		if (!placeOneNum()) {  //û��Ψһ��ѡ��,�������̽
#ifdef DEBUG
			cout << "����һ�����ã�����̽�ⷨ���,��ǰ��ż��" << endl;
			for (auto &v : pair) cout << v << " ";
			cout << endl;
#endif
			if (flag) {
				if (!placeOnePair()) {//�����˽�ռ�
#ifdef DEBUG
					cout << "û����ż���޷�̽�⡣��ӡ��ջ��" << endl;
					if (!bucket.empty()) {
						cout << bucket.top() << " ";
						bucket.pop();
					}
					cout << endl;
#endif
					flag = false;
					continue;
				}
			}
			else {
				flag = true;
				backUpdate();	//��ԭ֮ǰ��״̬
#ifdef DEBUG
				cout << "̽��ʧ��,�Ȳ�֧��˫��̽�⣬��ǰ��ż��" << endl;
				for (auto &v : pair) cout << v << " ";
				cout << endl;
				cout << "�ȷ��ص�̽��ǰ��״̬��" << endl;
#endif
				if (!placeNextPair()) {//��������
#ifdef DEBUG
					cout << "����������ӡ������" << endl;
#endif
					print();
					break;
				}
			}
		}//if (!placeOneNum())
	} //end of while
	if (isCompleted()) {
		cout << "���ɹ���" << endl;
		print();
		return true;
	}
	else { 
#ifdef DEBUG
		while (!bucket.empty()) {
			cout << bucket.top() << " ";
			bucket.pop();
		}
		cout << endl;
#endif
		print();
		return false;
	}
}

bool Cell::isAllFull() {
	//�ж�ÿ��ÿ��ÿ���Ƿ���1-9
	for (pos_t r = 1; r <= RANK; ++r)
		for (pos_t c = 1; c <= RANK; ++c)
			if (0 == at(r, c)) return false;
	return true;
}

bool Cell::isCompleted() {
	bool hash[RANK + 1];
	data_t dat;
	for (int r = 1; r <= RANK; ++r) {	  //������
		for (int i = 0; i <= RANK; ++i) hash[i] = false; //��ʼ��
		for (int c = 1; c <= RANK; ++c) { //������
			dat = at(r, c);
			if (0 == dat || hash[dat]) return false;
			else hash[dat] = true; //����������ݸ�Ϊtrue
		}
	}
	for (int r = 0; r < 3; r++) {//����9��������
		for (int c = 0; c < 3; c++) {
			for (int i = 0; i <= RANK; ++i) hash[i] = false; //����ǰ��ʼ��
			for (int i = 3 * r + 1; i <= 3 * (r + 1); ++i) {
				for (int j = 3 * c + 1; j <= 3 * (c + 1); ++j) {
					dat = at(i, j);
					if (0 == dat || hash[dat]) return false;
					else hash[dat] = true; //����������ݸ�Ϊtrue
				}
			}
		}
	}
	return true;
}

vector<int> Cell::gridSpace(pos_t row, pos_t col) {
	bool hash[RANK + 1] = { false }; //����������ݸ�Ϊtrue
	vector<int> res;
	data_t dat = at(row, col);
	if (0 == dat) { //���δ��д�ĵط�����
		for (int r = 1; r <= RANK; ++r) { //������
			dat = at(r, col);
			if (0 != dat && !hash[dat]) hash[dat] = true;
		}
		for (int c = 1; c <= RANK; ++c) { //������
			dat = at(row, c);
			if (0 != dat && !hash[dat]) hash[dat] = true;
		}
		int r = (row - 1) / 3, c = (col - 1) / 3; //���ڹ�
		for (int i = 3 * r + 1; i <= 3 * (r + 1); ++i) {
			for (int j = 3 * c + 1; j <= 3 * (c + 1); ++j) {
				if (i != row && j != col) dat = at(i, j);
				else continue;
				if (0 != dat && !hash[dat]) hash[dat] = true;
			}
		}
		for (int i = 1; i <= RANK; ++i)
			if (!hash[i]) res.push_back(i);
	}
	return res;
}

void Cell::gridSpaceAll() {
	for (pos_t r = 1; r <= RANK; ++r)
		for (pos_t c = 1; c <= RANK; ++c)
			zip[(r - 1)*RANK + (c - 1)] = gridSpace(r, c);
#ifdef OUTPUT
	cout << "Ok,��ռ���ȫ��������ϡ�" << endl;
#endif
}

void Cell::gridSpaceAll(pos_t row, pos_t col) {
	for (int r = 1; r <= RANK; ++r) { //������
		if (0 == at(r, col)) {        //δ��ֵ
			zip[(r - 1)*RANK + (col - 1)] = gridSpace(r, col);
		}
	}
	for (int c = 1; c <= RANK; ++c) { //������
		if (0 == at(row, c)) {		  //δ��ֵ
			zip[(row - 1)*RANK + (c - 1)] = gridSpace(row, c);
		}
	}
	int r = (row - 1) / 3, c = (col - 1) / 3; //���ڹ�
	for (int i = 3 * r + 1; i <= 3 * (r + 1); ++i) {
		for (int j = 3 * c + 1; j <= 3 * (c + 1); ++j) {
			if (i != row && j != col && 0 == at(i, j)) { //����ԭ�����к�����δ��ֵ
				zip[(i - 1)*RANK + (j - 1)] = gridSpace(i, j);
			}
		}
	}
}

void Cell::delVal(pos_t row, pos_t col, data_t dat) {
	vector<int> &vec = zip[(row - 1)*RANK + (col - 1)]; //���ܲ�����������Ҫ����
	if (vec.empty()) return;
#ifdef DEBUG
	cout << "ɾ��[" << dat << "]ǰ��(" << row << "," << col << ")���������Ϊ��";
	for (auto &v : vec) cout << v << " ";
	cout << endl;
#endif
	auto end = vec.end();
	auto it = find(vec.begin(), end, dat);
	if (it != end) vec.erase(it);
}

bool Cell::updateSpace(pos_t row, pos_t col) {
	data_t dat = at(row, col); //�������ݣ��Ա����
#ifdef OUTPUT
	cout << "��(" << row << "," << col << ")�����С��С���ɾ��[" << dat << "]��" << endl;
#endif
	if (0 == dat) {
#ifdef OUTPUT
		cout << "�����ˣ�(" << row << "," << col << ")��û��ֵ��" << endl;
#endif
		return false;
	}
	for (int r = 1; r <= RANK; ++r) { //������
		if (0 == at(r, col)) { //δ��ֵ
			delVal(r, col, dat);
		}
	}
	for (int c = 1; c <= RANK; ++c) { //������
		if (0 == at(row, c)) {//δ��ֵ
			delVal(row, c, dat);
		}
	}
	int r = (row - 1) / 3, c = (col - 1) / 3; //���ڹ�
	for (int i = 3 * r + 1; i <= 3 * (r + 1); ++i) {
		for (int j = 3 * c + 1; j <= 3 * (c + 1); ++j) {
			if (i != row && j != col && 0 == at(i, j)) { //����ԭ�����к�����δ��ֵ
				delVal(i, j, dat);
			}
		}
	}
#ifdef OUTPUT
	cout << "Ok,ȫ��ɾ��" << endl;
#endif
	return true;
}

bool Cell::assign(pos_t row, pos_t col, data_t dat) {
	pos_t pos;
	if (inRange(dat) && check(row, col, dat)) {
		pos = RANK*(row - 1) + (col - 1);
		if(cell[pos] == 0) cell[pos] = dat;
		else {
#ifdef DEBUG
			cout << "(" << row << "," << col << ")����ֵ��" << endl;
#endif
			return true;
		};
		bucket.push(pos * 10 + dat);
		if (isInTest) ++step;	//������̽״̬
#ifdef DEBUG
		cout << "[" << dat << "]�ɹ�������(" << row << "," << col << ")��" << endl;
#endif
		return true;
	}
	else {
#ifdef DEBUG
		cout << "[" << dat << "]���ܷ�����(" << row << "," << col << ")��" << endl;
#endif
		return false;
	}
};

data_t Cell::at(pos_t row, pos_t col)const {
	return cell[RANK*(row - 1) + (col - 1)];
};

bool Cell::inRange(data_t dat) {
	return (dat >= 0 && dat <= RANK);
}

bool Cell::check(pos_t row, pos_t col, data_t dat) {
	return (isOkRow(row, col, dat) && isOkCol(row, col, dat) && isOkGrid(row, col, dat));
}

bool Cell::checkAll() {
	data_t dat;
	for (int r = 1; r <= RANK; ++r)
		for (int c = 1; c <= RANK; ++c) {
			dat = at(r, c);
			if (dat != 0) {
				if (!check(r, c, dat)) return false;
			}
		}
	return true;
}

bool Cell::isOkRow(pos_t row, pos_t col, data_t dat) {
	for (int c = 1; c <= RANK; ++c) {
		if (dat == at(row, c)) {
			if (c != col) { //�ų�Դ��
#ifdef DEBUG
				cout << "data = [ " << dat << " ] isOkRow( " << row << "," << col << " ): fail;" << endl;
#endif
				return false;
			}
		}
	}
	return true;
}

bool Cell::isOkCol(pos_t row, pos_t col, data_t dat) {
	for (int r = 1; r <= RANK; ++r) {
		if (dat == at(r, col)) {
			if (r != row) { //�ų�Դ��
#ifdef DEBUG
				cout << "data = [ " << dat << " ],isOkCol( " << row << "," << col << " ): fail;" << endl;
#endif
				return false;
			}
		}
	}
	return true;
}

bool Cell::isOkGrid(pos_t row, pos_t col, data_t dat) {
	int r = (row - 1) / 3, c = (col - 1) / 3; //�ж�������һ��
	for (int i = 3 * r + 1; i <= 3 * (r + 1); ++i) {
		for (int j = 3 * c + 1; j <= 3 * (c + 1); ++j) {
			if (dat == at(i, j)) {
				if (i != row && j != col) { //�ų�Դ��
#ifdef DEBUG
					cout << "isOkGrid( " << row << "," << col << " ): fail;" << endl;
#endif
					return false;
				}
			}
		}
	}
	return true;
}

void Cell::print()const {
	data_t dat;
	cout << "��ǰ9X9����������Ϊ��" << endl;
	for (int r = 1; r <= RANK; ++r) {
		for (int c = 1; c <= RANK; ++c) {
			dat = at(r, c);
			if (0 == dat) cout << "_" << "  ";
			else cout << dat << "  ";
		}
		cout << endl;
	}
}