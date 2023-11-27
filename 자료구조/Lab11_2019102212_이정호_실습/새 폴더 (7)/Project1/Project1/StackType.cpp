// Implementation file for StackType.h

#include <cstddef>
#include <new>
#include "StackType.h"
void StackType::Push(ItemType newItem)
// Adds newItem to the top of the stack.
// Pre:  Stack has been initialized.
// Post: If stack is full, FullStack exception is thrown;
//       else newItem is at the top of the stack.

{
  if (IsFull())
    throw FullStack();
  else
  {
    NodeType* location;
    location = new NodeType;
    location->info = newItem;
    location->next = topPtr;
	topPtr = location;
	if (time_stamp == NULL) {
		time_stamp = location;
	}
	else if (time_stamp->time_stamp < location->time_stamp) {
		time_stamp = location;
	}
  }
}
void StackType::Pop()
// Removes top item from Stack and returns it in item.
// Pre:  Stack has been initialized.
// Post: If stack is empty, EmptyStack exception is thrown;
//       else top element has been removed.
{
  if (IsEmpty())
    throw EmptyStack();
  else
  {  
    NodeType* tempPtr,* time;
    tempPtr = topPtr;
	if (topPtr == time_stamp) {
		topPtr = topPtr->next;
	}
	else {
		while (true) {
			if (tempPtr->next == time_stamp) {
				tempPtr->next = time_stamp->next;
				time_stamp = topPtr;
				break;
			}
			tempPtr = tempPtr->next;
		}
	}
    delete tempPtr;

	time = topPtr;
	while (true) {
		if (time_stamp->time_stamp < time->time_stamp) {
			time_stamp = time;
		}
		if (time->next == NULL) {
			break;
		}
		time = time->next;
	}
  }
}
ItemType StackType::Top()
// Returns a copy of the top item in the stack.
// Pre:  Stack has been initialized.
// Post: If stack is empty, EmptyStack exception is thrown;
//       else a copy of the top element is returned.
{
  if (IsEmpty())
    throw EmptyStack();
  else
    return time_stamp->info;  
}
bool StackType::IsEmpty() const
// Returns true if there are no elements on the stack; false otherwise.
{
    return (topPtr == NULL);
}
bool StackType::IsFull() const
// Returns true if there is no room for another ItemType 
//  on the free store; false otherwise.
{
  NodeType* location;
  try
  {
    location = new NodeType;
    delete location;
    return false;
  }
  catch(std::bad_alloc)
  {
    return true;
  }
}

StackType::~StackType()
// Post: stack is empty; all items have been deallocated.
{
    NodeType* tempPtr;

    while (topPtr != NULL)
    {
        tempPtr = topPtr;
        topPtr = topPtr->next;
        delete tempPtr;
    }
}

StackType::StackType()	// Class constructor.
{
    topPtr = NULL;
}

