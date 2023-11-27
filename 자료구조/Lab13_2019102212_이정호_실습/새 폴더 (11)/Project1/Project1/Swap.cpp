#include "Student.h"

void Swap(Student& item1, Student& item2)
{
	Student tempItem;

	tempItem = item1;
	item1 = item2;
	item2 = tempItem;
}

void Swap(Student*& item1, Student*& item2) {
	Student* tempItem;

	tempItem = item1;
	item1 = item2;
	item2 = tempItem;
}