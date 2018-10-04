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
	for (auto it = zip.begin(); it != zip.end(); ++it) {//显性
		len = it->size();
		if (len == 1) placeOnly.push_back(it - zip.begin()); //保存长度为1的位置
	}

	vector<int> tmp; //备份唯一性的结果，以便在行列、宫扫描后统一操作而不会互相影响
	struct {
		array<int, RANK + 1> count;			 //统计每个区域的解空间集出现频次
		array<vector<int>, RANK + 1> record; //各值及频次所对应的格点坐标
	}nums;

	pos_t pos; vector<int> Two; int cnt = 0;
	for (int r = 1; r <= RANK; ++r) {	  //所在列

		for (int i = 0; i <= RANK; ++i) {//初始化
			nums.count[i] = 0; nums.record[i].clear();
		}

		for (int c = 1; c <= RANK; ++c) { //所在行
			pos = (r - 1)*RANK + (c - 1);
			for (auto &e : zip[pos]) {
				++nums.count[e]; //统计对应值的频率
				nums.record[e].push_back(pos);//对应位置添加坐标
			}
		}//行遍历结束

		cnt = 0;
		for (int i = 1; i <= RANK; ++i) {
			if (nums.count[i] == 1) {//添加唯一候选者,规则一
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
					cout << "保存数偶：" << Two[0] << "\t" << Two[1] << endl;
#endif
					pair.push_back(Two[0]); pair.push_back(Two[1]);
				}
			}
		}
		Two.clear(); //清除本次记录
	} //列遍历结束

	for (int r = 0; r < 3; r++) {//遍历9个宫即可
		for (int c = 0; c < 3; c++) {//宫内遍历

			for (int i = 0; i <= RANK; ++i) {//遍历前初始化
				nums.count[i] = 0; nums.record[i].clear();
			}

			for (int i = 3 * r + 1; i <= 3 * (r + 1); ++i) {
				for (int j = 3 * c + 1; j <= 3 * (c + 1); ++j) {
					pos = (i - 1)*RANK + (j - 1);
					for (auto &e : zip[pos]) {
						++nums.count[e];//统计对应值的频率
						nums.record[e].push_back(pos);//对应位置添加坐标
					}
				}
			}
			cnt = 0;
			for (int i = 1; i <= RANK; ++i) {
				if (nums.count[i] == 1) { //添加唯一候选者
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
						cout << "保存数偶：" << Two[0] << "\t" << Two[1] << endl;
#endif
						pair.push_back(Two[0]); pair.push_back(Two[1]);
					}
				}
			}
			Two.clear();
		}//宫内遍历结束
	}

	//对于隐性唯一的候选数要进行清理工作
	for (int& k : tmp) { //k = pos*10+i
		zip[k / 10].clear(); //清除所有候选数
		zip[k / 10].push_back(k % 10);//将来会计算长度是否为1
	}//更新数偶解空间
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
			old_pair = pair; //用于还原
			if (assign(r, c, dat) && updateSpace(r, c)) {
				isInTest = true; ++step; //记下当前这一步
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
	gridSpaceAll();			//更新所有空间
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
	if (!isInit) return false; //还没初始化完成
	vector<int> place;
	searchUniq(place);	   //调用函数来生成
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
		gridSpaceAll();            //构造解空间
		if (!placeOneNum()) {  //没有唯一候选数,则调用试探
#ifdef DEBUG
			cout << "规则一不顶用，启用探测法求解,当前数偶有" << endl;
			for (auto &v : pair) cout << v << " ";
			cout << endl;
#endif
			if (flag) {
				if (!placeOnePair()) {//更新了解空间
#ifdef DEBUG
					cout << "没有数偶，无法探测。打印堆栈：" << endl;
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
				backUpdate();	//还原之前的状态
#ifdef DEBUG
				cout << "探测失败,先不支持双重探测，当前数偶有" << endl;
				for (auto &v : pair) cout << v << " ";
				cout << endl;
				cout << "先返回到探测前的状态。" << endl;
#endif
				if (!placeNextPair()) {//更换出错
#ifdef DEBUG
					cout << "更换出错。打印数独：" << endl;
#endif
					print();
					break;
				}
			}
		}//if (!placeOneNum())
	} //end of while
	if (isCompleted()) {
		cout << "求解成功！" << endl;
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
	//判断每行每列每宫是否都有1-9
	for (pos_t r = 1; r <= RANK; ++r)
		for (pos_t c = 1; c <= RANK; ++c)
			if (0 == at(r, c)) return false;
	return true;
}

bool Cell::isCompleted() {
	bool hash[RANK + 1];
	data_t dat;
	for (int r = 1; r <= RANK; ++r) {	  //所在列
		for (int i = 0; i <= RANK; ++i) hash[i] = false; //初始化
		for (int c = 1; c <= RANK; ++c) { //所在行
			dat = at(r, c);
			if (0 == dat || hash[dat]) return false;
			else hash[dat] = true; //将填过的数据改为true
		}
	}
	for (int r = 0; r < 3; r++) {//遍历9个宫即可
		for (int c = 0; c < 3; c++) {
			for (int i = 0; i <= RANK; ++i) hash[i] = false; //遍历前初始化
			for (int i = 3 * r + 1; i <= 3 * (r + 1); ++i) {
				for (int j = 3 * c + 1; j <= 3 * (c + 1); ++j) {
					dat = at(i, j);
					if (0 == dat || hash[dat]) return false;
					else hash[dat] = true; //将填过的数据改为true
				}
			}
		}
	}
	return true;
}

vector<int> Cell::gridSpace(pos_t row, pos_t col) {
	bool hash[RANK + 1] = { false }; //将填过的数据改为true
	vector<int> res;
	data_t dat = at(row, col);
	if (0 == dat) { //针对未填写的地方而言
		for (int r = 1; r <= RANK; ++r) { //所在列
			dat = at(r, col);
			if (0 != dat && !hash[dat]) hash[dat] = true;
		}
		for (int c = 1; c <= RANK; ++c) { //所在行
			dat = at(row, c);
			if (0 != dat && !hash[dat]) hash[dat] = true;
		}
		int r = (row - 1) / 3, c = (col - 1) / 3; //所在宫
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
	cout << "Ok,解空间已全部更新完毕。" << endl;
#endif
}

void Cell::gridSpaceAll(pos_t row, pos_t col) {
	for (int r = 1; r <= RANK; ++r) { //所在列
		if (0 == at(r, col)) {        //未填值
			zip[(r - 1)*RANK + (col - 1)] = gridSpace(r, col);
		}
	}
	for (int c = 1; c <= RANK; ++c) { //所在行
		if (0 == at(row, c)) {		  //未填值
			zip[(row - 1)*RANK + (c - 1)] = gridSpace(row, c);
		}
	}
	int r = (row - 1) / 3, c = (col - 1) / 3; //所在宫
	for (int i = 3 * r + 1; i <= 3 * (r + 1); ++i) {
		for (int j = 3 * c + 1; j <= 3 * (c + 1); ++j) {
			if (i != row && j != col && 0 == at(i, j)) { //不是原来的行和列且未填值
				zip[(i - 1)*RANK + (j - 1)] = gridSpace(i, j);
			}
		}
	}
}

void Cell::delVal(pos_t row, pos_t col, data_t dat) {
	vector<int> &vec = zip[(row - 1)*RANK + (col - 1)]; //不能操作副本，而要本体
	if (vec.empty()) return;
#ifdef DEBUG
	cout << "删除[" << dat << "]前，(" << row << "," << col << ")处能填的数为：";
	for (auto &v : vec) cout << v << " ";
	cout << endl;
#endif
	auto end = vec.end();
	auto it = find(vec.begin(), end, dat);
	if (it != end) vec.erase(it);
}

bool Cell::updateSpace(pos_t row, pos_t col) {
	data_t dat = at(row, col); //读出数据，以便清除
#ifdef OUTPUT
	cout << "对(" << row << "," << col << ")所在行、列、宫删除[" << dat << "]。" << endl;
#endif
	if (0 == dat) {
#ifdef OUTPUT
		cout << "出错了，(" << row << "," << col << ")处没填值。" << endl;
#endif
		return false;
	}
	for (int r = 1; r <= RANK; ++r) { //所在列
		if (0 == at(r, col)) { //未填值
			delVal(r, col, dat);
		}
	}
	for (int c = 1; c <= RANK; ++c) { //所在行
		if (0 == at(row, c)) {//未填值
			delVal(row, c, dat);
		}
	}
	int r = (row - 1) / 3, c = (col - 1) / 3; //所在宫
	for (int i = 3 * r + 1; i <= 3 * (r + 1); ++i) {
		for (int j = 3 * c + 1; j <= 3 * (c + 1); ++j) {
			if (i != row && j != col && 0 == at(i, j)) { //不是原来的行和列且未填值
				delVal(i, j, dat);
			}
		}
	}
#ifdef OUTPUT
	cout << "Ok,全部删除" << endl;
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
			cout << "(" << row << "," << col << ")已填值。" << endl;
#endif
			return true;
		};
		bucket.push(pos * 10 + dat);
		if (isInTest) ++step;	//处于试探状态
#ifdef DEBUG
		cout << "[" << dat << "]成功放置在(" << row << "," << col << ")。" << endl;
#endif
		return true;
	}
	else {
#ifdef DEBUG
		cout << "[" << dat << "]不能放置在(" << row << "," << col << ")。" << endl;
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
			if (c != col) { //排除源点
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
			if (r != row) { //排除源点
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
	int r = (row - 1) / 3, c = (col - 1) / 3; //判断是在哪一宫
	for (int i = 3 * r + 1; i <= 3 * (r + 1); ++i) {
		for (int j = 3 * c + 1; j <= 3 * (c + 1); ++j) {
			if (dat == at(i, j)) {
				if (i != row && j != col) { //排除源点
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
	cout << "当前9X9的数独内容为：" << endl;
	for (int r = 1; r <= RANK; ++r) {
		for (int c = 1; c <= RANK; ++c) {
			dat = at(r, c);
			if (0 == dat) cout << "_" << "  ";
			else cout << dat << "  ";
		}
		cout << endl;
	}
}