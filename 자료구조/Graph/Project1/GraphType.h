#include "QueType.h"

template<class VertexType>
class GraphType {
public:
	GraphType();
	GraphType(int maxV);
	~GraphType();
	void AddVertex(VertexType);
	void AddEdge(VertexType, VertexType, int);
	int WeightIs(VertexType, VertexType);
	void GetToVertices(VertexType, QueType<VertexType>&);
	void DeleteEdge(VertexType fromVertex, VertexType toVertex);
	bool DepthFirstSearch(VertexType startVertex, VertexType endVertex);
private:
	int numVertices;
	int maxVertices;
	VertexType* vertices;
	int edges[50][50];
	bool* marks;
};

template<class VertexType>
GraphType<VertexType>::GraphType() {
	numVertices = 0;
	maxVertices = 50;
	vertices = new VertexType[50];
	marks = new bool[50];
}

template<class VertexType>
GraphType<VertexType>::GraphType(int maxV) {
	numVertices = 0;
	maxVertices = maxV;
	vertices = new VertexType[maxV];
	marks = new bool[maxV];
}

template<class VertexType>
GraphType<VertexType>::~GraphType() {
	delete[] vertices;
	delete[] marks;
}
const int NULL_EDGE = 0;

template<class VertexType>
void GraphType<VertexType>::AddVertex(VertexType vertex) {
	vertices[numVertices] = vertex;

	for (int index = 0; index < numVertices; index++) {
		edges[numVertices][index] = NULL_EDGE;
		edges[index][numVertices] = NULL_EDGE;
	}
	numVertices++;
}

template<class VertexType>
int IndexIs(VertexType* vertices, VertexType vertex) {
	int index = 0;

	while (!(vertex == vertices[index])) index++;
	return index;
}

template<class VertexType>
void GraphType<VertexType>::AddEdge(VertexType fromVertex, VertexType toVertex, int weight) {
	int row;
	int col;

	row = IndexIs(vertices, fromVertex);
	col = IndexIs(vertices, toVertex);
	edges[row][col] = weight;
}

template<class VertexType>
int GraphType<VertexType>::WeightIs
(VertexType fromVertex, VertexType toVertex) {
	int row;
	int col;

	row = IndexIs(vertices, fromVertex);
	col = IndexIs(vertices, toVertex);
	return edges[row][col];
}

template<class VertexType>
void GraphType<VertexType>::GetToVertices(VertexType vertex, QueType<VertexType>& adjVertices) {
	int fromIndex;
	int toIndex;

	fromIndex = IndexIs(vertices, vertex);

	for (toIndex = 0; toIndex < numVertices; toIndex++) if (edges[fromIndex][toIndex] != NULL_EDGE) adjVertices.Enqueue(vertices[toIndex]);
}

template <class VertexType>
void GraphType<VertexType>::DeleteEdge(VertexType fromVertex, VertexType toVertex) {
	int row;
	int col;

	row = IndexIs(vertices, fromVertex);
	col = IndexIs(vertices, toVertex);
	edges[row][col] = NULL_EDGE;
}

template <class VertexType>
bool GraphType<VertexType>::DepthFirstSearch(VertexType startVertex, VertexType endVertex) {
	QueType<VertexType> vertexQ;
	VertexType vertex;

	if (startVertex == endVertex) {
		cout << endVertex;
		return true;
	}

	while (!vertexQ.IsEmpty()) {
		vertexQ.Dequeue(vertex);

		if (vertex != startVertex) {
			if (DepthFirstSearch(vertex, endVertex)) {
				cout << " <- " << vertex;
				return true;
			}
		} else continue;
	}
	return false;
}