#include "Student.h"

void InsertItem(Student values[], int startIndex, int endIndex) {
	bool finished = false;
	int current = endIndex;
	bool moreToSearch = (current != startIndex);

	while (moreToSearch && !finished) {
		if (strcmp(values[current].getName(), values[current - 1].getName()) < 0) {
			Swap(values[current], values[current - 1]);
			current--;
			moreToSearch = (current != startIndex);
		}
		else {
			finished = true;
		}
	}
}

void InsertionSort(Student values[], int numValues) {
	for (int count = 0; count < numValues; count++) {
		InsertItem(values, 0, count);
	}
}