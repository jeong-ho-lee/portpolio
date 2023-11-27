#include <iostream>
#include "Student.h"
#include "HashTable.h"
using namespace std;

int main() {
	Student stu[100];
	stu[0].InitValue(2003200111, (char*)"A", 3.0);
	stu[1].InitValue(2004200121, (char*)"B", 3.2);
	stu[2].InitValue(2005200132, (char*)"C", 2.7);
	Student* stuPtrs[3];

	for (int k = 0; k < 3; k++) {
		stuPtrs[k] = &stu[k];
	}

	Student emptyItem;

	emptyItem.InitValue(0000000000, (char*)"None", 0.0);

	HashTable<Student> hashTable(emptyItem);

	for (int i = 0; i < 3; i++)
		hashTable.InsertItem(stu[i]);

	bool found = false;
	hashTable.RetrieveItem(stu[0], found);

	cout << found << endl;
	cout << stu[0].getName() << endl;
	return 0;
}