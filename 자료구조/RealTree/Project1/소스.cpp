#include "TreeType.h"
#include <iostream>
using namespace std;

int main() {
	TreeType tree;

	cout << tree.IsEmpty() << '\n';

	tree.InsertItem('E');
	tree.InsertItem('W');
	tree.InsertItem('D');
	tree.InsertItem('X');
	tree.InsertItem('C');
	tree.InsertItem('Y');
	tree.InsertItem('B');
	tree.InsertItem('Z');
	tree.InsertItem('A');
	tree.Print();
	cout << '\n';

	cout << tree.IsEmpty() << '\n';
	cout << tree.IsFull() << '\n';
	cout << tree.LeafCount() << '\n';

	return 0;
}