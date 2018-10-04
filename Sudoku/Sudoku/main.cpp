#include <iostream>
#include <ctime>
#include "Cell.h"
using namespace std;

int main() {

	Mat sudoku[] = {
		{0,0,5,3,0,0,0,0,0, 8,0,0,0,0,0,0,2,0, 0,7,0,0,1,0,5,0,0,
		 4,0,0,0,0,5,3,0,0, 0,1,0,0,7,0,0,0,6, 0,0,3,2,0,0,0,8,0,
		 0,6,0,5,0,0,0,0,9, 0,0,4,0,0,0,0,3,0, 0,0,0,0,0,9,7,0,0},//fail
		{4,0,6,0,0,0,1,0,5, 2,0,0,0,0,8,7,4,9, 0,0,7,4,0,1,0,0,8,
		 1,3,2,8,6,9,5,7,4, 8,0,4,0,0,0,9,0,3, 5,7,9,3,1,4,6,8,2,
		 7,0,0,5,0,3,8,0,0, 3,9,5,7,0,0,0,0,1, 6,0,8,0,0,0,3,0,7},//ok
		{0,0,0,0,0,0,5,0,0, 4,0,0,0,1,8,0,9,0, 0,1,8,0,0,2,0,0,3,
		 0,0,9,2,0,6,3,5,0, 0,4,0,0,0,0,0,7,0, 0,5,1,8,0,9,2,0,0,
		 1,0,0,9,0,0,6,3,0, 0,2,0,6,3,0,0,0,7, 0,0,5,0,0,0,0,0,0} //ok
	};
	Cell c;
	bool flag = c.init(sudoku[2]);
	if (flag) c.print();
	clock_t time = -clock();
	c.solve();
	time += clock();
	cout << "������" << double(time / CLOCKS_PER_SEC) << "secs." << endl;
	system("pause");
	return 0;
}