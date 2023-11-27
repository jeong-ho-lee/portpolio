#include <iostream>
using namespace std;

int main() {
	int grade1, grade2, grade3;
	int* pgrade1, * pgrade2, * pgrade3;

	pgrade1 = &grade1;
	pgrade2 = &grade2;
	pgrade3 = &grade3;

	cin >> grade1 >> grade2 >> grade3;

	cout << "Value : grade1 = " << grade1 << "\t\t\t\t*pgrade1 = " << *pgrade1 << "\t\t\t\t*&grade1 = \t" << *&grade1;
	cout << "\nAddr : &grade1 = " << &grade1 << "\t\tpgrade1 = " << pgrade1 << "\t\t&pgrade1 = \t" << *&pgrade1;
	cout << "Value : grade2 = " << grade2 << "\t\t\t\t*pgrade2 = " << *pgrade2 << "\t\t\t\t*&grade2 = \t" << *&grade2;
	cout << "\nAddr : &grade2 = " << &grade2 << "\t\tpgrade2 = " << pgrade2 << "\t\t&pgrade2 = \t" << *&pgrade2;
	cout << "Value : grade3 = " << grade3 << "\t\t\t\t*pgrade3 = " << *pgrade3 << "\t\t\t\t*&grade3 = \t" << *&grade3;
	cout << "\nAddr : &grade3 = " << &grade3 << "\t\tpgrade3 = " << pgrade3 << "\t\t&pgrade3 = \t" << *&pgrade3;

	return 0;
}