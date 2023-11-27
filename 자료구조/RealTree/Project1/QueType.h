#include <new>
#include <cstddef>

class FullQueue {};
class EmptyQueue {};

typedef char ItemType;
struct NodeType;

class QueType {
public:
    QueType();
    ~QueType();
    QueType(const QueType& anotherQue);
    void MakeEmpty();
    bool IsEmpty() const;
    bool IsFull() const;
    void Enqueue(ItemType newItem);
    void Dequeue(ItemType& item);
private:
    NodeType* front;
    NodeType* rear;
};