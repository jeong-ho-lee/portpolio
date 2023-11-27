#include "GraphType.h"

#include <iostream>
using namespace std;

int main() {
	GraphType<const char*> graph;

	graph.AddVertex("dog");
	graph.AddVertex("cat");
	graph.AddVertex("animal");
	graph.AddVertex("vertebrate");
	graph.AddVertex("oyster");
	graph.AddVertex("shellfish");
	graph.AddVertex("invertebrate");
	graph.AddVertex("crab");
	graph.AddVertex("poodle");
	graph.AddVertex("monkey");
	graph.AddVertex("banana");
	graph.AddVertex("dalmatian");
	graph.AddVertex("dachshund");

	graph.AddEdge("vertebrate", "animal", 10);
	graph.AddEdge("invertebrate", "animal", 20);
	graph.AddEdge("dog", "vertebrate", 30);
	graph.AddEdge("cat", "vertebrate", 40);
	graph.AddEdge("monkey", "vertebrate", 50);
	graph.AddEdge("shellfish", "invertebrate", 60);
	graph.AddEdge("crab", "shellfish", 70);
	graph.AddEdge("oyster", "invertebrate", 80);
	graph.AddEdge("poodle", "dog", 90);
	graph.AddEdge("dalmatian", "dog", 100);
	graph.AddEdge("dachshund", "dog", 110);

	cout << "Weight of 'verebrate' to 'animal' is " << graph.WeightIs("vertebrate", "animal") << endl;
	cout << "Weight of 'poodle' to 'dog' is " << graph.WeightIs("poodle", "dog") << endl;

	graph.DeleteEdge("poodle", "dog");
	cout << "Weight of poodle to dog is " << graph.WeightIs("poodle", "dog") << endl;

	cout << endl << "GetToVertices(dog, queue)" << endl;
	QueType<const char*> queue;
	graph.GetToVertices("dog", queue);
	while (!queue.IsEmpty()) {
		const char* item;
		queue.Dequeue(item);
		cout << item << endl;
	}

	cout << endl << endl;
	graph.DepthFirstSearch("dalmatian", "animal");
	cout << endl;

	graph.AddVertex("apple");
	graph.DeleteEdge("dog", "vertebrate");
	graph.AddEdge("dog", "oyster", 200);
	graph.AddEdge("dog", "banana", 200);
	graph.AddEdge("dog", "apple", 200);
	cout << endl << endl;
	graph.DepthFirstSearch("dalmatian", "animal");
	cout << endl;

	return 0;
}