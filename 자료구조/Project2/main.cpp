#include "Paint.h"
#include "QueType.h"
#include <iostream>
using namespace std;

int main() {
	int n;
	cin >> n;

	MyPaint paint(n);

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			paint.DrawPoint(i, j, 65 + i * n + j);
		}
	}
	cout << "\n";

	cout << "�ʱ�ȭ\n";
	paint.Print();
	cout << "\n";

	cout << "�� ���� �� ���\n";
	paint.DrawPoint(1, 0, 65);
	paint.DrawPoint(1, 1, 66);
	paint.Print();
	cout << "\n";

	cout << "�� ä���\n";
	paint.Fill(0, 0, 66);
	paint.Print();
	cout << "\n";

	paint.Fill(0, 0, 67);
	paint.Print();
	cout << "\n";

	cout << "�ǵ�����\n";
	paint.Undo(1);
	paint.Print();
	cout << "\n";

	cout << "�� ���� �� ���\n";
	paint.DrawPoint(4, 4, 65);
	paint.Print();
	cout << '\n';

	cout << "�ǵ����� 2��\n";
	paint.Undo(2);
	paint.Print();

	return 0;
}