#include <iostream>
#include "Student.h"
#include "SelectionSort.h"
#include "BubbleSort.h"
#include "InsertionSort.h"
#include "HeapSort.cpp"
using namespace std;

int main() {
	Student stu[100];
	stu[0].InitValue(2003200111, (char*)"¿Ãøı¿Á", 3.0);
	stu[1].InitValue(2004200121, (char*)"±«ø¿¡ÿ", 3.2);
	stu[2].InitValue(2005200132, (char*)"±Ë¡¯¿œ", 2.7);

	// SelectionSort(stu, 3);
	// BubbleSort(stu, 3);
	// InsertionSort(stu, 3);
	HeapSort(stu, 3);
	Print(cout, stu, 3);

	int ary[9];
	ary[0] = 25;
	ary[1] = 17;
	ary[2] = 36;
	ary[3] = 17;
	ary[4] = 2;
	ary[5] = 100;
	ary[6] = 1;
	ary[7] = 19;
	ary[8] = 7;

	cout << "sum of Heights" << GetHeigthSum(stu, 9) << endl;
	HeapSort(ary, 9);
	cout << ary << endl;

	return 0;
}