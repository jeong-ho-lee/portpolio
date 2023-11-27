#include <iostream>
#include <vector>
#include <string>
using namespace std;
struct SongList {
	string name;
	int popular;
	vector<string> songs;
	SongList() {}
	SongList(string _n, int _p, vector<string> _s) : name(_n), popular(_p), songs(_s) {}
};

void Swap(SongList& one, SongList& two);

struct HeapType {
	void ReheapDown(int root, int bottom);
	void ReheapUp(int root, int bottom);
	void HeapDown(int root, int bottom);
	void HeapUp(int root, int bottom);
	void Enqueue(SongList _s);
	//�迭 ����
	SongList elements[10];
	int numElements = 0;
	void Print();
};

void Swap(SongList& one, SongList& two) {
	SongList temp;
	temp = one;
	one = two;
	two = temp;
}

void HeapType::ReheapUp(int root, int bottom) {
	int parent;

	if (bottom > root) {
		parent = (bottom - 1) / 2;
		if (elements[parent].popular < elements[bottom].popular) {
			Swap(elements[parent], elements[bottom]);
			ReheapUp(root, parent);
		}
	}
}

void HeapType::ReheapDown(int root, int bottom) {
	int maxChild;
	int rightChild;
	int leftChild;

	leftChild = root * 2 + 1;
	rightChild = root * 2 + 2;
	if (leftChild <= bottom) {
		if (leftChild == bottom) maxChild = leftChild;
		else {
			if (elements[leftChild].popular <= elements[rightChild].popular) maxChild = rightChild;
			else maxChild = leftChild;
		}

		if (elements[root].popular < elements[maxChild].popular) {
			Swap(elements[root], elements[maxChild]);
			ReheapDown(maxChild, bottom);
		}
	}
}

void HeapType::HeapUp(int root, int bottom) {
	int parent;
	bool reheaped = false;

	while (bottom > root && !reheaped) {
		parent = (bottom - 1) / 2;

		if (elements[parent].popular < elements[bottom].popular) {
			Swap(elements[parent], elements[bottom]);
			bottom = parent;
		}
		else {
			reheaped = true;
		}
	}
}

void HeapType::HeapDown(int root, int bottom) {
	int maxChild, leftChild, rightChild;
	bool reheaped = false;

	leftChild = root * 2 + 1;

	while (leftChild <= bottom && !reheaped) {
		if (leftChild == bottom) maxChild = leftChild;
		else {
			rightChild = root * 2 + 2;
			maxChild = (elements[leftChild].popular <= elements[rightChild].popular) ? maxChild = rightChild : maxChild = leftChild;
		}

		if (elements[root].popular < elements[maxChild].popular) {
			Swap(elements[root], elements[maxChild]);
			root = maxChild;
			leftChild = root * 2 + 1;
		}
		else {
			reheaped = true;
		}
	}
}

void HeapType::Enqueue(SongList _s) {
	elements[numElements] = _s;
	ReheapUp(0, numElements - 1);
	numElements++;
}

void HeapType::Print() {
	cout << "��õ ��� : " << elements[0].name << " �α⵵ " << elements[0].popular << ' ';
	for (auto& elem : elements[0].songs) cout << '\"' << elem << "\", ";
	Swap(elements[0], elements[numElements - 1]);
	numElements--;
	ReheapDown(0, numElements - 1);
}

//������ �迭 ���·� �����ϰ� �Ǹ� �޸� ���� �̵��� �ִ�.

void Swap2(SongList*& one, SongList*& two) {
	SongList* temp;
	temp = one;
	one = two;
	two = temp;
}

struct HeapType2 {
	void ReheapDown(int root, int bottom);
	void ReheapUp(int root, int bottom);
	void HeapDown(int root, int bottom);
	void HeapUp(int root, int bottom);
	void Enqueue(SongList* _s);
	//������ �迭 ����
	SongList* elements[10];
	int numElements = 0;
	void Print();
};

void HeapType2::ReheapUp(int root, int bottom) {
	int parent;

	if (bottom > root) {
		parent = (bottom - 1) / 2;
		if (elements[parent]->popular < elements[bottom]->popular) {
			Swap2(elements[parent], elements[bottom]);
			ReheapUp(root, parent);
		}
	}
}

void HeapType2::ReheapDown(int root, int bottom) {
	int maxChild;
	int rightChild;
	int leftChild;

	leftChild = root * 2 + 1;
	rightChild = root * 2 + 2;
	if (leftChild <= bottom) {
		if (leftChild == bottom) maxChild = leftChild;
		else {
			if (elements[leftChild]->popular <= elements[rightChild]->popular) maxChild = rightChild;
			else maxChild = leftChild;
		}

		if (elements[root]->popular < elements[maxChild]->popular) {
			Swap2(elements[root], elements[maxChild]);
			ReheapDown(maxChild, bottom);
		}
	}
}

void HeapType2::HeapUp(int root, int bottom) {
	int parent;
	bool reheaped = false;

	while (bottom > root && !reheaped) {
		parent = (bottom - 1) / 2;

		if (elements[parent]->popular < elements[bottom]->popular) {
			Swap2(elements[parent], elements[bottom]);
			bottom = parent;
		}
		else {
			reheaped = true;
		}
	}
}

void HeapType2::HeapDown(int root, int bottom) {
	int maxChild, leftChild, rightChild;
	bool reheaped = false;

	leftChild = root * 2 + 1;

	while (leftChild <= bottom && !reheaped) {
		if (leftChild == bottom) maxChild = leftChild;
		else {
			rightChild = root * 2 + 2;
			maxChild = (elements[leftChild]->popular <= elements[rightChild]->popular) ? maxChild = rightChild : maxChild = leftChild;
		}

		if (elements[root]->popular < elements[maxChild]->popular) {
			Swap2(elements[root], elements[maxChild]);
			root = maxChild;
			leftChild = root * 2 + 1;
		}
		else {
			reheaped = true;
		}
	}
}

void HeapType2::Enqueue(SongList* _s) {
	elements[numElements] = _s;
	ReheapUp(0, numElements - 1);
	numElements++;
}

void HeapType2::Print() {
	cout << "��õ ��� : " << elements[0]->name << " �α⵵ " << elements[0]->popular << ' ';
	for (auto& elem : elements[0]->songs) cout << '\"' << elem << "\", ";
	Swap2(elements[0], elements[numElements - 1]);
	numElements--;
	ReheapDown(0, numElements - 1);
}