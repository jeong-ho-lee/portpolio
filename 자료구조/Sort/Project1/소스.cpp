#include <iostream>
using namespace std;

template <class ItemType>
void Swap(ItemType& A, ItemType& B) {
	ItemType temp = A;
	A = B;
	B = temp;
}

template <class ItemType>
int MinIndex(ItemType values[], int start, int end) {
	int indexOfMin = start;

	for (int index = start + 1; index <= end; index++) if (values[index] < values[indexOfMin]) indexOfMin = index;

	return indexOfMin;
}

template <class ItemType>
void SelectionSort(ItemType values[], int numValues) {
	int endIndex = numValues - 1;

	for (int current = 0; current < endIndex; current++) Swap(values[current], values[MinIndex(values, current, endIndex)]);
}

template <class ItemType>
void BubbleUp(ItemType values[], int start, int end) {
	for (int index = end; index > start; index--) if (values[index] < values[index - 1]) Swap(values[index], values[index - 1]);
}

template <class ItemType>
void BubbleSort(ItemType values[], int numValues) {
	int current = 0;

	while (current < numValues - 1) {
		BubbleUp(values, current, numValues - 1);
		current++;
	}
}

template <class ItemType>
int BubbleUp2(ItemType values[], int start, int end) {
	int flag = 0;

	for (int index = end; index > start; index--)
		if (values[index] < values[index - 1]) {
			Swap(values[index], values[index - 1]);
			flag++;
		}

	return flag;
}

template <class ItemType>
void BubbleSort2(ItemType values[], int numValues) {
	int current = 0;

	while ((current < numValues - 1) && BubbleUp2(values, current, numValues - 1)) current++;
}

template <class ItemType>
void InsertItem(ItemType values[], int start, int end) {
	bool finished = false;
	int current = end;
	bool moreToSearch = (current != start);

	while(moreToSearch && !finished)
		if (values[current] < values[current - 1]) {
			Swap(values[current], values[current - 1]);
			current--;
			moreToSearch = (current != start);
		} else {
			finished = true;
		}
}

template <class ItemType>
void InsertionSort(ItemType values[], int numValues) {
	for (int count = 0; count < numValues; count++) InsertItem(values, 0, count);
}

template <class ItemType>
void ReHeapDown(ItemType values[], int root, int bottom) {
	int maxChild;
	int rightChild;
	int leftChild;

	leftChild = root * 2 + 1;
	rightChild = root * 2 + 2;

	if (leftChild <= bottom) {
		if (leftChild == bottom) maxChild = leftChild;
		else {
			if (values[leftChild] <= values[rightChild]) maxChild = rightChild;
			else maxChild = leftChild;
		}
		if(values[root] < values[maxChild]){
			Swap(values[root], values[maxChild]);
			ReHeapDown(values, maxChild, bottom);
		}
	}
}

template <class ItemType>
void HeapSort(ItemType values[], int numValues) {
	int index;

	for (index = numValues / 2 - 1; index >= 0; index--) ReHeapDown(values, index, numValues - 1);
	for (index = numValues - 1; index >= 1; index--) {
		Swap(values[0], values[index]);
		ReHeapDown(values, 0, index - 1);
	}
}

template <class ItemType>
void Split(ItemType values[], int first, int last) {
	int splitVal = values[last];
	int i = first - 1;

	for(int j = first; j <= last; j++)
		if (values[j] < splitVal) {
			i++;
			Swap(&values[i], &values[j]);
		}

	Swap(&values[i + 1], &values[last]);


}

template <class ItemType>
void Split(ItemType values[], int first, int last, int& splitPoint) {
	ItemType pivotValue = values[first];
	int left = first + 1;
	int right = last;
	bool done = false;

	while (!done) {
		while (left <= right && values[left] <= pivotValue) left++;
		while (values[right] >= pivotValue && right >= left) right--;

		if (right < left) done = true;
		else Swap(values[left], values[right]);
	}

	Swap(values[first], values[right]);
	splitPoint = right;
}

template <class ItemType>
void QuickSort(ItemType values[], int first, int last) {
	if (first < last) {
		int splitPoint;

		Split(values, first, last, splitPoint);
		QuickSort(values, first, splitPoint - 1);
		QuickSort(values, splitPoint + 1, last);
	}
}

template <class ItemType>
void Merge(ItemType values[], int firstLeft, int lastLeft, int firstRight, int lastRight) {
	int size = lastRight - firstLeft + 1;
	ItemType* tempArray = new ItemType[size];
	int index = 0;
	int left = firstLeft;
	int right = firstRight;

	while (left <= lastLeft && right <= lastRight) {
		if (values[left] <= values[right]) {
			tempArray[index] = values[left];
			left++;
		}
		else {
			tempArray[index] = values[right];
			right++;
		}
		index++;
	}

	// 왼쪽 부분 배열에 남은 요소들을 복사
	while (left <= lastLeft) {
		tempArray[index] = values[left];
		left++;
		index++;
	}

	// 오른쪽 부분 배열에 남은 요소들을 복사
	while (right <= lastRight) {
		tempArray[index] = values[right];
		right++;
		index++;
	}

	// 임시 배열의 값들을 원래 배열에 복사
	for (int i = 0; i < size; i++) {
		values[firstLeft + i] = tempArray[i];
	}
}



template <class ItemType>
void MergeSort(ItemType values[], int first, int last) {
	if (first < last) {
		int middle = (first + last) / 2;
		
		MergeSort(values, first, middle);
		MergeSort(values, middle + 1, last);
		
		Merge(values, first, middle, middle + 1, last);
	}
}

int main() {
	int value[6][10];

	srand((unsigned)time(0));
	for (int i = 0; i < 6; i++) for (int j = 0; j < 10; j++) value[i][j] = rand() % 10;

	for (int j = 0; j < 10; j++) cout << value[0][j] << ' '; cout << '\n';
	SelectionSort(value[0], 10);
	for (int j = 0; j < 10; j++) cout << value[0][j] << ' '; cout << "\n\n";

	for (int j = 0; j < 10; j++) cout << value[1][j] << ' '; cout << '\n';
	BubbleSort2(value[1], 10);
	for (int j = 0; j < 10; j++) cout << value[1][j] << ' '; cout << "\n\n";

	for (int j = 0; j < 10; j++) cout << value[2][j] << ' '; cout << '\n';
	InsertionSort(value[2], 10);
	for (int j = 0; j < 10; j++) cout << value[2][j] << ' '; cout << "\n\n";

	for (int j = 0; j < 10; j++) cout << value[3][j] << ' '; cout << '\n';
	HeapSort(value[3], 10);
	for (int j = 0; j < 10; j++) cout << value[3][j] << ' '; cout << "\n\n";

	for (int j = 0; j < 10; j++) cout << value[4][j] << ' '; cout << '\n';
	QuickSort(value[4], 0, 10);
	for (int j = 0; j < 10; j++) cout << value[4][j] << ' '; cout << "\n\n";

	for (int j = 0; j < 10; j++) cout << value[5][j] << ' '; cout << '\n';
	MergeSort(value[5], 0, 10);
	for (int j = 0; j < 10; j++) cout << value[5][j] << ' ';

	return 0;
}