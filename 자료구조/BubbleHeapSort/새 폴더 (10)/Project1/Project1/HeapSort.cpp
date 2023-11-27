#pragma once
#include "Student.h"

template<class ItemType>
void ReheapDown(ItemType elements[], int root, int bottom)
// Post: Heap property is restored.
{
	int maxChild;
	int rightChild;
	int leftChild;

	leftChild = root * 2 + 1;
	rightChild = root * 2 + 2;
	if (leftChild <= bottom)
	{
		if (leftChild == bottom)
			maxChild = leftChild;
		else
		{
			if (elements[leftChild] <= elements[rightChild])
				maxChild = rightChild;
			else
				maxChild = leftChild;
		}
		if (elements[root] < elements[maxChild])
		{
			Swap(elements[root], elements[maxChild]);
			ReheapDown(elements, maxChild, bottom);
		}
	}
}

template<class ItemType>
void ReheapUp(ItemType elements[], int root, int bottom)
// Post: Heap property is restored.
{
	int parent;

	if (bottom > root)
	{
		parent = (bottom - 1) / 2;
		if (elements[parent] < elements[bottom])
		{
			Swap(elements[parent], elements[bottom]);
			ReheapUp(elements, root, parent);
		}
	}
}


template<class ItemType>
void HeapSort(ItemType values[], int numValues) {

	int index;

	for (index = numValues / 2 - 1; index >= 0; index--)
		ReheapDown(values, index, numValues - 1);

	for (index = numValues - 1; index >= 1; index--) {
		Swap(values[0], values[index]);
		ReheapDown(values, 0, index - 1);
	}
}


template <class ItemType>
int GetHeigthSum(ItemType values[], int numValues) {

	int index, sum = 0;

	for (index = 0; index < numValues; index++)
		sum += GetHeight(values, index, numValues - 1);

	return sum;
}

template <class ItemType>
int GetHeight(ItemType values[], int start, int numValues) {

	if (start > (numValues - 1) || (start * 2 + 1) < numValues)
		return 0;

	int l_height = GetHeight(values, start * 2 + 1, numValues);
	int r_height = GetHeight(values, start * 2 + 1, numValues);


	return (l_height > r_height) ? l_height + 1 : r_height + 1;
}