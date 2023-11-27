#include "Heap.h"
#include <iostream>
#include <vector>
#include <string>
using namespace std;

int main() {
	vector<string> temp;
	temp.push_back("Queen Card");
	temp.push_back("Oh My God");
	temp.push_back("Nxde");
	temp.push_back("Tomboy");

	SongList s1("�ҿ�", 90, temp);
	
	temp.clear();
	temp.push_back("��� ���");
	temp.push_back("�׶� �׷���");

	SongList s2("���缮", 100, temp);

	temp.clear();
	temp.push_back("�� �˾ƿ�");
	temp.push_back("�Ͽ���");
	temp.push_back("���� �̵���");

	SongList s3("������", 65, temp);

	temp.clear();
	temp.push_back("����");
	temp.push_back("Funny Rock");
	temp.push_back("������");

	SongList s4("BUZZ", 80, temp);

	temp.clear();
	temp.push_back("�� �޸���");
	temp.push_back("��Ŀ�� ���� ������");
	temp.push_back("���� �����");
	temp.push_back("���� �ƴ��Ѱ�");

	SongList s5("ũ���׳�", 60, temp);

	HeapType heap;
	heap.Enqueue(s1);
	heap.Enqueue(s2);
	heap.Enqueue(s3);
	heap.Enqueue(s4);
	heap.Enqueue(s5);
	heap.Print();
	cout << '\n';
	heap.Print();
	cout << '\n';
	heap.Print();
	cout << '\n';
	heap.Print();
	cout << '\n';
	heap.Print();

	return 0;
}