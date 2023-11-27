#include "Student.h"

void MergeSort(Student values[], int first, int last);

void Merge(Student values[], int leftFirst, int leftLast, int rightFirst, int rightLast) {
	int arySize = rightLast - leftFirst - 1;
	Student* tempArray = new Student[arySize];
	int index = leftFirst;
	int saveFirst = leftFirst;

	while ((leftFirst <= leftLast) && (rightFirst <= rightLast)) {
		if (strcmp(values[leftFirst].getName(), values[rightFirst].getName()) <= 0) {
			tempArray[index] = values[leftFirst];
			leftFirst++;
		}
		else {
			tempArray[index] = values[rightFirst];
			rightFirst++;
		}
		index++;
	}

	while (leftFirst <= leftLast) {
		tempArray[index] = values[leftFirst];
		leftFirst++;
		index++;
	}

	while (rightFirst <= rightLast) {
		tempArray[index] = values[rightFirst];
		rightFirst++;
		index++;
	}

	for (index = saveFirst; index <= rightLast; index++) {
		values[index] = tempArray[index];
		delete[] tempArray;
	}
}

void MergeSort(Student values[], int first, int last) {
	if (first < last) {
		int middle = (first + last) / 2;
		MergeSort(values, first, middle);
		MergeSort(values, middle + 1, last);
		Merge(values, first, middle, middle + 1, last);
	}
}