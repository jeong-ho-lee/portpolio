#include "UnsortedType.h"
#include "stddef.h"

template <typename ItemType>
UnsortedType<ItemType>::UnsortedType() {
	length = 0;
	listData = NULL;
	currentPos = NULL;
}

template <typename ItemType>
UnsortedType<ItemType>::~UnsortedType() {
	NodeType<ItemType>* tempPtr;

	while (listData != NULL)
	{
		tempPtr = listData;
		listData = listData->next;
		delete tempPtr;
	}
}

template <typename ItemType>
void UnsortedType<ItemType>::MakeEmpty() {
	NodeType<ItemType>* tempPtr;

	while (listData != NULL)
	{
		tempPtr = listData;
		listData = listData->next;
		delete tempPtr;
	}
	length = 0;
}

template<typename ItemType>
bool UnsortedType<ItemType>::IsFull() const {
	NodeType<ItemType>* location;
	/*try
	{*/
	location = new NodeType<ItemType>;
	delete location;
	return false;
	/*}
	catch(bad_alloc exception)
	{
	  return true;
	}*/
}

template <typename ItemType>
int UnsortedType<ItemType>::LengthIs() const {
	return length;
}

template <typename ItemType>
void UnsortedType<ItemType>::RetrieveItem(ItemType& item, bool& found) {
	bool moreToSearch;
	NodeType<ItemType>* location;

	location = listData;
	found = false;
	moreToSearch = (location != NULL);

	while(moreToSearch && !found) {
		if (item == location->info) {
			found = true;
			item = location->info;
		} else {
			location = location->next;
			moreToSearch = (location != NULL);
		}
	}
}

template <typename ItemType>
void UnsortedType<ItemType>::InsertItem(ItemType item) {
	NodeType<ItemType>* location;

	location = new NodeType<ItemType>;
	location->info = item;
	location->next = listData;
	listData = location;
	length++;
}

//4
template <typename ItemType>
void UnsortedType<ItemType>::DeleteItem(ItemType item) {
	bool moreToSearch;
	RetrieveItem(item, moreToSearch);

	while (moreToSearch) {
		NodeType<ItemType>* location = listData;
		NodeType<ItemType>* tempLocation;

		if (item == listData->info)
		{
			tempLocation = location;
			listData = listData->next;
		}
		else
		{
			while (!(item == (location->next)->info))
				location = location->next;

			tempLocation = location->next;
			location->next = (location->next)->next;
		}
		delete tempLocation;
		length--;
		RetrieveItem(item, moreToSearch);
	}
}

template <typename ItemType>
void UnsortedType<ItemType>::ResetList() {
	currentPos = NULL;
}

template <typename ItemType>
void UnsortedType<ItemType>::GetNextItem(ItemType& item) {
	if (currentPos == NULL) currentPos = listData;

	item = currentPos->info;
	currentPos = currentPos->next;
}

//3 - D
template <typename ItemType>
void UnsortedType<ItemType>::MergeLists(UnsortedType<ItemType>& other, UnsortedType<ItemType>& result) {
	NodeType<ItemType>* ptr = listData;

	while (!ptr == NULL) {
		result.InsertItem(ptr->info);
		ptr = ptr->next;
	}

	ptr = other.listData;

	while (!ptr == NULL) {
		result.InsertItem(ptr->info);
		ptr = ptr->next;
	}
}