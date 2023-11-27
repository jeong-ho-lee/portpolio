#include "Student.h"

void Swap(Student& item1, Student& item2)
// Post: Contents of item1 and item2 have been swapped.
{
  Student tempItem;

  tempItem = item1;
  item1 = item2;
  item2 = tempItem;
}