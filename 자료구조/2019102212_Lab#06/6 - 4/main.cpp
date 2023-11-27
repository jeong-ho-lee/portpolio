#include "UnsortedType.cpp"
#include <iostream>

using namespace std;

// 3 - C
template <typename ItemType>
void MergeLists(UnsortedType<ItemType>& l_a, UnsortedType<ItemType>& l_b, UnsortedType<ItemType>& result) {
	for (int i = l_a.LengthIs(); i; i--) {
		ItemType item;
		l_a.GetNextItem(item);
		result.InsertItem(item);
	}

	for (int i = l_b.LengthIs(); i; i--) {
		ItemType item;
		l_b.GetNextItem(item);
		result.InsertItem(item);
	}
}

int main() {
	UnsortedType<int> l_a;
	UnsortedType<int> l_b;
	UnsortedType<int> result;

	l_a.InsertItem(1);
	l_a.InsertItem(3);
	l_a.InsertItem(5);
	l_a.InsertItem(7);
	l_a.InsertItem(9);

	l_b.InsertItem(2);
	l_b.InsertItem(4);
	l_b.InsertItem(6);
	l_b.InsertItem(8);
	l_b.InsertItem(10);

	MergeLists(l_a, l_b, result);
	result.ResetList();
	
	for (int i = result.LengthIs(); i; i--) {
		int item;
		result.GetNextItem(item);
		cout << item << "\n";
	}
	
	cout << "\n";

	result.MakeEmpty();
	l_a.MergeLists(l_b, result);
	result.ResetList();

	for (int i = result.LengthIs(); i; i--) {
		int item;
		result.GetNextItem(item);
		cout << item << "\n";
	}

	cout << "\n";

	result.InsertItem(1);
	result.DeleteItem(1);
	result.ResetList();

	for (int i = result.LengthIs(); i; i--) {
		int item;
		result.GetNextItem(item);
		cout << item << "\n";
	}

	return 0;
}