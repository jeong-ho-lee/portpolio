#pragma once

class back {
public:
	back() {
		deque = new char* [10];
		currentPos = -1;
	}

	void push(char* dlist) {
		currentPos = (currentPos + 1) % 10;
		deque[currentPos] = dlist;
	}

	char* undo(int n) {
		return deque[(currentPos + 10 - (n - 1)) % 10];
	}

	~back() {
		delete[] deque;
	}
private:
	char** deque;
	int currentPos;
};

class MyPaint {
public:
	void DrawPoint(int x, int y, int rgb);
	MyPaint(int n);
	~MyPaint();
	void Print();
	void Fill(int x, int y, int rgb);
	void Undo(int n);
private:
	int length;
	char** pixel;
	back backtemp;
};