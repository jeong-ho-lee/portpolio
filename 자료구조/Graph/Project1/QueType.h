#include <iostream>
using namespace std;

class FullQueue {};

class EmptyQueue {};

template <class ItemType>
struct NodeType;

template <class ItemType>
class QueType {
public:
    QueType();
    ~QueType();
    void MakeEmpty();
    bool IsEmpty() const;
    bool IsFull() const;
    void Enqueue(ItemType newItem);
    void Dequeue(ItemType& item);
private:
    NodeType<ItemType>* front;
    NodeType<ItemType>* rear;
};

template <class ItemType>
struct NodeType {
    ItemType info;
    NodeType* next;
};


template <class ItemType>
QueType<ItemType>::QueType() {
    front = NULL;
    rear = NULL;
}

template <class ItemType>
void QueType<ItemType>::MakeEmpty() {
    NodeType<ItemType>* tempPtr;

    while (front != NULL) {
        tempPtr = front;
        front = front->next;
        delete tempPtr;
    }
    rear = NULL;
}

template <class ItemType>
QueType<ItemType>::~QueType() {
    MakeEmpty();
}

template<class ItemType>
bool QueType<ItemType>::IsFull() const {
    NodeType<ItemType>* location;

    location = new NodeType<ItemType>;
    delete location;
    return false;
}

template <class ItemType>
bool QueType<ItemType>::IsEmpty() const {
    return (front == NULL);
}

template <class ItemType>
void QueType<ItemType>::Enqueue(ItemType newItem) {
    if (IsFull()) throw FullQueue();
    else {
        NodeType<ItemType>* newNode;

        newNode = new NodeType<ItemType>;
        newNode->info = newItem;
        newNode->next = NULL;

        if (rear == NULL) front = newNode;
        else rear->next = newNode;
        rear = newNode;
    }
}

template <class ItemType>
void QueType<ItemType>::Dequeue(ItemType& item) {
    if (IsEmpty()) throw EmptyQueue();
    else {
        NodeType<ItemType>* tempPtr;

        tempPtr = front;
        item = front->info;
        front = front->next;

        if (front == NULL) rear = NULL;
        delete tempPtr;
    }
}