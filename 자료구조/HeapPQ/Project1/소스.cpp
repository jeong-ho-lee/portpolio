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

	SongList s1("소연", 90, temp);
	
	temp.clear();
	temp.push_back("비와 당신");
	temp.push_back("그땐 그랬지");

	SongList s2("유재석", 100, temp);

	temp.clear();
	temp.push_back("난 알아요");
	temp.push_back("하여가");
	temp.push_back("교실 이데아");

	SongList s3("서태지", 65, temp);

	temp.clear();
	temp.push_back("가시");
	temp.push_back("Funny Rock");
	temp.push_back("겁쟁이");

	SongList s4("BUZZ", 80, temp);

	temp.clear();
	temp.push_back("말 달리자");
	temp.push_back("서커스 매직 유랑단");
	temp.push_back("밤이 깊었네");
	temp.push_back("좋지 아니한가");

	SongList s5("크라잉넛", 60, temp);

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