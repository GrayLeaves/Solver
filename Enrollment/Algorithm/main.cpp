#include <iostream>
#include <vector>
#include <algorithm>
#define SIZE 32
//#include "SeqList.h"
/*算法思想：
1.分治法
2.减治法
3.回溯法
4.动态规划法
5.贪心算法
6.分支限界法
7.迭代和递归
*/
using namespace std;
/* run this program using the console pauser or
add your own getch, system("pause") or input loop */
int main(int argc, char** argv) {
	/*SeqList lst_1;
	cout << "Initial:" << endl;
	for(int i=0; i<lst_1.size; ++i){
		lst_1.data[i] = i;
		cout << lst_1.data[i] << "\t";
	}
	cout << endl;

	Reverse(lst_1);
	cout << "Result:" << endl;
	for(int i=0; i<lst_1.size; ++i){
		cout << lst_1.data[i] << "\t";
	}
	cout << endl;*/
	vector<int> vec(SIZE);
	for(int i=0; i<SIZE; ++i)
        vec[i] = i+1;
    default_random_engine eng;    // default engine.
    shuffle(vec.begin(),vec.end(),eng);
    cout << "Before sorted:" << endl;
    for(auto i : vec){
        cout << i << " ";
    } cout << endl;

    select_sort(vec);

    cout << "After sorted:" << endl;
    for(auto i : vec){
        cout << i << " ";
    } cout << endl;

	return 0;
}
