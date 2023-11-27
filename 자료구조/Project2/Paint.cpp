#include "Paint.h"
#include "QueType.h"
#include <iostream>
using namespace std;

void MyPaint::DrawPoint(int x, int y, int rgb) {
	char* temp = new char[length * length];
	for (int i = 0; i < length; i++) {
		for (int j = 0; j < length; j++) {
			temp[i * length + j] = pixel[i][j];
		}
	}
	backtemp.push(temp);

	pixel[x][y] = (char)rgb;
}

MyPaint::MyPaint(int n) {
	length = n;
	pixel = new char*[n];
	for (int i = 0; i < n; i++) {
		pixel[i] = new char[n];
	}

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			pixel[i][j] = '0';
		}
	}
}

MyPaint::~MyPaint() {
	for (int i = 0; i < length; i++) {
		delete[] pixel[i];
	}
	delete[] pixel;
}

void MyPaint::Print() {
	for (int i = 0; i < length; i++) {
		for (int j = 0; j < length; j++) {
			cout << pixel[i][j] << "\t";
		}
		cout << "\n";
	}
}
void MyPaint::Fill(int x, int y, int rgb) {
	if (pixel[x][y] == (char)rgb) return;

	char* temp = new char[length * length];
	for (int i = 0; i < length; i++) {
		for (int j = 0; j < length; j++) {
			temp[i * length + j] = pixel[i][j];
		}
	}
	backtemp.push(temp);

	QueType<int> xqueue;
	QueType<int> yqueue;
	char before = pixel[x][y];

	xqueue.Enqueue(x);
	yqueue.Enqueue(y);

	while (!xqueue.IsEmpty() && !yqueue.IsEmpty()) {
		int xtemp, ytemp;
		
		xqueue.Dequeue(xtemp);
		yqueue.Dequeue(ytemp);

		if (pixel[xtemp][ytemp] == before) {
			pixel[xtemp][ytemp] = rgb;

			if (!xtemp) {
				if (!ytemp) {
					xqueue.Enqueue(xtemp);
					yqueue.Enqueue(ytemp + 1);

					xqueue.Enqueue(xtemp + 1);
					yqueue.Enqueue(ytemp);
				}
				else if (ytemp == length - 1) {
					xqueue.Enqueue(xtemp);
					yqueue.Enqueue(ytemp - 1);

					xqueue.Enqueue(xtemp + 1);
					yqueue.Enqueue(ytemp);
				}
				else {
					xqueue.Enqueue(xtemp);
					yqueue.Enqueue(ytemp + 1);

					xqueue.Enqueue(xtemp);
					yqueue.Enqueue(ytemp - 1);

					xqueue.Enqueue(xtemp + 1);
					yqueue.Enqueue(ytemp);
				}
			}
			else if (xtemp == length - 1) {
				if (!ytemp) {
					xqueue.Enqueue(xtemp);
					yqueue.Enqueue(ytemp + 1);

					xqueue.Enqueue(xtemp - 1);
					yqueue.Enqueue(ytemp);
				}
				else if (ytemp == length - 1) {
					xqueue.Enqueue(xtemp);
					yqueue.Enqueue(ytemp - 1);

					xqueue.Enqueue(xtemp - 1);
					yqueue.Enqueue(ytemp);
				}
				else {
					xqueue.Enqueue(xtemp);
					yqueue.Enqueue(ytemp + 1);

					xqueue.Enqueue(xtemp);
					yqueue.Enqueue(ytemp - 1);

					xqueue.Enqueue(xtemp - 1);
					yqueue.Enqueue(ytemp);
				}
			}
			else {
				if (!ytemp) {
					xqueue.Enqueue(xtemp);
					yqueue.Enqueue(ytemp + 1);

					xqueue.Enqueue(xtemp + 1);
					yqueue.Enqueue(ytemp);

					xqueue.Enqueue(xtemp - 1);
					yqueue.Enqueue(ytemp);
				}
				else if (ytemp == length - 1) {
					xqueue.Enqueue(xtemp);
					yqueue.Enqueue(ytemp - 1);

					xqueue.Enqueue(xtemp + 1);
					yqueue.Enqueue(ytemp);

					xqueue.Enqueue(xtemp - 1);
					yqueue.Enqueue(ytemp);
				}
				else {
					xqueue.Enqueue(xtemp);
					yqueue.Enqueue(ytemp + 1);

					xqueue.Enqueue(xtemp);
					yqueue.Enqueue(ytemp - 1);

					xqueue.Enqueue(xtemp + 1);
					yqueue.Enqueue(ytemp);

					xqueue.Enqueue(xtemp - 1);
					yqueue.Enqueue(ytemp);
				}
			}
		}
	}
}

void MyPaint::Undo(int n) {
	char* temp1 = backtemp.undo(n);

	char* temp = new char[length * length];
	for (int i = 0; i < length; i++) {
		for (int j = 0; j < length; j++) {
			temp[i * length + j] = pixel[i][j];
		}
	}
	backtemp.push(temp);

	for (int i = 0; i < length; i++) {
		for (int j = 0; j < length; j++) {
			pixel[i][j] = temp1[i * length + j];
		}
	}
}